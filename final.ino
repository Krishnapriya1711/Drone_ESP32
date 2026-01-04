#include <WiFi.h>
#include "DHT.h"

#define MQ7_PIN    32
#define MQ135_PIN  35
#define DUST_LED   25
#define DUST_VO    34
#define DHTPIN     4
#define DHTTYPE    DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "xxxxxxxxxxxxxx";
const char* password = "xxxxxxxxxxxx";

WiFiServer server(80);

// -------------------- READ DUST SENSOR --------------------
int readDustRaw() {
  digitalWrite(DUST_LED, LOW);
  delayMicroseconds(280);
  int val = analogRead(DUST_VO);
  delayMicroseconds(40);
  digitalWrite(DUST_LED, HIGH);
  delayMicroseconds(9680);
  return val;
}

// -------------------- AIR QUALITY STATUS --------------------
String getAirStatus(int mq7, int mq135, float dust) {

  int score = 0;

  if (mq7 < 2000) score++;
  if (mq135 < 1000) score++;
  if (dust < 80) score++;

  if (score == 3) return "GOOD";
  if (score == 2) return "MODERATE";
  if (score == 1) return "POOR";
  return "HAZARDOUS";
}

// -------------------- HTML WEBPAGE --------------------
String makePage(int mq7, int mq135, float dust, float t, float h, String status) {

  String color =
    (status == "GOOD") ? "green" :
    (status == "MODERATE") ? "orange" :
    (status == "POOR") ? "red" : "purple";

  return
"<!DOCTYPE html><html><head>"
"<meta http-equiv='refresh' content='3'/>"
"<style>"
"body{font-family:Arial;text-align:center;background:#eef2f3;padding:30px;}"
".box{padding:15px;margin:10px;background:#fff;border-radius:10px;"
"box-shadow:0 0 8px rgba(0,0,0,0.2);font-size:20px;}"
".status{font-size:35px;font-weight:bold;color:" + color + ";}"
"</style>"
"</head><body>"

"<h1>ESP32 Live Sensor Dashboard</h1>"

"<div class='box'>MQ7 Value: <b>" + String(mq7) + "</b></div>"
"<div class='box'>MQ135 Value: <b>" + String(mq135) + "</b></div>"
"<div class='box'>Dust Concentration: <b>" + String(dust, 2) + "</b></div>"
"<div class='box'>Temperature: <b>" + String(t, 1) + "</b></div>"
"<div class='box'>Humidity: <b>" + String(h, 1) + " %</b></div>"

"<h2>Air Quality Status</h2>"
"<div class='status'>" + status + "</div>"

"</body></html>";
}

// -------------------- SETUP --------------------
void setup() {
  Serial.begin(115200);

  pinMode(DUST_LED, OUTPUT);
  digitalWrite(DUST_LED, HIGH);

  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

// -------------------- LOOP --------------------
void loop() {

  WiFiClient client = server.available();
  if (!client) return;

  while (!client.available()) delay(1);
  client.readStringUntil('\n');  // ignore header

  // -------- Read sensors --------
  int mq7 = analogRead(MQ7_PIN);
  int mq135 = analogRead(MQ135_PIN);

  int rawDust = readDustRaw();
  float voltage = rawDust * (3.3 / 4095.0);       // correct for ESP32
  float dustDensity = 170.0 * voltage - 0.1;      // mg/m3

  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();

  // -------- Status --------
  String status = getAirStatus(mq7, mq135, dustDensity);

  // -------- Send response --------
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(makePage(mq7, mq135, dustDensity, temp, hum, status));

  delay(1);
  client.stop();
}
