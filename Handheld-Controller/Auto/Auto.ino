#include <esp_now.h>
#include <WiFi.h>

#include <ESP32Servo.h>

#define DC_MOTOR_PIN_1 6
#define DC_MOTOR_PIN_2 4

#define SERVO_MOTOR_PIN 10
Servo MY_SERVO;
#define ANGLE_STRAIGHT 60
#define ANGLE_LEFT 45
#define ANGLE_RIGHT 80

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int left;
  int right;
  int forwards;
  int backwards;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  Serial.println(myData.forwards);

  if(myData.forwards == myData.backwards) {
    digitalWrite(DC_MOTOR_PIN_1, LOW);
    digitalWrite(DC_MOTOR_PIN_2, LOW);
  } else if(myData.forwards == 0) {
    digitalWrite(DC_MOTOR_PIN_1, HIGH);
    digitalWrite(DC_MOTOR_PIN_2, LOW);
  } else if(myData.backwards == 0) {
    digitalWrite(DC_MOTOR_PIN_1, LOW);
    digitalWrite(DC_MOTOR_PIN_2, HIGH);
  }

  if(myData.left == myData.right) {
    MY_SERVO.write(ANGLE_STRAIGHT);
  } else if(myData.left == 0) {
    MY_SERVO.write(ANGLE_LEFT);
  } else if(myData.right == 0) {
    MY_SERVO.write(ANGLE_RIGHT);
  }
  
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set Motor pins
  pinMode(DC_MOTOR_PIN_1, OUTPUT);
  pinMode(DC_MOTOR_PIN_2, OUTPUT);
  pinMode(SERVO_MOTOR_PIN, OUTPUT);
  MY_SERVO.attach(SERVO_MOTOR_PIN);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}
 
void loop() {

}