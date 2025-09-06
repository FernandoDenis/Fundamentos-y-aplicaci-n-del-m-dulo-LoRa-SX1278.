// UNO TRANSMISOR: LoRa + Botón -> JSON {Equipo, Contador}
// Librerías: LoRa by Sandeep Mistry

#include <SPI.h>
#include <LoRa.h>

#define LORA_SS   10
#define LORA_RST   9
#define LORA_DIO0  2

#define BTN_PIN    3   
#define TEAM_ID   "JFMDKAVG"  

// Ajustes LoRa
const long FREQ = 433E6;  // Cambio de frequencia dependiendo de tu Lora
const int  TX_POWER = 10; 

// ISR y debounce
volatile bool btnISR = false;     
unsigned long lastPressMs = 0;
const unsigned long debounceMs = 120;
unsigned long counter = 0;

void onButton() {                 
  btnISR = true;               
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  pinMode(BTN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), onButton, FALLING);

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(FREQ)) {
    Serial.println("Fallo al inicializar LoRa");
    while (1) {}
  }
  LoRa.setTxPower(TX_POWER);
  // Opcionales de radio:
  // LoRa.setSpreadingFactor(7);        // 7..12
  // LoRa.setSignalBandwidth(125E3);    // 125k, 250k, 500k
  // LoRa.setCodingRate4(5);            // 5..8

  Serial.println("LoRa inicializado (TX). Pulsa el botón para enviar.");
}

void loop() {
  if (btnISR) {
    btnISR = false;  // consumimos el evento

    unsigned long now = millis();
    // Debounce en el loop (mejor que dentro del ISR)
    if (now - lastPressMs > debounceMs) {
      // Confirmamos que el botón realmente está presionado (activo en LOW)
      if (digitalRead(BTN_PIN) == LOW) {
        lastPressMs = now;
        counter++;

        // Armamos JSON manual (ahorra RAM en UNO)
        String payload = String("{\"Equipo\":\"") + TEAM_ID +
                         "\",\"Contador\":" + String(counter) + "}";

        LoRa.beginPacket();
        LoRa.print(payload);
        LoRa.endPacket();

        Serial.print("Enviado: ");
        Serial.println(payload);
      }
    }
  }
}
