#include <WiFi.h>
#include <esp_now.h>

#define leftSW 3
#define rightSW 2
#define backwardsSW 1
#define forwardsSW 0

#define testLED 19

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x58, 0xCF, 0x79, 0xDA, 0x33, 0x94};

typedef struct message {
  int left;
  int right;
  int forwards;
  int backwards;
} message;

message data;

esp_now_peer_info peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
    // Init Serial Monitor
  Serial.begin(115200);

  pinMode(testLED, OUTPUT);

  pinMode(leftSW, INPUT_PULLUP);
  pinMode(rightSW, INPUT_PULLUP);
  pinMode(forwardsSW, INPUT_PULLUP);
  pinMode(backwardsSW, INPUT_PULLUP);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  digitalWrite(testLED, LOW);
  data.forwards = digitalRead(forwardsSW);
  data.backwards = digitalRead(backwardsSW);
  data.left = digitalRead(leftSW);
  data.right = digitalRead(rightSW);

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &data, sizeof(data));

  if (result == ESP_OK) {
 
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(50);
}
