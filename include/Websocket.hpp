String GetJson()
{
   String response;
   // StaticJsonDocument<3000> jsonDoc;
   StaticJsonDocument<4000> jsonDoc;
   jsonDoc["WFEstatus"] = WiFi.status() == WL_CONNECTED ? F("<span class='label label-success'>CONECTADO</span>") : F("<span class='label label-important'>DESCONECTADO</span>");
   jsonDoc["WFDBM"] = WiFi.status() == WL_CONNECTED ? String(WiFi.RSSI()) : F("0");
   jsonDoc["WFPRC"] = WiFi.status() == WL_CONNECTED ? String(round(1.88 * (WiFi.RSSI() + 100)), 0) : F("0");
   jsonDoc["cpu"] = String(TempCPU);
   jsonDoc["ram"] = String(ESP.getFreeHeap() * 100 / ESP.getHeapSize());
   jsonDoc["PMEM"] = String(round(SPIFFS.usedBytes() * 100 / SPIFFS.totalBytes()), 0);
   jsonDoc["MQTTStatus"] = client.connected() ? F("<span class='label label-success'>CONECTADO</span>") : F("<span class='label label-important'>DESCONECTADO</span>");
   jsonDoc["MQTTBroker"] = client.connected() ? F(mqttserver) : F("--");

   // jsonDoc["Led2Estado"] = String(Led2Estado);

   jsonDoc["TemperaturaH1"] = String(TemperaturaH1);
   jsonDoc["TemperaturaH2"] = String(TemperaturaH2);
   jsonDoc["TemperaturaAmbiente"] = String(TemperaturaAmbiente);
   jsonDoc["TensionGeneral"] = String(TensionGeneral);
   jsonDoc["TensionH1"] = String(TensionH1);
   jsonDoc["magnetismoInterno"] = String(magnetismoInterno);
   jsonDoc["EstadoDeSistema"] = String(EstadoDeSistema);
   jsonDoc["AlarmaIntrusion"] = String(AlarmaIntrusion);
   jsonDoc["AlarmaH1"] = String(AlarmaH1);
   jsonDoc["AlarmaH2"] = String(AlarmaH2);
   jsonDoc["EstadoPuerto0"] = String(EstadoPuerto0);
   jsonDoc["EstadoPuerto1"] = String(EstadoPuerto1);
   jsonDoc["EstadoPuerto2"] = String(EstadoPuerto2);
   jsonDoc["Corriente1"] = String(Corriente1);
   jsonDoc["Corriente2"] = String(Corriente2);
   jsonDoc["Gas"] = String(Gas);

   // PZEM
   jsonDoc["voltage1"] = String(voltage1);
   jsonDoc["voltage1Max"] = String(voltage1Max);
   jsonDoc["voltage1Min"] = String(voltage1Min);

   jsonDoc["current1"] = String(current1);
   jsonDoc["current1Max"] = String(current1Max);
   jsonDoc["current1Min"] = String(current1Min);

   jsonDoc["power1"] = String(power1);
   jsonDoc["power1Max"] = String(power1Max);
   jsonDoc["power1Min"] = String(power1Min);

   jsonDoc["energy1"] = String(energy1);
   jsonDoc["energy1Min"] = String(energy1Min);
   jsonDoc["energy1Max"] = String(energy1Max);

   jsonDoc["frequency1"] = String(frequency1);
   jsonDoc["frequency1Min"] = String(frequency1Min);
   jsonDoc["frequency1Max"] = String(frequency1Max);

   jsonDoc["pf1"] = String(pf1);
   jsonDoc["pf1Min"] = String(pf1Min);
   jsonDoc["pf1Max"] = String(pf1Max);

   //Reloj
   jsonDoc["horas"] = String(horas);
   jsonDoc["minutos"] = String(minutos);
   jsonDoc["segundos"] = String(segundos);


   serializeJson(jsonDoc, response);
   return response;
}

String MQTTJson()
{

   String response;
   StaticJsonDocument<300> jsonDoc;

   jsonDoc["Tipo"] = "ESP32";
   jsonDoc["Nombre"] = id;
   jsonDoc["Serie"] = device_id;
   jsonDoc["WIFI_RSSI"] = WiFi.status() == WL_CONNECTED ? String(WiFi.RSSI()) : F("0");
   jsonDoc["TempCPU"] = String(TempCPU);
   jsonDoc["RAM_F_%"] = String(ESP.getFreeHeap() * 100 / ESP.getHeapSize());
   // ESP.getHeapSize() -> 100 %
   // ESP.getFreeHeap() -> X
   // % De RAM Libre  ESP.getFreeHeap()*100 / ESP.getHeapSize()
   jsonDoc["MEM_F_%"] = String(round(SPIFFS.usedBytes() * 100 / SPIFFS.totalBytes()), 0);
   // SPIFFS.totalBytes() -> 100 %
   // SPIFFS.usedBytes() -> X
   // % De Memoria Libre SPIFFS.usedBytes()*100 / SPIFFS.totalBytes())
   jsonDoc["Estado"] = "online";
   // jsonDoc["RELAY1"] = digitalRead(RELAY1);

   // jsonDoc["Led2Estado"] = String(Led2Estado);

   serializeJson(jsonDoc, response);
   return response;
}

void ProcessRequest(AsyncWebSocketClient *client, String request)
{

   String command = request;

   if (command == "1")
   {
   }
   else
   {
   }
}
