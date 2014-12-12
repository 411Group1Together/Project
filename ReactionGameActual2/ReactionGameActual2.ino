//arduino compiler has no brain!

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9340.h"

#if defined(__SAM3X8E__)
  #undef __FlashStringHelper::F(string_literal)
  #define F(string_literal) string_literal
#endif

#define _sclk 13
#define _miso 12
#define _mosi 11
#define _cs 10
#define _dc 9
#define _rst 8

Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _rst);

const int SwitchOne = 6;
const int SwitchTwo = 4;
const int SwitchThree = 2;
const int SwitchFour = A2;
const int SwitchReset = A0;

const int MAX_LONG = 2147483647;

boolean playerOneJoined = false;
boolean playerTwoJoined = false;
boolean playerThreeJoined = false;
boolean playerFourJoined = false;

// This is how many milliseconds a button must be done to be a press
const int ButtonMinPress = 25;

// Button state is the last state of the button
// Button count is how many times through the loop the button is the same value
// Button pressed is true when button is stable (low) for N counts then is released (high)
// Button pressed gets cleared by game logic after it checks the value
int buttonState1 = 1;
int buttonCount1 = 0;
int buttonState2 = 1;
int buttonCount2 = 0;
int buttonState3 = 1;
int buttonCount3 = 0;
int buttonState4 = 1;
int buttonCount4 = 0;
int buttonStateR = 1;
int buttonCountR = 0;

boolean buttonPressed1 = false;
boolean buttonPressed2 = false;
boolean buttonPressed3 = false;
boolean buttonPressed4 = false;

int winner = 0;

typedef enum {
  GameState_Startup,
  GameState_WaitForPlayButton,
  GameState_WaitForPlayersToJoin,
  GameState_DisplayNumOfPlayers,
  GameState_DisplayRandomGo,
  GameState_WaitForPlayerInputOrTimeout,
  GameState_EvaluatePlayerScores,
  GameState_DisplayWinner,
  GameState_WaitForPlayAgain,
  
} GameState;  

int gGameState;

long beginTime = 0;
long startGameTime = 0;
long choosePlayersTime = 0;
long playAgainTimer = 0;

// Check the four player buttons for down and up
void scanButtons() {
	int b1;
	int b2;
	int b3;
	int b4;
	
	b1 = digitalRead(SwitchOne);
	b2 = digitalRead(SwitchTwo);
	b3 = digitalRead(SwitchThree);
	b4 = digitalRead(SwitchFour);
	
	// Check if button 1 changed state
	if (b1 != buttonState1) {
		// If button was released after a long enough press, report it
		if (b1 == HIGH && buttonCount1 >= ButtonMinPress) {
			buttonPressed1 = true;
		}
		// Reset button count because it changed state
		buttonCount1 = 0;
	}
	else {
		// The button state didn't change this time, increment counter
		if (buttonCount1 < ButtonMinPress) buttonCount1++;
	}
	
	// Check if button 2 changed state
	if (b2 != buttonState2) {
		// If button was released after a long enough press, report it
		if (b2 == HIGH && buttonCount2 >= ButtonMinPress) {
			buttonPressed2 = true;
		}
		// Reset button count because it changed state
		buttonCount2 = 0;
	}
	else {
		// The button state didn't change this time, increment counter
		if (buttonCount2 < ButtonMinPress) buttonCount2++;
	}
	
	// Check if button 3 changed state
	if (b3 != buttonState3) {
		// If button was released after a long enough press, report it
		if (b3 == HIGH && buttonCount3 >= ButtonMinPress) {
			buttonPressed3 = true;
		}
		// Reset button count because it changed state
		buttonCount3 = 0;
	}
	else {
		// The button state didn't change this time, increment counter
		if (buttonCount3 < ButtonMinPress) buttonCount3++;
	}

	// Check if button 4 changed state
	if (b4 != buttonState4) {
		// If button was released after a long enough press, report it
		if (b4 == HIGH && buttonCount4 >= ButtonMinPress) {
			buttonPressed4 = true;
		}
		// Reset button count because it changed state
		buttonCount4 = 0;
	}
	else {
		// The button state didn't change this time, increment counter
		if (buttonCount4 < ButtonMinPress) buttonCount4++;
	}
	
	// Remember the state of the button for next time
	buttonState1 = b1;
	buttonState2 = b2;
	buttonState3 = b3;
	buttonState4 = b4;
}

void advanceGameState(int nextState){

	// Update global
	gGameState = nextState;
	// Clear any pending buttons
	buttonPressed1 = false;
	buttonPressed2 = false;
	buttonPressed3 = false;
	buttonPressed4 = false;
}

void resetGame(){
  playerOneJoined = false;
  playerTwoJoined = false;
  playerThreeJoined = false;
  playerFourJoined = false;
    advanceGameState(GameState_Startup);
}


