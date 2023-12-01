#define AIN1 5
#define AIN2 6
#define BIN1 10
#define BIN2 9

void setup() {
  Serial.begin(9600);
  
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  
}
 
void loop() {
  for (int i = 0; i <= 255; i ++){
  analogWrite(AIN1,i); 
  analogWrite(AIN2,0);
  analogWrite(BIN1,0); 
  analogWrite(BIN2,i);
  delay(10);

}

for (int i = 255; i >= 0; i--){
  analogWrite(AIN1,i); 
  analogWrite(AIN2,0);
  analogWrite(BIN1,0); 
  analogWrite(BIN2,i);
  delay(10);
}

  analogWrite(AIN1,0); 
  analogWrite(AIN2,0);
  analogWrite(BIN1,0); 
  analogWrite(BIN2,0);
  delay(1000);

for (int i = 0; i <= 255; i ++){
  analogWrite(AIN1,0); 
  analogWrite(AIN2,i);
  analogWrite(BIN1,i); 
  analogWrite(BIN2,0);
  delay(10);
}

for (int i = 255; i >=0; i--){
  analogWrite(AIN1,0); 
  analogWrite(AIN2,i);
  analogWrite(BIN1,i); 
  analogWrite(BIN2,0);
  delay(10);
}
  
}
