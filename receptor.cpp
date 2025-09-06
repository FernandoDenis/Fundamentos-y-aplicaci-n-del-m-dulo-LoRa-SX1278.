// ESP32 GATEWAY: Recibe LoRa JSON -> añade RSSI -> publica MQTT
// Librerías: LoRa by Sandeep Mistry, PubSubClient (MQTT), WiFi.h

#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <LoRa.h>

// Configuracion del Wifi
const char* WIFI_SSID = "iPhone_de_Fernando";
const char* WIFI_PASS = "tobias1560";

const char* MQTT_HOST = "test.mosquitto.org";
const uint16_t MQTT_PORT = 1883;
// Tópico de salida de MQTT
const char* MQTT_TOPIC = "iot/lora/campus/gateway";

// Pines LoRa (ESP32)
#define LORA_SS   5
#define LORA_RST 14
#define LORA_DIO0 26
const long FREQ = 433E6;  

WiFiClient espClient;
PubSubClient mqtt(espClient);

void wifiConnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.print("\nWiFi OK. IP: "); Serial.println(WiFi.localIP());
}

void mqttConnect() {
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  Serial.print("Conectando MQTT");
  while (!mqtt.connected()) {
    String cid = String("esp32-gw-") + String((uint32_t)ESP.getEfuseMac(), HEX);
    if (mqtt.connect(cid.c_str())) {
      Serial.println("\nMQTT conectado");
    } else {
      Serial.print(".");
      delay(700);
    }
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  wifiConnect();
  mqttConnect();

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(FREQ)) {
    Serial.println("Fallo al inicializar LoRa");
    while (1) {}
  }
  Serial.println("LoRa inicializado (RX). Esperando paquetes...");
}

void loop() {
  if (!mqtt.connected()) mqttConnect();
  mqtt.loop();

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Leemos el JSON entrante (desde el UNO)
    String in = "";
    while (LoRa.available()) in += (char)LoRa.read();

    int rssi = LoRa.packetRssi();

    // Construimos JSON de salida:
    // { "Equipo":"JFMD","Contador":N,"RSSI":-xx,"timestamp":1234567890 }
    String out = String("{\"RSSI\":") + String(rssi) + ",\"timestamp\":" + String((uint32_t)time(NULL)) + ",";
    // Si el entrante ya es {"Equipo":"X","Contador":N}, lo insertamos
    // quitando la llave final y agregando "RSSI" + "timestamp"
    int endIdx = in.lastIndexOf('}');
    String trimmed = (endIdx >= 0) ? in.substring(0, endIdx) : in;
    if (trimmed.length() > 0 && trimmed[0] == '{') {
      out = trimmed + String(",\"RSSI\":") + String(rssi) +
            String(",\"timestamp\":") + String((uint32_t)time(NULL)) + String("}");
    } else {
      // Si llegara basura, publicamos al menos RSSI solo
      out = String("{\"payload\":\"") + in + String("\",\"RSSI\":") + String(rssi) +
            String(",\"timestamp\":") + String((uint32_t)time(NULL)) + String("}");
    }

    Serial.print("RX: "); Serial.print(in);
    Serial.print("  RSSI: "); Serial.println(rssi);

    // Publica a MQTT
    mqtt.publish(MQTT_TOPIC, out.c_str());
    Serial.print("MQTT -> "); Serial.print(MQTT_TOPIC);
    Serial.print(" : "); Serial.println(out);
  }
}
