/*****************************************************************************************
 * Fifth version of ligh project
 * Code cleanup from previous
 * Uses analog input
 * Has IR remote functionality
 *****************************************************************************************/

#include "FastLED.h"
#include "arduinoFFT.h"
#include <IRremote.h>

#define BRIGHTNESS          16

#define NUM_LEDS 180        // How many leds in your strip?
#define updateLEDS 3        // How many do you want to update every millisecond?
CRGB leds[NUM_LEDS];        // Define the array of leds

// Define the digital I/O PINS..
#define DATA_PIN 13          // led data transfer

//////////////////////////////////////////FUNCTIONALITY METHOD VARIABLES//////////////////////////////////////////

int currBrightness = 15;
int deviationThresh = 5;

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


int soundPin = A0;
int sensorValue = 0;
int initialSensorValue = 0;
int baseSensorValue = 0;
int samples = 20;

//////////////////////////////////////////SETUP//////////////////////////////////////////

int RECV_PIN = 12; // Infrared receiving pin
IRrecv irrecv(RECV_PIN); // Create a class object used to receive class
decode_results results; // Create a decoding results class object


void setup() { 
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  pinMode(soundSensor, INPUT);
  for(int i = 0; i < NUM_LEDS ; i++) {
    leds[i] = CRGB(0,0,0);
  }
  FastLED.show();

  FastLED.setBrightness(BRIGHTNESS);

  for(int i = 0; i < samples; i++) {
    initialSensorValue += analogRead (soundPin);
  }
  baseSensorValue = initialSensorValue/samples;

  irrecv.enableIRIn();
}


//////////////////////////////////////////PATTERN LIST//////////////////////////////////////////

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { fill_grad, sinelon, rainbow, bpm, juggle, soundLight };
//SimplePatternList gPatterns = { soundLight };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0;

//////////////////////////////////////////LOOP//////////////////////////////////////////

unsigned long t_wait = 0;

void loop() { 

  //soundLight();
  //sinelon();

  if (irrecv.decode(&results)) { // Waiting for decoding
    //Serial.println(results.value, HEX); // Print out the decoded results

    irrecv.resume();

    t_wait = millis();

    Serial.println("Waiting");
    while(!irrecv.decode(&results) && millis() - t_wait < 1000) {
      
    }
    Serial.println("Done");
    Serial.println(results.value, HEX); // Print out the decoded results
    handleControl(results.value); // Handle the commands from remote control
    delay(200);
    irrecv.resume(); // Receive the next value
    
    
    
  }
  
  
  gPatterns[gCurrentPatternNumber]();
  
  FastLED.show();

  //printColor(nc);
  delay(10);

  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  //EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
    
}


void handleControl(unsigned long value) {
  switch(value) {
    //Change mode
    case 0xFF30CF:
      setFillGrad();
      break;
    case 0xFF18E7:
      setSinelon();
      break;
    case 0xFF7A85:
      setRainbow();
      break;
    case 0xFF10EF:
      setBpm();
      break;
    case 0xFF38C7:
      setJuggle();
      break;
    case 0xFF5AA5:
      setSoundLight();
      break;

      
    //Change brightness
    case 0xFF02FD:
      increaseBrightness();
      break;
    case 0xFF9867:
      decreaseBrightness();
      break;
    //Change sound sensitivity
    case 0xFF906F:
      increaseSensitivity();
      break;
    case 0xFFE01F:
      decreaseSensitivity();
      break;
    //Reset base sound
    case 0xFF22DD:
      resetBaseSound();
      break;
  }
  
}

//////////////////////////////////////////FUNCTIONALITY METHODS//////////////////////////////////////////

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void setFillGrad() {
  gCurrentPatternNumber = 0;
}

void setSinelon() {
  gCurrentPatternNumber = 1;
}

void setRainbow() {
  gCurrentPatternNumber = 2;
}

void setBpm() {
  gCurrentPatternNumber = 3;
}

void setJuggle() {
  gCurrentPatternNumber = 4;
}

void setSoundLight() {
  gCurrentPatternNumber = 5;
}


void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  if (gCurrentPatternNumber == 2){
    resetBaseSound();
  }
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void increaseBrightness() {
  if(currBrightness <= 250){
    currBrightness += 5;
    FastLED.setBrightness(currBrightness);
  }
}

void decreaseBrightness() {
  if(currBrightness >=5){
    currBrightness -= 5;
    FastLED.setBrightness(currBrightness);
  }
}

void increaseSensitivity() {
  if(deviationThresh < 20) {
    deviationThresh += 2;
  }
}

void decreaseSensitivity() {
  if(deviationThresh > 0){
    deviationThresh -= 2;
  }
}

void resetBaseSound() {
  initialSensorValue = 0;
  for(int i = 0; i < samples; i++) {
    initialSensorValue += analogRead (soundPin);
  }
  baseSensorValue = initialSensorValue/samples;
}

//////////////////////////////////////////MODES//////////////////////////////////////////

void soundLight() {
  unsigned long time = millis();

  // Shift all LEDs to the right by updateLEDS number each time
  for(int i = NUM_LEDS - 1; i >= updateLEDS; i--) {
    leds[i] = leds[i - 1];
  }

  // Get the pitch and brightness to compute the new color
  int vol = volume();
  Color nc;
  setColor(&nc, 0, 0, 100);

  if(vol > deviationThresh) {
    setColor(&nc, 100, 0, 0);
    ///////////////////////////////////////Serial.println (vol);
  }
    
  // Set the left most updateLEDs with the new color
  for(int i = 0; i < updateLEDS; i++) {
    leds[i] = CRGB(nc.r, nc.g, nc.b);
  }
}


int volume() {
  millisCurrent = millis();
  millisElapsed = millisCurrent - millisLast;
  
  while(millisElapsed < SAMPLE_TIME) {
    millisCurrent = millis();
    millisElapsed = millisCurrent - millisLast;
    if(analogRead(soundPin) - baseSensorValue > 2 || analogRead(soundPin) - baseSensorValue < -2) {
      sampleBufferValue++;
    }
  }

  ////////////////////////////////////Serial.println(sampleBufferValue);
    
  savedSampleBufferValue = sampleBufferValue;
  sampleBufferValue = 0;
  millisLast = millisCurrent;
  
  return savedSampleBufferValue;
}
  

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
  leds[pos] += CHSV( 0, 0, 192);
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void fill_grad() {
  uint8_t starthue = beatsin8(5, 0, 255);
  uint8_t endhue = beatsin8(7, 0, 255);
  
  if (starthue < endhue) {
    fill_gradient(leds, NUM_LEDS, CHSV(starthue,255,255), CHSV(endhue,255,255), FORWARD_HUES);    // If we don't have this, the colour fill will flip around. 
  } else {
    fill_gradient(leds, NUM_LEDS, CHSV(starthue,255,255), CHSV(endhue,255,255), BACKWARD_HUES);
  }
  
}


//////////////////////////////////////////HELPER METHODS//////////////////////////////////////////



void setColor(Color *c, int r, int g, int b) {
  c->r = r;
  c->g = g;
  c->b = b;
}
