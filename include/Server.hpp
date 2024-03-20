/* Instancia de AsyncWebServer */
AsyncWebServer server(84);

// Cargar Información de las paginas al Servidor --------------------------------------
void InitServer()
{
  /**********************************************/
  server.serveStatic("/bootstrap-responsive.min.css", SPIFFS, "/bootstrap-responsive.min.css").setDefaultFile("bootstrap-responsive.min.css");
  server.serveStatic("/bootstrap.min.css", SPIFFS, "/bootstrap.min.css").setDefaultFile("bootstrap.min.css");
  server.serveStatic("/styles.css", SPIFFS, "/styles.css").setDefaultFile("styles.css");
  server.serveStatic("/jquery.easy-pie-chart.css", SPIFFS, "/jquery.easy-pie-chart.css").setDefaultFile("jquery.easy-pie-chart.css");
  server.serveStatic("/bootstrap.min.js", SPIFFS, "/bootstrap.min.js").setDefaultFile("bootstrap.min.js");
  server.serveStatic("/jquery-1.9.1.min.js", SPIFFS, "/jquery-1.9.1.min.js").setDefaultFile("jquery-1.9.1.min.js");
  server.serveStatic("/jquery.easy-pie-chart.js", SPIFFS, "/jquery.easy-pie-chart.js").setDefaultFile("jquery.easy-pie-chart.js");
  server.serveStatic("/modernizr.min.js", SPIFFS, "/modernizr.min.js").setDefaultFile("modernizr.min.js");
  server.serveStatic("/sweetalert2.min.css", SPIFFS, "/sweetalert2.min.css").setDefaultFile("sweetalert2.min.css");
  server.serveStatic("/sweetalert2.min.js", SPIFFS, "/sweetalert2.min.js").setDefaultFile("sweetalert2.min.js");
  server.serveStatic("/scripts.js", SPIFFS, "/scripts.js").setDefaultFile("scripts.js");
  server.serveStatic("/glyphicons-halflings.png", SPIFFS, "/glyphicons-halflings.png").setDefaultFile("glyphicons-halflings.png");
  server.serveStatic("/glyphicons-halflings-white.png", SPIFFS, "/glyphicons-halflings-white.png").setDefaultFile("glyphicons-halflings-white.png");
  server.serveStatic("/logo.png", SPIFFS, "/logo.png").setDefaultFile("logo.png");
  server.serveStatic("/morris.css", SPIFFS, "/morris.css").setDefaultFile("morris.css");
  server.serveStatic("/jquery.knob.js", SPIFFS, "/jquery.knob.js").setDefaultFile("jquery.knob.js");
  server.serveStatic("/raphael-min.js", SPIFFS, "/raphael-min.js").setDefaultFile("raphael-min.js");
  server.serveStatic("/morris.min.js", SPIFFS, "/morris.min.js").setDefaultFile("morris.min.js");

  server.serveStatic("/jquery.flot.js", SPIFFS, "/jquery.flot.js").setDefaultFile("jquery.flot.js");
  server.serveStatic("/jquery.flot.categories.js", SPIFFS, "/jquery.flot.categories.js").setDefaultFile("jquery.flot.categories.js");

  server.serveStatic("/jquery.flot.pie.js", SPIFFS, "/jquery.flot.pie.js").setDefaultFile("jquery.flot.pie.js");
  server.serveStatic("/jquery.flot.time.js", SPIFFS, "/jquery.flot.time.js").setDefaultFile("jquery.flot.time.js");
  server.serveStatic("/jquery.flot.stack.js", SPIFFS, "/jquery.flot.stack.js").setDefaultFile("jquery.flot.stack.js");
  server.serveStatic("/jquery.flot.resize.js", SPIFFS, "/jquery.flot.resize.js").setDefaultFile("jquery.flot.resize.js");

  server.serveStatic("/script.js", SPIFFS, "/script.js").setDefaultFile("script.js");

  /**********************************************/
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
      // Index.html
      File file = SPIFFS.open(F("/index.html"), "r");
      
      if (file){
        file.setTimeout(100);
        String s = file.readString();
        file.close();
        // Actualiza contenido dinamico del html
        s.replace(F("#id#"), id);
        s.replace(F("#serie#"), device_id);
        /* Bloque WIFI */
        s.replace(F("#WFEstatus#"), WiFi.status() == WL_CONNECTED ? F("<span class='label label-success'>CONECTADO</span>") : F("<span class='label label-important'>DESCONECTADO</span>"));
        s.replace(F("#WFSSID#"), WiFi.status() == WL_CONNECTED ? F(ssid) : F("--"));
        s.replace(F("#sysIP#"), ipStr(WiFi.status() == WL_CONNECTED ? WiFi.localIP() : WiFi.softAPIP()));
        s.replace(F("#WFDBM#"), WiFi.status() == WL_CONNECTED ? String(WiFi.RSSI()) : F("0"));
        s.replace(F("#WFPRC#"), WiFi.status() == WL_CONNECTED ? String(round(1.88 * (WiFi.RSSI() + 100)), 0) : F("0"));
        /* Bloque MQTT */
        s.replace(F("#MQTTStatus#"), client.connected() ? F("<span class='label label-success'>CONECTADO</span>") : F("<span class='label label-important'>DESCONECTADO</span>"));
        s.replace(F("#MQTTBroker#"), client.connected() ? F(mqttserver) : F("--"));
        s.replace(F("#MQTTUser#"), F(mqttuser));
        s.replace(F("#MQTTID#"), F(idMqtt));

        s.replace(F("#horas#"), String(horas));
        s.replace(F("#minutos#"), String(minutos));
        s.replace(F("#segundos#"), String(segundos));

        /*Bloque LED
        s.replace(F("#Led2Estado#"), F(Led2Estado));*/
        //s.replace(F("#Led2Estado#"), String(Led2Estado));
        //s.replace(F("#RELAY1#"),String(RELAY1));

        //Bloque nuevo

        s.replace(F("#TemperaturaH1#"), String(TemperaturaH1));
        s.replace(F("#TemperaturaH2#"), String(TemperaturaH2));
        s.replace(F("#TemperaturaAmbiente#"), String(TemperaturaAmbiente));
        s.replace(F("#TensionGeneral#"), String(TensionGeneral));
        s.replace(F("#TensionH1#"), String(TensionH1));
        s.replace(F("#TensionH2#"), String(TensionH2));
        s.replace(F("#magnetismoInterno#"), String(magnetismoInterno));

       // s.replace(F("#EstadoDeSistema#"), String(EstadoDeSistema));
        s.replace(F("#EstadoDeSistema#"), EstadoDeSistema == 0 ? F("<span class='label label-success'>NO ALARMADO</span>") : F("<span class='label label-important'>ALARMADO</span>"));
        s.replace(F("#AlarmaIntrusion#"), AlarmaIntrusion == 0 ? F("<span class='label label-success'>NO ALARMADO</span>") : F("<span class='label label-important'>ALARMADO</span>"));
        s.replace(F("#AlarmaH1#"), AlarmaH1 == 0 ? F("<span class='label label-success'>NO ALARMADO</span>") : F("<span class='label label-important'>ALARMADO</span>"));
        s.replace(F("#AlarmaH2#"), AlarmaH2 == 0 ? F("<span class='label label-success'>NO ALARMADO</span>") : F("<span class='label label-important'>ALARMADO</span>"));
        s.replace(F("#EstadoPuerto0#"), EstadoPuerto0 == 0 ? F("<span class='label label-success'>NO ALARMADO</span>") : F("<span class='label label-important'>ALARMADO</span>"));
        s.replace(F("#EstadoPuerto1#"), EstadoPuerto1 == 0 ? F("<span class='label label-success'>NO ALARMADO</span>") : F("<span class='label label-important'>ALARMADO</span>"));
        s.replace(F("#EstadoPuerto2#"), EstadoPuerto2 == 0 ? F("<span class='label label-success'>NO ALARMADO</span>") : F("<span class='label label-important'>ALARMADO</span>"));

 
        s.replace(F("#Corriente1#"), String(Corriente1));
        s.replace(F("#Corriente2#"), String(Corriente2));
        s.replace(F("#Gas#"), String(Gas));

        s.replace(F("#horas#"), String(horas));
        s.replace(F("#minutos#"), String(minutos));
        s.replace(F("#segundos#"), String(segundos));

        
        // Read the data from the sensor
        //PZEM

        s.replace(F("#voltage1#"), String(voltage1));
        s.replace(F("#voltage1Max#"), String(voltage1Max));
        s.replace(F("#voltage1Min#"), String(voltage1Min));

        s.replace(F("#current1#"), String(current1));
        s.replace(F("#current1Min#"), String(current1Min));
        s.replace(F("#current1Max#"), String(current1Max));

        s.replace(F("#power1#"), String(power1));
        s.replace(F("#power1Min#"), String(power1Min));
        s.replace(F("#power1Max#"), String(power1Max));

        s.replace(F("#energy1#"), String(energy1));
        s.replace(F("#energy1Min#"), String(energy1Min));
        s.replace(F("#energy1Max#"), String(energy1Max));

        s.replace(F("#frequency1#"), String(frequency1));
        s.replace(F("#frequency1Min#"), String(frequency1Min));
        s.replace(F("#frequency1Max#"), String(frequency1Max));

        s.replace(F("#pf1#"), String(pf1));
        s.replace(F("#pf1Min#"), String(pf1Min));
        s.replace(F("#pf1Max#"), String(pf1Max));

        /* Bloque pie chart */
        s.replace(F("#SWFI#"), WiFi.status() == WL_CONNECTED ? String(round(1.88 * (WiFi.RSSI() + 100)), 0) : F("0"));
        s.replace(F("#PMEM#"), String(round(SPIFFS.usedBytes() * 100 / SPIFFS.totalBytes()), 0));
        s.replace(F("#ram#"), String(ESP.getFreeHeap() * 100 / ESP.getHeapSize()));
        s.replace(F("#cpu#"), String(TempCPU));


        // Envia dados al navegador
        request->send(200, "text/html", s);  
      }else{
        request->send(500, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                                                                 "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                                    "Swal.fire({title: 'Error!',"
                                                               " text: 'Error 500 Internal Server Error',"
                                                               " icon: 'error',"
                                                               " confirmButtonText: 'Cerrar'}).then((result) => {"
                                                                                                  "if (result.isConfirmed){"
                                                                                                       "window.location = '/';"
                                                                                                   "};"
                                                                                               "})"
                                                 "</script><body></html>");
        log(F("\nError: Config - ERROR leyendo el archivo"));
      } });

  server.on("/indexMed", HTTP_GET, [](AsyncWebServerRequest *request)
            {
      // Index.html
      File file = SPIFFS.open(F("/indexMed.html"), "r");
      if (file){
        file.setTimeout(100);
        String s = file.readString();
        file.close();
        // Actualiza contenido dinamico del html
        s.replace(F("#id#"), id);
        s.replace(F("#serie#"), device_id);
       
        //Bloque nuevo

        
        s.replace(F("#horas#"), String(horas));
        s.replace(F("#minutos#"), String(minutos));
        s.replace(F("#segundos#"), String(segundos));

        s.replace(F("#TemperaturaH1#"), String(TemperaturaH1));
        s.replace(F("#TemperaturaH2#"), String(TemperaturaH2));
        s.replace(F("#TemperaturaAmbiente#"), String(TemperaturaAmbiente));
        s.replace(F("#TensionGeneral#"), String(TensionGeneral));
        s.replace(F("#TensionH1#"), String(TensionH1));
        s.replace(F("#TensionH2#"), String(TensionH2));
        s.replace(F("#magnetismoInterno#"), String(magnetismoInterno));

       // s.replace(F("#EstadoDeSistema#"), String(EstadoDeSistema));
        s.replace(F("#EstadoDeSistema#"), EstadoDeSistema == 0 ? F("<span class='label label-success'>NO ALARMADO</span>") : F("<span class='label label-important'>ALARMADO</span>"));
        s.replace(F("#AlarmaIntrusion#"), AlarmaIntrusion == 0 ? F("<span class='label label-success'>NO ALARMADO</span>") : F("<span class='label label-important'>ALARMADO</span>"));
        s.replace(F("#AlarmaH1#"), AlarmaH1 == 0 ? F("<span class='label label-success'>NO ALARMADO</span>") : F("<span class='label label-important'>ALARMADO</span>"));
        s.replace(F("#AlarmaH2#"), AlarmaH2 == 0 ? F("<span class='label label-success'>NO ALARMADO</span>") : F("<span class='label label-important'>ALARMADO</span>"));
        s.replace(F("#EstadoPuerto0#"), EstadoPuerto0 == 0 ? F("<span class='label label-success'>NO ALARMADO</span>") : F("<span class='label label-important'>ALARMADO</span>"));
        s.replace(F("#EstadoPuerto1#"), EstadoPuerto1 == 0 ? F("<span class='label label-success'>NO ALARMADO</span>") : F("<span class='label label-important'>ALARMADO</span>"));
        s.replace(F("#EstadoPuerto2#"), EstadoPuerto2 == 0 ? F("<span class='label label-success'>NO ALARMADO</span>") : F("<span class='label label-important'>ALARMADO</span>"));

 
        s.replace(F("#Corriente1#"), String(Corriente1));
        s.replace(F("#Corriente2#"), String(Corriente2));
        s.replace(F("#Gas#"), String(Gas));

        
        // Read the data from the sensor
        //PZEM

        s.replace(F("#voltage1#"), String(voltage1));
        s.replace(F("#voltage1Max#"), String(voltage1Max));
        s.replace(F("#voltage1Min#"), String(voltage1Min));

        s.replace(F("#current1#"), String(current1));
        s.replace(F("#current1Min#"), String(current1Min));
        s.replace(F("#current1Max#"), String(current1Max));

        s.replace(F("#power1#"), String(power1));
        s.replace(F("#power1Min#"), String(power1Min));
        s.replace(F("#power1Max#"), String(power1Max));

        s.replace(F("#energy1#"), String(energy1));
        s.replace(F("#energy1Min#"), String(energy1Min));
        s.replace(F("#energy1Max#"), String(energy1Max));

        s.replace(F("#frequency1#"), String(frequency1));
        s.replace(F("#frequency1Min#"), String(frequency1Min));
        s.replace(F("#frequency1Max#"), String(frequency1Max));

        s.replace(F("#pf1#"), String(pf1));
        s.replace(F("#pf1Min#"), String(pf1Min));
        s.replace(F("#pf1Max#"), String(pf1Max));

        // Envia dados al navegador
        request->send(200, "text/html", s);  
      }else{
        request->send(500, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                                                                 "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                                    "Swal.fire({title: 'Error!',"
                                                               " text: 'Error 500 Internal Server Error',"
                                                               " icon: 'error',"
                                                               " confirmButtonText: 'Cerrar'}).then((result) => {"
                                                                                                  "if (result.isConfirmed){"
                                                                                                       "window.location = '/';"
                                                                                                   "};"
                                                                                               "})"
                                                 "</script><body></html>");
        log(F("\nError: Config - ERROR leyendo el archivo"));
      } });
  /**********************************************/
  server.on("/configwifi", HTTP_GET, [](AsyncWebServerRequest *request)
            {
        // Config
        File file = SPIFFS.open(F("/configwifi.html"), "r");
        if (file){
          file.setTimeout(100);
          String s = file.readString();
          file.close();
          // Atualiza el contenido al cargar
          s.replace(F("#id#"), id);
          s.replace(F("#ssid#"), ssid);
          //sección ap
          s.replace(F("#nameap#"), String(nameap));
          // Send data
          request->send(200, "text/html", s);
        }else{
          request->send(500, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                                                                  "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                                      "Swal.fire({title: 'Error!',"
                                                                " text: 'Error 500 Internal Server Error',"
                                                                " icon: 'error',"
                                                                " confirmButtonText: 'Cerrar'}).then((result) => {"
                                                                                                    "if (result.isConfirmed){"
                                                                                                        "window.location = '/';"
                                                                                                    "};"
                                                                                                "})"
                                                  "</script><body></html>");
          log(F("\nError: Config - ERROR leyendo el archivo"));
        } });
  /**********************************************/
  server.on("/configmqtt", HTTP_GET, [](AsyncWebServerRequest *request)
            {
        // Config
        File file = SPIFFS.open(F("/configmqtt.html"), "r");
        if (file){
          file.setTimeout(100);
          String s = file.readString();
          file.close();
          // Atualiza el contenido al cargar
          s.replace(F("#idMqtt#"), idMqtt);
          s.replace(F("#mqttuser#"), mqttuser);
          s.replace(F("#mqttserver#"), mqttserver);
          // Send data
          request->send(200, "text/html", s);
        }else{
          request->send(500, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                                                                  "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                                      "Swal.fire({title: 'Error!',"
                                                                " text: 'Error 500 Internal Server Error',"
                                                                " icon: 'error',"
                                                                " confirmButtonText: 'Cerrar'}).then((result) => {"
                                                                                                    "if (result.isConfirmed){"
                                                                                                        "window.location = '/';"
                                                                                                    "};"
                                                                                                "})"
                                                  "</script><body></html>");
          log(F("\nError: Config - ERROR leyendo el archivo"));
        } });
  /**********************************************/
  server.on("/confwifiSave", HTTP_POST, [](AsyncWebServerRequest *request)
            {
        String response;
        StaticJsonDocument<300> doc;
        // Graba Configuración desde Config
        // Verifica número de campos recebidos
        // ESP32 envia 5 campos
        if (request->params() == 5){
          String s;
          // ID
          if(request->hasArg("id"))
          s = request->arg("id");
          s.trim();
          if (s == ""){
            s = deviceID();
          }
          strlcpy(id, s.c_str(), sizeof(id));
          // SSID
          if(request->hasArg("ssid"))
          s = request->arg("ssid");
          s.trim();
          if (s == ""){
            s = ssid;
          }
          strlcpy(ssid, s.c_str(), sizeof(ssid));
          // PW SSID
          if(request->hasArg("pw"))
          s = request->arg("pw");
          s.trim();
          if (s != ""){
            // Actualiza contraseña
            strlcpy(pw, s.c_str(), sizeof(pw));
          }
          // Nombre AP
          if(request->hasArg("nameap"))
          s = request->arg("nameap");
          s.trim();
          if (s != ""){
            // Atualiza ssid ap
            strlcpy(nameap, s.c_str(), sizeof(nameap));
          }
          // Contraseña AP
          if(request->hasArg("passwordap"))
          s = request->arg("passwordap");
          s.trim();
          if (s != ""){
            // Atualiza contraseña ap
            strlcpy(passwordap, s.c_str(), sizeof(passwordap));
          }
          //Parpadeo de los lEDS     
          led();
          // Graba configuracion
          if (configSave()){
            request->send(200, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                            "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                                      "Swal.fire({title: 'Hecho!',"
                                                                " text: 'Configuración WIFI guardada, se requiere reiniciar el Equipo',"
                                                                " icon: 'success',"
                                                                " showCancelButton: true,"
                                                                " confirmButtonColor: '#3085d6',"
                                                                " cancelButtonColor: '#d33',"
                                                                " confirmButtonText: 'Si, reiniciar',"
                                                                " cancelButtonText: 'Cancelar',"
                                                                " reverseButtons: true"
                                                                " }).then((result) => {"
                                                                              "if (result.isConfirmed){"
                                                                                  "window.location = 'reboot';"
                                                                              "}else if ("
                                                                                  "result.dismiss === Swal.DismissReason.cancel"
                                                                                "){"
                                                                                  "history.back();"
                                                                                "}"
                                                                          "})"
                                                  "</script><body></html>");
          }else{
            request->send(200, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                                                                  "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                                      "Swal.fire({title: 'Error!',"
                                                                " text: 'No se pudo guardar, Falló la configuración WIFI',"
                                                                " icon: 'error',"
                                                                " confirmButtonText: 'Cerrar'}).then((result) => {"
                                                                                                    "if (result.isConfirmed){"
                                                                                                        "history.back();"
                                                                                                    "};"
                                                                                                "})"
                                                  "</script><body></html>");
            log(F("\nError: ConfigSave - ERROR salvando Configuración"));
          }
        }else{
            request->send(200, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                                                                  "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                                      "Swal.fire({title: 'Error!',"
                                                                " text: 'No se pudo guardar, Error de parámetros WIFI',"
                                                                " icon: 'error',"
                                                                " confirmButtonText: 'Cerrar'}).then((result) => {"
                                                                                                    "if (result.isConfirmed){"
                                                                                                        "history.back();"
                                                                                                    "};"
                                                                                                "})"
                                                  "</script><body></html>");
        } });
  /**********************************************/
  server.on("/confmqttSave", HTTP_POST, [](AsyncWebServerRequest *request)
            {
      String response;
      StaticJsonDocument<300> doc;
      // Graba Configuración desde Config
      // Verifica número de campos recebidos
      // ESP32 envia 4 campos
      if (request->params() == 4){
        String s;
        // Graba mqttuser
        if(request->hasArg("mqttuser"))
        s = request->arg("mqttuser");
        s.trim();
        if (s != ""){
          // Atualiza usuario mqtt
          strlcpy(mqttuser, s.c_str(), sizeof(mqttuser));
        }
        // Graba mqttpass
        if(request->hasArg("mqttpass"))
        s = request->arg("mqttpass");
        s.trim();
        if (s != ""){
          // Atualiza contraseña del user mqtt
          strlcpy(mqttpass, s.c_str(), sizeof(mqttpass));
        }
        // Graba mqttserver
        if(request->hasArg("mqttserver"))
        s = request->arg("mqttserver");
        s.trim();
        if (s != ""){
          // Atualiza server mqtt
          strlcpy(mqttserver, s.c_str(), sizeof(mqttserver));
        }
        // Graba mqtt ID
        if(request->hasArg("idMqtt"))
        s = request->arg("idMqtt");
        s.trim();
        if (s != ""){
          // Atualiza mqtt ID
          strlcpy(idMqtt, s.c_str(), sizeof(idMqtt));
        }
        //Parpadeo de los LED     
        led();
        // Guarda la configuracion
        if (configSaveMQTT()){
          request->send(200, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                            "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                                    "Swal.fire({title: 'Hecho!',"
                                                                " text: 'Configuración MQTT guardada, se requiere reiniciar el Equipo',"
                                                                " icon: 'success',"
                                                                " showCancelButton: true,"
                                                                " confirmButtonColor: '#3085d6',"
                                                                " cancelButtonColor: '#d33',"
                                                                " confirmButtonText: 'Si, reiniciar',"
                                                                " cancelButtonText: 'Cancelar',"
                                                                " reverseButtons: true"
                                                                " }).then((result) => {"
                                                                            "if (result.isConfirmed){"
                                                                                  "window.location = 'reboot';"
                                                                              "}else if ("
                                                                                "result.dismiss === Swal.DismissReason.cancel"
                                                                              "){"
                                                                                  "history.back();"
                                                                              "}"
                                                                          "})"                                                                        
                                                  "</script><body></html>");
        }else{
          request->send(200, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                                                                  "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                                    "Swal.fire({title: 'Error!',"
                                                                " text: 'No se pudo guardar, Falló la configuración MQTT',"
                                                                " icon: 'error',"
                                                                " confirmButtonText: 'Cerrar'}).then((result) => {"
                                                                                                  "if (result.isConfirmed){"
                                                                                                        "history.back();"
                                                                                                    "};"
                                                                                                "})"
                                                  "</script><body></html>");
          log(F("\nError: ConfigSave - ERROR salvando Configuración"));
        }
      }else{
          request->send(200, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                                                                  "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                                    "Swal.fire({title: 'Error!',"
                                                                " text: 'No se pudo guardar, Error de parámetros MQTT',"
                                                                " icon: 'error',"
                                                                " confirmButtonText: 'Cerrar'}).then((result) => {"
                                                                                                  "if (result.isConfirmed){"
                                                                                                        "history.back();"
                                                                                                    "};"
                                                                                                "})"
                                                  "</script><body></html>");
      } });
  /**********************************************/
  server.on("/reconfig", HTTP_GET, [](AsyncWebServerRequest *request)
            {
        // Reinicia Config
        configReset();
        // Reinicia Config MQTT
        configResetMQTT();
      
        // Graba la configuracion
        if (configSave() && configSaveMQTT()){
          request->send(200, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                            "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                                    "Swal.fire({title: 'Hecho!',"
                                                                " text: 'Configuración restablecida, se requiere reiniciar el Equipo',"
                                                                " icon: 'success',"
                                                                " showCancelButton: true,"
                                                                " confirmButtonColor: '#3085d6',"
                                                                " cancelButtonColor: '#d33',"
                                                                " confirmButtonText: 'Si, reiniciar',"
                                                                " cancelButtonText: 'Cancelar',"
                                                                " reverseButtons: true"
                                                                " }).then((result) => {"
                                                                            "if (result.isConfirmed){"
                                                                                  "window.location = 'reboot';"
                                                                              "}else if ("
                                                                                "result.dismiss === Swal.DismissReason.cancel"
                                                                              "){"
                                                                                  "history.back();"
                                                                              "}"
                                                                          "})"
                                                  "</script><body></html>");
        }else{
          request->send(200, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                                                                  "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                                    "Swal.fire({title: 'Error!',"
                                                                " text: 'Falló restablecer de la configuración',"
                                                                " icon: 'error',"
                                                                " confirmButtonText: 'Cerrar'}).then((result) => {"
                                                                                                  "if (result.isConfirmed){"
                                                                                                        "history.back();"
                                                                                                    "};"
                                                                                                "})"
                                                  "</script><body></html>");
          log(F("\nError: Reconfigurar - ERROR reinicio Configuración"));
        } });
  /**********************************************/

  /*
    server.on("/Led2On", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      digitalWrite (2,HIGH);
      request->send(200, "application/json","{}");
      Led2Estado=true;

    });
    server.on("/Led2Off", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      digitalWrite (2,LOW);
      request->send(200, "application/json","{}");
      Led2Estado=false;
    });
  */

  // ConfigGen

  /**********************************************/

  server.on("/configGenA", HTTP_POST, [](AsyncWebServerRequest *request)
            {
        String response;
        StaticJsonDocument<300> doc;
        // Graba Configuración desde Config
        // Verifica número de campos recebidos
        // ESP32 envia 5 campos
        if (request->params() == 6)
        {
          String s;


          // Dia
          if(request->hasArg("dia"))
          {
            s = request->arg("dia");
            s.trim();
          if (s == "")
            s = "1";

          dia=s.toInt();
          }
          
        
          // Mes
          if(request->hasArg("mes"))
          {s = request->arg("mes");
          s.trim();
          if (s == "")
            s = "1";
          
          mes=s.toInt();
          }

          // año

          if(request->hasArg("ano"))
          {s = request->arg("ano");
          s.trim();
          if (s == "")
            s = "20";
          
          ano=s.toInt();
          }
          // Horas
          if(request->hasArg("horas"))
          {s = request->arg("horas");
          s.trim();
          if (s == "")
            s = "1";
          
          horas=s.toInt();
          }

          //Minutos
          if(request->hasArg("minutos"))
          {s = request->arg("minutos");
          s.trim();
          if (s == "")
            s = "1";
          
          minutos=s.toInt();
          }

          //Segundos
          if(request->hasArg("segundos"))
          {s = request->arg("segundos");
          s.trim();
          if (s == "")
            s = "1";
          
          segundos=s.toInt();
          }

          //Parpadeo de los lEDS     
          led();
          // Graba configuracion
          if (configSave()){
            request->send(200, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                            "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                                      "Swal.fire({title: 'Hecho!',"
                                                                " text: 'Configuración fecha y hora guardada, se requiere reiniciar el Equipo',"
                                                                " icon: 'success',"
                                                                " showCancelButton: true,"
                                                                " confirmButtonColor: '#3085d6',"
                                                                " cancelButtonColor: '#d33',"
                                                                " confirmButtonText: 'Si',"
                                                                " cancelButtonText: 'Cancelar',"
                                                                " reverseButtons: true"
                                                                " }).then((result) => {"
                                                                              "if (result.isConfirmed){"
                                                                                  "window.location = 'configGen';"
                                                                              "}else if ("
                                                                                  "result.dismiss === Swal.DismissReason.cancel"
                                                                                "){"
                                                                                  "history.back();"
                                                                                "}"
                                                                          "})"
                                                  "</script><body></html>");
          }else{
            request->send(200, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                                                                  "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                                      "Swal.fire({title: 'Error!',"
                                                                " text: 'No se pudo guardar, Falló la configuración fecha y hora',"
                                                                " icon: 'error',"
                                                                " confirmButtonText: 'Cerrar'}).then((result) => {"
                                                                                                    "if (result.isConfirmed){"
                                                                                                        "history.back();"
                                                                                                    "};"
                                                                                                "})"
                                                  "</script><body></html>");
            log(F("\nError: ConfigSave - ERROR salvando Configuración"));
          }
        }else{
            request->send(200, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                                                                  "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                                      "Swal.fire({title: 'Error!',"
                                                                " text: 'No se pudo guardar, Error de parámetros fecha y hora',"
                                                                " icon: 'error',"
                                                                " confirmButtonText: 'Cerrar'}).then((result) => {"
                                                                                                    "if (result.isConfirmed){"
                                                                                                        "history.back();"
                                                                                                    "};"
                                                                                                "})"
                                                  "</script><body></html>");
        } });

  server.on("/configGen", HTTP_GET, [](AsyncWebServerRequest *request)
            {
        // Config
        Serial.println("Estoy en server ON ConfigGen");
        delay (1000);
        File file = SPIFFS.open(F("/configGen.html"), "r");
        if (file){
          file.setTimeout(100);
          String s = file.readString();
          file.close();

        
          // Atualiza el contenido al cargar

          //s.replace(F("#idMqtt#"), idMqtt);
          //s.replace(F("#mqttuser#"), mqttuser);
          //s.replace(F("#mqttserver#"), mqttserver);

          s.replace(F("#horas#"), String(horas));
          s.replace(F("#minutos#"), String(minutos));
          s.replace(F("#segundos#"), String(segundos));
          s.replace(F("#dia#"), String(dia));
          s.replace(F("#mes#"), String(mes));
          s.replace(F("#ano#"), String(ano));
          // Send data
          request->send(200, "text/html", s);
        }else{
          request->send(500, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                                                                  "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                                      "Swal.fire({title: 'Error!',"
                                                                " text: 'Error 500 Internal Server Error',"
                                                                " icon: 'error',"
                                                                " confirmButtonText: 'Cerrar'}).then((result) => {"
                                                                                                    "if (result.isConfirmed){"
                                                                                                        "window.location = '/';"
                                                                                                    "};"
                                                                                                "})"
                                                  "</script><body></html>");
          log(F("\nError: Config - ERROR leyendo el archivo"));
        } });
  /**********************************************/

  server.on("/Historicos", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    // Config
    File file = SPIFFS.open(F("/Historicos.html"), "r");
    if (file)
    {
      file.setTimeout(100);
      String s = file.readString();
      file.close();
      request->send(200, "text/html", s);
    }
    else
    {
      request->send(500, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                      "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                      "Swal.fire({title: 'Error!',"
                                      " text: 'Error 500 Internal Server Error',"
                                      " icon: 'error',"
                                      " confirmButtonText: 'Cerrar'}).then((result) => {"
                                      "if (result.isConfirmed){"
                                      "window.location = '/';"
                                      "};"
                                      "})"
                                      "</script><body></html>");
      log(F("\nError: Config - ERROR leyendo el archivo"));
    } });

  server.on("/Historicos2", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    // Config
    File file = SPIFFS.open(F("/Historicos2.html"), "r");
    if (file)
    {
      file.setTimeout(100);
      String s = file.readString();
      file.close();
      request->send(200, "text/html", s);
    }
    else
    {
      request->send(500, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                      "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                      "Swal.fire({title: 'Error!',"
                                      " text: 'Error 500 Internal Server Error',"
                                      " icon: 'error',"
                                      " confirmButtonText: 'Cerrar'}).then((result) => {"
                                      "if (result.isConfirmed){"
                                      "window.location = '/';"
                                      "};"
                                      "})"
                                      "</script><body></html>");
      log(F("\nError: Config - ERROR leyendo el archivo"));
    } });

  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(404, "text/html", "<html><meta charset='UTF-8'><head><link href='bootstrap.min.css' rel='stylesheet' media='screen'><link rel='stylesheet' href='sweetalert2.min.css'>"
                                                      "<script src='jquery-1.9.1.min.js'><script src='bootstrap.min.js'></script></script><script src='sweetalert2.min.js'></script></head><body><script>"
                                                      "Swal.fire({title: 'Error 404!',"
                                                      " text: 'Página no encontrada',"
                                                      " icon: 'warning',"
                                                      " confirmButtonText: 'Cerrar'}).then((result) => {"
                                                      "if (result.isConfirmed){"
                                                      "history.back();"
                                                      "};"
                                                      "})"
                                                      "</script><body></html>"); });
  /**********************************************/
  server.begin();
  log("\nInfo: HTTP server iniciado");
  /**********************************************/
}