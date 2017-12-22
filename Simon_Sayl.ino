/*A Simon Says style memory game.

Features:
  - LCD display
  - Music and tunes
  - 1 and 2 player modes
  - 1 player mode has 4 difficulty settings
 
Circuit setup:
Buttons are tied between ground and there designated pins. Internal
pull-up resistors are in use. Buttons are set up as follows:
  - button 1 to pin 6
  - button 2 to pin 7
  - button 3 to pin 8
  - button 4 to pin 9

LEDs are tired between ground and a 100 Ohm resistor to there
designated pins. LEDs will be set up as follows:
  - Red LED goes with button 1 and ties to pin 14 (A0)
  - Yellow LED goes with button 2 and ties to pin 15 (A1)
  - Green LED goes with button 3 and ties to pin 16 (A2)
  - Blue LED goes with button 4 and ties to pin 17 (A3)

The speaker will need one lead on pin 6 and the other tied
to ground.

The LCD will the connected as follows:
  - LCD RS pin to digital pin 12
  - LCD Enable pin to digital pin 11
  - LCD D4 pin to digital pin 5
  - LCD D5 pin to digital pin 4
  - LCD D6 pin to digital pin 3
  - LCD D7 pin to digital pin 2
  - LCD R/W pin to ground
  - 10K Pot Data Pin VEE (Pot Middle Pin)
  - ends to +5V and ground
  - wiper to LCD VO pin 

*/

#include <Tone.h> //call to tone library
#include <LiquidCrystal.h> //call to LiquidCrustal library

Tone speakerpin; //enables the speaker

//sets up the tones and note durations for the different pieces of music used
int starttune[] = {NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_G4};
int duration2[] = {100, 200, 100, 200, 100, 400, 100, 100, 100, 100, 200, 100, 500};
int note[] = {NOTE_C4, NOTE_C4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5};
int duration[] = {100, 100, 100, 300, 100, 300};

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);//pins for LCD display

boolean button[] = {6, 7, 8, 9}; //The four button input pins
boolean ledpin[] = {14, 15, 16, 17 };  // LED pins

int turn = 0;  // turn counter
int turnlimit = 0;  //difficulty level setting
int buttonstate = 0;  // button state checker
int randomArray[100]; //Intentionally long to store up to 100 inputs (doubtful anyone will get this far)
int inputArray[100];  //used for checking button pushes against the array
int game = 0; //check sum for menu system
int player = 1; //Player turn designator for 2 player game
int progress = 0; //tracks progress of 2 player game
int tune = 0; //controls music is 2 player game


