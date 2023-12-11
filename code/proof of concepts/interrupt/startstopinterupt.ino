const byte interruptPin = 2;  // Pin waarop de schakelaar is aangeslotenvolatile bool isRobotRunning = trye;  // Variabele om de status van de robot bij te houden
volatile byte isRobotRunning = LOW;
int led = 4;

void setup() {
  pinMode(interruptPin, INPUT_PULLUP);  // Zet de schakelaar-pin als input met pull-up weerstand
  attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt, FALLING); // Stel een interrupt in voor de schakelaar op de rising edge
  pinMode(led, OUTPUT);
}

void loop() {
  if (isRobotRunning){
  blink();  
    }
 else{
  digitalWrite (led, LOW);
 }
}
void blink(){
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(1000); 
}

void interrupt() {
   static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) 
  {
    isRobotRunning = !isRobotRunning;
  }
  last_interrupt_time = interrupt_time;
  }