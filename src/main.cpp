/***************************************************
 * CUBAELECTRONICA
 * Configuración con JSON e SPIFFS
 * Plataforma ESP32
 * Proyecto Admin Panel for ESP32
 * 05/2021 - Ing. Yamir Hidalgo Peña
 * admin@cubaelectronica.com
 * https://cubaelectronica.com
 * Licencia: MIT

 ___           _             ___    _                  _
(  _`\        ( )           (  _`\ (_ )               ( )_                     _
| ( (_) _   _ | |_      _ _ | (_(_) | |    __     ___ | ,_) _ __   _     ___  (_)   ___    _ _
| |  _ ( ) ( )| '_`\  /'_` )|  _)_  | |  /'__`\ /'___)| |  ( '__)/'_`\ /' _ `\| | /'___) /'_` )
| (_( )| (_) || |_) )( (_| || (_( ) | | (  ___/( (___ | |_ | |  ( (_) )| ( ) || |( (___ ( (_| |
(____/'`\___/'(_,__/'`\__,_)(____/'(___)`\____)`\____)`\__)(_)  `\___/'(_) (_)(_)`\____)`\__,_)

**************** Librerias *************************/

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <SPIFFS.h>
#include "esp32-hal-cpu.h"
#include <DNSServer.h>
#include <PZEM004Tv30.h>

/************** Archivos de Apoyo ******************/
#include "header.hpp"
#include "functions.hpp"
#include "ESP32_MQTT.hpp"
#include "ConfigReset.hpp"
#include "ConfigRead.hpp"
#include "ConfigSave.hpp"
#include "ESP32_WIFI.hpp"
// nuevo codico
#include "ESP32_API.hpp"
#include "Websocket.hpp"
#include "Server.hpp"
#include "ESP32_WS.hpp"

#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#endif

PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);

/************** Setup ******************/
void setup()
{
  /* SERIAL BAUDRATE */
  Serial.begin(115200);
  /* CPU FREQU */
  setCpuFrequencyMhz(240);
  log("\nInfo: Iniciando Setup");
  /* Configurar los Pines */
  configPines();
  /* Parpadeo LEDs */
  led();
  lastSendWS = 0;
  /* SPIFFS */
  if (!SPIFFS.begin())
  {
    log(F("\nError: Fallo la inicialización del SPIFFS ERROR"));
    while (true)
      ;
  }
  /* Lee la Configuración */
  configRead();
  /* Guarda la Configuración */
  configSave();
  /* Configuracion WIFI */
  WiFi.disconnect(true);
  delay(10);
  // WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);

  configWiFi();
  // Lee la Configuración MQTT
  configReadMQTT();
  // Guarda la Configuración MQTT
  configSaveMQTT();
  /* MQTT */
  if (MQTT)
  {
    client.setServer(mqttserver, 1883);
    client.setCallback(callback);
    log("\nInfo: Cliente MQTT Habilitado");
  }
  else
  {
    log("\nInfo: Cliente MQTT Deshabilitado");
  }
  /* Iniciamos el WebSocket */
  InitWebSockets();
  /* Iniciamos el Server */
  InitServer();
  /* Listo */
  log("\nInfo: Setup completado");

  //**********************************
  //****SET Puertos*******************
  //**********************************
  pinMode(Puerto0, OUTPUT);
  pinMode(Puerto1, OUTPUT);
  pinMode(Puerto2, OUTPUT);

  pinMode(PuertoCorriente1, INPUT);
  pinMode(PuertoCorriente2, INPUT);
  pinMode(PuertoGas, INPUT);
  pinMode(PuertoTemperaturaH1, INPUT);
  pinMode(PuertoTemperaturaH2, INPUT);
  pinMode(PuertoTemperaturaAmbiente, INPUT);

  pinMode(PuertoTensionGeneral, INPUT);
  pinMode(PuertoTensionH1, INPUT);
  pinMode(PuertoTensionH2, INPUT);

  pinMode(PuertoAlarmaIntrusion, INPUT);
  pinMode(PuertoAlarmaH1, INPUT);
  pinMode(PuertoAlarmaH2, INPUT);

  //************************************
  //********Definion de funciones*******
  //************************************

  EstadoDeSistema = 1;
  AlarmaIntrusion = 1;
  AlarmaH1 = 1;
  AlarmaH2 = 1;
  CalculoRO();
}

