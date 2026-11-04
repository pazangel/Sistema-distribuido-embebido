#include <WiFi.h>
#include "secrets.h"// copia y pega todo lo que contiene secrets.h a este codigo

void setup() {
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  /*
  WiFi.begin enciende y configura el radio WiFi, guarda el nombre de la red y la contraseña, empieza el proceso de conexión:

1. buscar la red (el router con ese nombre)
2. autenticarse (la contraseña)
3. asociarse al router
4. pedir una IP al DHCP 
  */

  while (WiFi.status() != WL_CONNECTED) {// si es diferente de conectado, WiFi.status() nos dice en qué punto está la conexión con el router en ese momento
    delay(500);
    Serial.print(".");
  }
/*
  podemos comparara a WiFi.status() con diferentes valores:

+--------------------+--------------------------------------------------------------------+
| Valor              | Significado                                                        |
+--------------------+--------------------------------------------------------------------+
| WL_IDLE_STATUS     | está en proceso, todavía no termina                                |
+--------------------+--------------------------------------------------------------------+
| WL_NO_SSID_AVAIL   | no encuentra la red (nombre mal escrito, fuera de alcance o 5 GHz) |
+--------------------+--------------------------------------------------------------------+
| WL_CONNECT_FAILED  | falló al conectar (por ejemplo, contraseña incorrecta)             |
+--------------------+--------------------------------------------------------------------+
| WL_CONNECTED       | conectado al router y con IP                                       |
+--------------------+--------------------------------------------------------------------+
| WL_DISCONNECTED    | no está conectado                                                  |
+--------------------+--------------------------------------------------------------------+
| WL_CONNECTION_LOST | estaba conectado y se perdió                                       |
+--------------------+--------------------------------------------------------------------+

*/

  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());// la funcion WiFi.localIP() es como ipconfig en windows o ip addr en Linux, nos devuelve la dirección IP que el router le dio al ESP32.
}

void loop() {
}

/*
=========== Diferencia entre #include <...> y #include "..." ===========

<archivo.h>: busca en las carpetas del sistema o del microcontrolador y de las librerías instaladas

"archivo.h": busca primero en la carpeta de mi proyecto y, si no lo encuentra,sigue buscando en las mismas carpetas que <...>

si a una libreria que no esta dentro de la carpeta de mi proyecto o no es mia le ponemos "..." tambien la va a encontrar porque primero checa en la carpeta del proyecto y si no
en las carpetas del sistema o del microcontrolador, pero se recomienda que a las librerias de terceros le pongamos <...> para aclarar que es de terceros y no mia  

Nota: en Arduino, <secrets.h> también compila porque el compilador agrega la carpeta
del sketch a la lista de búsqueda. En C normal (gcc) daría error. Pendiente: revisar
el orden de esa lista con la salida detallada de compilación (-I) y probarlo en C normal.
*/
