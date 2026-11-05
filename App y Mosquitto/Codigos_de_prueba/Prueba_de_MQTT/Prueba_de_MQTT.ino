#include <WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"   // SSID_LOCAL, PASS_LOCAL, BROKER_IP

WiFiClient clienteWiFi;// WiFiClient es la clase que abre una conexión TCP desde la placa hacia un servidor de la red (en este caso la laptop). 
//Se usa después de WiFi.begin(), cuando la placa ya tiene IP. Sirve para usar servicios que funcionan sobre TCP como MQTT o HTTP, creamos un objeto para 

PubSubClient mqtt(clienteWiFi);// el objeto mqtt sabe armar y leer los paquetes MQTT, pero no sabe por dónde enviarlos, y por eso le pasamos el objeto clienteWiFi       

void setup() {
  Serial.begin(9600);
// ======== Conectarase a wifi =============
  WiFi.begin(SSID_LOCAL, PASS_LOCAL);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("\nIP: ");
  Serial.println(WiFi.localIP());

  mqtt.setServer(SERVIDOR_IP, 1883);// con esta funcion es le decimos que de este dispositivo o servidor queremos acceder a este pruerto donde esta el servicio MQTT, 
  //para decir a que dispositivo servidor se le pone la ip del dispositivo o servidor
  mqtt.setKeepAlive(60);// si en 60s el cliente ya no responde, el broker lo da por caido o desconectado, de esta forma se entenera el broker que el cliente se desconecto, 
  //setKeepAlive manda ping al broker para decirle que sigue conectado
}

void loop() {
      while (!mqtt.connected()) {// la funcion mqtt.connected() Verifica si la sesión MQTT esta activa y devuelve true si sí y false si no, 
  //Mientras este desconectado vueleve a intentar conectarse, una vez que la condicion if (mqtt.connect("nodo3")) de true se vuelve a verificar en el while, 
  //ahora seria true con not cambia a false y se sale del ciclo
    Serial.print("Conectando a MQTT... ");
    if (mqtt.connect("nodo3")) { // la funcion mqtt.connect() es la que conecta a mqtt, aqui usa la direccion IP del servidor(Laptop) y el puerto donde esta el servicio, despues manda un paquete al broker con
    /*
     - el identificador: "nodo3" Es el nombre con el que el broker reconoce a ese cliente
     - versión del protocolo (MQTT 3.1.1)

     devuelve true si el broker aceptó, false si no
    */
      Serial.println("conectado");
    } else {// si el broker no acepta 
      Serial.print("fallo, codigo ");
      Serial.println(mqtt.state());//La funcion mqtt.state() devuelve en que estado esta el cliente con la coneccion MQTT, puede devolver valores diferentes segun el estado
/*
+-------+-------------------------------------------------------------------------------------------------+
| Valor | Significado                                                                                     |
+-------+-------------------------------------------------------------------------------------------------+
| 0     | conectado                                                                                       |
+-------+-------------------------------------------------------------------------------------------------+
| -1    | desconectado limpiamente (porque llamaste a disconnect)                                         |
+-------+-------------------------------------------------------------------------------------------------+
| -2    | falló al conectar (broker apagado, IP equivocada o algo bloqueando el puerto, como el firewall) |
+-------+-------------------------------------------------------------------------------------------------+
| -3    | se perdió la conexión que ya existía                                                            |
+-------+-------------------------------------------------------------------------------------------------+
| -4    | se acabó el tiempo esperando respuesta (keep-alive o CONNACK)                                   |
+-------+-------------------------------------------------------------------------------------------------+
| 1     | el broker rechazó la conexión: protocolo no soportado                                           |
+-------+-------------------------------------------------------------------------------------------------+
| 2     | el broker rechazó la conexión: identificador inválido                                           |
+-------+-------------------------------------------------------------------------------------------------+
| 3     | el broker rechazó la conexión: servidor no disponible                                           |
+-------+-------------------------------------------------------------------------------------------------+
| 4     | el broker rechazó la conexión: usuario o contraseña incorrectos                                 |
+-------+-------------------------------------------------------------------------------------------------+
| 5     | el broker rechazó la conexión: no autorizado                                                    |
+-------+-------------------------------------------------------------------------------------------------+
*/
      delay(2000);                          // espera y reintenta
    }
  }
  mqtt.loop();// Mantiene el keep-alive mandando pings y si fuera suscriptor, además recibiria y procesaria lo que llegara del broker.

    mqtt.publish("saludo", "Hola Mundo"); // La funcion mqtt.publish es para mandar mensajes al broker, y tiene una estructura, mqtt.publish(topic, mensaje)
    Serial.println("Publicado");

    delay(2000);
}

/*
broker: un broker es un programa que recibe todos los paquetes y los reparte a todos los clientes que estan suscritos, este recibe los paquetes por el puerto

Mosquitto: Mosquitto es el nombre del programa tal cual, el que usa el protocolo MQTT y el broker

topic: topic es una etiqueta que se le pone a cada mensaje esta la define quien publica en el broker y para repartir el mensaje el broker compara la etiqueta de cada mensaje contra las
peticiones de los clientes, y manda una copia a los que coinciden, a estas peticiones se les conoce como suscripciones
*/