void setup() 
{
  lcd.begin(16, 2);    //Prints game name on the LCD
  lcd.setCursor(2,0);
  lcd.print("Total");
  lcd.setCursor(8,1);
  lcd.print("Recall!");
  
  speakerpin.begin(13); // speaker is on pin 6

  for(int x=0; x<4; x++)  // LED pins are outputs
  {
    pinMode(ledpin[x], OUTPUT);
  }
  
  for(int x=0; x<4; x++) 
  {
    pinMode(button[x], INPUT);  // button pins are inputs
    digitalWrite(button[x], HIGH);  // enable internal pull-up; buttons start in high position; logic reversed
  }

  randomSeed(analogRead(5)); //Adds more randomness with the Array for 1 player game
  
  for (int thisNote = 0; thisNote < 13; thisNote ++) 
  { // play the next note:
    speakerpin.play(starttune[thisNote]); // hold the note:
    
    if (thisNote==0 || thisNote==2 || thisNote==4 || thisNote== 6)
    {
      digitalWrite(ledpin[0], HIGH); //designates which LED to light based on the note being played
    }
    
    if (thisNote==1 || thisNote==3 || thisNote==5 || thisNote== 7 || thisNote==9 || thisNote==11)
    {
      digitalWrite(ledpin[1], HIGH);
    }
    
    if (thisNote==8 || thisNote==12)
    {
      digitalWrite(ledpin[2], HIGH);
    }  
    
    if (thisNote==10)
    {   
      digitalWrite(ledpin[3], HIGH);
    }
    
    delay(duration2[thisNote]); // stop for the next note:
    
    speakerpin.stop();
    
    digitalWrite(ledpin[0], LOW);
    digitalWrite(ledpin[1], LOW);
    digitalWrite(ledpin[2], LOW);
    digitalWrite(ledpin[3], LOW);
    delay(25);
    }
  delay(1000);
  Menu();
}
 
 
void Menu() //menu display
{
  lcd.clear();
  lcd.print(" Sellect number");
  lcd.setCursor(3,1);
  lcd.print("of players");
  delay(1500);  
}
  
  
void loop()
{ //menu system for selecting 1 or 2 player game
  if (game == 0) //no game selected
  {
    lcd.clear();
    
    for(int y=0; y<4; y++)
    {
      lcd.setCursor(0,0);
      lcd.print("1 Player: RED");
      lcd.setCursor(0,1);
      lcd.print("2 Player: YELLOW");
       
      buttonstate = digitalRead(button[y]);
      
      if (buttonstate == LOW && button[y] == 6)
      {
        button1(); //calls for the RED button's LED to light and tone to play
        game = 1; //1 player game
      }
      
      if (buttonstate == LOW && button[y] == 7)
      {
        button2(); //calls for the Yellow button's LED to light and tone to play
        game = 2; //2 player game
      }
    }
  }
  
  if (game == 1 && turnlimit > 1) //as long as 1 turn has been taken in the 1 player game
  {
    play1(); //calls to the 1 player game
  }
  
  if (game ==1 && turnlimit < 1)  //no turns have been taken in the 1 player game
  {
    play1menu(); //calls to the menu to select number of turns for 1 player game
  }
      
  if (game == 2)
  {
    play2(); //calls to 2 player game
  }   
}

 
void play1menu() //menu for selecting 1 player difficulty
{
  lcd.clear();
  lcd.print("Red=10 Yellow=20");
  lcd.setCursor(0,1);
  lcd.print("Green=30 Blue=40");
  delay(100);
  
  for(int y=0; y<4; y++)
  {
    buttonstate = digitalRead(button[y]);
    if (buttonstate == LOW && button[y] == 6)
    {
      button1();  ////calls for the RED button's LED to light and tone to play
      turnlimit = 10; //sets number of turns
    }
    
    if (buttonstate == LOW && button[y] == 7)
    {
      button2();
      turnlimit = 20;
    }
    
    if (buttonstate == LOW && button[y] == 8)
    {
      button3();
      turnlimit = 30;
    }
    
    if (buttonstate == LOW && button[y] == 9)
    {
      button4();
      turnlimit = 40;
    }
  }
}


