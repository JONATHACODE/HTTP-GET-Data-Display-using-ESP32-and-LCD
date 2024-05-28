#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const int ledPin = 2;

LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* serverName = "http://159.89.206.172/input_data/data.php";

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  lcd.print(ssid);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(1000);
    Serial.print(".");
    lcd.setCursor(15, 1);
    lcd.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi connected");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
    
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("");
    Serial.println("Failed to connect to WiFi.");
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi connect");
    lcd.setCursor(0, 1);
    lcd.print("failed");

    while (WiFi.status() != WL_CONNECTED) {
      digitalWrite(ledPin, HIGH);
      delay(500);
      digitalWrite(ledPin, LOW);
      delay(500);
    }
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println(payload);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Data:");
      lcd.setCursor(0, 1);
      lcd.print(payload);

    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("HTTP Error");
      lcd.setCursor(0, 1);
      lcd.print(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
    digitalWrite(ledPin, LOW);
  }

  delay(10000);
}
