const int buttonPin = 33; // Pushbutton pin
const int ledPin = 26;    // LED pin

int buttonState = 0, i;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); 
  Serial.begin(115200);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {
   for(i=0;i<10;i++){
    digitalWrite(ledPin, LOW);
    delay(50);
    digitalWrite(ledPin, HIGH);
    delay(50);
   }
  }
  else {
    digitalWrite(ledPin, HIGH);
 
  }
}


