# 📘 Fundamentos y aplicación del módulo LoRa SX1278 – JFMDKAVG

## 1. Objetivo General
Elaborar un README que sirva como guía exhaustiva y profesional para la actividad “Fundamentos y aplicación del módulo LoRa SX1278”, facilitando la comprensión del propósito, la instalación, el uso y el contexto ético de un radio LoRa en entornos inteligentes.

## 2. Objetivos Específicos
- Explicar el principio de funcionamiento de la tecnología LoRa y su rol en arquitecturas de sistemas inteligentes.
- Instruir paso a paso en la configuración del hardware y la instalación del software, garantizando la reproducibilidad de la práctica.
- Implementar un transmisor (Arduino UNO) que envíe datos en formato JSON y un gateway (ESP32) que reciba, agregue métricas como RSSI y publique a un broker MQTT.
- Validar el funcionamiento del sistema mediante pruebas de campo, midiendo RSSI, alcance y pérdidas de paquetes, con visualización en una aplicación web y exportación a CSV.
- Reflexionar sobre las consideraciones éticas y de seguridad en el uso de tecnologías LoRa dentro de entornos IoT, promoviendo un uso responsable y seguro. 

## 3. Competencias
- Manejo de módulos LoRa SX1278  
- Integración hardware–software en IoT  
- Uso de protocolos MQTT y JSON  
- Documentación técnica profesional  

## 4. Tabla de Contenidos
1. Objetivo General  
2. Objetivos Específicos  
3. Competencias  
4. Tabla de Contenidos  
5. Descripción  
6. Requisitos  
7. Instalación y Configuración  
8. Conexiones de Hardware  
9. Parámetros Técnicos del SX1278  
10. Uso y Ejemplos de Código  
11. Resultados de Prueba  
12. Consideraciones Éticas y de Seguridad  
13. Formato de Salida (JSON)  
14. Solución de Problemas  
15. Contribuciones  
16. Referencias  

## 5. Descripción
La presente práctica se centra en la implementación de un sistema de comunicación inalámbrica de largo alcance y bajo consumo mediante el uso del módulo LoRa SX1278. Esta tecnología resulta fundamental en el desarrollo de entornos inteligentes, ya que permite enlazar dispositivos IoT distribuidos en áreas extensas con un consumo energético mínimo, lo que favorece la eficiencia y la escalabilidad de los sistemas. En este caso, se utiliza un nodo transmisor basado en Arduino UNO y un gateway con ESP32, donde se transmite información en formato JSON y se publica a través de MQTT para su visualización en un dashboard web. La relevancia de la práctica radica en que reproduce un escenario real de monitorización y control en ciudades inteligentes, campus universitarios o aplicaciones agrícolas, mostrando cómo LoRa puede integrarse de manera práctica en arquitecturas de IoT críticas para la gestión de datos y la toma de decisiones.

## 6. Requisitos
**Hardware:** Arduino UNO, ESP32, 2 módulos SX1278, antenas, botón, protoboard, cables Dupont  
**Software:** Arduino IDE, Librería LoRa, PubSubClient, MQTT.js o Paho  
**Conocimientos previos:** C++ (Arduino), MQTT, JSON  

## 7. Instalación y Configuración
1. Clonar este repositorio  
2. Instalar librerías **LoRa** y **PubSubClient** en Arduino IDE  
3. Configurar credenciales WiFi en el ESP32  
4. Verificar frecuencia (433 o 915 MHz)  
5. Abrir el dashboard (`dashboard_lora.html`) en el navegador  

## 8. Conexiones de Hardware

| Señal SX1278 | Arduino UNO | ESP32 | Función |
|--------------|-------------|-------|---------|
| 3V3          | 3V3         | 3V3   | Alimentación |
| GND          | GND         | GND   | Tierra común |
| NSS          | D10         | GPIO5 | Chip Select |
| MOSI         | D11         | GPIO23| SPI MOSI |
| MISO         | D12         | GPIO19| SPI MISO |
| SCK          | D13         | GPIO18| SPI Clock |
| RST          | D9          | GPIO14| Reset |
| DIO0         | D2          | GPIO26| Interrupción |

📸 _[Aquí inserta fotos de tu armado físico]_  

## 9. Parámetros Técnicos del SX1278

| Parámetro                | Valor típico        | Unidad de medida |
|---------------------------|---------------------|------------------|
| Voltaje de operación      | 3.3                 | V                |
| Corriente en transmisión  | 120                 | mA               |
| Frecuencia de operación   | 433 / 868 / 915     | MHz              |
| Potencia de salida        | hasta 20            | dBm              |
| Interfaz de comunicación  | SPI                 | –                |


## 10. Uso y Ejemplos de Código

