void setup() {
  // put your setup code here, to run once:
  pinMode(21, INPUT);
  pinMode(22, INPUT);
  Serial.print("Starting throttle test");

}

void loop() {
  // put your main code here, to run repeatedly:
  double p1 = analogRead(21);
  double p2 = analogRead(22);
  Serial.println("Port 21: " + String(p1) + " – Port 22: " + String(p2));
  delay(100);
}
