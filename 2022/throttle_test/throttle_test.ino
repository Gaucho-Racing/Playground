void setup() {
  // put your setup code here, to run once:
  pinMode(21, INPUT);
  pinMode(22, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  double max1 = 0;
  double min1 = 1000;
  double max2 = 0;
  double min2 = 1000;
  int repeat = 0;
  
  Serial.println("Starting throttle test");
  
//  while (true) {
  while (repeat < 100) {
    double p1 = analogRead(21);
    if (p1 > max1) max1 = p1;
    if (p1 < min1) min1 = p1;
    double p2 = analogRead(22);
    if (p2 > max2) max2 = p2;
    if (p2 < min2) min2 = p2;
    Serial.println("Port 21: " + String(p1) + " – Port 22: " + String(p2));
    delay(100);
    repeat++;
  }
  Serial.println("========= Port 21 =========");
  Serial.println("Min: " + String(min1));
  Serial.println("Max: " + String(max1));
  Serial.println("========= Port 22 =========");
  Serial.println("Min: " + String(min2));
  Serial.println("Max: " + String(max2));
  delay(5000);
}