void play2() //2 player game
{
  lcd.clear();
  
  if (player == 1) //displays player 1's turn
  { 
    lcd.setCursor(0,0);           
    lcd.print("Player 1 Turn:"); 
    lcd.print(turn);    
  }
  
  if (player == 2) //displays player 2's turn
  { 
    lcd.setCursor(0,0);      
    lcd.print("Player 2 Turn:");
    lcd.print(turn);     
  }    
  
  lcd.setCursor(0,1);
  lcd.print("CHOOSE A BUTTON");
  delay(150);
  
  if (tune == 0) //if tune hasn't been played play the tune
  {
    digitalWrite(ledpin[0], HIGH);
    digitalWrite(ledpin[1], HIGH);
    digitalWrite(ledpin[2], HIGH);
    digitalWrite(ledpin[3], HIGH);
  
    for (int thisNote = 0; thisNote < 6; thisNote ++) 
    { // play the next note:
      speakerpin.play(note[thisNote]); // hold the note:
      delay(duration[thisNote]); // stop for the next note:
      speakerpin.stop();
      delay(25);
      tune = 1;  //tune has been played
    }
    
    digitalWrite(ledpin[0], LOW);
    digitalWrite(ledpin[1], LOW);
    digitalWrite(ledpin[2], LOW);
    digitalWrite(ledpin[3], LOW);
    delay(1000);
  }  
  
  for(int v=0; v<4; v++)
  {      
    buttonstate = digitalRead(button[v]);
        
    if (buttonstate == LOW && button[v] == 6)
    { //Checking for button push
      button1();
      randomArray[progress] = 1;  //stores button push in array
      
      if (player == 1)
      {
        player++;  //player 2's turn
        progress++;
        delay(500);
        output();  //Calls to loop for displaying the stored sequence
        tune = 0;  //tune can play again is sequence is enter correctly
        return;        
      }
      if (player == 2)
      {
        player--;  //player 1's turn
        progress++;
        delay(500);
        output();
        tune = 0;
        return;        
      }      
    }
    
    if (buttonstate == LOW && button[v] == 7)
    {
      button2();
      randomArray[progress] = 2;
      
      if (player == 1)
      {
        player++;
        progress++;
        delay(500);
        output();
        tune = 0;        
        return;        
      }
      if (player == 2)
      {
        player--;
        progress++;
        delay(500);
        output(); 
        tune = 0;       
        return;        
      }            
    }

    if (buttonstate == LOW && button[v] == 8)
    {
      button3();
      randomArray[progress] = 3;
      
      if (player == 1)
      {
        player++;
        progress++;
        delay(500);
        output();
        tune = 0;
        return;        
      }
      if (player == 2)
      {
        player--;
        progress++;
        delay(500);
        output();
        tune = 0;
        return;        
      }            
    }

    if (buttonstate == LOW && button[v] == 9)
    {
      button4();
      randomArray[progress] = 4;
      
      if (player == 1)
      {
        player++;
        progress++;
        delay(500);
        output();
        tune = 0;
        return;        
      }
      if (player == 2)
      {
        player--;
        progress++;
        delay(500);
        output();
        tune = 0;
        return;        
      }            
    }       
  } 
}


void output()  //outputs the sequence stored in the array by lighting the LEDs and playing the tones.
{        
  lcd.clear();
  if (player == 1)  //player 1's turn
  { 
    lcd.setCursor(0,0);           
    lcd.print("Player 1 Turn:");
    lcd.print(turn);
  }
  
  if (player == 2)  //player 2's turn
  { 
    lcd.setCursor(0,0);      
    lcd.print("Player 2 Turn:");
    lcd.print(turn);
  }    
  
  lcd.setCursor(0,1);
  lcd.print("WATCH THE LIGHTS");
  delay(1000);
  
  for (int x=0; x <= turn; x++)
  {
    for(int y=0; y<4; y++)
    {
      if (randomArray[x] == 1 && ledpin[y] == 14) 
      {  //if statements to display the stored values in the array
        button1();
      }
      
      if (randomArray[x] == 2 && ledpin[y] == 15) 
      {
        button2();
      }
      
      if (randomArray[x] == 3 && ledpin[y] == 16) 
      {
        button3();
      }
      
      if (randomArray[x] == 4 && ledpin[y] == 17) 
      {
        button4();
      }
    }
  }
  lcd.clear();
  
  if (player == 1)  //player 1's turn to mimic the sequence
  { 
    lcd.setCursor(0,0);           
    lcd.print("Player 1 Turn:");
    lcd.print(turn);
  }
  
  if (player == 2)  //player 2's turn to mimic the sequence
  { 
    lcd.setCursor(0,0);      
    lcd.print("Player 2 Turn:");
    lcd.print(turn);
  }    
  
  lcd.setCursor(0,1);
  lcd.print("REPEAT SEQUENCE");
  delay(150);      
  input();  //calls to loop allowing player to mimic the sequence
 }
 
 
