#include <Adafruit_NeoPixel.h>

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);

char temp;
int rainbowChange = 255;
int bright = 255;
int index;
char ch;
char volumeChar;
char color; 
uint16_t numPixels = strip.numPixels();
uint16_t currentOnPixels;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  if(Serial.available()) {
    ch = Serial.read();
    Mode(ch, numPixels, strip.Color(0, 0, 0), strip.Color(255, 0, 0), strip.Color(0, 255, 0), strip.Color(0, 0, 255), strip.Color(255, 255, 0), strip.Color(127, 127, 127));
  } 
}

void volumeUpDown(char volumeChar) {
  if(volumeChar == 'u' && numPixels < strip.numPixels()) {
    numPixels += 4;
    if(temp == 'r') {
      colorWipe(strip.Color(255, 0, 0), 1, numPixels);
    }
    else if(temp =='b') {    
      colorWipe(strip.Color(0, 0, 255), 1, numPixels);
    }
  }
  else if(volumeChar == 'd' && numPixels-6 > 0) {
    numPixels -= 4;
    colorDelete(strip.Color(0, 0, 0), 1, numPixels);         
  }
}

void brightnessUpDown(char brtChar) {
  if(brtChar == 'u' && bright <= 225) {
    bright+=30;
    colorWipe(strip.Color(0, bright, 0), 1, numPixels);
  }
  else if(brtChar == 'd' && bright > 40) {
    bright -= 30;
    colorWipe(strip.Color(0, bright, 0), 1, numPixels);  
  }
}

void rainbowUpDown(char rainbowChar) {
  if(rainbowChar == 'u') {
    rainbowChange += 20;
    rainbowChange %= 255;
    rainbow(2, numPixels, rainbowChange);        
  }
  else if(rainbowChar == 'd' && rainbowChange > 25) {
    rainbowChange -= 20;
    rainbowChange %= 255;
    rainbow(2, numPixels, rainbowChange);  
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait, uint16_t numPixels) {
  for(uint16_t i=0; i<=numPixels; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void colorDelete(uint32_t c, uint8_t wait, uint16_t numPixels) {
  for(uint16_t i=strip.numPixels(); i>numPixels; i--) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait, uint16_t numPixels,uint16_t rainbowChange) {
  uint16_t i;
  for(i=0; i<=numPixels; i++) {
    strip.setPixelColor(i, Wheel((i+rainbowChange) & 255));
  }
  strip.show();
  delay(wait);
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait, uint16_t numPixels) {
  for (int rainbowChange=0; rainbowChange<10; rainbowChange++) {  
    for (int q=0; q < 3; q++) {
      for (int i=0; i <= numPixels; i=i+3) {
        strip.setPixelColor(i+q, c);  
      }
      strip.show();

      delay(wait);

      for (int i=0; i <= numPixels; i=i+3) {
        strip.setPixelColor(i+q, 0); 
      }
    }
  }
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void Mode(char ch, uint16_t numPixels, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f) {
  if(ch == 'a') { 
    colorWipe(a, 15, numPixels); //OFF
  }
  else if(ch == 'r') { 
    colorWipe(b, 15, numPixels); // Red
    temp = 'r';
    while(Serial.available() <=0); 
    volumeChar = Serial.read(); 
    volumeUpDown(volumeChar);
  }
  else if(ch == 'g') {
    colorWipe(strip.Color(0, bright, 0), 15, numPixels); // Green
    while(Serial.available() <=0); 
    volumeChar = Serial.read();
    brightnessUpDown(volumeChar);
  }
  else if(ch == 'b') {
    colorWipe(d, 15, numPixels); // Blue
    temp = 'b';
    while(Serial.available() <=0);  
    volumeChar = Serial.read(); 
    volumeUpDown(volumeChar);
  }
  
  else if(ch == 'x') {  //rainbow
    while(Serial.available() <=0); 
    volumeChar = Serial.read(); 
    rainbowUpDown(volumeChar);
  }
  else if(ch == 'y') { 
    theaterChase(f, 15, numPixels); //Starlight
  }
}
