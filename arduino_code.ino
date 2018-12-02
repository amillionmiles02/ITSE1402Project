#include <Servo.h>

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

const int ON = HIGH;
const int OFF = LOW;

Servo tempServo;

//LIGHT PINS

int lights[] = {2,3,4,5,6,7,8,9,10};
int num_lights = 9;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  //----PINS SETUP-----

  //lights
  for (int i = 0; i < num_lights; i++)
  {
    pinMode(lights[i], OUTPUT);
  }

  //access door
  pinMode(11, OUTPUT);

  //temperature
  tempServo.attach(12);

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    //do something...

    char section = inputString[1];

    switch (section)
    {
      case 'L':
        turnLights();
      break;
      
      case 'T':
        temperature();
      break;
      
      case 'A':
        door();
      break;
    }
    
    inputString = "";
    stringComplete = false;
  }
}

//------Functions-------

//lights
void turnLights()
{
  int light = inputString[2] - 48;
  int on = inputString[3] - 48;

  if(on == 1)
  {
    Serial.print("Light ");
    Serial.print(light);
    Serial.println(" is on");
    digitalWrite(lights[light-1], HIGH);
  }
  else
  {
    Serial.print("Light ");
    Serial.print(light);
    Serial.println(" is off");
    digitalWrite(lights[light-1], LOW);
  }
}

//access door
void door()
{
  int doorOpen = inputString[2];
  if (doorOpen == 49)
  {
    Serial.println("Access door is open");
    digitalWrite(11, HIGH);
  }
  else
  {
    Serial.println("Access door is closed");
    digitalWrite(11, LOW);
  }
}

//temperature

void temperature()
{
  String string_value = inputString.substring(2);
  int value = string_value.toInt();

  if (value >= 30 && value <= 130)
  {
    int _value = 140 - value;
    Serial.print("Temperature set to ");
    Serial.println(value);

    tempServo.write(_value);
  }
  
  
  
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