void play1() //1 player game
{   
  lcd.clear();
  lcd.print("Turn: ");  //what turn your on
  lcd.print(turn);
  lcd.setCursor(0,1);
  lcd.print("Goal: ");  //how many turns to complete to win
  lcd.print(turnlimit);
  digitalWrite(ledpin[0], HIGH);
  digitalWrite(ledpin[1], HIGH);
  digitalWrite(ledpin[2], HIGH);
  digitalWrite(ledpin[3], HIGH);
  
  for (int thisNote = 0; thisNote < 6; thisNote ++) 
  { // play the next note:
    speakerpin.play(note[thisNote]); // hold the note:
    delay(duration[thisNote]); // stop for the next note:
    speakerpin.stop();
    delay(25);
  }
  
  digitalWrite(ledpin[0], LOW);
  digitalWrite(ledpin[1], LOW);
  digitalWrite(ledpin[2], LOW);
  digitalWrite(ledpin[3], LOW);
  delay(1000);
  
  for (int t=turn; t <= turn; t++)
  { //Limited by the turn variable
    randomArray[t] = random(1, 5); //Assigning a random number (1-4) to the array, y being the turn count
    for (int x=0; x <= turn; x++)
    {
      for(int y=0; y<4; y++)
      {
        if (randomArray[x] == 1 && ledpin[y] == 14) 
        {  //if statements to display the stored values in the array
          button1();
        }
        
        if (randomArray[x] == 2 && ledpin[y] == 15) 
        {
          button2();
        }
        
        if (randomArray[x] == 3 && ledpin[y] == 16) 
        {
          button3();
        }
        
        if (randomArray[x] == 4 && ledpin[y] == 17) 
        {
          button4();
        }
      }
    }
  }
  input();
}
 
 
void input() 
{ //loop for 1 and 2 player games to mimic the sequence
  for (int x=0; x <= turn;)
  { //Statement controlled by turn count
    for(int y=0; y<4; y++)
    {
      buttonstate = digitalRead(button[y]);
      
      if (buttonstate == LOW && button[y] == 6)
      { //Checking for button push
        button1();
        inputArray[x] = 1;
              
        if (inputArray[x] != randomArray[x]) 
        { //Checks value input by user and checks it against
          fail(); //the value in the same spot on the generated array
        } //The fail function is called if it does not match
        x++;
      }
      
      if (buttonstate == LOW && button[y] == 7)
      {
        button2();
        inputArray[x] = 2;
                
        if (inputArray[x] != randomArray[x]) 
        {
          fail();  //call to the loop to end the game
        }
        x++;
      }
      
      if (buttonstate == LOW && button[y] == 8)
      {
        button3();
        inputArray[x] = 3;
                
        if (inputArray[x] != randomArray[x]) 
        {
          fail();
        }
        x++;
      }

      if (buttonstate == LOW && button[y] == 9)
      {
        button4();
        inputArray[x] = 4;
        
        if (inputArray[x] != randomArray[x]) 
        {
          fail();
        }
        x++;
      }
    }
  }
  
  delay(500);
  turn++; //Increments the turn count, also the last action before starting the output function over again
  if (game == 1 && turn > turnlimit)  //if you've completed the last turn in single player game
  {
    win1(); //calls to winner loop for single player game
  }
  
  if (game == 2 && turn > 99) //after 99 turns the array is maxed out
  {
    win2();
  }
}


void win2()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("******DRAW******");
  lcd.setCursor(0,1);
  lcd.print("!ARRAY OVERFLOW!");
  digitalWrite(ledpin[0], HIGH);
  digitalWrite(ledpin[1], HIGH);
  digitalWrite(ledpin[2], HIGH);
  digitalWrite(ledpin[3], HIGH);
  speakerpin.play(NOTE_C4, 600);
  delay(650);
  speakerpin.play(NOTE_C4, 600);
  delay(650);
  speakerpin.play(NOTE_C4, 600);
  delay(2000);
  digitalWrite(ledpin[0], LOW);
  digitalWrite(ledpin[1], LOW);
  digitalWrite(ledpin[2], LOW);
  digitalWrite(ledpin[3], LOW);
  
  for (int x=0; x<100; x++) //clears array
  {
    randomArray[x] = 0;
  }
  
  progress = 0;  //resets progress for 2 player game
  player = 1;  //2 player game's player reset
  turn = -1; //resets turn value so the game starts over without need for a reset button
  game = 0;  //resets game selection
  turnlimit = 0;  //resets turn limit for 1 player game
  Menu();  ////back to the top of the program
}


