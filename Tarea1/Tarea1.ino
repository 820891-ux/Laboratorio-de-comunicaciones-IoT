int valorADC;

void setup() {
  Serial.begin(9600);
}

void loop() {

  valorADC = analogRead(A0);

  Serial.print("ADC: ");
  Serial.println(valorADC);

  delay(1000);
}