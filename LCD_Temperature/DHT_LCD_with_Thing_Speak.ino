#include <DHT.h>
#include <LiquidCrystal.h>
#include <ThingSpeak.h>
#include <WiFi.h>
#include <random>
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

// ThingSpeak parameters
const char *ssid = "Archer 73";
const char *password = "Archer@73_102#";
const unsigned long channelID = 2201589;
const char *apiKey = "M6GKK40AB3Q7YE42";

// Update interval in milliseconds (10 minutes)
unsigned long updateInterval = 5 * 60;

unsigned long previousUpdateTime = 0;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println("\nTest msg");
  lcd.begin(16, 2); // Initialize the LCD display
  lcd.print("ESP32-S3");
  lcd.setCursor(0, 1);
  lcd.print("LCD & DHT11");

  dht.begin(); // Initialize the DHT11 sensor

  WiFi.begin(ssid, password); // Connect to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    // blink dot on lcd position 0, 15
    lcd.setCursor(0, 14);
    lcd.print(".");
    delay(1000);
    lcd.setCursor(0, 14);
    lcd.print(" ");
  }
  pinMode(LED, OUTPUT);
  ThingSpeak.begin(client); // Initialize ThingSpeak
  int randomValue = random(10, 31);
  ThingSpeak.setField(3, randomValue);
}

void loop() {
  // take serial input like ut:20 to set update interval for 20 second
  if (Serial.available() > 0) {
    String input = Serial.readString();
    if (input.startsWith("ut:")) {
      updateInterval = input.substring(3).toInt();
    }
    Serial.println("New update interval is : " + String(updateInterval));
  }

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

  // Update ThingSpeak every 10 minutes
  unsigned long currentMillis = millis();
  if ((currentMillis / 1000) - previousUpdateTime >= updateInterval) {
    previousUpdateTime = (currentMillis / 1000);
    updateThingSpeak(temperature, humidity);
  }

  delay(2000); // Delay between readings
}

void updateThingSpeak(float temperature, float humidity) {
  ThingSpeak.setField(1, temperature); // Set temperature value
  ThingSpeak.setField(2, humidity);    // Set humidity value

  int updateStatus = ThingSpeak.writeFields(channelID, apiKey);
  if (updateStatus == 200) {
    Serial.println("ThingSpeak update successful");
    SUCCESS_MSG();
  } else {
    Serial.print("Error updating ThingSpeak. Status: ");
    Serial.println(updateStatus);
    ERROR_MSG();
  }
}
void SUCCESS_MSG() {
  // set curser to first row, first last column and print "tick symbol"
  lcd.setCursor(0, 15);
  digitalWrite(LED, HIGH);
  lcd.print("+");
}
void ERROR_MSG() {
  // set curser to first row, first last column and print "tick symbol"
  lcd.setCursor(0, 15);
  digitalWrite(LED, LOW);
  lcd.print("-");
}
