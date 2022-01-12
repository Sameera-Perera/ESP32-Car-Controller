#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <analogWrite.h>

#define S_FORWARD 11
#define S_BACKWARD 12
#define S_LEFT 13
#define S_RIGHT 14
#define S_FORWARD_LEFT 15
#define S_FORWARD_RIGHT 16
#define S_BACKWARD_LEFT 17
#define S_BACKWARD_RIGHT 18

#define M_FORWARD 21
#define M_BACKWARD 22
#define M_LEFT 23
#define M_RIGHT 24
#define M_FORWARD_LEFT 25
#define M_FORWARD_RIGHT 26
#define M_BACKWARD_LEFT 27
#define M_BACKWARD_RIGHT 28

#define F_FORWARD 31
#define F_BACKWARD 32
#define F_LEFT 33
#define F_RIGHT 34
#define F_FORWARD_LEFT 35
#define F_FORWARD_RIGHT 36
#define F_BACKWARD_LEFT 37
#define F_BACKWARD_RIGHT 38

#define STOP 0
#define START 1

// Motor A
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 14;

// Motor B
int motor2Pin1 = 16;
int motor2Pin2 = 17;
int enable2Pin = 4;

// Setting PWM properties 1
const int freq1 = 30000;
const int pwmChannel = 0;
const int resolution1 = 8;
int dutyCycle = 200;

// Setting PWM properties 1
const int freq2 = 30000;
const int pwmChanne2 = 1;
const int resolution2 = 8;
int dutyCycle2 = 200;

const char* ssid     = "MyDevelopmentBoard Car";
const char* password = "12345678";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


void setup(void)
{
  // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  // sets the pins as outputs:
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq1, resolution1);
  ledcSetup(pwmChanne2, freq2, resolution2);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcAttachPin(enable2Pin, pwmChanne2);

  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  server.begin();
  Serial.println("HTTP server started");

}

void loop()
{
  ws.cleanupClients();
}

void rotateMotor1(int x) {
  if (x == 1) {
    Serial.println("Moving Forward");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
  } else if (x == -1) {
    Serial.println("Moving Backward");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
  } else {
    Serial.println("STOP");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
  }
}

void rotateMotor2(int x) {
  if (x == 1) {
    Serial.println("Moving Forward");
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
  } else if (x == -1) {
    Serial.println("Moving Backward");
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
  } else {
    Serial.println("STOP");
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
  }
}

void speedControl(int motor1, int motor2) {
  ledcWrite(pwmChannel, motor1);
  ledcWrite(pwmChanne2, motor2);
}

void processCarMovement(String inputValue)
{
  Serial.printf("Got value as %s %d\n", inputValue.c_str(), inputValue.toInt());
  switch (inputValue.toInt())
  {

    case S_FORWARD:
      rotateMotor1(1);
      rotateMotor2(1);
      speedControl(215, 215);
      break;

    case S_BACKWARD:
      rotateMotor1(-1);
      rotateMotor2(-1);
      speedControl(215, 215);
      break;

    case S_LEFT:
      rotateMotor1(1);
      rotateMotor2(0);
      speedControl(215, 215);
      break;

    case S_RIGHT:
      rotateMotor1(0);
      rotateMotor2(1);
      speedControl(215, 215);
      break;

    case S_FORWARD_LEFT:
      rotateMotor1(1);
      rotateMotor2(1);
      speedControl(215, 180);
      break;

    case S_FORWARD_RIGHT:
      rotateMotor1(1);
      rotateMotor2(1);
      speedControl(180, 215);
      break;

    case S_BACKWARD_LEFT:
      rotateMotor1(-1);
      rotateMotor2(-1);
      speedControl(215, 180);
      break;

    case S_BACKWARD_RIGHT:
      rotateMotor1(-1);
      rotateMotor2(-1);
      speedControl(180, 215);
      break;


    case M_FORWARD:
      rotateMotor1(1);
      rotateMotor2(1);
      speedControl(235, 235);
      break;

    case M_BACKWARD:
      rotateMotor1(-1);
      rotateMotor2(-1);
      speedControl(235, 235);
      break;

    case M_LEFT:
      rotateMotor1(1);
      rotateMotor2(0);
      speedControl(215, 215);
      break;

    case M_RIGHT:
      rotateMotor1(0);
      rotateMotor2(1);
      speedControl(235, 235);
      break;

    case M_FORWARD_LEFT:
      rotateMotor1(1);
      rotateMotor2(1);
      speedControl(235, 200);
      break;

    case M_FORWARD_RIGHT:
      rotateMotor1(1);
      rotateMotor2(1);
      speedControl(200, 235);
      break;

    case M_BACKWARD_LEFT:
      rotateMotor1(-1);
      rotateMotor2(-1);
      speedControl(235, 200);
      break;

    case M_BACKWARD_RIGHT:
      rotateMotor1(-1);
      rotateMotor2(-1);
      speedControl(200, 235);
      break;

    case F_FORWARD:
      rotateMotor1(1);
      rotateMotor2(1);
      speedControl(255, 255);
      break;

    case F_BACKWARD:
      rotateMotor1(-1);
      rotateMotor2(-1);
      speedControl(255, 255);
      break;

    case F_LEFT:
      rotateMotor1(1);
      rotateMotor2(0);
      speedControl(255, 255);
      break;

    case F_RIGHT:
      rotateMotor1(0);
      rotateMotor2(1);
      speedControl(255, 255);
      break;

    case F_FORWARD_LEFT:
      rotateMotor1(1);
      rotateMotor2(1);
      speedControl(255, 220);
      break;

    case F_FORWARD_RIGHT:
      rotateMotor1(1);
      rotateMotor2(1);
      speedControl(220, 255);
      break;

    case F_BACKWARD_LEFT:
      rotateMotor1(-1);
      rotateMotor2(-1);
      speedControl(255, 220);
      break;

    case F_BACKWARD_RIGHT:
      rotateMotor1(-1);
      rotateMotor2(-1);
      speedControl(220, 255);
      break;

    case STOP:
      rotateMotor1(0);
      rotateMotor2(0);
      break;

    default:
      rotateMotor1(0);
      rotateMotor2(0);
      break;
  }
}

void onWebSocketEvent(AsyncWebSocket *server,
                      AsyncWebSocketClient *client,
                      AwsEventType type,
                      void *arg,
                      uint8_t *data,
                      size_t len)
{
  switch (type)
  {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      ws.text(client->id(), String("connected"));
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      processCarMovement("0");
      break;
    case WS_EVT_DATA:
      AwsFrameInfo *info;
      info = (AwsFrameInfo*)arg;
      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
      {
        std::string myData = "";
        myData.assign((char *)data, len);
        processCarMovement(myData.c_str());
      }
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
    default:
      break;
  }
}
