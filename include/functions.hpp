/* Funciones Personalizadas YHP */

/**********************************************/
/*  Genera un log en el puerto Serial */
void log(String s)
{
  Serial.println(s);
}
/**********************************************/
/*  Retorna IPAddress en formato "n.n.n.n" de IP a String */
String ipStr(const IPAddress &ip)
{
  String sFn = "";
  for (byte bFn = 0; bFn < 3; bFn++)
  {
    sFn += String((ip >> (8 * bFn)) & 0xFF) + ".";
  }
  sFn += String(((ip >> 8 * 3)) & 0xFF);
  return sFn;
}
/**********************************************/
/*  De HEX a String */
String hexStr(const unsigned long &h, const byte &l = 8)
{
  String s;
  s = String(h, HEX);
  s.toUpperCase();
  s = ("00000000" + s).substring(s.length() + 8 - l);
  return s;
}
/**********************************************/
/*  Create a Unique ID from MAC address */
String idUnique()
{
  // Retorna los ultimos 4 Bytes del MAC rotados
  char idunique[15];
  uint64_t chipid = ESP.getEfuseMac();
  uint16_t chip = (uint16_t)(chipid >> 32);
  snprintf(idunique, 15, "%04X", chip);
  return idunique;
}
/**********************************************/
// ID del Dispositivo para La Base de Datos
const String device_id = hexStr(ESP.getEfuseMac()) + "CE" + String(idUnique());
/*  ESP32 utiliza funcion getEfuseMac() */
String deviceID()
{
  return "ESP32" + hexStr(ESP.getEfuseMac()) + String(idUnique());
}
/**********************************************/
/*  Configurar los Pines de Salida */
void configPines()
{

  pinMode(WIFILED, OUTPUT);
  // pinMode(MQTTLED, OUTPUT);
  // pinMode(2,OUTPUT);
  // digitalWrite(MQTTLED, LOW);
  digitalWrite(WIFILED, LOW);
}
/**********************************************/
/*  Parpadeo de los LEDs */
void led()
{
  for (int i = 0; i < 4; i++)
  {
    // digitalWrite(MQTTLED, HIGH);
    digitalWrite(WIFILED, HIGH);
    delay(100);
    // digitalWrite(MQTTLED, LOW);
    digitalWrite(WIFILED, LOW);
    // delay(100);
  }
}
/**********************************************/

void CalculoRO()
{
  C_Gas = 0;
  Serial.print("Calculando RO:");
  for (int i = 0; i < 300; i++)
  {
    C_Gas = C_Gas + analogRead(PuertoGas);
    delay(10);
  }
  C_Gas = C_Gas / 300;
  C_Gas = 3.11 * (C_Gas / 4095);
  Serial.println("Gas-Volt:");
  Serial.println(C_Gas);
  // C_Gas=(3.3-C_Gas)/C_Gas
  RO = RL * (5 - C_Gas) / C_Gas; // Reemplazo 3.3 por 5
  RO = RO / 9.8;
  Serial.println(RO);
}

void Reloj()
{
  unsigned long tiempoActual = millis();

  // Verificar si ha pasado un segundo
  if (tiempoActual - tiempoAnterior >= 1000)
  {
    // Actualizar el tiempo

    segundos++;
    if (segundos >= 60)
    {
      segundos = 0;
      minutos++;
      if (minutos >= 60)
      {
        minutos = 0;
        horas++;
        if (horas >= 24)
        {
          horas = 0;
        }
      }
    }

    // Mostrar el tiempo en los pines correspondientes

    // Actualizar el tiempo anterior
  }
}
