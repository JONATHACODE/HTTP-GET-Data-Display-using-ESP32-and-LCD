#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Ganti dengan kredensial jaringan Anda
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Define the LED pin
const int ledPin = 2;

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// URL dari mana data akan diambil
const char* serverName = "http://159.89.206.172/input_data/data.php";

void setup() {
  // Initialize serial monitor
  Serial.begin(115200);

  // Initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // LED off initially

  // Initialize I2C
  Wire.begin(21, 22); // SDA: GPIO 21, SCL: GPIO 22

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting to");
  lcd.setCursor(0, 1);
  lcd.print(ssid);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // Tambahkan timeout untuk koneksi
  unsigned long startAttemptTime = millis();

  // Loop untuk menunggu koneksi
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(1000);
    Serial.print(".");
    lcd.setCursor(15, 1);
    lcd.print(".");
  }

  // Cek status koneksi
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Update LCD display
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi connected");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
    
    // Turn on the LED to indicate successful connection
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("");
    Serial.println("Failed to connect to WiFi.");
    
    // Update LCD display
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi connect");
    lcd.setCursor(0, 1);
    lcd.print("failed");

    // Blink the LED to indicate failure
    while (true) {
      digitalWrite(ledPin, HIGH);
      delay(500);
      digitalWrite(ledPin, LOW);
      delay(500);
    }
  }
}

void loop() {
  // Cek koneksi WiFi sebelum membuat permintaan
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName); // Inisialisasi objek HTTP

    int httpResponseCode = http.GET(); // Membuat permintaan GET

    if (httpResponseCode > 0) { // Jika kode respons > 0, permintaan berhasil
      String payload = http.getString(); // Mendapatkan payload sebagai string
      Serial.println(payload); // Tampilkan payload di serial monitor

      // Tampilkan payload di LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Data:");
      lcd.setCursor(0, 1);
      lcd.print(payload);

    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      
      // Tampilkan error di LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("HTTP Error");
      lcd.setCursor(0, 1);
      lcd.print(httpResponseCode);
    }

    http.end(); // Menutup koneksi
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(10000); // Tunggu 10 detik sebelum mengirim permintaan lagi
}
