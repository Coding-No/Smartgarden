#define BLYNK_TEMPLATE_ID "TMPL65tFMSU_7"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "zmSL2gE4GGjgin9vixZlC771wK5anAF1"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN D6         // Pin yang digunakan untuk sensor DHT22
#define DHTTYPE DHT22     // Tipe sensor DHT22



char auth[] = "zmSL2gE4GGjgin9vixZlC771wK5anAF1";  // Masukkan token otentikasi Blynk
char ssid[] = "Gratiss";  // Masukkan nama jaringan WiFi Anda
char pass[] = "Jangan Beli";  // Masukkan kata sandi WiFi Anda

#define RELAY_PIN D5  // Pin yang digunakan untuk relay 1
#define RELAY_PIN_2 D7 // Pin yang digunakan untuk relay 2
#define RELAY_PIN_3 D4 // Pin yang digunakan untuk relay 3

bool systemEnabled = true; // Sistem diaktifkan secara default
bool relay2State = false;  // State relay 2 (Off)
bool relay3State = false;  // State relay 3 (Off)

BlynkTimer timer;
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2); //Alamat lcd

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(RELAY_PIN_3, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Matikan relay 1 saat booting
  digitalWrite(RELAY_PIN_2, HIGH); // Matikan relay 2 saat booting
  digitalWrite(RELAY_PIN_3, HIGH); // Matikan relay 3 saat booting

  dht.begin();

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Suhu  : ");
  lcd.setCursor(0, 1);
  lcd.print("Lembab: ");
}

void loop()
{
  Blynk.run();
  timer.run();

  float temperature = dht.readTemperature(); // Baca suhu dari sensor DHT22
  float humidity = dht.readHumidity();       // Baca kelembaban dari sensor DHT22

  if (!isnan(temperature) && !isnan(humidity)) {
    Blynk.virtualWrite(V5, temperature); // Kirim data suhu ke pin virtual V5
    Blynk.virtualWrite(V6, humidity);    // Kirim data kelembaban ke pin virtual V6

    lcd.setCursor(8, 0);
    lcd.print(temperature, 1);
    lcd.print(" C");

    lcd.setCursor(8, 1);
    lcd.print(humidity, 1);
    lcd.print(" %");

    if (temperature > 31 && !relay2State) {
      activateRelay2();
    }
    else if (temperature < 30 && relay2State) {
      deactivateRelay2();
    }

    if (temperature < 27 && !relay3State) {
  activateRelay3();
}
else if (temperature > 28 && relay3State) {
  deactivateRelay3();
}

  }
}

// Fungsi untuk menghidupkan relay 1
void activateRelay()
{
  if (!systemEnabled) {
    digitalWrite(RELAY_PIN, LOW); // Hidupkan relay 1
    Serial.println("Relay 1 diaktifkan");
    timer.setTimeout(10000L, deactivateRelay); // Matikan relay 1 setelah 20 detik
    systemEnabled = true; // Aktifkan sistem
  }
}

// Fungsi untuk mematikan relay 1
void deactivateRelay()
{
  if (systemEnabled) {
    digitalWrite(RELAY_PIN, HIGH); // Matikan relay 1
    Serial.println("Relay 1 dinonaktifkan");
    timer.setTimeout(86400000L, activateRelay); // Hidupkan relay 1 setelah 40 detik
    systemEnabled = false; // Nonaktifkan sistem
  }
}

// Fungsi untuk menghidupkan relay 2
void activateRelay2()
{
  if (!relay2State) {
    digitalWrite(RELAY_PIN_2, LOW); // Hidupkan relay 2
    Serial.println("Relay 2 diaktifkan");
    relay2State = true; // Set state relay 2 menjadi On
  }
}

// Fungsi untuk mematikan relay 2
void deactivateRelay2()
{
  if (relay2State) {
    digitalWrite(RELAY_PIN_2, HIGH); // Matikan relay 2
    Serial.println("Relay 2 dinonaktifkan");
    relay2State = false; // Set state relay 2 menjadi Off
  }
}

// Fungsi untuk menghidupkan relay 3
void activateRelay3()
{
  if (!relay3State) {
    digitalWrite(RELAY_PIN_3, LOW); // Hidupkan relay 3
    Serial.println("Relay 3 diaktifkan");
    relay3State = true; // Set state relay 3 menjadi On
  }
}

// Fungsi untuk mematikan relay 3
void deactivateRelay3()
{
  if (relay3State) {
    digitalWrite(RELAY_PIN_3, HIGH); // Matikan relay 3
    Serial.println("Relay 3 dinonaktifkan");
    relay3State = false; // Set state relay 3 menjadi Off
  }
}

// Fungsi untuk memproses tombol di aplikasi Blynk
BLYNK_WRITE(V1)
{
  int value = param.asInt(); // Membaca status tombol On/Off di pin virtual V1

  if (value == 1)
  {
    activateRelay();
  }
  else
  {
    deactivateRelay();
  }
}

// Fungsi untuk memproses tombol reset di aplikasi Blynk
BLYNK_WRITE(V2)
{
  int value = param.asInt(); // Membaca status tombol reset di pin virtual V2

  if (value == 1)
  {
    systemEnabled = true; // Aktifkan sistem
    digitalWrite(RELAY_PIN, HIGH); // Matikan relay 1
    digitalWrite(RELAY_PIN_2, HIGH); // Matikan relay 2
    digitalWrite(RELAY_PIN_3, HIGH); // Matikan relay 3
    Serial.println("Sistem direset");
  }
}

// Fungsi untuk memproses tombol On/Off relay 2 di aplikasi Blynk
BLYNK_WRITE(V3)
{
  int value = param.asInt(); // Membaca status tombol On/Off di pin virtual V3

  if (value == 1)
  {
    activateRelay2();
  }
  else
  {
    deactivateRelay2();
  }
}

// Fungsi untuk memproses tombol On/Off relay 3 di aplikasi Blynk
BLYNK_WRITE(V4)
{
  int value = param.asInt(); // Membaca status tombol On/Off di pin virtual V4

  if (value == 1)
  {
    activateRelay3();
  }
  else
  {
    deactivateRelay3();
  }
}
