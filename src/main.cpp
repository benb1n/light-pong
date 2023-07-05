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

int CurrentLight = 0;
int speedOfReturn = 50;
int Speed_Of_Pong = 30;
bool Is_Beginning = true;
bool Moving_Forward = true;

uint32_t Red = strip.Color(255, 0, 0);
uint32_t Black = strip.Color(0, 0, 0);

int LightBefore = CurrentLight - 1;
int LightAfter = CurrentLight + 1;


void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("Whats up");

  initDisplay();

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  
  Forward_Button.setDebounceTime(DEBOUNCE_TIME);
}


void loop()
{
  Forward_Button.loop();
  Backward_Button.loop();

  if (Moving_Forward == true)
  {
    strip.setPixelColor(CurrentLight, Red);
    strip.setPixelColor(LightAfter, Black); 
    strip.setPixelColor(LightBefore, Black);  
    strip.show();
    
    CurrentLight++;
    LightAfter++;
    LightBefore++;
    delay(Speed_Of_Pong);
    
    if (Backward_Button.isPressed() && CurrentLight > 29)
    {
      Moving_Forward = false;
      switch (CurrentLight) {
        case 53:
        Speed_Of_Pong = Speed_Of_Pong * 2.5;
        break;
        case 54:
        Speed_Of_Pong = Speed_Of_Pong * 1.5;
        break;
        case 55:
        Speed_Of_Pong = Speed_Of_Pong * 0.9;
        break;
        case 56:
        Speed_Of_Pong = Speed_Of_Pong * 0.66;
        break;
        case 57:
        Speed_Of_Pong = Speed_Of_Pong * 0.33;
        break;
      }
    }
  }
  else
  {
    strip.setPixelColor(CurrentLight, Red); 
    strip.setPixelColor(LightAfter, Black); 
    strip.setPixelColor(LightBefore, Black);  
    strip.show();
    
    CurrentLight--;
    LightAfter--;
    LightBefore--;
    delay(Speed_Of_Pong);
    
    if (Forward_Button.isPressed() && CurrentLight < 28)
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
  }
}

