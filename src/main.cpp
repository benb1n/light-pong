#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ezButton.h>
#include <SPI.h>
#include <Wire.h>
#include "LightPongDisplay.h"

#define LED_PIN 5
#define LED_COUNT 57 // How many NeoPixels are attached to the Arduino?
#define DEBOUNCE_TIME 50
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int Forward_Button_Pin = 4;
int Backward_Button_Pin = 2;

ezButton Forward_Button(Forward_Button_Pin);
ezButton Backward_Button(Backward_Button_Pin);

int Yellow_Score = 0;
int Red_Score = 0;

int CurrentLight = 0;
int speedOfReturn = 50;
int Speed_Of_Pong = 30;

bool Yellow_Won = false;
bool Red_Won = false;
bool Is_Beginning = true;
bool Moving_Forward = true;

int Game_Mode = 1;

// 0 is game is starting again
// 1 is game is in motion
// 2 is the game is over and one side has won

uint32_t Red = strip.Color(255, 0, 0);
uint32_t Yellow = strip.Color(255, 110, 0);
uint32_t Black = strip.Color(0, 0, 0);

int LightBefore = CurrentLight - 1;
int LightAfter = CurrentLight + 1;

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void reverseColorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=57; i>strip.numPixels(); i--) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("Whats up");

  initDisplay();

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  
  Forward_Button.setDebounceTime(DEBOUNCE_TIME);



    CurrentLight = 0;
    LightBefore = CurrentLight - 1;
    LightAfter = CurrentLight + 1;


    Moving_Forward = true;
    Red_Won = false;
    Yellow_Won = false;
}


void loop()
{
    

  if (Game_Mode == 0 && Yellow_Won == false && Red_Won == true)
  {
    //reverseColorWipe((255, 0, 0), 100);
    //strip.show();
    delay(3000);

    CurrentLight = 0;
    LightBefore = CurrentLight - 1;
    LightAfter = CurrentLight + 1;
    Red_Score++;

    Speed_Of_Pong = 30;

    Moving_Forward = true;
    Red_Won = false;
    Yellow_Won = false;

    Game_Mode = 1;
  } 
  else if (Game_Mode == 0 && Yellow_Won == true && Red_Won == false)
  {
    //colorWipe((255, 105, 0), 100);
    //strip.show();
    delay(3000);

    CurrentLight = 57;
    LightBefore = CurrentLight - 1;
    LightAfter = CurrentLight + 1;
    Yellow_Score++;

    Speed_Of_Pong = 30;

    Moving_Forward = false;
    Yellow_Won = false;
    Red_Won = false;

    Game_Mode = 1;
  }

  Forward_Button.loop();
  Backward_Button.loop();

  if (Moving_Forward == true)
  {

    if (Game_Mode == 1)
    {
      strip.setPixelColor(CurrentLight, Red);
      strip.show();
    }
    else
    {
      strip.setPixelColor(CurrentLight, Black);
      strip.show();
    }

    CurrentLight++;
    LightAfter++;
    LightBefore++;
    delay(Speed_Of_Pong);

    strip.setPixelColor(LightAfter, Black); 
    strip.setPixelColor(LightBefore, Black);  
    strip.show();
    
    if (Backward_Button.isPressed() && CurrentLight > 52 && CurrentLight < 58)
    {
      Moving_Forward = false;

      switch (CurrentLight) {
        case 53:
        Speed_Of_Pong = Speed_Of_Pong * 2;
        break;
        case 54:
        Speed_Of_Pong = Speed_Of_Pong * 1.25;
        break;
        case 55:
        Speed_Of_Pong = Speed_Of_Pong * 0.8;
        break;
        case 56:
        Speed_Of_Pong = Speed_Of_Pong * 0.5;
        break;
        case 57:
        Speed_Of_Pong = Speed_Of_Pong * 0.3;
        break;
      }
    }
    else if (CurrentLight > 57)
    {
      Yellow_Won = false;
      Red_Won = true;

      Game_Mode = 0;

      strip.setPixelColor(CurrentLight, Black);
      strip.show();
    }
    else if (Backward_Button.isPressed() && CurrentLight < 53 && CurrentLight > 28)
    {
      Yellow_Won = false;
      Red_Won = true;

      Game_Mode = 0;
      strip.setPixelColor(CurrentLight, Black);
      strip.show();
    }
  }
  else
  {

    if (Game_Mode == 1)
    {
      strip.setPixelColor(CurrentLight, Yellow);
      strip.show();
    }
    else
    {
      strip.setPixelColor(CurrentLight, Black);
      strip.show();
    } 

    CurrentLight--;
    LightAfter--;
    LightBefore--;
    delay(Speed_Of_Pong);

    strip.setPixelColor(LightAfter, Black); 
    strip.setPixelColor(LightBefore, Black);  
    strip.show();
    
    if (Forward_Button.isPressed() && CurrentLight > -1 && CurrentLight < 5)
    {
      Moving_Forward = true;

      switch (CurrentLight) {
      case 4:
      Speed_Of_Pong = Speed_Of_Pong * 2.5;
      break;
      case 3:
      Speed_Of_Pong = Speed_Of_Pong * 1.5;
      break;
      case 2:
      Speed_Of_Pong = Speed_Of_Pong * 0.9;
      break;
      case 1:
      Speed_Of_Pong = Speed_Of_Pong * 0.66;
      break;
      case 0:
      Speed_Of_Pong = Speed_Of_Pong * 0.33;
      break;
      }
    }
    else if (CurrentLight < -1)
    {
      Yellow_Won = true;
      Red_Won = false;

      Game_Mode = 0;
      strip.setPixelColor(CurrentLight, Black);
      strip.show();
    }
    else if (Forward_Button.isPressed() && CurrentLight < 27 && CurrentLight > 4)
    {
      Yellow_Won = true;
      Red_Won = false;

      Game_Mode = 0;
      strip.setPixelColor(CurrentLight, Black);
      strip.show();
    }
  }
}

