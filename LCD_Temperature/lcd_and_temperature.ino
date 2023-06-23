// 18/JUNE/2023 11:44 PM
// Implementing temperature sensor with esp32 s3 and using LCD for output
#include <DHT.h>
#include <LiquidCrystal.h>

// LCD display pins
const int rs = 38;
const int en = 39;
const int d4 = 40;
const int d5 = 41;
const int d6 = 42;
const int d7 = 2;
const int LED = 19;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Initialize the DHT11 sensor
#define DHTPIN 16 // Change the pin if necessary
DHT dht(DHTPIN, DHT11);

void setup() {
  Serial.begin(115200);
  Serial.println("\nTest msg:");

  lcd.begin(16, 2); // Initialize the LCD display
  lcd.print("ESP32-S3");
  lcd.setCursor(0, 1);
  lcd.print("LCD & DHT11");
  pinMode(LED, OUTPUT);
  dht.begin(); // Initialize the DHT11 sensor
}

void loop() {
  // Read temperature and humidity from the DHT11 sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Display temperature and humidity on the LCD display
  lcd.clear();
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print(" %");

  digitalWrite(LED, HIGH);
  delay(1000); // Delay between readings

  digitalWrite(LED, LOW);
  delay(1000);
}
