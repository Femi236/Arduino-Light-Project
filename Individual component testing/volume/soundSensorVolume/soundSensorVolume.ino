const int OUT_PIN = 8;
const int SAMPLE_TIME = 200;
const int soundSensor = 8;
unsigned long millisCurrent;
unsigned long millisLast = 0;
unsigned long millisElapsed = 0;

int sampleBufferValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(soundSensor, INPUT);
}

void loop() {

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
