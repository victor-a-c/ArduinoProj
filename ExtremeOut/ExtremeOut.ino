#include <Servo.h>
#include <LiquidCrystal.h>

// LCD object -- Parameters: (RS, E, D4, D5, D6, D7):
LiquidCrystal lcd = LiquidCrystal(2, 3, 4, 5, 6, 7);

//outside temp LED  pins
int LEDt1 = 8; // Temp High - RED
int LEDt2 = 9; // Temp Medium - YELLOW
int LEDt3 = 10; // Temp Low - GREEN

// Rain Sensor FC-37/YL-83/HM-RD ---------------------------------------------------
int RainD = 1; // Digital pin of Rain Sensor -- It is fine to use Pin 1 as long as you dont use the Serial library
int RainA = A2; // Analog pin of Rain Sensor
int RainAnalogVal;
int RainDigitalVal;
int ThresholdValue = 500;
bool IsRaining = false;

//Door/window status LED  pins
int LEDs1 = 11; // door closed - RED
int LEDs2 = 12; // door open - GREEN

// Servo-------------------------------------------------------------
int servo = 13;
int pos = 0;
Servo MyServo;
bool DoorStat = false;
bool calibrated = false;

//temperature sensor
int tmp = A0;
int celsius = 0;

//Other
bool prev;
bool current;

void setup()
{
  Serial.begin(9600);

  // LCD's number of columns and rows------------------------------
  lcd.begin(16, 2);

  //temp sensor----------------------------------------------------
  pinMode(tmp, INPUT);

  //All LEDs-------------------------------------------------------
  pinMode(LEDt1, OUTPUT);
  pinMode(LEDt2, OUTPUT);
  pinMode(LEDt3, OUTPUT);
  pinMode(LEDs1, OUTPUT);
  pinMode(LEDs2, OUTPUT);

  //Sets up servo---------------------------------------------------
  MyServo.attach(servo, 544, 2500);

  
}

void loop()
{
  servoCalibrationSequence();
  delay(500);
  
  GetTemp();
  GetRainStat();
  TempResponse();
  PrintToLCD();
  Serial.println(celsius);

  if(IsRaining == false)
  {
    delay(300000); // Delays 5 minutes since temperature changes are usually not very fast
  }
}

void GetTemp()
{
  celsius = map(((analogRead(tmp) - 20) * 3.04), 0, 1023, -40, 125);
  if(celsius > 30 || celsius < 10)
  {
    digitalWrite(LEDt1, HIGH); // Temp too high or low, RED
    // turn off other temp LEDs
    digitalWrite(LEDt2, LOW);
    digitalWrite(LEDt3, LOW);
  }
  else if(celsius < 30 && celsius > 26)
  {
    digitalWrite(LEDt2, HIGH); // Temp Moderate, YELLOW
    digitalWrite(LEDt1, LOW);
    digitalWrite(LEDt3, LOW);
  }
  else if(celsius < 26 && celsius > 10)
  {
    digitalWrite(LEDt3, HIGH); // Temp GOOD, GREEN
    digitalWrite(LEDt1, LOW);
    digitalWrite(LEDt2, LOW);
  }

  
}

void TempResponse()
{
  if((celsius > 33 || celsius < 10)||(IsRaining == true)) // too cold or too hot
  {
    if(DoorStat == true) // if door is open
    {
      CloseDoorSequence();
    }
  }
  else if (celsius < 29 && celsius > 10)
  {
    if(DoorStat == false) // if door closed
    {
      OpenDoorSequence();
    }
  }
}

void PrintToLCD()
{
  // Set the cursor on the third column and the first row
  lcd.setCursor(2, 0);
  lcd.print("Temp: ");
  lcd.print(celsius);
  // Set the cursor on the third column and the second row:
  lcd.setCursor(2, 1);
  lcd.print("D stat: "); 
  GetDoorStat();
}

void GetDoorStat()
{
  if(DoorStat == false)
  {
    lcd.print("Closed");
  }
  else if (DoorStat == true)
  {
    lcd.print("Open  ");
  }
  else
  {
    lcd.print("Stat Unknown");
    lcd.clear();
  }
}

void servoCalibrationSequence()
{
  if(calibrated == false)
  {
    Serial.print("----THE SERVO IS BEING CALIBRATED----\n");
    for (pos = 0; pos < 30; pos ++) 
    {
      // tell servo to go to position in variable 'pos'
      MyServo.write(pos);
      // wait 10 ms for servo to reach the position
      delay(10); // Wait for 10 millisecond(s)
    }
    for (pos = 30; pos > 0; pos --) 
    {
      // tell servo to go to position in variable 'pos'
      MyServo.write(pos);
      // wait 10 ms for servo to reach the position
      delay(10); // Wait for 10 millisecond(s)
    }
    calibrated = true;
  }
}

void OpenDoorSequence()
{
  digitalWrite(LEDs2, HIGH); // Open door LED
  digitalWrite(LEDs1, LOW);
  DoorStat = true;
  // sweep the servo from 0 to 180 degrees in steps of 1 degrees
  // opens up pet door/window
  for (pos = 0; pos <= 140; pos ++) 
  {
    // tell servo to go to position in variable 'pos'
    MyServo.write(pos);
    // wait 15 ms for servo to reach the position
    delay(15); // Wait for 15 millisecond(s)
  }
}

void CloseDoorSequence()
{
  digitalWrite(LEDs1, HIGH); // Closed Door LED
  digitalWrite(LEDs2, LOW);
  //closes door/window
  if(pos > 0)
  {
    for (pos = 140; pos > 0; pos --) 
    {
      MyServo.write(pos);
      // wait 15 ms for servo to reach the position
      delay(15); // Wait for 15 millisecond(s)
    }
  }
  DoorStat = false;
}

void GetRainStat()
{
  RainDigitalVal = digitalRead(RainD);
  if(digitalRead(RainD) == LOW) // Output goes down when it rains
  {
    Serial.println("Digital value : wet");
    IsRaining = true;
        
    delay(10); 
  }
  else
  {
    Serial.println("Digital value : dry");
    delay(10); 
    IsRaining = false;
  }
}

void readings()
{
  Serial.print("Temperature: ");
  Serial.println(celsius);

  Serial.print("Door Status: ");
  Serial.println(DoorStat);
}
