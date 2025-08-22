const int trigPin = 12;  
const int echoPin = 13; 
float duration, cm;  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
    pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT); 
}

void loop() {
  // put your main code here, to run repeatedly:
 digitalWrite(trigPin, LOW);  
  delayMicroseconds(2);  
  digitalWrite(trigPin, HIGH);  
  delayMicroseconds(10);  
  digitalWrite(trigPin, LOW);  

  duration = pulseIn(echoPin, HIGH, 23529.4);  
  cm = duration / 58.8235;  

  Serial.print("Distance: ");  
  Serial.print(cm);  
  Serial.println("cm");  
  delay(250);
}


 
