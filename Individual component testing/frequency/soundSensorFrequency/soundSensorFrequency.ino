#include "arduinoFFT.h"

#define SAMPLES 128
#define SAMPLING_FREQUENCY 2048

arduinoFFT FFT = arduinoFFT();

unsigned int samplingPeriod;
unsigned long microSeconds;

double vReal[SAMPLES];
double vImag[SAMPLES];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY));

}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0; i<SAMPLES; i++) {
    microSeconds = micros();

    vReal[i] = analogRead(0);
    vImag[i] = 0;

  }

  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

  double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
  Serial.println(peak);

  //while(1);
  

}
