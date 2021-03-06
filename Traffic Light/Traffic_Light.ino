// Variables for LED pins
int G = 7; // Green, sets pin number for color Green
int Y = 6; // Yellow, sets pin number for color Yellow
int R = 5; // Red, sets pin number for color Red

int PG = 8; // Pedestrian Green light
int PR = 9; // Pedestrian red

//Delay to simulate real traffic light
int del_g = 3000;
int del_y = 1000;
int del_r = 9000;

void setup() {
 
pinMode(G, OUTPUT);
pinMode(Y, OUTPUT);
pinMode(R, OUTPUT);

pinMode(PG, OUTPUT);
pinMode(PR, OUTPUT);
}

void loop() {
  // call methods bellow

  tlGreen();
  delay(del_g);
  tlYellow();
  delay(del_y);
  tlRed();
  delay(del_r);
  PGwarn();

}
  
// methods for traffic light "modes"
void tlGreen()
{
  digitalWrite(G, HIGH);
  digitalWrite(PR, HIGH);
  digitalWrite(Y, LOW);
  digitalWrite(R, LOW);
  digitalWrite(PG, LOW);
}

void tlYellow()
{
  digitalWrite(G, LOW);
  digitalWrite(Y, HIGH);
  digitalWrite(PR, HIGH);
  digitalWrite(R, LOW);
  digitalWrite(PG, LOW);
}

void tlRed()
{
  digitalWrite(G, LOW);
  digitalWrite(Y, LOW);
  digitalWrite(R, HIGH);
  digitalWrite(PG, HIGH);
  digitalWrite(PR, LOW);
}

void PGwarn()
{
  //Green pedestrian light blinks to signal that it is about to become red
  for(int i = 0; i <= 3; i++)
  {
    digitalWrite(PG, HIGH);
    delay(150);
    digitalWrite(PG, LOW);
    delay(150);
  }
}
