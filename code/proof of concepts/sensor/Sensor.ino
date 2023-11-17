const int sensor[] = {A0, A1, A2, A3, A4, A5};

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);
  Serial.print("Values ");
  for(int i = 0; i<6; i++)
  {
    Serial.print(analogRead(sensor[i]));
    Serial.print(" ");
  }
  Serial.println();
  delay(1000);
}
