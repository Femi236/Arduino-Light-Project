#include "FastLED.h"
#include "arduinoFFT.h"


#define NUM_LEDS 60        // How many leds in your strip?
#define updateLEDS 4        // How many do you want to update every millisecond?
#define COLOR_SHIFT 180000  // Time for colours to shift to a new spectrum (in ms)
CRGB leds[NUM_LEDS];        // Define the array of leds

// Define the digital I/O PINS..
#define DATA_PIN 13          // led data transfer
#define PITCH_PIN 0         // pitch input from frequency to voltage converter
#define BRIGHT_PIN 8        // brightness input from amplified audio signal

// Don't touch these, internal color variation variables
unsigned long setTime = COLOR_SHIFT;
int shiftC = 0;
int mulC = 2;

// Define color structure for rgb
struct color {
  int r;
  int g;
  int b;
};
typedef struct color Color;

/////////////////////////////////////////VOLUME/////////////////////////////////////////

const int OUT_PIN = 8;
const int SAMPLE_TIME = 50;
const int soundSensor = 8;
unsigned long millisCurrent;
unsigned long millisLast = 0;
unsigned long millisElapsed = 0;

int sampleBufferValue = 0;
int savedSampleBufferValue = sampleBufferValue;


void setup() { 
    Serial.begin(9600);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    pinMode(soundSensor, INPUT);
    for(int i = 0; i < NUM_LEDS ; i++) {
      leds[i] = CRGB(0,0,0);
    }
    FastLED.show();
}

void loop() { 

  

  unsigned long time = millis();
/*
  // Shift the color spectrum by 200 on set intervals (setTime)
  if(time / (double)setTime >= 1) {
    setTime = time + COLOR_SHIFT;
    //Serial.println(setTime);
    shiftC += 200;
    mulC++;
    if(shiftC >= 600) {
      shiftC = 0;
    }
    if(mulC > 3) {
      mulC = 2;
    }
  }
*/

 // Shift all LEDs to the right by updateLEDS number each time
  for(int i = NUM_LEDS - 1; i >= updateLEDS; i--) {
    leds[i] = leds[i - updateLEDS];
  }

//  
//
//  // Get the pitch and brightness to compute the new color
  int newPitch = (analogRead(PITCH_PIN));
//  //int currB = volume()/1500;//analogRead(BRIGHT_PIN)/4000;
//  
 Color nc = pitchConv(39, 1);

  
  
  millisCurrent = millis();
  millisElapsed = millisCurrent - millisLast;
  //Serial.println(digitalRead(soundSensor));
  if (digitalRead(soundSensor) == HIGH) {
    sampleBufferValue++;
  }

  if (millisElapsed > SAMPLE_TIME) {
    Serial.println(sampleBufferValue);

  if(sampleBufferValue >0){
  for(int i = NUM_LEDS - 1; i >= updateLEDS; i--) {
    leds[i] = CRGB(0,100,0);
  }
  
  }
    
    savedSampleBufferValue = sampleBufferValue;
    sampleBufferValue = 0;
    millisLast = millisCurrent;

  }

//  //Serial.println(savedSampleBufferValue);
//  nc = pitchConv(currPitch, savedSampleBufferValue);

  //printColor(nc);

  // Set the left most updateLEDs with the new color
  for(int i = 0; i < updateLEDS; i++) {
    leds[i] = CRGB(nc.r, nc.g, nc.b);
  }

  
  FastLED.show();

  //printColor(nc);
  delay(1);

    
  }
  



int volume() {
  millisCurrent = millis();
  millisElapsed = millisCurrent - millisLast;

//Serial.println(digitalRead(soundSensor));
  if (digitalRead(soundSensor) == HIGH) {
    sampleBufferValue++;
  }

  if (millisElapsed > SAMPLE_TIME) {
    savedSampleBufferValue = sampleBufferValue;
    //Serial.println(sampleBufferValue);
    sampleBufferValue = 0;
    millisLast = millisCurrent;
    return savedSampleBufferValue;
    
  }

  return savedSampleBufferValue;
}


/**
 * Converts the analog brightness reading into a percentage
 * 100% brightness is 614.. about 3 volts based on frequency to voltage converter circuit
 * The resulting percentage can simply be multiplied on the rgb values when setting our colors,
 * for example black is (0,0,0) so when volume is off we get 0v and all colors are black (leds are off)
 */
double convBrightness(int b) {
  //double c = b / 614.0000;
  if( b < 0 ) {
    return 0;
  }
//  else if(c > 1) {
//    c = 1.00;
//  }
  return b;
}

/**
 * Creates a new color from pitch and brightness readings
 * int p         analogRead(pitch) representing the voltage between 0 and 5 volts
 * double b      analogRead(brightness) representing volume of music for LED brightness
 * returns Color structure with rgb values, which appear synced to the music
 */
Color pitchConv(int p, int b) {
  Color c;
  double bright = convBrightness(b);

  //Serial.println(p);
  if(p < 40) {
    setColor(&c, 255, 0, 0);
  }
  else if(p >= 40 && p <= 77) {
    int b = (p - 40) * (255/37.0000);
    setColor(&c, 255, 0, b);
  }
  else if(p > 77 && p <= 205) {
    int r = 255 - ((p - 78) * 2);
    setColor(&c, r, 0, 255);
  }
  else if(p >= 206 && p <= 238) {
    int g = (p - 206) * (255/32.0000);
    setColor(&c, 0, g, 255);
  }
  else if(p <= 239 && p <= 250) {
    int r = (p - 239) * (255/11.0000);
    setColor(&c, r, 255, 255);
  }
  else if(p >= 251 && p <= 270) {
    setColor(&c, 255, 255, 255);
  }
  else if(p >= 271 && p <= 398) {
    int rb = 255-((p-271)*2);
    setColor(&c, rb, 255, rb);
  }
  else if(p >= 398 && p <= 653) {
    setColor(&c, 0, 255-(p-398), (p-398));
  }
  else {
    setColor(&c, 255, 0, 0);
  }
  setColor(&c, c.r * bright, c.g * bright, c.b * bright);
  return c;
}

void setColor(Color *c, int r, int g, int b) {
  c->r = r;
  c->g = g;
  c->b = b;
}

// Prints color structure data
void printColor(Color c) {
  Serial.print("( ");
  Serial.print(c.r);
  Serial.print(", ");
  Serial.print(c.g);
  Serial.print(", ");
  Serial.print(c.b);
  Serial.println(" )");
}
