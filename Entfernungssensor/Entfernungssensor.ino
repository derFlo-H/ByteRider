#include<array>

const int SIO = 0; // SER
const int SCLK = 1; // SRCLK
const int RCLK = 2; // RCLK
const int CLEAR = 3; // SRCLR
const int LED = 7;

const int TRIG = 18;
const int ECHO = 19;
float distance = 0.0;
float timing = 0.0;

std::array<int,16> sixty = {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0};
std::array<int,16> forty = {0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0};
std::array<int,16> twenty = {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0};
std::array<int,16> off = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void setup() {
  // Pin modes
  pinMode(SIO, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(CLEAR, OUTPUT);
  pinMode(LED, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Clear Register
  digitalWrite(CLEAR, LOW);
  digitalWrite(CLEAR, HIGH);

  digitalWrite(TRIG, LOW);

  Serial.begin(9600);

}

std::array<int,16> shiftLeft(std::array<int,16> originalArray) {
  // shift all bits in array to the right. Last element shifts over into the first element
  std::array<int,16> shiftedArray;
  
  shiftedArray[15] = originalArray[0];
  for(int i = 14; i > 0; i--) {
    shiftedArray[i] = originalArray[i+1];
  }

  return shiftedArray;

}

std::array<int,16> shiftRight(std::array<int,16> originalArray) {
  // shift all bits in array to the right. Last element shifts over into the first element
  std::array<int,16> shiftedArray;
  
  shiftedArray[0] = originalArray[15];
  for(int i = 1; i < 16; i++) {
    shiftedArray[i] = originalArray[i-1];
  }

  return shiftedArray;

}

void writeStates(std::array<int,16> states) {
  // for each bit in states: write this bit
  digitalWrite(SCLK, LOW);
  for(int i = 0; i < 16; i++) {
    writeBit(states[i]);
  }
  digitalWrite(SCLK, HIGH);
}

void writeBit(int bit) {
  digitalWrite(RCLK, LOW);
  digitalWrite(SIO, bit);
  digitalWrite(RCLK, HIGH);
}

void loop() {

  digitalWrite(TRIG, LOW);
  delay(2);

  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);
  timing = pulseIn(ECHO, HIGH);
  distance = (timing * 0.034) / 2;
  
  // !!! Ultraschalsensor benötigt eine verbundene 9V Batterie, sonst funktioniert dieser nicht !!!

  if(distance >= 60) {
    writeStates(sixty);
  } else if(distance >= 40) {
    writeStates(forty);
  } else if(distance >= 20) {
    writeStates(twenty);
  } else {
    writeStates(off);
  }

}