void setup(){
  // open a serial connection to display values
  Serial.begin(9600);
  tft.begin();
  pinMode(SwitchOne,INPUT);
  pinMode(SwitchTwo,INPUT);
  pinMode(SwitchThree,INPUT);
  pinMode(SwitchFour,INPUT);
  pinMode(SwitchReset,INPUT);
  boolean reset = false;
}

void loop(){ 
  
  int idleLoops = 0;
  
  resetGame();
  boolean done = false;
  
  long playerOneTime = MAX_LONG;
  long playerTwoTime = MAX_LONG;
  long playerThreeTime = MAX_LONG;
  long playerFourTime = MAX_LONG;
  
  do {
	// Check buttons first thing every time through the loop
    scanButtons();
	
    switch (gGameState) {
      
      case GameState_Startup:
        splash(ILI9340_GREEN,ILI9340_YELLOW,ILI9340_BLUE);
        delay(2000);
        rules();
        delay(5000);
        playerOneTime = MAX_LONG;
        playerTwoTime = MAX_LONG;
        playerThreeTime = MAX_LONG;
        playerFourTime = MAX_LONG;
        play();
        startGameTime = millis();
        advanceGameState(GameState_WaitForPlayButton);
        break;
        
      case GameState_WaitForPlayButton:
        buttonStateR = digitalRead(SwitchReset);
        if (buttonStateR == HIGH){   
            if (buttonPressed1){
              howManyPlayers();
              choosePlayersTime = millis();
              advanceGameState(GameState_WaitForPlayersToJoin);
            }
            if (millis() > startGameTime + 10000){
                resetGame(); 
            }
        }
        else{
          resetGame();
        }
        break;
        
      case GameState_WaitForPlayersToJoin:
        buttonStateR = digitalRead(SwitchReset);
        if (buttonStateR == HIGH){
          if (buttonPressed1){
            playerOneJoined = true;
          }
          if(buttonPressed2){
            playerOneJoined = true;
            playerTwoJoined = true;
          }
          if(buttonPressed3){
            playerOneJoined = true;
            playerTwoJoined = true;
            playerThreeJoined = true;
          }
          if (buttonPressed4){  
            playerOneJoined = true;
            playerTwoJoined = true;
            playerThreeJoined = true;
            playerFourJoined = true;
           }
          if (millis() > choosePlayersTime + 5000) {
            // Maybe we should just 
            if(playerOneJoined == false && playerTwoJoined == false && playerThreeJoined == false && playerFourJoined == false){
              tft.fillScreen(ILI9340_BLACK);
              tft.setCursor(50,50);
              tft.setTextColor(ILI9340_RED);
              tft.println("  ERROR");
              tft.println("");
              tft.println("Reset Game");
              delay(1000);
			  advanceGameState(GameState_Startup);
            }    
            else{
              // Done adding players, show how many joined on display
              advanceGameState(GameState_DisplayNumOfPlayers);
            }
          }
        }
        else{
          resetGame();
        }
        break;
        
      case GameState_DisplayNumOfPlayers:
        buttonStateR = digitalRead(SwitchReset);
        if (buttonStateR == HIGH){
          //display number of players
          displayNumberOfPlayers();
          delay(2000);
          advanceGameState(GameState_DisplayRandomGo);
        }
        else{
         resetGame(); 
        }
        break;
        
      case GameState_DisplayRandomGo:
        buttonStateR = digitalRead(SwitchReset);
        if (buttonStateR == HIGH){
          // Go has triggered, draw it on the display, and move on
          go();
          beginTime = millis();
          Serial.println(beginTime);
          advanceGameState(GameState_WaitForPlayerInputOrTimeout);
        }
        else{
         resetGame(); 
        }
        break;
        
      case GameState_WaitForPlayerInputOrTimeout:
        buttonStateR = digitalRead(SwitchReset);
        if (buttonStateR == HIGH){
          if (buttonPressed1){
          // record when player press his button
            if(playerOneTime == MAX_LONG){
              playerOneTime = millis() - beginTime;
            }
          }
          if (buttonPressed2){
          // record when player press his button
            if(playerTwoTime == MAX_LONG){
            playerTwoTime = millis() - beginTime;
            }
          }
          if (buttonPressed3){
          // record when player press his button
            if(playerThreeTime == MAX_LONG){
            playerThreeTime = millis() - beginTime;
            }
          }
          if (buttonPressed4){
          // record when player press his button
            if(playerFourTime == MAX_LONG){
            playerFourTime = millis() - beginTime;
            }
          }
          if (millis() > beginTime + 3000) {
            roundCompleted();
            delay(2000);
            advanceGameState(GameState_EvaluatePlayerScores);
          }
        }
         else{
           resetGame();
         }
        break;
        
      case GameState_EvaluatePlayerScores:
        buttonStateR = digitalRead(SwitchReset);
        if (buttonStateR == HIGH){
          if(playerOneJoined == true && playerTwoJoined == false && playerThreeJoined == false && playerFourJoined == false){
            winner = 1;
          } 
          if(playerOneJoined == true && playerTwoJoined == true && playerThreeJoined == false && playerFourJoined == false){
            if(playerOneTime != MAX_LONG && playerOneTime <= playerTwoTime){
              winner = 1; 
            }
            if(playerTwoTime != MAX_LONG && playerTwoTime < playerOneTime){
              winner = 2; 
             }
          }
          if(playerOneJoined == true && playerTwoJoined == true && playerThreeJoined == true && playerFourJoined == false){
             if(playerOneTime != MAX_LONG && playerOneTime <= playerTwoTime && playerOneTime <= playerThreeTime){
               winner = 1; 
             }
            if(playerTwoTime != MAX_LONG && playerTwoTime < playerOneTime && playerTwoTime <= playerThreeTime){
              winner = 2; 
            }
            if(playerThreeTime != MAX_LONG && playerThreeTime < playerOneTime && playerThreeTime < playerTwoTime){
              winner = 3; 
            }
          }
          if(playerOneJoined == true && playerTwoJoined == true && playerThreeJoined == true && playerFourJoined == true){
            if(playerOneTime != MAX_LONG && playerOneTime <= playerTwoTime && playerOneTime <= playerThreeTime && playerOneTime <= playerFourTime){
              winner = 1; 
            }
            if(playerTwoTime != MAX_LONG && playerTwoTime < playerOneTime && playerTwoTime <= playerThreeTime && playerTwoTime <= playerFourTime){
              winner = 2; 
            }
            if(playerThreeTime != MAX_LONG && playerThreeTime < playerOneTime && playerThreeTime < playerTwoTime && playerThreeTime <= playerFourTime){
              winner = 3; 
            }
            if(playerFourTime != MAX_LONG && playerFourTime < playerOneTime && playerFourTime < playerTwoTime && playerFourTime < playerThreeTime){
              winner = 4; 
            }
          }
        advanceGameState(GameState_DisplayWinner);
        }
        else{
         resetGame(); 
        }
        break;
        
      case GameState_DisplayWinner:
      Serial.println(winner);
        buttonStateR = digitalRead(SwitchReset);
        if (buttonStateR == HIGH){
          // update the display with the player number
          //wrote display for this but may need to change based on eval of player scores function
          displayWinner();
          delay(1000);
          if(winner == 1){
            tft.fillScreen(ILI9340_BLACK);
            tft.setCursor(120,50);
            tft.setTextSize(3);
            tft.println(playerOneTime);
            tft.println("");
            tft.println("   milliseconds"); 
          }
          if(winner == 2){
            tft.fillScreen(ILI9340_BLACK);
            tft.setCursor(120,50);
            tft.setTextSize(3);
            tft.println(playerTwoTime);
            tft.println("");
            tft.println("   milliseconds"); 
          }
          if(winner == 3){
            tft.fillScreen(ILI9340_BLACK);
            tft.setCursor(120,50);
            tft.setTextSize(3);
            tft.println(playerThreeTime);
            tft.println("");
            tft.println("   milliseconds"); 
          }
          if(winner == 4){
            tft.fillScreen(ILI9340_BLACK);
            tft.setCursor(120,50);
            tft.setTextSize(3);
            tft.println(playerFourTime);
            tft.println("");
            tft.println("   milliseconds"); 
          }
          winner = 0;
          delay(3000);
          playAgain();
          playAgainTimer = millis();
          advanceGameState(GameState_WaitForPlayAgain);
        }
        else{
         resetGame(); 
        }
        break;
        
      case GameState_WaitForPlayAgain:
        buttonStateR = digitalRead(SwitchReset);
        if (buttonStateR == HIGH){
          if(millis() > playAgainTimer + 2500){
            if (buttonPressed1){
              // Restart the game, with the same number of players
              advanceGameState(GameState_DisplayNumOfPlayers);
              playerOneTime = MAX_LONG;
              playerTwoTime = MAX_LONG;
              playerThreeTime = MAX_LONG;
              playerFourTime = MAX_LONG;
            }
          }
          if (millis() > playAgainTimer + 5000) {
            tft.fillScreen(ILI9340_BLACK);
            tft.setCursor(50,50);
            tft.setRotation(3);
            tft.setTextSize(4);
            tft.setTextColor(ILI9340_RED);
            tft.println("Game Over");
            delay(2000);
            done = true;
          }
        }
        else{
         resetGame(); 
        }
        break;
    }
    // throttle button and game logic  (mostly needed for button timing)
    delay(1);
  } while (!done);  
}
