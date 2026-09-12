BBTimer time0(BB_TIMER0);

volatile bool leer = false;
int valor;

void leerADC() {
  leer = true;
}

void setup() {
  Serial.begin(9600);

  time0.setupTimer(10000000, leerADC);
  time0.timerStart();
}

void loop() {
  if (leer == true) {
    valor = analogRead(4);
    Serial.println(valor);

    leer = false;
  }
}
