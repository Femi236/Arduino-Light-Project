
//defines
#define WINDOW 0
#define LIN_OUT 1
#define FFT_N 128 // set to x point fft

// Define various ADC prescaler (slight loss in precision)
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

int ADCexecutionTime;
int sampleRate;
int sampleDelay;
int outMultiplier;
//includes
#include "arduinoFFT.h"

void setup()
{
  Serial.begin(115200);
  //Serial.println("\n=====Program Starting=====\n");

  // set up the ADC
  ADCSRA &= ~PS_128;  // remove bits set by Arduino library
  ADCSRA |= PS_32;    // set our own prescaler to 32
  ADCexecutionTime = 30; //about how long a sample takes with prescaler 32
 
  sampleRate = 10000; //can measure 0 through sampleRate/2 hertz
  sampleDelay = (1000000L / (long)sampleRate) - ADCexecutionTime;
  //outMultiplier = (sampleRate >> 1) / (FFT_N >> 1); //frequency range of each bin
}
int high = 0, temp = 0, f = 0;
int t = 0;
void loop()
{
  //start sampling
  for(int i = 0; i < FFT_N; i++)
  {
    fft_input[i << 1] = analogRead(A0) - 512;
    fft_input[(i << 1) + 1] = 0;
    delayMicroseconds(sampleDelay);
  }

  //these 2 functions take 2.8ms to complete with FFT_N = 128
  fft_reorder(); // reorder the data before doing the fft
  fft_run(); // process the data in the fft

  //send data through serial for processing to graph it
  high = 0;
  Serial.write(255); //im using this as the start character
  for(int i = 0; i < (FFT_N >> 1); i++)
  {
    high = ((unsigned int)sq(fft_input[i << 1]) + (unsigned int)sq(fft_input[(i << 1) + 1])) >> 1; //the bin value really = sqrt(sq(re) + sq(im)) but this is faster and is still easy to read
    if(high > 254)high = 254;
    Serial.write((byte)high); //send to processing to view it
  }
}
