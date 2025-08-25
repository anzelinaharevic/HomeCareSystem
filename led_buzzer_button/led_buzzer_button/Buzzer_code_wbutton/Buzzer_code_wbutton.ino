const int buttonPin = 33; // Pushbutton pin
const int buzzerPin = 19; // Buzzer pin

int buttonState = 0;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); 
  Serial.begin(115200);
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) { // Button pressed
    
    // Frequency sweep up
    for (int freq = 1000; freq <= 5000; freq += 50) {
      tone(buzzerPin, freq);
      delay(10); // give buzzer time
    }

    // Frequency sweep down
    for (int freq = 5000; freq >= 1000; freq -= 50) {
      tone(buzzerPin, freq);
      delay(10);
    }

    noTone(buzzerPin); // Stop buzzer

  } else {
    noTone(buzzerPin);
  }
}
