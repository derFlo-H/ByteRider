const int forward = 6;
const int backward = 4;

int speed = 0;

void setup() {
  pinMode(forward, OUTPUT);
  pinMode(backward, OUTPUT);

}

void loop() {
  delay(25);
  driveForwards(speed);
  if(speed < 255) {
    speed++;
  }

}

void driveForwards(int speed) {

  analogWrite(forward, speed);
  digitalWrite(backward, LOW);

}

void driveBackwards(int speed) {

  digitalWrite(forward, LOW);
  analogWrite(backward, speed);

}

void halt() {

  digitalWrite(forward, LOW);
  digitalWrite(backward, LOW);

}
