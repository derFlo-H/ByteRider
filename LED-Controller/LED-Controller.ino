#include<array>

const int SIO = 0; // SER
const int SCLK = 1; // SRCLK
const int RCLK = 2; // RCLK
const int CLEAR = 3; // SRCLR
const int LED = 7;

std::array<int,16> on = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
std::array<int,16> off = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
std::array<int,16> one = {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};

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
  Serial.begin(9600);

}

std::array<int,16> shiftStates(std::array<int,16> originalArray) {
  // shift all bits in array to the right. Last element shifts over into the first element
  std::array<int,16> shiftedArray;
  
  shiftedArray[0] = originalArray[15];
  for(int i = 1; i < 16; i++) {
    shiftedArray[i] = originalArray[i-1];
  }

  return shiftedArray;

}

void loop() {
  
  writeStates(one);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  one = shiftStates(one);
  delay(500);

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
