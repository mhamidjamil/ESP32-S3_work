//$ 11/JULY/2023 10:50 PM
//~ Version 2.0.0
//! # symbol not update when wifi connection re-established.

#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <ThingSpeak.h>
#include <WiFi.h>
#include <Wire.h>
#include <random>

#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

// Initialize the DHT11 sensor
#define DHTPIN 16 // Change the pin if necessary
DHT dht(DHTPIN, DHT11);

// ThingSpeak parameters
const char *ssid = "Archer 73";
const char *password = "Archer@73_102#";
const unsigned long channelID = 2201589;
const char *apiKey = "M6GKK40AB3Q7YE42";

unsigned long updateInterval = 2 * 60;
unsigned long previousUpdateTime = 0;
unsigned int last_update = 0; // in seconds
WiFiClient client;

String END_VALUES = "  ";
String line_1 = "Temp: 00.0 C";
String line_2 = "                ";

void connect_wifi();
void lcd_print();
const int LED = 20;
void setup() {
  // Initialize the I2C interface with your desired SDA and SCL pins
  Wire.begin(21, 19);

  // Initialize the LCD display
  lcd.begin(LCD_COLUMNS, LCD_ROWS);

  // Turn on the backlight
  lcd.setBacklight(HIGH);
  Serial.begin(115200);
  Serial.println("\nRandom number: " + String(random(10, 31)));
  dht.begin(); // Initialize the DHT11 sensor
  connect_wifi();
  pinMode(LED, OUTPUT);
  ThingSpeak.begin(client); // Initialize ThingSpeak
  ThingSpeak.setField(4, random(10, 31));
  END_VALUES.setCharAt(1, '#');
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
  int humidity = dht.readHumidity();

  char temperatureStr[5];
  dtostrf(temperature, 4, 1, temperatureStr);

  line_1 =
      line_1.substring(0, 6) + String(temperatureStr) + " C  " + END_VALUES;

  line_2 = "Hu: " + String(humidity) + " % /" + get_time();
  lcd_print();
  if (((millis() / 1000) - previousUpdateTime) >= updateInterval) {
    previousUpdateTime = (millis() / 1000);
    updateThingSpeak(temperature, humidity);
  }

  delay(2000); // Delay between readings
}

void updateThingSpeak(float temperature, int humidity) {
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
  digitalWrite(LED, HIGH);
  END_VALUES.setCharAt(1, '+');
  last_update = (millis() / 1000);
}
void ERROR_MSG() {
  // set curser to first row, first last column and print "tick symbol"
  digitalWrite(LED, LOW);
  END_VALUES.setCharAt(1, '-');
  connect_wifi();
}
void connect_wifi() {
  WiFi.begin(ssid, password); // Connect to Wi-Fi
  int i = 0;
  while (!wifi_connected()) {
    if (i > 10) {
      Serial.println("Timeout: Unable to connect to WiFi");
      break;
    }
    delay(500);
    i++;
    END_VALUES.setCharAt(0, '?');
    lcd_print();
    delay(500);
    END_VALUES.setCharAt(0, ' ');
  }
  Serial.println("Wi-Fi connected successfully");
  if (wifi_connected()) {
    END_VALUES.setCharAt(0, '*');
    lcd_print();
  } else {
    END_VALUES.setCharAt(0, '!');
    lcd_print();
  }
}
bool wifi_connected() {
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  } else {
    return false;
  }
}
void lcd_print() {
  lcd.clear();
  lcd.print(line_1);
  lcd.setCursor(0, 1);
  lcd.print(line_2);
}
String get_time() {
  unsigned int sec = (millis() / 1000) - last_update;
  if (sec < 60) {
    return (String(sec) + " s");
  } else if ((sec >= 60) && (sec < 3600)) {
    return (String(sec / 60) + "m" + String(sec % 60) + "s");
  } else {
    Serial.println("Issue spotted sec value: " + String(sec));
    connect_wifi();
    return String(-1);
  }
  return "X";
}