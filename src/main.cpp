#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ArduinoJson.h>
#include "secrets.h"   // WIFI_SSID, WIFI_PASSWORD, MQTT_SERVER_IP, MQTT_SERVER_PORT, CLIENT_ID, ACCESS_TOKEN

// ======== DHT-config ========
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ======== Global variables ========
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Standard interval if one is not retrieved from thingsboard
unsigned long measureIntervalSec = 600;

#define SLEEPTIME 600

unsigned long previousMeasureMillis = 0;

void measureAndSend() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Misslyckades läsa från DHT!");
    return;
  }

  // Build the JSON payload
  String payload = "{\"temperature\":";
  payload += temperature;
  payload += ",\"humidity\":";
  payload += humidity;
  payload += "}";

  mqttClient.publish("v1/devices/me/telemetry", payload.c_str());
  Serial.println("Published data to ThingsBoard!");
  Serial.printf("Temp: %.1f °C, Humidity: %.1f %%\n", temperature, humidity);
}

// ----------------------------------------------------------------
//  Callback for MQTT (Attribute updates etc)
// ----------------------------------------------------------------
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received a message from topic: ");
  Serial.println(topic);

  StaticJsonDocument<256> doc;
  DeserializationError err = deserializeJson(doc, payload, length);
  if (err) {
    Serial.println("Could not parse JSON in callback.");
    return;
  }

  // Updates on topic "v1/devices/me/attributes"
  if (strcmp(topic, "v1/devices/me/attributes") == 0) {
    if (!doc.isNull()) {
      if (doc.containsKey("deepSleepSec")) {
        measureIntervalSec = doc["deepSleepSec"];
        Serial.printf("deepSleepSec updated to: %lu\n", measureIntervalSec);
      }
    }
  }
  // 2) Response on attribute requests: "v1/devices/me/attributes/response/..."
  else if (String(topic).startsWith("v1/devices/me/attributes/response/")) {
    JsonObject sharedObj = doc["shared"];
    if (!sharedObj.isNull()) {
      if (sharedObj.containsKey("deepSleepSec")) {
        measureIntervalSec = sharedObj["deepSleepSec"];
        Serial.printf("Retrieved deepSleepSec: %lu\n", measureIntervalSec);
      }
    }
  }
}

void setupWifi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP-address: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.printf("Attempting connection to TB on %s:%d ...\n", MQTT_SERVER_IP, MQTT_SERVER_PORT);

    if (mqttClient.connect(CLIENT_ID, ACCESS_TOKEN, NULL)) {
      Serial.println("Connected to ThingsBoard!");
    } else {
      Serial.printf("Failed (error code %d). Reattempting connection in 5s...\n", mqttClient.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(300);

  dht.begin();

  setupWifi();
  mqttClient.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  mqttClient.setCallback(mqttCallback);

  connectMQTT();

  mqttClient.subscribe("v1/devices/me/attributes");

  mqttClient.publish("v1/devices/me/attributes/request/1", "{\"sharedKeys\":\"deepSleepSec\"}");

  Serial.println("Finished setting up");

  measureAndSend();
}

void loop() {
  mqttClient.loop();
  if (!mqttClient.connected()) {
    connectMQTT();
  }

  if (measureIntervalSec > SLEEPTIME) {
    measureAndSend();

    Serial.printf("deepSleepSec = %lu (> %d). Entering deep sleep...\n", measureIntervalSec, SLEEPTIME);

    mqttClient.disconnect();
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    delay(50);

    esp_deep_sleep(measureIntervalSec * 1000000ULL);
  }

  if (millis() - previousMeasureMillis >= measureIntervalSec * 1000UL) {
    previousMeasureMillis = millis();
    Serial.println("\n--- New measurement ---");
    measureAndSend();
  }

  delay(100);
}