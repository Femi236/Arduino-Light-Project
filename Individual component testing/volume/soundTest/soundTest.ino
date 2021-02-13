/**********************************************************************************************************
 * Testing whether I get better sound values using analogRead
 **********************************************************************************************************/



int soundPin = A0;
int sensorValue = 0;
int initialSensorValue = 0;
int baseSensorValue = 0;
int samples = 20;


void setup () 
{
Serial.begin (9600);
for(int i = 0; i < samples; i++) {
  initialSensorValue += analogRead (soundPin);
}
baseSensorValue = initialSensorValue/samples;
}
 
void loop () 
{
sensorValue = analogRead (soundPin);
if(sensorValue - baseSensorValue > 2 || sensorValue - baseSensorValue < -2) {
  Serial.println (sensorValue);
}

delay (10);
}
