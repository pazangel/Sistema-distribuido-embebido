#include <WiFi.h>

// Traduce el codigo numerico de seguridad a texto legible
const char* tipoSeguridad(wifi_auth_mode_t tipo) {
  switch (tipo) {
    case WIFI_AUTH_OPEN:            return "Abierta";
    case WIFI_AUTH_WEP:             return "WEP";
    case WIFI_AUTH_WPA_PSK:         return "WPA";
    case WIFI_AUTH_WPA2_PSK:        return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK:    return "WPA/WPA2";
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-Enterprise";
    case WIFI_AUTH_WPA3_PSK:        return "WPA3";
    case WIFI_AUTH_WPA2_WPA3_PSK:   return "WPA2/WPA3";
    default:                        return "Otra";
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);// La radio WiFi de la ESP32 puede trabajar de cuatro formas
/*
+---------------+-------------------------------------------------------------+
| Modo          | Qué hace la placa                                           |
+---------------+-------------------------------------------------------------+
| WIFI_OFF      | Radio apagada                                               |
+---------------+-------------------------------------------------------------+
| WIFI_STA      | Cliente (estación): se puede conectar a una red wifi        |
+---------------+-------------------------------------------------------------+
| WIFI_AP       | Punto de acceso: crea su propia red, funcionando como router|
+---------------+-------------------------------------------------------------+
| WIFI_AP_STA   | puede ser punto de acceso y cliente a la vez                |
+---------------+-------------------------------------------------------------+
*/
  WiFi.disconnect();// se desconecta de cualquier red que alla estado conectada 
  delay(100);
}

void loop() {
  Serial.println("Escaneando...");
  int n = WiFi.scanNetworks(); // se detiene aqui un momento y escanea las redes disponibles, nos retorna el numero de redes disponibles

  if (n == 0) {// si n es igual a 0
    Serial.println("No se encontro ninguna red");
  } else {// caso contrario si encontro redes
    for (int i = 0; i < n; i++) {// iteramos mientras i sea menor que n
      Serial.print(i + 1);
      Serial.print(") ");
      Serial.print(WiFi.SSID(i));// accede a un arreglo con el nombre de las redes en la posicion i
      Serial.print("  | RSSI: ");
      Serial.print(WiFi.RSSI(i));// accede a un arreglo con la fuerza de la señal de la red en la posicion i
      Serial.print(" dBm  | canal: ");
      Serial.print(WiFi.channel(i));// accede a un arreglo con el canal de la red en la posicion i
      Serial.print("  | seguridad: ");
      Serial.println(tipoSeguridad(WiFi.encryptionType(i)));// accede a un arreglo con el tipo de reguridad de la red en la posicion i
    }
  }

  WiFi.scanDelete();// libera el bloque de memoria ram del esp32 donde se guardaron los resultado de las redes 
  delay(5000);
}