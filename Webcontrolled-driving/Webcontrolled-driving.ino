#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

#include <ESP32Servo.h>

#define DC_MOTOR_PIN_1 6
#define DC_MOTOR_PIN_2 4

#define SERVO_MOTOR_PIN 10
Servo MY_SERVO;
#define ANGLE_STRAIGHT 60
#define ANGLE_LEFT 40
#define ANGLE_RIGHT 85

// just copy paste HTML file content
const char html[] = R"==(<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>ByteRider Controller</title>
        <style>
            body {overflow: hidden;}
            .container {
                display: grid;
                grid-template-columns: auto auto auto;
                gap: 10px;
                width: 85vw;
                height: 85vh;
                margin: 20px;
            }
            .container > button {
              -webkit-user-select: none;
                user-select: none;
                background-color: #00c3ff;
                border: 1px solid rgb(0, 60, 255);
                border-radius: 10px;
                padding: 10px;
                font-size: 30px;
                text-align: center;
            }

            h1 {
              -webkit-user-select: none;
                user-select: none;
                font-size: 5vw;
                grid-column-start: 1;
                grid-column-end: 3;
                grid-row: 1;
            }

            button:active {
              -webkit-user-select: none;
              user-select: none;
              background-color: #ffffff;
            }
            .left {
              -webkit-user-select: none;
              user-select: none;
                grid-row: 2;
                grid-column: 1;
            }
            .top {
              -webkit-user-select: none;
              user-select: none;
                grid-row: 1;
                grid-column: 3;
                margin-left: 10vw;
            }
            .right {
              -webkit-user-select: none;
              user-select: none;
                grid-row: 2;
                grid-column: 2;
            }
            .down {
              -webkit-user-select: none;
              user-select: none;
                grid-row: 2;
                grid-column: 3;
                margin-left: 10vw;
            }
        </style>
        
    </head>
    <body>
      <script>

            function drive(dir, act) {
                var xhr = new XMLHttpRequest();
                xhr.open("GET", "/update?direction="+dir+"&do="+act, true);
                xhr.send();
            }

        </script>
            <div class="container">
                <h1>ByteRider</h1>
                <button class="left" ontouchstart="drive('left', 1)" ontouchend="drive('left', 0)">←</button>
                <button class="top" ontouchstart="drive('forwards', 1)" ontouchend="drive('forwards', 0)">↑</button>
                <button class="down" ontouchstart="drive('backwards', 1)" ontouchend="drive('backwards', 0)">↓</button> 
                <button class="right" ontouchstart="drive('right', 1)" ontouchend="drive('right', 0)">→</button>           
            </div>
    </body>
</html>)==";

// ssid is the name that will be displayed
const char* ssid = "BRUMM BRUMM";
const char* password = "12345678";
AsyncWebServer server(80);
DNSServer dnsServer;

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(__unused AsyncWebServerRequest *request) const override{
    //request->addInterestingHeader("ANY");
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    AsyncResponseStream* response = request->beginResponseStream("text/html");
    response->print(html);
    request->send(response);
  }
};

// GET /update?direction=<dir>&do=<act>
// reacts to GET request and drives motors based on that
void requestHandler(String dir, String act) {
  int set;
  // convert String act to int set
  if(act == "1") {
    set = 1;
  } else {
    set = 0;
  }
  
  // Forwards and Backwards driving
  if(dir == "forwards") {
    digitalWrite(DC_MOTOR_PIN_1, set);
    digitalWrite(DC_MOTOR_PIN_2, LOW);
  }
  if(dir == "backwards") {
    digitalWrite(DC_MOTOR_PIN_2, set);
    digitalWrite(DC_MOTOR_PIN_1, LOW);
  }

  // Steering
  if(dir == "left" && set == 1) {
    MY_SERVO.write(ANGLE_LEFT);
  } else if(dir == "right" && set == 1) {
    MY_SERVO.write(ANGLE_RIGHT);
  } else if(dir == "right" || dir == "left") {
    MY_SERVO.write(ANGLE_STRAIGHT);
  }

}

void setup() {
  Serial.begin(115200);

  // Set Motor pins
  pinMode(DC_MOTOR_PIN_1, OUTPUT);
  pinMode(DC_MOTOR_PIN_2, OUTPUT);
  pinMode(SERVO_MOTOR_PIN, OUTPUT);
  MY_SERVO.attach(SERVO_MOTOR_PIN);

  // WiFi Server
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());

  // server.on automatically runs when requested by client

  // When a device connects the server will serve the HTML
  server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", html);
    Serial.println("Send HTML");
  });

  // buttons send to /update and this function just parses the request data to the request handler
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam("direction") && request->hasParam("do")) {
      inputMessage1 = request->getParam("direction")->value();
      inputMessage2 = request->getParam("do")->value();
      requestHandler(inputMessage1, inputMessage2);
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("Direction: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });
  // Start the server
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.setTTL(300);
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler());
  server.begin();
}

void loop() {
  dnsServer.processNextRequest();

}
