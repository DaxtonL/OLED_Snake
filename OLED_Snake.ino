#include "Button.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define gridX 12
#define gridY 8

int grid[gridX][gridY];
const int gridSize = 5;
const int gridOffsetX = 32;
const int gridOffsetY = 20;

Button LButton(9);
Button UpButton(10);
Button RButton(11);
Button DownButton(12);

int playerX = 5;
int playerY = 4;

int appleX = 8;
int appleY = 4;

int dir = 0;
int snakeLength = 0;
const int startSnakeLength = 3;

bool playing = false;

int score = 0;

const long gameSpeed = 135;
unsigned long currentTime;
unsigned long startTime;

const long textTime = 200;

const int startImageXPos = 10;
const int startImageYPos = 5;
const int startImageLength = 12;
const int startImageWidth = 8;
const int startImageHeight = 4;
const int startImageMin = 5;
const int startImageMax = 64;

int switchStartImage = 1;

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.display();

  currentTime = millis();

  pinMode(A0, INPUT);
  randomSeed(analogRead(A0));
}

void loop() {
  display.clearDisplay();
  currentTime = millis();
  HandleInput();

  if (playing){
    DrawScore();
    if (currentTime - startTime >= gameSpeed){
      if (playing){
        UpdateGame();
        UpdateGrid();          
      }  
      startTime = currentTime;
    }    
    if(playing)
      DrawGrid(); 
  }
  else{
    DrawStartImage();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(18, 20);
    display.println("SNAKE");
    display.setTextSize(1);
    display.setCursor(20, 45);
    display.println("Press to Start");        

    if(currentTime - startTime >= textTime){
      if(switchStartImage < 100)
        switchStartImage++;
      else
        switchStartImage = 1;
      startTime = currentTime;
    }
  
    if(LButton.getButtonDown()||RButton.getButtonDown()||UpButton.getButtonDown()||DownButton.getButtonDown())
      StartGame();
  }

  display.display();
  delay(10);
}

void HandleInput(){
  LButton.updateButton();
  RButton.updateButton();
  UpButton.updateButton();
  DownButton.updateButton();
  
  if(LButton.getButtonDown() == true && dir != 2)
    dir = 4;
  if(UpButton.getButtonDown() == true && dir != 3)
    dir = 1;
  if(RButton.getButtonDown() == true && dir != 4)
    dir = 2;
  if(DownButton.getButtonDown() == true && dir != 1)
    dir = 3;
}

void UpdateGame(){
  int newX = playerX;
  int newY = playerY;
  switch(dir){
  case 1:
    newY--;
    break;
  case 2:
    newX++;
    break;
    case 3:
    newY++;
    break;
  case 4:
    newX--;
    break;
  }

  //check for hit self
  if (grid[newX][newY] > 0){
    EndGame();   
  }
  //check for out of bounds
  if (newX < 0){
    EndGame();   
  }
  else if(newX > gridX-1){
    EndGame();   
  }
  else if (newY < 0){
    EndGame();   
  }
  else if(newY > gridY-1){
    EndGame();   
  }

  //check for apple collision
  if (grid[newX][newY] == -1){
    score+=10;
    snakeLength++;
    NewApplePos();
  }
  
  playerX = newX;
  playerY = newY;
  grid[playerX][playerY] = 1;  

  grid[appleX][appleY] = -1;
}

void UpdateGrid(){
  for(int x = 0; x < gridX; x++){
    for (int y = 0; y < gridY; y++){
      if (grid[x][y] > 0)
        grid[x][y] = grid[x][y]+1;
      if (grid[x][y] > snakeLength+1)
         grid[x][y] = 0;
    }
  }
}

void DrawGrid(){
  display.drawRect(gridOffsetX-2, gridOffsetY-2, (gridX * gridSize)+2, (gridY * gridSize)+2, WHITE);
  for(int x = 0; x < gridX; x++){
    for (int y = 0; y < gridY; y++){
      int xPos = (x * gridSize) + gridOffsetX;
      int yPos = (y * gridSize) + gridOffsetY;
      if (grid[x][y] == -1)
        display.drawRect(xPos+1, yPos+1, 3, 3, WHITE);
      else if(grid[x][y] > 0)
        display.fillRect(xPos+1, yPos+1, 3, 3, WHITE);
    }
  }
}

void ResetGrid(){
  for(int x = 0; x < gridX; x++){
    for (int y = 0; y < gridY; y++){
      grid[x][y] = 0;  
    }
  }
}

void DrawScore(){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(gridOffsetX, 0);
  display.println("Score: " + String(score));                
}

void DrawStartImage(){
  for (int part = 1; part < startImageLength; part++){
    if((switchStartImage+part)%2 == 0){
      display.fillRect(startImageXPos+(part*startImageWidth),startImageYPos,startImageWidth,startImageHeight,WHITE);
    }
    else{
      display.fillRect(startImageXPos+(part*startImageWidth),startImageYPos+startImageHeight,startImageWidth,startImageHeight,WHITE);      
    }
  }
}

void StartGame(){
  ResetGrid();
  score = 0;
  playerX = 2;
  playerY = 4;
  appleX = 8;
  appleY = 4;
  dir = 2;
  snakeLength = startSnakeLength;
  playing = true;  
}

void EndGame(){
  playing = false;      
}

void NewApplePos(){
  grid[appleX][appleY] = 1;
  appleX = random(0,gridX);
  appleY = random(0,gridY);
  if(grid[appleX][appleY] > 0){
    NewApplePos();
  }
}
