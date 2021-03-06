#include <LiquidCrystal.h>

// LCD object -- Parameters: (RS, E, D4, D5, D6, D7):
LiquidCrystal lcd = LiquidCrystal(2, 3, 4, 5, 6, 7);

int G1 = 8; // Green1
int R1 = 9; // Red1
int G2 = 10; // Green2
int R2 = 11; //Red2

int ldr1 = A0; 
int ldr2 = A1;

int switchPin1 = 12;
int switchState1 = 0;

int switchPin2 = 13;
int switchState2 = 0;

void setup() 
{
  Serial.begin(9600);
  
  // LCD's number of columns and rows
  lcd.begin(16, 2);

  // LED pins
  pinMode(R1, OUTPUT);
  pinMode(G1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(G2, OUTPUT);

  // LDR pins
  pinMode(ldr1, INPUT);
  pinMode(ldr2, INPUT);

  // Slide Switch Pins
  pinMode(switchPin1, INPUT);
  pinMode(switchPin2, INPUT);
}
void loop() 
{
  // reads pin A0 and A1(0 to 1023)
  ldr1 = analogRead(A0);
  ldr2 = analogRead(A1);
  
  //read the state of the switch value
  switchState1 = digitalRead(switchPin1);
  switchState2 = digitalRead(switchPin2);

  if(switchState2 == HIGH) // Turns LCD ON/OFF
  {
    // Set the cursor on the third column and the first row
    lcd.setCursor(2, 0);
    lcd.print("LDR 1 = ");
    lcd.print(ldr1);
    // Set the cursor on the third column and the second row:
    lcd.setCursor(2, 1);
    lcd.print("LDR 2 = "); 
    lcd.print(ldr2);
  }
  else
  {
    lcd.clear(); // Clears LCD in case something was printed beforehand
  }
  

  if (switchState1 == HIGH) // Turns LED Indicators ON/OFF
  {
    // Indicators for the first LDR (sector 1)
    if (ldr1 > 400) 
    {
      digitalWrite(R1, LOW);
      digitalWrite(G1, HIGH);    
    }
    else 
    {
      digitalWrite(G1, LOW);
      digitalWrite(R1, HIGH);
    }

    // Indicators for the second LDR (sector 2)
    if (ldr2 > 400) 
    {
      digitalWrite(R2, LOW);
      digitalWrite(G2, HIGH);    
    }
    else 
    {
      digitalWrite(G2, LOW);
      digitalWrite(R2, HIGH);
    }
  }
  else // Tuns all LEDs OFF in case they had been turned ON previously
  {
    digitalWrite(G1, LOW);
    digitalWrite(G2, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
  }
  delay(500); 
}
