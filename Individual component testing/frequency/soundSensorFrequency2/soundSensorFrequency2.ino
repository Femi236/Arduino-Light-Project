#include "arduinoFFT.h"

#define SAMPLES 128
#define SAMPLING_FREQUENCY 1024

arduinoFFT FFT = arduinoFFT();

unsigned int samplingPeriod;
unsigned long microSeconds;

double vReal[SAMPLES];
double vImag[SAMPLES];

const int soundSensor = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY));
  pinMode(soundSensor, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0; i<SAMPLES; i++) {
    microSeconds = micros();

    vReal[i] = analogRead(0);
    vImag[i] = 0;

    while(micros() < (microSeconds + samplingPeriod)) {
      //Serial.println(0);
    }

  }

  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

  double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
//  if(digitalRead(soundSensor) == LOW) {
//    Serial.println(peak);
//  }

  Serial.println(digitalRead(soundSensor));
  
  //Serial.println(peak);
  
  //while(1);
  

}
