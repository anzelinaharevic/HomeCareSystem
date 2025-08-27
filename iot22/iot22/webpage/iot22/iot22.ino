#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "index.h"
#include <Wire.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 3;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {15, 2, 4, 16};
byte colPins[COLS] = {17, 5, 18};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

int pin[] = {2,5,8,0}; 
int adminPin[] = {1,0,0,0}; 
int pinEntered[4];
int pinIndex = 0;

const int trigPin = 32;
const int echoPin = 35;
float duration, cm;

const int buttonPin = 33;
const int ledPin = 26;
const int buzzerPin = 12;

unsigned long lastBlink = 0;
unsigned long lastToneUpdate = 0;
int blinkState = LOW;
bool buzzerActive = false;
int buzzerFreq = 1000;

bool emergencyState = false;
bool ultrasonicEnabled = true;

const char* ssid = "eoin";
const char* password = "password1";                 
WebServer server(80);

// button  
void buttonStatus() {
  int reading = digitalRead(buttonPin);

  if (reading == HIGH) {  
    if (millis() - lastBlink >= 100) {
      lastBlink = millis();
      blinkState = !blinkState;
      digitalWrite(ledPin, blinkState);
    }
    
    if (millis() - lastToneUpdate >= 20) {
      lastToneUpdate = millis();
      tone(buzzerPin, buzzerFreq);
      buzzerFreq += 50;
      if (buzzerFreq > 5000) buzzerFreq = 1000;
      Emergency();
    }
  } else {
    digitalWrite(ledPin, HIGH); 
    noTone(buzzerPin);
    buzzerFreq = 1000;
  }
}

// keypad
void keypadPassword() {
  char key = customKeypad.getKey();

  if (key) {
    if (key == '#') {  
      ultrasonicEnabled = true;
      Serial.println("Ultrasonic enabled.");
      pinIndex = 0;
      return;
    }

    pinEntered[pinIndex] = key - '0';
    pinIndex++;
    Serial.print(key);

    if (pinIndex == 4) {
      Serial.println("");

      if (checkPin(pinEntered, pin)) {
          Serial.println("PIN correct");
          emergencyState = false;
        }
        else if (checkPin(pinEntered, adminPin)) {
          setNewPin();
        }
        else {
          Serial.println("Incorrect PIN");
        }
      pinIndex = 0;
    }
  }
}

bool checkPin(int entered[], int correct[]) {
  for (int i = 0; i < 4; i++) {
    if (entered[i] != correct[i]) {
      return false;
    }
  }
  return true;
}

void setNewPin() {
  Serial.println("Enter new 4-digit PIN: ");
  int newPin[4];
  int index = 0;

  while (index < 4) {
    char key = customKeypad.getKey();
    if (key) {
      newPin[index] = key - '0';
      index++;
      Serial.print(key);
    }
  }
for (int i = 0; i < 4; i++) {
  pin[i] = newPin[i];
}

Serial.println("");
Serial.println("New PIN saved!");

}

void Emergency() {
  emergencyState = true;
  Serial.println("EMERGENCY STATE ACTIVATED!");
}

// ultrasonic
String ultrasonic() {
  if (!ultrasonicEnabled) {
    return "0.00";
  }
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);
  cm = (duration * 0.0343) / 2;

  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.println("cm");

  return String(cm,2);
}

// HANDLERS
void handleStatus() {
  server.send(200, "text/plain", emergencyState ? "1" : "0");
}

void handledistance() {
  server.send(200, "text/plain", ultrasonic());
}

void handleRoot() {
  server.send(200, "text/html", homePagePart1);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: " + server.uri();
  server.send(404, "text/plain", message);
}

// setup
void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); 

  Serial.println("Starting ESP32...");
  Wire.begin(21, 22);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected. IP: " + WiFi.localIP().toString());

  if (MDNS.begin("esp32")) Serial.println("MDNS responder started");

  server.on("/", handleRoot);
  server.on("/Callbutton", handleStatus);
  server.on("/Ultrasonic", handledistance);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

// loop
void loop() {
  server.handleClient();
  buttonStatus();
  keypadPassword();
  delay(10);
}