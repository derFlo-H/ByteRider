#include<array>

const int SIO = 0; // SER
const int SCLK = 1; // SRCLK
const int RCLK = 2; // RCLK
const int CLEAR = 3; // SRCLR
const int LED = 7;

std::array<int,16> state = {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0};
std::array<int,16> off = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int dir = 1; // Direction 1=right 0=left

void setup() {
  // Pin modes
  pinMode(SIO, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(CLEAR, OUTPUT);
  pinMode(LED, OUTPUT);

  // Clear Register
  digitalWrite(CLEAR, LOW);
  digitalWrite(CLEAR, HIGH);

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
  
  if(dir == 1) {

    state = shiftRight(state);

  } else {

    state = shiftLeft(state);

  }

  writeStates(state);
  delay(1000);

  if(state[12] == 1) {
    dir = 0;
  } else if (state[8] == 1) {
    dir = 1;
  }

}

