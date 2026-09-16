#include <Arduino_LSM9DS1.h>

float x, y, z;
int plusThreshold = 30;
int minusThreshold = -30;

void setup() {

  Serial.begin(9600);
  while (!Serial);

  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");

  Serial.println();
  Serial.println("Gyroscope in degrees/second");
}

void loop() {

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);
  }

  if (y > plusThreshold) {

    Serial.println("Collision front");

    Serial.print("X: ");
    Serial.print(x);

    Serial.print("  Y: ");
    Serial.println(y);

    delay(500);
  }

  if (y < minusThreshold) {

    Serial.println("Collision back");

    Serial.print("X: ");
    Serial.print(x);

    Serial.print("  Y: ");
    Serial.println(y);

    delay(500);
  }

  if (x < minusThreshold) {

    Serial.println("Collision right");

    Serial.print("X: ");
    Serial.print(x);

    Serial.print("  Y: ");
    Serial.println(y);

    delay(500);
  }

  if (x > plusThreshold) {

    Serial.println("Collision left");

    Serial.print("X: ");
    Serial.print(x);

    Serial.print("  Y: ");
    Serial.println(y);

    delay(500);
  }
}