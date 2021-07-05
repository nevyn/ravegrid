#include <Adafruit_NeoPixel.h>

int ledPin = 5;
int ledCount = 45;

Adafruit_NeoPixel strip(ledCount, ledPin, NEO_GRB + NEO_KHZ800);



void setup() 
{
  Serial.begin(9600);           //Serial monitor used to determine limit values
  
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.setBrightness(150); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip.show();            // Turn OFF all pixels ASAP
}

int getWait()
{
  int pinValue = analogRead(A2) % 255;   //Read the analogue input
  if(pinValue > 100)
    return 100;
  return pinValue;
}

void loop() {
  int wait = 0;
  int which = analogRead(A2)/255;
  if(which == 0) {
    rainbow(wait);
  } else if(which == 1) {
    Serial.println("cycle");               //Display the read value in the Serial monitor
    rainbowCycle(wait);
  } else if(which == 2) {
    Serial.println("chase");               //Display the read value in the Serial monitor
    theaterChaseRainbow(wait);
  } else {
    Serial.println("random");               //Display the read value in the Serial monitor
    long randNumberR;  //RED random number variable
    long randNumberG;  //GREEN random number variable
    long randNumberB;  //BLUE random number variable
  
    randNumberR = random(0, 128);   //Random RED value
    randNumberG = random(0, 128);   //Random GREEN value
    randNumberB = random(0, 128);   //Random BLUE value
    colorWipe(strip.Color(randNumberR, randNumberG, randNumberB), wait); // Red
  }
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(getWait());
    if(analogRead(A2)/255 != 3) return;
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(getWait());
    if(analogRead(A2)/255 != 0) return;
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(getWait());
    if(analogRead(A2)/255 != 1) return;
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(getWait());

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(getWait());
      if(analogRead(A2)/255 != 2) return;

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
