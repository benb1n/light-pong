#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ezButton.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSansBold9pt7b.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN    5

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 57

#define DEBOUNCE_TIME 50

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
int Speed_Of_Pong = 30;

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

void initDisplay() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Welcome to Light Pong");
  display.display();
  delay(1000);

  display.setFont(&FreeSansBold9pt7b);
  display.setTextColor(WHITE);
  display.setCursor(0, 40);
  display.println("Ready player 1");
  display.display();
}

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

  //initDisplay();

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
    Serial.println(Forward_Button.isPressed());
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
   