En este apartado se muestran fragmentos esenciales de código que permiten comprender el flujo de trabajo de la práctica: el nodo transmisor (Arduino UNO), el gateway (ESP32) y la aplicación web que se conecta por MQTT.

**Arduino UNO (Transmisor – envío JSON por botón):**
```cpp
#define TEAM_ID "JFMDKAVG" 

if (digitalRead(BTN_PIN) == LOW) {
  counter++;
  String payload = String("{\"Equipo\":\"") + TEAM_ID + 
                   "\",\"Contador\":" + counter + "}";
  LoRa.beginPacket();
  LoRa.print(payload);
  LoRa.endPacket();
} 
```
**ESP32 (Gateway – recibe, añade RSSI y publica MQTT):**
```cpp
const char* MQTT_TOPIC = "iot/lora/JFMDKAVG/gateway";

String out = in.substring(0, endIdx) + 
             ",\"RSSI\":" + rssi + 
             ",\"timestamp\":" + ts + "}";
mqtt.publish(MQTT_TOPIC, out.c_str());
```
**Aplicación Web (suscripción MQTT.js – tabla + CSV):**
```cpp
const url = "wss://test.mosquitto.org:8081/mqtt";
const topic = "iot/lora/JFMDKAVG/gateway";

const client = mqtt.connect(url);
client.on("connect", () => client.subscribe(topic));
client.on("message", (t, p) => {
  const data = JSON.parse(p.toString());
  // Insertar fila en tabla con Timestamp, Equipo, Contador y RSSI
});
```
## 11. Resultados de Prueba

Durante las pruebas se ejecutó la comunicación punto a punto, confirmando que el transmisor envió mensajes en formato JSON y que el gateway los recibió, añadió RSSI y los publicó en MQTT. La aplicación web mostró en tiempo real los datos en una tabla y permitió exportarlos en CSV.
En este repositorio se puede observar el codigo del funcionamiento.

## 12. Consideraciones Éticas y de Seguridad
- Respetar las normas del espectro ISM al trabajar con LoRa.
- Nunca encender el módulo sin antena, ya que puede dañarse.
- No publicar credenciales de WiFi ni datos sensibles en el repositorio.
- Para aplicaciones reales, se recomienda habilitar TLS y autenticación en el broker MQTT.

## 13. Formato de Salida (JSON)
El formato estándar del mensaje publicado es:
{
  "Equipo": "JFMDKAVG",
  "Contador": 5,
  "RSSI": -72,
  "timestamp": 1693928392
}
Este JSON facilita la integración con otros sistemas IoT y el análisis en plataformas de Big Data.

## 14. Solución de Problemas
- LoRa no inicializa: verificar que la alimentación sea 3.3 V estable.
- No hay recepción: confirmar que ambos nodos usen la misma frecuencia (433 o 915 MHz).
- MQTT no conecta: probar con puertos 8080/8081 y alternar SSL = Sí/No.
- La web no muestra datos: revisar consola del navegador para ver errores de conexión WS.

## 15. Contribuciones
Para contribuir a este proyecto:
1. Haz un fork del repositorio.
2. Crea una rama nueva: feature/nombre.
3. Realiza commits claros y documentados.
4. Abre un Pull Request para revisión.

## 16. Referencias (APA)

- Arduino. (s. f.). *Arduino Uno Rev3 technical reference*. Arduino AG. [https://docs.arduino.cc/hardware/uno-rev3](https://docs.arduino.cc/hardware/uno-rev3)  
- Espressif Systems. (2022). *ESP32 Series datasheet*. Espressif. [https://www.espressif.com](https://www.espressif.com)  
- IETF. (2014). *The WebSocket Protocol (RFC 6455)*. Internet Engineering Task Force. [https://datatracker.ietf.org/doc/html/rfc6455](https://datatracker.ietf.org/doc/html/rfc6455)  
- IETF. (2014). *The JavaScript Object Notation (JSON) Data Interchange Format (RFC 7159)*. Internet Engineering Task Force. [https://datatracker.ietf.org/doc/html/rfc7159](https://datatracker.ietf.org/doc/html/rfc7159)  
- LoRa Alliance. (2018). *LoRaWAN® 1.0.3 specification*. LoRa Alliance. [https://lora-alliance.org](https://lora-alliance.org)  
- OASIS. (2014). *MQTT version 3.1.1*. OASIS Standard. [https://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html](https://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html)  
- O’Reilly, L. (2007). *RESTful Web Services*. O’Reilly Media.  
- Semtech Corporation. (2016). *SX1278 LoRa Transceiver Datasheet (Rev. F)*. Semtech.  
- Semtech Corporation. (2015). *LoRa Modem Design Guide*. Semtech.  