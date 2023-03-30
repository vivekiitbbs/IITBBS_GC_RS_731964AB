#include <TFT.h> // Hardware-specific library
#include <SPI.h>
#include <Wire.h>

#include <Adafruit_Sensor.h> 

#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
#define CS   10
#define DC   9
#define RESET  8
#define BLACK 0x0000
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF

#define WHITE 0xFFFF
// pin definition for the Leonardo
// #define CS   7
// #define DC   0
// #define RESET  1

TFT myScreen = TFT(CS, DC, RESET);
using namespace std;
//#include "olcConsoleGameEngine.h"
  int fPlayerPosX = 12;
  int fPlayerPosY = 27;
  int fPlayerVelX = 0;
  int fPlayerVelY = 0;
  bool bPlayerOnGround = false;
  float time1=0,ttime=0;
  String sLevel[30] = {"B...............................B",
  "B..............................B",
  "B.....BBBBBBB..................B",
  "B..............................B",
  "B.........................BBBBBB",
  "B###...........................B",
  "B..............#######.........B",
  "B..............................B",
  "BBBBBBB........................B",
  "B.........####..............###B",
  "B..............................B",
  "B..............BBBBBBBB........B",
  "B..............................B",
  "B.......BBBB...................B",
  "B........................GGGGGGB",
  "B...............#####..........B",
  "B..............................B",
  "B......................########B",
  "B....BBBBBBBBBBBB..............B",
  "B..............................B",
  "B..............................B",
  "B..........GGGGGGGGGG..........B",
  "B....................GGG.......B",
  "B..............................B",
  "BBBBBBB.................#######B",
  "B..............................B",
  "BGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGB"};
    
void setup()
{myScreen.begin();

  myScreen.background(0,0,0);  // clear the screen with black
  Serial.begin(9600);  

  }
void loop()
{
 sensors_event_t event; 
  time1=time1 - millis()*1000;
   Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");

   Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");

   Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");
     if ((event.acceleration.y)>0 && bPlayerOnGround==true)
      {
        fPlayerVelY = -60;
      }

      if ((event.acceleration.y)<0)
      {
        fPlayerVelY = 6;
      }

      if ((event.acceleration.x)<0)
      {
        fPlayerVelX += ( -200) * time1;
      
      }

      if (event.acceleration.x>0)
      {
        fPlayerVelX += ( 200) * time1;
       
      }

      
 

    // Gravity
    fPlayerVelY += 1 ;

    // Drag
    if (bPlayerOnGround)
    {
      fPlayerVelX += -20 * fPlayerVelX ;
      if (fabs(fPlayerVelX) < 0.01f)
        fPlayerVelX = 0;
    }

    // Clamp velocities
    if (fPlayerVelX > 500)
      fPlayerVelX = 500;

    if (fPlayerVelX < -500)
      fPlayerVelX = -500;

    if (fPlayerVelY > 100)
      fPlayerVelY = 100;

    if (fPlayerVelY < -100)
      fPlayerVelY = -100;

    // Calculate potential new position
    int fNewPlayerPosX = fPlayerPosX + (fPlayerVelX );
    int fNewPlayerPosY = fPlayerPosY + (fPlayerVelY);
    
    
    // Check for Collision
    if (fPlayerVelX < 0) // Moving Left
    {
      if (((sLevel[fPlayerPosY])[fNewPlayerPosX ]) != '.' || ((sLevel[fPlayerPosY])[fNewPlayerPosX ]) != '.')
      {
        fNewPlayerPosX = (int)fNewPlayerPosX +1;
        fPlayerVelX = 0;
      }
    }
    else // Moving Right
    {
      if (((sLevel[fNewPlayerPosY])[fNewPlayerPosX + 1]) != '.' || ((sLevel[fNewPlayerPosY])[fNewPlayerPosX + 1]) != '.')
      {
        fNewPlayerPosX = (int)fNewPlayerPosX;
        fPlayerVelX = 0;

      }
    }

    bPlayerOnGround = false;
    if (fPlayerVelY < 0) // Moving Up
    {
      if (((sLevel[fNewPlayerPosY])[fNewPlayerPosX ]) != '.' || ((sLevel[fNewPlayerPosY])[fNewPlayerPosX ]) != '.')
      {
        fNewPlayerPosY = (int)fNewPlayerPosY +1;
        fPlayerVelY = 0;
      }
    }
    else // Moving Down
    {
      if (((sLevel[fNewPlayerPosY + 1])[fNewPlayerPosX ]) != '.' || ((sLevel[fNewPlayerPosY + 1])[fNewPlayerPosX ]) != '.')
      {
        fNewPlayerPosY = (int)fNewPlayerPosY;
        fPlayerVelY = 0;
        bPlayerOnGround = true; 
       
      }
    }

    // Apply new position
    fPlayerPosX = fNewPlayerPosX;
    fPlayerPosY = fNewPlayerPosY;
    // Draw Level
    
    
 
    //time calculation 
    /*if (fPlayerPosY == 1)
    {
      auto stop = high_resolution_clock::now();
      auto duration = duration_cast<milliseconds>(stop - start);
      cout << duration.count() << endl;
    }*/
    // Draw visible tile map
   
   
      for (int x = 0; x < 33; x++)
      {
        for (int y = 2; y <30; y++)
        {
          wchar_t sTileID = (sLevel[y])[x];
          switch (sTileID)
          {
          case '.': // Sky
           myScreen.fillRect(4*x,4*y,4,4,CYAN);
           // Fill(x * nTileWidth, (y)*nTileHeight, (x + 1) * nTileWidth, ((y)+1) * nTileHeight, PIXEL_SOLID, FG_CYAN);
            break;
          case '#': // Solid Block
            myScreen.fillRect(4*x,4*y,4,4,WHITE);
            //Fill(x * nTileWidth, (y)*nTileHeight, (x + 1) * nTileWidth, ((y)+1) * nTileHeight, PIXEL_SOLID, FG_WHITE);
            break;
          case 'G': // Ground Block
           myScreen.fillRect(4*x,4*y,4,4,WHITE);
           // Fill(x * nTileWidth, (y)*nTileHeight, (x + 1) * nTileWidth, ((y)+1) * nTileHeight, PIXEL_SOLID, FG_WHITE);
            break;
          case 'B': // Brick Block
            myScreen.fillRect(4*x,4*y,4,4,RED);
            //Fill(x * nTileWidth, (y)*nTileHeight, (x + 1) * nTileWidth, ((y)+1) * nTileHeight, PIXEL_SOLID, FG_RED);
            break;
          default:
            myScreen.fillRect(4*x,4*y,4,4,BLACK);
            //Fill(x * nTileWidth, (y)*nTileHeight, (x + 1) * nTileWidth, ((y)+1) * nTileHeight, PIXEL_SOLID, FG_BLACK);
            break;
          }
        }
        //myScreen.fillRect(4*5,4*10,6,,GREEN);
      }
      //Fill((fPlayerPosX), (fPlayerPosY), (fPlayerPosX + 1.0f), (fPlayerPosY +1.0f), PIXEL_SOLID, FG_GREEN);   
      myScreen.fillRect(fPlayerPosX,fPlayerPosY,6,6,BLACK);
      delay(500);
      
  } 
 
