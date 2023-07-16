/* Defines */

// GPIO14 LED INDICADOR WIFI
#define WIFILED 14
// GPIO32 LED INDICADOR MQTT

#define PuertoCorriente1 36         // edicion de Corriente Corriente1 Original 36
#define PuertoCorriente2 39         // Medicion de Corriente Corriente2  Original
#define PuertoGas 34                // Medicion de Corriente Gas   Original 34
#define PuertoTemperaturaH1 35      // Medicion de Temperatura TemperaturaH1
#define PuertoTemperaturaH2 32      // Medicion de Temperatura TemperaturaH2
#define PuertoTemperaturaAmbiente 2 // Medicion de Temperatura Ambiente 33 - 2
#define PuertoTensionGeneral 33     // Medicion temperatura original 2 -> 33
#define PuertoTensionH1 4
#define PuertoTensionH2 15

#define PuertoAlarmaIntrusion 15
#define PuertoAlarmaH1 2
#define PuertoAlarmaH2 4

#define Puerto0 23 // Salida FAN1
#define Puerto1 22 // Salida FAN2
#define Puerto2 21

/*
#define MQTTLED 32
 GPIO LED2
#define LED2 2

boolean Led2Estado;
*/
/* Variables Globales */
// Variable para no bloquear la reconexion del MQTT
long lastSendWS = 0;
// Tamaño del Objeto JSON
const size_t JSON_CONFIG = JSON_OBJECT_SIZE(5) + 500;
// New mqtt
const size_t JSONMQTT_SIZE = JSON_OBJECT_SIZE(6) + 500;
// ID del dispositivo
char id[30];
// Red WiFi
char ssid[30];
// Contraseña da Red WiFi
char pw[30];
// Para ssid AP
char nameap[31];
// Para ssid AP Password
char passwordap[63];
// Habilitar MQTT Broker
boolean MQTT = true;
// Usuario MQTT Broker
char mqttuser[30];
// ID MQTT Broker
char idMqtt[30];
// Password Usuario MQTT Broker
char mqttpass[30];
// Para Guardar el servidor del MQTT Broker
char mqttserver[30];
// Guardará la última vez que se actualizó el LED
unsigned long previousMillis = 0;
// Intervalo en el cual parpadear (milisegundos)
const long interval = 300;
// Utiliza ledState para configurar el LED
bool ledState = LOW;
// Nuevas Variables declaradas
// Variable para Temperatura de CPU
float TempCPU;
// nuevas variables
// Tiempo para envio por MQTT new
int timeMQTT;
// MQTT
long lastMsg = 0;
// Variable para que la reconexion del MQTT no bloqueante
long lastReconnectAttempt = 0;
// Tamaño del mensaje
char payload[255];
// Tamaño del topico
char topico[150];

/****************************************
 * Sensor Temp Interno CPU
 ****************************************/
// Para ESP32 temperatura del CPU
#ifdef __cplusplus
extern "C"
{
#endif
    uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

/* Instancias */

/****************************************
 * Init MQTT
 ****************************************/
WiFiClient espClient;
PubSubClient client(espClient);
/****************************************
 * Init DNS
 ****************************************/
const byte DNSSERVER_PORT = 53;
DNSServer dnsServer;

// Variables para medir variables de ambiente
//******************************************

float TemperaturaH1;       // Temperatura 1
float TemperaturaH2;       // Temperatura 2
float TemperaturaAmbiente; // Temperatura Ambiente
float TensionGeneral;      // Tension General
char TensionGeneralC[10];
float TensionH1;           // Tension H1
float TensionH2;           // Tension H2
float magnetismoInterno;   // Magnetismo del ESP32
int EstadoDeSistema;
int AlarmaIntrusion;
int AlarmaH1;
int AlarmaH2;
int EstadoPuerto0;
int EstadoPuerto1;
int EstadoPuerto2;
int Corriente1; // Corriente 1
int Corriente2; // Corriente 2
int Gas;        // Corriente 3

const float RL = 18; // Kohm
float RS;
float RO;

float Intencidad1;
float Intencidad2;
float Intencidad3;
float C_RMS1;
float C_RMS2;
float C_RMS3;
float C_Gas;
float OffsetCorriente1;

int GAS_LPG = 0;
int GAS_CO = 1;
int GAS_SMOKE = 2;
int GAS_H = 3;

long iPPM_LPG;
long iPPM_CO;
long iPPM_Smoke;
