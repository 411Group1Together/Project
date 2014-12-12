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

boolean playerOneJoined = false;
boolean playerTwoJoined = false;
boolean playerThreeJoined = false;
boolean playerFourJoined = false;

int buttonState1 = 1;
int buttonState2 = 1;
int buttonState3 = 1;
int buttonState4 = 1;
int buttonStateR = 1;

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

GameState gGameState;

//Number of ms passed since start of game (+ = 10)
long gameTime = 0;
long beginTime = 0;
long startGameTime = 0;

//Setting the game state to startup and other stuff
void resetGame(){
gGameState = GameState_Startup;
gameTime = 0;
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
  
  long playerOneTime = 0;
  long playerTwoTime = 0;
  long playerThreeTime = 0;
  long playerFourTime = 0;
  
  do {
    
    switch (gGameState) {
      
      case GameState_Startup:
        splash(ILI9340_GREEN,ILI9340_YELLOW,ILI9340_BLUE);
        rules();
        delay(5000);
        playerOneTime = 0;
        playerTwoTime = 0;
        playerThreeTime = 0;
        playerFourTime = 0;
        gGameState = GameState_WaitForPlayButton;
        break;
        
      case GameState_WaitForPlayButton:
        buttonStateR = digitalRead(SwitchReset);
        if (buttonStateR == HIGH){
        play();
        buttonState1 = digitalRead(SwitchOne);
        if (buttonState1 == LOW){
           gGameState = GameState_WaitForPlayersToJoin;
        }
        startGameTime = millis();
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
        idleLoops++;
        howManyPlayers();
        buttonState1 = digitalRead(SwitchOne);
        if (buttonState1 == LOW){
          playerOneJoined = true;
        }
        buttonState2 = digitalRead(SwitchTwo);
        if(buttonState2 == LOW){
          playerTwoJoined = true;
        }
        buttonState3 = digitalRead(SwitchThree);
        if(buttonState3 == LOW){
          playerThreeJoined = true;
        }
        buttonState4 = digitalRead(SwitchFour);
         if (buttonState4 == LOW){
          playerFourJoined = true;
         }
        if (idleLoops > 10) {
            if(playerOneJoined == false && playerTwoJoined == false && playerThreeJoined == false && playerFourJoined == false){
              tft.fillScreen(ILI9340_BLACK);
              tft.println("ERROR");
              delay(1000);
              idleLoops = 0;
             gGameState = GameState_Startup; 
            }    
            else{
              // Done adding players, show how many joined on display
              idleLoops = 0;
              gGameState = GameState_DisplayNumOfPlayers;
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
        gGameState = GameState_DisplayRandomGo;
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
          gGameState = GameState_WaitForPlayerInputOrTimeout;
        }
        else{
         resetGame(); 
        }
        break;
        
      case GameState_WaitForPlayerInputOrTimeout:
        buttonState1 = digitalRead(SwitchOne);
        if (buttonState1 == LOW){
          // record when player press his button
          if(playerOneTime == 0){
          playerOneTime = millis() - beginTime;
          }
        }
        buttonState2 = digitalRead(SwitchTwo);
        if (buttonState2 == LOW){
          // record when player press his button
          if(playerTwoTime == 0){
          playerTwoTime = millis() - beginTime;
          }
        }
        buttonState3 = digitalRead(SwitchThree);
        if (buttonState3 == LOW){
          // record when player press his button
          if(playerThreeTime == 0){
          playerThreeTime = millis() - beginTime;
          }
        }
        buttonState4 = digitalRead(SwitchFour);
        if (buttonState4 == LOW){
          // record when player press his button
          if(playerFourTime == 0){
          playerFourTime = millis() - beginTime;
          }
        }
        if (millis() > beginTime + 3000) {
          roundCompleted();
          delay(2000);
          gGameState = GameState_EvaluatePlayerScores;
        }
        break;
        
      case GameState_EvaluatePlayerScores:
        Serial.println("Eavl");
        Serial.println(playerOneTime);
        Serial.println(playerTwoTime);
        Serial.println(playerOneJoined);
        //buttonStateR = digitalRead(SwitchReset);
        //if (buttonStateR == HIGH){
        if(playerOneJoined == true && playerTwoJoined == false && playerThreeJoined == false && playerFourJoined == false){
          winner = 1;
        } 
        if(playerOneJoined == true && playerTwoJoined == true && playerThreeJoined == false && playerFourJoined == false){
          if(playerOneTime <= playerTwoTime){
           winner = 1; 
          }
          if(playerTwoTime < playerOneTime){
           winner = 2; 
          }
        }
        if(playerOneJoined == true && playerTwoJoined == true && playerThreeJoined == true && playerFourJoined == false){
           if(playerOneTime <= playerTwoTime && playerOneTime <= playerThreeTime){
           winner = 1; 
          }
          if(playerTwoTime < playerOneTime && playerTwoTime <= playerThreeTime){
           winner = 2; 
          }
           if(playerThreeTime < playerOneTime && playerThreeTime < playerTwoTime){
           winner = 3; 
          }
        }
        if(playerOneJoined == true && playerTwoJoined == true && playerThreeJoined == true && playerFourJoined == true){
           if(playerOneTime <= playerTwoTime && playerOneTime <= playerThreeTime && playerOneTime <= playerFourTime){
           winner = 1; 
          }
          if(playerTwoTime < playerOneTime && playerTwoTime <= playerThreeTime && playerTwoTime <= playerFourTime){
           winner = 2; 
          }
           if(playerThreeTime < playerOneTime && playerThreeTime < playerTwoTime && playerThreeTime <= playerFourTime){
           winner = 3; 
          }
            if(playerFourTime < playerOneTime && playerFourTime < playerTwoTime && playerFourTime < playerThreeTime){
           winner = 3; 
          }
        }
        gGameState = GameState_DisplayWinner;
        //}
        //else{
         //resetGame(); 
        //}
        break;
        
      case GameState_DisplayWinner:
      Serial.println(winner);
        buttonStateR = digitalRead(SwitchReset);
        if (buttonStateR == HIGH){
        // update the display with the player number
        //wrote display for this but may need to change based on eval of player scores function
        displayWinner();
        if(winner == 1){
          tft.setTextSize(3);
         tft.println(playerOneTime + "ms"); 
        }
         if(winner == 2){
           tft.setTextSize(3);
         tft.println(playerTwoTime + "ms"); 
        }
         if(winner == 3){
           tft.setTextSize(3);
         tft.println(playerThreeTime + "ms"); 
        }
         if(winner == 4){
         tft.setTextSize(3);
         tft.println(playerFourTime + "ms"); 
        }
        delay(3000);
          playAgain();
          gGameState = GameState_WaitForPlayAgain;
        }
        else{
         resetGame(); 
        }
        break;
        
      case GameState_WaitForPlayAgain:
        buttonStateR = digitalRead(SwitchReset);
        if (buttonStateR == HIGH){
         buttonState1 = digitalRead(SwitchOne);
        if (buttonState1 == LOW){
          // Restart the game, with the same number of players
          gGameState = GameState_DisplayNumOfPlayers;
          gameTime = 0;
        }
        else{
         tft.fillScreen(ILI9340_BLACK);
         tft.setCursor(50,50);
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
  } while (!done);  
}

