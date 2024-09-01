#include "SerialCommand.h"
#include "EEPROMAnything.h"

#define SerialGate Serial1
#define Baudrate 9600
#define leftForward 5
#define leftBackward 6
#define rightForward 9
#define rightBackward 10

SerialCommand sCmd(SerialGate);
bool debug;
bool running = LOW;
unsigned long previous, calculationTime;
const int sensor[] = {A0, A1, A2, A3, A4, A5};

struct param_t
{
  unsigned long cycleTime;
  int black[6];
  int white[6];
  float Kp;
  float diff;
  int power;
  float position; 
  
  /* andere parameters die in het eeprom geheugen moeten opgeslagen worden voeg je hier toe ... */
} params;
int normalised[6];
void setup()
{
  SerialGate.begin(Baudrate);
  analogReference(DEFAULT);

  sCmd.addCommand("set", onSet);
  sCmd.addCommand("debug", onDebug);
  sCmd.addCommand("calibrate", onCalibrate);
  sCmd.addCommand("start", onStart);
  sCmd.addCommand("stop", onStop);
  sCmd.setDefaultHandler(onUnknownCommand);

  EEPROM_readAnything(0, params);

  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  SerialGate.println("ready");
}

void loop()
{
  sCmd.readSerial();
 
  unsigned long current = micros();
  if (current - previous >= params.cycleTime)
  {
    previous = current;

    /* code die cyclisch moet uitgevoerd worden programmeer je hier ... */
    /* normaliseren en interpoleren sensor */
    for (int i = 0; i < 6; i++)
    {
      long value = analogRead(sensor[i]);
      normalised[i] = map(value, params.black[i], params.white[i], 1000,0);
    }

     int index = 0;
    for (int i = 1; i < 6; i++) if (normalised[i] > normalised[index]) index = i;

   if(normalised[index] < 700) running = false;

    if (index == 0) params.position = -30;
    else if (index == 5) params.position = 30;
    else 
    {
    long sZero = normalised[index];
    long sMinusOne = normalised[index-1];
    long sPlusOne = normalised[index+1];

    float b = ((float) (sPlusOne - sMinusOne)) / 2;
    float a = sPlusOne - b - sZero;

    params.position = -b / (2 * a);  
    params.position += index - 2.5;   
    params.position *= 15; 
    } //sensor distance in mm
     
    float error  = -params.position;
    float output = error * params.Kp;

    output = constrain(output, -510, 510);

    int powerLeft = 0;
    int powerRight = 0;
    
  if(running)  if (output <= 0){
  powerLeft = constrain(params.power + params.diff * output, -255, 255);
  powerRight = constrain(powerLeft - output, -255, 255);
  powerLeft = powerRight + output;
  }
  else{
  powerRight = constrain(params.power - params.diff * output, -255, 255);
  powerLeft = constrain(powerRight + output, -255, 255);
  powerRight = powerLeft - output;
  }
    
  analogWrite(leftForward, powerLeft > 0 ? powerLeft : 0);
  analogWrite(leftBackward, powerLeft < 0 ? -powerLeft : 0);
  analogWrite(rightForward, powerRight > 0 ? powerRight : 0);
  analogWrite(rightBackward, powerRight < 0 ? -powerRight : 0);


    /* aansturen motoren */
  }
  


  unsigned long difference = micros() - current;
  if (difference > calculationTime) calculationTime = difference;
}

void onUnknownCommand(char *command)
{
  SerialGate.print("unknown command: \"");
  SerialGate.print(command);
  SerialGate.println("\"");
}

void onSet()
{
  char* param = sCmd.next();
  char* value = sCmd.next();  
  
  if (strcmp(param, "cycle") == 0) params.cycleTime = atol(value);
  /* parameters een nieuwe waarde geven via het set commando doe je hier ... */
  if (strcmp(param, "power") == 0) params.power = atoi(value);
  if (strcmp(param, "diff") == 0) params.diff = atof(value);
  if (strcmp(param, "kp") == 0) params.Kp = atof(value);
  EEPROM_writeAnything(0, params);
}

void onDebug()
{
  SerialGate.print("cycle time: ");
  SerialGate.println(params.cycleTime);
  
  /* parameters weergeven met behulp van het debug commando doe je hier ... */
  SerialGate.print("black: ");
  for (int i = 0; i < 6; i++)
  {
    SerialGate.print(params.black[i]);
    SerialGate.print(" ");
  }
  SerialGate.println(" ");
  
  SerialGate.print("white: ");
  for (int i = 0; i < 6; i++)
  {
    SerialGate.print(params.white[i]);
    SerialGate.print(" ");
  }
  SerialGate.println(" ");

  SerialGate.print("normalised: ");
  for (int i = 0; i < 6; i++)
  {
  SerialGate.print(normalised[i]);
  SerialGate.print(" ");
  }
  SerialGate.println(" ");

  SerialGate.print("Position: ");
  SerialGate.println(params.position);

  SerialGate.print("Kp: ");
  SerialGate.println(params.Kp);

   SerialGate.print("Diff: ");
  SerialGate.println(params.diff);

  SerialGate.print("Power: ");
  SerialGate.println(params.power);

  SerialGate.print("calculation time: ");
  SerialGate.println(calculationTime);
  calculationTime = 0;
}

void onCalibrate()
{
  char* param = sCmd.next();

  if (strcmp(param, "black") == 0)
  {
   SerialGate.print("start calibrating black... ");
    for (int i = 0; i < 6; i++) params.black[i]=analogRead(sensor[i]);
    SerialGate.println("done");
  }
  else if (strcmp(param, "white") == 0)
  {
    SerialGate.print("start calibrating white... ");    
    for (int i = 0; i < 6; i++) params.white[i]=analogRead(sensor[i]);  
    SerialGate.println("done");      
  }

  EEPROM_writeAnything(0, params);
}
void onStart(){
  running = true;
  SerialGate.println("STARTED");
}

void onStop(){
  running = false;
  SerialGate.println("STOPPED");
}
