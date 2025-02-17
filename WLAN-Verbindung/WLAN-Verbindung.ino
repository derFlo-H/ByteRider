#include<array>
#include<WiFi.h>

const int SIO = 0; // SER
const int SCLK = 1; // SRCLK
const int RCLK = 2; // RCLK
const int CLEAR = 3; // SRCLR
const int LED = 7;

const char *ssid = "AP_ESP32";
const char *password = NULL;

std::array<int,16> off = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
std::array<int,16> on = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

void connected(WiFiEvent_t event, WiFiEventInfo_t info) {
  writeStates(on);
}

void disconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  writeStates(off);
}

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

  Serial.begin(115200);

  WiFi.onEvent(connected, WiFiEvent_t::ARDUINO_EVENT_WIFI_AP_STACONNECTED);
  WiFi.onEvent(disconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_AP_STADISCONNECTED);

  WiFi.softAP(ssid, password);

  IPAddress ip = WiFi.softAPIP();
  Serial.print("Access Point IP: ");
  Serial.println(ip);

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
  
}