/************** Bucle Infinito ******************/
void loop()
{
  Reloj(); // Calcula hora

  Serial.println(horas);
  Serial.println(minutos);
  Serial.println(segundos);
  /*** Usar si va a tardar mucho tiempo el Loop */
  // yield();
  /************* Portal Captivo en el ESP32 *****/
  dnsServer.processNextRequest();
  /************ Enviar JSON por WS cada segundo */
  if (millis() - lastSendWS > 1000)
  {
    lastSendWS = millis();
    WsMessage(GetJson(), "");
  }
  /************ Leer la temp del CPU *************/
  TempCPU = (temprature_sens_read() - 32) / 1.8;
  /************ MQTT envio de Datos  *************/
  if (MQTT)
  {

    if (!client.connected() && WiFi.status() == WL_CONNECTED)
    {
      // para parpadear un led cuando esta conectandose al wifi no bloqueante
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval)
      {
        // * Guarda la última vez que parpadeó el LED.
        previousMillis = currentMillis;
        // * Si el LED está apagado, enciéndalo y viceversa:
        ledState = not(ledState);
        // * configurar el LED con el ledState de la variable:
        // digitalWrite(MQTTLED, ledState);
      }
      long now = millis();
      // intenta conectarse a cada un minuto.
      if (now - lastReconnectAttempt > 1000)
      {
        lastReconnectAttempt = now;
        // Attempt to reconnect
        if (reconnect())
        {
          lastReconnectAttempt = 0;
        }
      }
    }
    else
    {
      // Client connected
      client.loop();
      // digitalWrite(MQTTLED, LOW);
    }

    // Publicar segun el Tiempo MQTT el mensaje
    if (client.connected())
    {
      if (millis() - lastMsg > timeMQTT)
      {

        lastMsg = millis();
        // Usuario MQTT Para Armar el Topico
        const String userMQTT = mqttuser;

        String payload_to_send = MQTTJson();
        payload_to_send.toCharArray(payload, 300);
        String topic_aux_to_send = userMQTT + "/" + device_id + "/valores";
        topic_aux_to_send.toCharArray(topico, 150);

        if (client.publish(topico, payload))
        {
          log("\nInfo: Mansaje enviado por MQTT");
        }
        else
        {
          log("\nError: Mansaje no enviado por MQTT");
        }
      }
    }
  }

  int C_RMS1_1;
  Corriente1 = 0;          // Corriente 1
  Corriente2 = 0;          // Corriente 2
  Gas = 0;                 // Corriente 3
  TemperaturaH1 = 0;       // Temperatura 1
  TemperaturaH2 = 0;       // Temperatura 2
  TemperaturaAmbiente = 0; // Temperatura Ambiente
  TensionGeneral = 0;      // Tension General
  TensionH1 = 0;           // Tension H1
  TensionH2 = 0;           // Tension H2

  //*******************************
  //**********Medicion PZEM********
  //*******************************

  // Print the custom address of the PZEM
  Serial.print("Custom Address:");
  Serial.println(pzem.readAddress(), HEX);

  // Read the data from the sensor
  voltage1 = pzem.voltage();
  if (voltage1 > voltage1Max)
    voltage1Max = voltage1;
  if (voltage1 < voltage1Min)
    voltage1Max = voltage1;

  current1 = pzem.current();
  if (current1 >= current1Max)
    current1Max = current1;
  else
    current1Min = current1;

  power1 = pzem.power();
  if (power1 >= power1Max)
    power1Max = power1;
  else
    power1Min = power1;

  energy1 = pzem.energy();
  if (energy1 >= energy1Max)
    energy1Max = energy1;
  else
    energy1Min = energy1;

  frequency1 = pzem.frequency();
  if (frequency1 >= frequency1Max)
    frequency1Max = frequency1;
  else
    frequency1Min = frequency1;

  pf1 = pzem.pf();
  if (pf1 >= pf1Max)
    pf1Max = pf1;
  else
    pf1Min = pf1;

  // Check if the data is valid
  if (isnan(voltage1))
  {
    Serial.println("Error reading voltage");
  }
  else if (isnan(current1))
  {
    Serial.println("Error reading current");
  }
  else if (isnan(power1))
  {
    Serial.println("Error reading power");
  }
  else if (isnan(energy1))
  {
    Serial.println("Error reading energy");
  }
  else if (isnan(frequency1))
  {
    Serial.println("Error reading frequency");
  }
  else if (isnan(pf1))
  {
    Serial.println("Error reading power factor");
  }
  else
  {

    // Print the values to the Serial console
    Serial.print("Voltage: ");
    Serial.print(voltage1);
    Serial.println("V");
    Serial.print("Current: ");
    Serial.print(current1);
    Serial.println("A");
    Serial.print("Power: ");
    Serial.print(power1);
    Serial.println("W");
    Serial.print("Energy: ");
    Serial.print(energy1, 3);
    Serial.println("kWh");
    Serial.print("Frequency: ");
    Serial.print(frequency1, 1);
    Serial.println("Hz");
    Serial.print("PF: ");
    Serial.println(pf1);
  }

  Serial.println();

  //*******************************

  //*****************************
  //***Medicion de Corriente*****
  //*****************************

  C_RMS1 = 0;
  Serial.println("Inicio toma de muestras");
  for (int i = 0; i < 3000; i++) // 10000
  {

    Corriente1 = analogRead(PuertoCorriente1);
    // Corriente2 = analogRead(PuertoCorriente2);

    if (Corriente1 > 4095) // Corta serca de 1023 (corriente maxima del conversor hay que ver si existen corrientes picos mayores)
    {

      // digitalWrite(Puerto1, HIGH);
    }
    else
    {
      // digitalWrite(Puerto1, LOW);
    }

    Intencidad1 = (Corriente1 * (37.8786 / 2047)) - 37.8786; // OffsetCorriente1; //37.8786    -- 37.8786 / 2300)
    // Intencidad2 = (Corriente2 * (37.8786 / 2047)) - 37.8786;
    // Gas = Gas * (37.8786 / 2047)) - 37.8786;

    C_RMS1 = C_RMS1 + pow(Intencidad1, 2);
    // C_RMS2 = C_RMS2 + pow(Intencidad2, 2);

    // C_RMS3 = C_RMS3 + pow(Intencidad3, 2); Elimino para medir GAS

    TemperaturaH1 = TemperaturaH1 + analogRead(PuertoTemperaturaH1);
    // TemperaturaH2 = TemperaturaH2 + analogRead(PuertoTemperaturaH2);
    // TemperaturaAmbiente = TemperaturaAmbiente; //+ analogRead(PuertoTemperaturaAmbiente);
    TensionGeneral = TensionGeneral + analogRead(PuertoTensionGeneral);
  }
  Serial.println("Fin toma de muestras");
  delay(10);

  TemperaturaH1 = TemperaturaH1 / 3000;
  // TemperaturaH2 = TemperaturaH2/3000;
  // TemperaturaAmbiente = TemperaturaAmbiente/3000;
  C_RMS1 = C_RMS1 / 3000; // Calibrado 24/11/2020 con Goldstar DM7333.
  // C_RMS2 = C_RMS2 / 3000;
  // C_RMS3 = C_RMS3 / 3000;

  // C_Gas=RL*(4096-C_Gas)/(C_Gas*RO);
  C_Gas = 0;
  for (int i = 1; i < 300; i++)
  {
    C_Gas = C_Gas + analogRead(PuertoGas);
    // delay(10);
  }
  C_Gas = C_Gas / 300;

  C_Gas = 3.3 * (C_Gas / 4095);
  Serial.println("Gas-Volt:");
  Serial.println(C_Gas);
  C_Gas = RL * (5 - C_Gas) / C_Gas; // cabio 3.3 por 5
  Serial.println("RS:");
  Serial.println(C_Gas);
  Serial.println("RO:");
  Serial.println(RO);
  C_Gas = C_Gas / RO;

  Serial.println("RS/RO:");
  Serial.println(C_Gas);

  iPPM_LPG = 964.02 * (pow(C_Gas, -2.101));
  if (iPPM_LPG <= 100)
    iPPM_LPG = 0;
  if (iPPM_LPG > 10000)
    iPPM_LPG = 22222;

  // iPPM_CO = MQGetGasPercentage(C_Gas/RO,GAS_CO);
  // iPPM_Smoke = MQGetGasPercentage(C_Gas/RO,GAS_SMOKE);

  C_RMS1 = sqrt(C_RMS1) - 0.60;
  C_RMS1_1 = 100 * C_RMS1;
  Serial.print("C_RMS1 flota: ");
  Serial.print(C_RMS1);
  C_RMS1 = int(C_RMS1);
  Serial.print("  C_RMS1: ");
  Serial.print(C_RMS1);
  Serial.print("  C_RMS2: ");
  Serial.print(C_RMS2);
  Serial.print(" C_RMS3: ");
  Serial.print(C_RMS3);
  Serial.print(" C_GAS: ");
  Serial.print(C_Gas);

  Serial.print(" IPPM_LPG: ");
  Serial.print(iPPM_LPG);

  if (C_RMS1_1 < 150)
  {
    C_RMS1 = 0;
    Serial.println("C_RMS1_1 < 150");
  }
  else
  {
    if (C_RMS1_1 >= 150 && C_RMS1_1 <= 500)
    {
      C_RMS1 = int((C_RMS1 * 1.35));
      Serial.println("C_RMS1 >= 1 && C_RMS1 <= 6");
    }
    else
    {
      C_RMS1 = int((C_RMS1 * 1.10) + 0.5); // *1.3
      Serial.println("C_RMS1 = C_RMS1");
    }
  }

  Serial.println(C_RMS1);
  //***************************************
  //*******Fin Medion de Corriente*********
  //***************************************

  //***************************************
  //**********Medicion de temperatura******
  //***************************************

  // TemperaturaH1 = TemperaturaH1 / 10000;
  TemperaturaH1 = (TemperaturaH1 - 2763) * (-0.0184) + 25;
  // TemperaturaH2 = TemperaturaH2 / 10000;
  TemperaturaH2 = (TemperaturaH2 - 2763) * (-0.0184) + 25;
  // TemperaturaAmbiente = TemperaturaAmbiente / 10000;
  TemperaturaAmbiente = (TemperaturaAmbiente - 2763) * (-0.0184) + 25;

  TensionGeneral = 0;
  TensionGeneral = TensionGeneral / 21000; // 34900;
  TensionGeneral = (TensionGeneral * 1.1515) - 56.5;
  TensionGeneral = int(TensionGeneral);

  // TensionGeneral = 221.9; //Borrar
  // TensionH1 = TensionH1 / 10000;
  TensionH1 = 0; // Borrar
  // TensionH2 = TensionH2 / 10000;
  TensionH2 = 0; // Borrar
  magnetismoInterno = hallRead();

  if (TemperaturaH1 > 60)
  {
    digitalWrite(Puerto0, HIGH);
    EstadoPuerto0 = 1;
  }
  else
  {
    digitalWrite(Puerto0, LOW);
    EstadoPuerto0 = 0;
  }

  AlarmaIntrusion = digitalRead(PuertoAlarmaIntrusion);
  AlarmaH1 = digitalRead(PuertoAlarmaH1);
  AlarmaH2 = digitalRead(PuertoAlarmaH2);
  Serial.print("AlarmaIntrusion: ");
  Serial.print(AlarmaIntrusion);
  Serial.print("  AlarmaAlarmaH1: ");
  Serial.println(AlarmaH1);
  Serial.print("  AlarmaH2: ");
  Serial.print(AlarmaH2);
  Serial.print("  EstadoPuerto0: ");
  Serial.print(EstadoPuerto0);
  Serial.print("  EstadoPuerto1: ");
  Serial.println(EstadoPuerto1);
  Serial.print("  EstadoPuerto2: ");
  Serial.println(EstadoPuerto2);
  Serial.print(" Tensinon General: ");
  Serial.println(TensionGeneral);
  Serial.println(String(TensionGeneral));

  /* ***********************************************************
  **** Reviso tiempo con millisecons y disparo si paso un minuto
  ***************************************************************
  */
}