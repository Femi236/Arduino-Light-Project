#include "arduinoFFT.h"

#define SAMPLES 128
#define SAMPLING_FREQUENCY 2048

/////////////////////////////////////////VOLUME/////////////////////////////////////////

const int OUT_PIN = 8;
const int SAMPLE_TIME = 50;
const int soundSensor = 8;
unsigned long millisCurrent;
unsigned long millisLast = 0;
unsigned long millisElapsed = 0;

int sampleBufferValue = 0;
int savedSampleBufferValue = sampleBufferValue;

/////////////////////////////////////////FREQUENCY/////////////////////////////////////////

arduinoFFT FFT = arduinoFFT();

unsigned int samplingPeriod;
unsigned long microSeconds;

double vReal[SAMPLES];
double vImag[SAMPLES];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY));
  pinMode(soundSensor, INPUT);

}

void loop() {
  //frequency();
  volume();

}

void frequency() {
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
}

void volume() {
  millisCurrent = millis();
  millisElapsed = millisCurrent - millisLast;

//Serial.println(digitalRead(soundSensor));
  if (digitalRead(soundSensor) == HIGH) {
    sampleBufferValue++;
  }

  if (millisElapsed > SAMPLE_TIME) {
    savedSampleBufferValue = sampleBufferValue;
    Serial.println(savedSampleBufferValue);
    sampleBufferValue = 0;
    millisLast = millisCurrent;
  }
  //Serial.println(savedSampleBufferValue);
}

void volume2() {
  millisCurrent = millis();
  millisElapsed = millisCurrent - millisLast;

//Serial.println(digitalRead(soundSensor));
  if (digitalRead(soundSensor) == HIGH) {
    sampleBufferValue++;
  }

  if (millisElapsed > SAMPLE_TIME) {
    Serial.println(sampleBufferValue);
    sampleBufferValue = 0;
    millisLast = millisCurrent;
  }
}