void win1() //single player winner
{
  lcd.clear();
  lcd.print("YOU WIN!");
  digitalWrite(ledpin[0], HIGH);
  digitalWrite(ledpin[1], HIGH);
  digitalWrite(ledpin[2], HIGH);
  digitalWrite(ledpin[3], HIGH);
  speakerpin.play(NOTE_G4, 300);
  delay(350);
  speakerpin.play(NOTE_G4, 100);
  delay(150);
  speakerpin.play(NOTE_C5, 600);
  delay(1000);
  digitalWrite(ledpin[0], LOW);
  digitalWrite(ledpin[1], LOW);
  digitalWrite(ledpin[2], LOW);
  digitalWrite(ledpin[3], LOW);
  game = 0; //resets game selection
  turn = -1; //resets turn value so the game starts over without need for a reset button
  turnlimit = 0; //resets turn limit for 1 player game
  Menu(); //back to the top of the program
}
  

void fail() 
{ //function used if the player fails to match the sequence
  lcd.clear(); 
  
  if (game == 2 && player == 1)  //2 player game with player 1 as looser
    { 
      lcd.setCursor(0,0);           
      lcd.print("Player 1!");     
    }
    if (game == 2 && player == 2) //2 player game with player 2 as looser
    { 
      lcd.setCursor(0,0);      
      lcd.print("Player 2!");     
    }    
  
  lcd.setCursor(0,1);
  lcd.print("YOU LOOSE!");

  for (int y=0; y<=2; y++)
  { //Flashes lights for failure
    digitalWrite(ledpin[0], HIGH);
    digitalWrite(ledpin[1], HIGH);
    digitalWrite(ledpin[2], HIGH);
    digitalWrite(ledpin[3], HIGH);
    speakerpin.play(NOTE_G3, 300);
    delay(200);
    digitalWrite(ledpin[0], LOW);
    digitalWrite(ledpin[1], LOW);
    digitalWrite(ledpin[2], LOW);
    digitalWrite(ledpin[3], LOW);
    speakerpin.play(NOTE_C3, 300);
    delay(200);
  }
  delay(500);
  
  for (int x=0; x<100; x++) //clears array
  {
    randomArray[x] = 0;
  }
  
  progress = 0;  //resets progress for 2 player game
  player = 1;  //2 player game's player reset
  turn = -1; //resets turn value so the game starts over without need for a reset button
  game = 0;  //resets game selection
  turnlimit = 0;  //resets turn limit for 1 player game
  Menu();  ////back to the top of the program
}


void button1()  //LED displayed and tone sounded for RED LED and Button 1
{
  digitalWrite(ledpin[0], HIGH);
  speakerpin.play(NOTE_G3, 100);
  delay(200);
  digitalWrite(ledpin[0], LOW);
  delay(300);
}


void button2()
{
  digitalWrite(ledpin[1], HIGH);
  speakerpin.play(NOTE_A3, 100);
  delay(200);
  digitalWrite(ledpin[1], LOW);
  delay(300);
}


void button3()
{
  digitalWrite(ledpin[2], HIGH);
  speakerpin.play(NOTE_B3, 100);
  delay(200);
  digitalWrite(ledpin[2], LOW);
  delay(300);
}


void button4()
{
  digitalWrite(ledpin[3], HIGH);
  speakerpin.play(NOTE_C4, 100);
  delay(200);
  digitalWrite(ledpin[3], LOW);
  delay(300);
}


