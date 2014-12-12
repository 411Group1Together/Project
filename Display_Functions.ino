unsigned long splash(uint16_t color1, uint16_t color2, uint16_t color3){
  int x1, y1, x2, y2,
  w = tft.width(),
  h = tft.height();
  x1 = 50;
  y1 = 0;
  y2    = h - 1;
    tft.fillScreen(ILI9340_RED);
    tft.fillScreen(ILI9340_GREEN);
    tft.fillScreen(ILI9340_BLUE);
    tft.fillScreen(ILI9340_BLACK);
    delay(1000);
    for(x2=-5; x2<w; x2+=6) tft.drawLine(x1,y1,x2,y2,color1);
    for(x2=50; x2<w+50; x2+=6) tft.drawLine(x1,y1,x2,y2,color2);
    for(x2=100; x2<w+75; x2+=6) tft.drawLine(x1,y1,x2,y2,color3);
    tft.setCursor(50,120);
    tft.setRotation(3);
    tft.setTextColor(ILI9340_CYAN);
    tft.setTextSize(4);
    tft.println("THINK FAST");    
}
unsigned long rules(){
  tft.fillScreen(ILI9340_BLACK);
  tft.setCursor(0,0);
  tft.setRotation(3);
  tft.setTextSize(3);
  tft.println("RULES");
  tft.setTextSize(2);
  tft.setTextColor(ILI9340_GREEN);
  tft.println();
  tft.println("Choose number of players");
  tft.println();
  tft.println("Don't push the button");
  tft.println("until you get the cue!");
  tft.println();
  tft.println("Be the first to push");
  tft.println("your button when you see");
  tft.println("the cue!");
}
unsigned long go(){
 tft.fillScreen(ILI9340_BLACK);
 tft.setCursor(120,110);
 tft.setTextSize(3);
 tft.setTextColor(ILI9340_GREEN);
 tft.println("Ready");
 tft.setCursor(120,110);
 tft.fillScreen(ILI9340_BLACK);
 tft.println("Set");
 int randomTime = random(2000, 5000);
 delay(randomTime);
 tft.fillScreen(ILI9340_BLACK);
 tft.setCursor(120,110);
 tft.setTextSize(5);
 tft.setTextColor(ILI9340_RED);
 tft.println("GO!");
}
unsigned long howManyPlayers(){
  tft.fillScreen(ILI9340_BLACK);
  tft.setCursor(50,50);
  tft.setRotation(3);
  tft.setTextSize(4);
  tft.setTextColor(ILI9340_GREEN);
  tft.println("Push Your"); 
  tft.println("  Button to");
  tft.println("    Join");
  tft.println("  the Game!");
}
unsigned long displayNumberOfPlayers(){
  tft.fillScreen(ILI9340_BLACK);
  tft.setCursor(50,50);
  tft.setRotation(3);
  tft.setTextSize(4);
  tft.setTextColor(ILI9340_GREEN);
  if(playerOneJoined == true && playerTwoJoined == false && playerThreeJoined == false && playerFourJoined == false){
    tft.println("One Player");
    tft.println(" ");
    tft.println("  Game Ready");
  } 
  if(playerOneJoined == true && playerTwoJoined == true && playerThreeJoined == false && playerFourJoined == false){
    tft.println("Two Player");
    tft.println(" ");
    tft.println("  Game Ready");
  }
  if(playerOneJoined == true && playerTwoJoined == true && playerThreeJoined == true && playerFourJoined == false){
    tft.setCursor(20,50);
    tft.println("Three Player");
    tft.println(" ");
    tft.println("  Game Ready");
  }
  if(playerOneJoined == true && playerTwoJoined == true && playerThreeJoined == true && playerFourJoined == true){
    tft.setCursor(35,50);
    tft.println("Four Player");
    tft.println(" ");
    tft.println("  Game Ready");
  }
}
unsigned long roundCompleted(){
  tft.fillScreen(ILI9340_BLACK);
  tft.setCursor(55,50);
  tft.setRotation(3);
  tft.setTextSize(4);
  tft.setTextColor(ILI9340_GREEN);
  tft.println("Round       Completed");
}
unsigned long displayWinner(){
  if(winner == 1){
   tft.fillScreen(ILI9340_BLACK);
   tft.setCursor(50,50);
   tft.setRotation(3);
   tft.setTextSize(4);
   tft.setTextColor(ILI9340_RED);
   tft.println(" Winner:");
   tft.println(" ");
   tft.setTextSize(6);
   tft.setCursor(50,85);
   tft.println("Player");
   tft.setCursor(50,135);
   tft.println(" ONE!");
  }
  if(winner == 2){
   tft.fillScreen(ILI9340_BLACK);
   tft.setCursor(50,50);
   tft.setRotation(3);
   tft.setTextSize(4);
   tft.setTextColor(ILI9340_RED);
   tft.println(" Winner:");
   tft.println(" ");
   tft.setTextSize(6);
   tft.setCursor(50,85);
   tft.println("Player");
   tft.setCursor(50,135);
   tft.println(" TWO!");
  }
  if(winner == 3){
   tft.fillScreen(ILI9340_BLACK);
   tft.setCursor(50,50);
   tft.setRotation(3);
   tft.setTextSize(4);
   tft.setTextColor(ILI9340_RED);
   tft.println(" Winner:");
   tft.println(" ");
   tft.setTextSize(6);
   tft.setCursor(50,85);
   tft.println("Player");
   tft.setCursor(50,135);
   tft.println("THREE!");
  }
  if(winner == 4){
   tft.fillScreen(ILI9340_BLACK);
   tft.setCursor(50,50);
   tft.setRotation(3);
   tft.setTextSize(4);
   tft.setTextColor(ILI9340_RED);
   tft.println(" Winner:");
   tft.println(" ");
   tft.setTextSize(6);
   tft.setCursor(50,85);
   tft.println("Player");
   tft.setCursor(50,135);
   tft.println(" FOUR!");
  }
 }
 unsigned long playAgain(){
 tft.fillScreen(ILI9340_BLACK);
 tft.setCursor(40,50);
 tft.setRotation(3);
 tft.setTextSize(4);
 tft.setTextColor(ILI9340_GREEN);
 tft.println("Play Again?");
 tft.setTextSize(3);
 tft.println("   Press Player");
 tft.println("    One Button");
}
unsigned long play(){
 tft.fillScreen(ILI9340_BLACK);
 tft.setCursor(40,50);
 tft.setRotation(3);
 tft.setTextSize(2);
 tft.setTextColor(ILI9340_GREEN);
 tft.println("Want to Play?");
 tft.setTextSize(3);
 tft.println("   Press Player");
 tft.println("    One Button");
}
