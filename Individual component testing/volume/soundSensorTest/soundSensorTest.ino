int soundSensor = 2;
int LED = 3;

void setup() {
  // put your setup code here, to run once:

  pinMode(soundSensor, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int statusSensor = digitalRead(soundSensor);
  Serial.println(statusSensor);

  if(statusSensor == 1) {
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
  }

}
