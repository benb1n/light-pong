#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ezButton.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN    5

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 57

#define DEBOUNCE_TIME 50

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int Forward_Button_Pin = 4;//15;
int Backward_Button_Pin = 2;
ezButton Forward_Button(Forward_Button_Pin);
ezButton Backward_Button(Backward_Button_Pin);
int speedOfReturn = 50;
int CurrentLight = 0;
bool Moving_Forward = true;
bool Is_Beginning = true;
int Speed_Of_Pong = 50;

uint32_t Red = strip.Color(255, 0, 0);
uint32_t Black = strip.Color(0, 0, 0);

int LightBefore = CurrentLight - 1;
int LightAfter = CurrentLight + 1;



// void colorWipe(uint32_t color, uint32_t colorOfBlack, int wait) {
//   for(;CurrentLight<strip.numPixels(); CurrentLight++) { // For each pixel in strip...
//     strip.setPixelColor(CurrentLight, color); 
//     strip.setPixelColor(CurrentLight-1, colorOfBlack);
//     strip.show();                          //  Update strip to match
//     delay(wait);                           //  Pause for a moment
//   }
// }


// void colorWipeReverse(uint32_t color, uint32_t colorOfBlack, int wait) {
//   for(;CurrentLight>0; CurrentLight--) { // For each pixel in strip...
//     strip.setPixelColor(CurrentLight, color);     
//     strip.setPixelColor(CurrentLight+1, colorOfBlack);    //  Set pixel's color (in RAM)
//     strip.show();                          //  Update strip to match
//     delay(wait);         
//   }
// }

// void IRAM_ATTR yurMom()
// {
//   Test_Button = true;
// }

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("Whats up");

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
 // pinMode(pushButton_pin, INPUT_PULLUP);
  //attachInterrupt(pushButton_pin, yurMom, FALLING);

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
    Serial.println(Forward_Button.isPressed());
    if (Backward_Button.isPressed())
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
    Serial.println(Forward_Button.isPressed());
    if (Forward_Button.isPressed())
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

// void old_dry_bones_loop() {



//   if (Is_Beginning == true)
//   {
//    colorWipe(strip.Color(255,   0,   0), strip.Color(0, 0, 0), speedOfReturn);
//   }
//   Is_Beginning = false;

//   if (Test_Button == true)
//   {
//     Test_Button = false;
//     if (Moving_Forward == true)
//     {
//       colorWipeReverse(strip.Color(255,   0,   0), strip.Color(0, 0, 0), speedOfReturn); // Red
//       Moving_Forward = false;
//     }
//     else
//     {
//       colorWipe(strip.Color(255,   0,   0), strip.Color(0, 0, 0), speedOfReturn);
//       Moving_Forward = true;
//     }
//   }


//   // Fill along the length of the strip in various colors...
//   //colorWipe(strip.Color(255,   0,   0), strip.Color(0, 0, 0), speedOfReturn); // Red
//   //speedOfReturn = speedOfReturn / 2;
//   //colorWipeReverse(strip.Color(255,   0,   0), strip.Color(0, 0, 0), speedOfReturn); // Red

  
  
//   // colorWipe(strip.Color(0,   0,   0), 50); // Red
//   // colorWipeReverse(strip.Color(255,   0,   0), 50); // Red
//   // colorWipeReverse(strip.Color(0,   0,   0), 50); // Red
// }
   



