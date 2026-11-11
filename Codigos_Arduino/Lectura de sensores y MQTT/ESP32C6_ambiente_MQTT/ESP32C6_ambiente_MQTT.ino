#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <DHT.h>   
#include <WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"   // SSID_LOCAL, PASS_LOCAL, SERVIDOR_IP 

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085); // clase y objeto para controlar al sensor y un ID para identificar el sensor puede ser cualquier numero solo que sea distinto de otro sensor

WiFiClient clienteWiFi;// WiFiClient es la clase que abre una conexión TCP desde la placa hacia un servidor de la red (en este caso la laptop). 
//Se usa después de WiFi.begin(), cuando la placa ya tiene IP. Sirve para usar servicios que funcionan sobre TCP como MQTT o HTTP, creamos un objeto para 

PubSubClient mqtt(clienteWiFi);// el objeto mqtt sabe armar y leer los paquetes MQTT, pero no sabe por dónde enviarlos, y por eso le pasamos el objeto clienteWiFi       


unsigned long UltimaLecturaSensores = 0;

//pines I2C
#define PIN_SDA 6
#define PIN_SCL 7

//pin y modelo del DHT
#define DHTPIN 20         // pin digital
#define DHTTYPE DHT11     // modelo del sensor (la librería también soporta DHT22)

//pin del LDR y tiempo de ultima lectura
#define PIN_LDR 0 // GPIO0 (entrada analogica)
unsigned long UltimaLectura = 0;   

DHT dht(DHTPIN, DHTTYPE); // objeto que representa alsensor, guarda los datos que le pasamos el pin y el modelo del sensor

// variable global para la ip
String ip = "";


void setup() {
  Serial.begin(9600);
// ======== conexioon a una red ===============
  WiFi.begin(SSID_LOCAL, PASS_LOCAL); // busca la red y luego introduce la contraseña de la red

  while (WiFi.status() != WL_CONNECTED) {// si es diferente de conectado, WiFi.status() nos dice en qué punto está la conexión con el router en ese momento
    delay(500);
    Serial.print(".");
  }

// ======== coneccion a MQTT ================
  mqtt.setServer(SERVIDOR_IP, 1883);// con esta funcion es le decimos que de este dispositivo o servidor queremos acceder a este pruerto donde esta el servicio MQTT, 
  //para decir a que dispositivo servidor se le pone la ip del dispositivo o servidor
  mqtt.setKeepAlive(5);// si en 5s el cliente ya no responde, el broker lo da por caido o desconectado, de esta forma se entenera el broker que el cliente se desconecto, una vez pase este tiempo manda el mensaje de mensajeOffline

// ======== Inciar BMP180 ===============
  Serial.println("Iniciando BMP180...");
  Wire.setPins(PIN_SDA, PIN_SCL);// guarda los pines para despues inicializar la comunicacion I2C
  //Wire.begin(PIN_SDA, PIN_SCL);

  if (!bmp.begin()) { // si el sensor no responde correctamente o no se inicia, bmp.begin internamente tiene a wire.begin este toma los pines que le asignamos para inicializar la comunicacion I2C
    Serial.println("No se detecta el BMP180, revisa conexiones!");
  }
  else{
    Serial.println("BMP180 detectado exito");
  }
//======= Inciar DHT11 =============
    dht.begin(); // configura el pin como INPUT_PULLUP
}

void loop() {
// ============= verificacion de coneccion con MQTT ==========
while (!mqtt.connected()) {// la funcion mqtt.connected() Verifica si la sesión MQTT esta activa y devuelve true si sí y false si no, 
  //Mientras este desconectado vueleve a intentar conectarse, una vez que la condicion if (mqtt.connect("esp32c6")) de true se vuelve a verificar en el while, 
  //ahora seria true con not cambia a false y se sale del ciclo

    ip = WiFi.localIP().toString();// obtiene la direccion ip y la convierte a string
    Serial.print("Conectando a MQTT... ");
    String mensajeOffline = "{\"estado\":\"offline\",\"ip\":\"" + ip + "\"}";
/*
  la funcion mqtt.connect() es la que conecta a mqtt. Usa la direccion IP del servidor (Laptop)
  y el puerto que se guardaron antes con mqtt.setServer(), y manda un paquete al broker con:
   - el identificador: "esp32c6", el nombre con el que el broker reconoce a ese cliente
   - la version del protocolo (MQTT 3.1.1)
   - el mensaje de ultima voluntad (LWT): el broker lo publica solo si el nodo se cae sin avisar

  Parametros: ID del cliente, topic del will, QoS (0: mandar solamente una vez el mensaje
  a los que estan suscritos al topic), retain (true: el broker guarda el mensaje y se lo entrega
  a cada suscriptor nuevo en el momento en que se suscribe al topic) y el mensaje.

  devuelve true si el broker acepto, false si no
*/
    if (mqtt.connect("esp32c6", "esp32c6/status", 0, true, mensajeOffline.c_str())) { 
      Serial.println("conectado");
      String mensajeOnline  = "{\"estado\":\"online\",\"ip\":\""  + ip + "\"}";// una vez conectado creamos un mensaje y lo publicamos en un topic llamado esp32c6/status
      mqtt.publish("esp32c6/status", mensajeOnline.c_str(), true);// el true sirve para que el broker guarde el mensaje y se lo entregue a cada suscriptor nuevo en el momento en que se suscribe al topic
    } else {// si el broker no acepta 
      Serial.print("fallo, codigo ");
      Serial.println(mqtt.state());//La funcion mqtt.state() devuelve en que estado esta el cliente con la coneccion MQTT, puede devolver valores diferentes segun el estado
      delay(2000); // espera y reintenta
    }
  }

mqtt.loop();// Mantiene el keep-alive mandando pings los cuales se usan para decirle al broker que sigue conectado y para que el broker conteste que sigue escuchando 
//y si fuera suscriptor recibiria y procesaria lo que llegara del broker, si despues de los segundos que se pusieron en mqtt.setKeepAlive(); ya no manda pings el clinete, el broker lo da por caido

if (millis()-UltimaLecturaSensores >= 2000) {// si el tiempo trancurrido actual menos el tiempo transcurrido de la ultima lectura de los sensores es mayor o igual a 2000
UltimaLecturaSensores = millis();// guarda el tiempo transcurrido de esta lectura

  // ====== BMP180 =================
  sensors_event_t event; // estructura sensors_event_t de la libreria <Adafruit_Sensor.h>, la variable event es de tipo de dato compuesto y ella guarda todo lo que contiene la misma estructura
  bmp.getEvent(&event); //  getEvent pide al sensor los datos atraves de wire y los guarda en event, es decir en la primera variable llamada pressure

  if (event.pressure) { // Si la lectura de presión es válida (no cero)
    // Temperatura
    float temperature; ///event.temperature NO existe en esta librería se necesita varible extra
    bmp.getTemperature(&temperature);// le estamos diciendo que mida la temperatura y almacena el valor en la varibale temperature, geTemoperature ocuapa la direccion de memoria de la variable donde se va a guardar el valor
    Serial.print("[BMP180]: Temperatura: ");
    Serial.print(temperature);
    Serial.print(" °C   |    ");

    // Presión
    Serial.print("Presion: ");
    Serial.print(event.pressure); //Dentro de event, la presión queda en event.pressure, accedemos al valor con event.pressure
    Serial.print(" hectopascal || ");

    String mensajeBmp = "{\"temp\":" + String(temperature, 1) + ",\"presion\":" + String(event.pressure, 1) + "}";
    mqtt.publish("esp32c6/bmp180", mensajeBmp.c_str());
  }

// ======== DHT11 =======================
  float humedad = dht.readHumidity(); // pide los 40 bits y devuelve el %
  float temperatura = dht.readTemperature(); // pide los 40 bits y devuelve en °C

  Serial.print("[DHT11]: Humedad: ");
  Serial.print(humedad);
  Serial.print(" %  |  Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" C || ");

  // Si la lectura de humedad o temperatura no es un numero 
  if (isnan(humedad) || isnan(temperatura)) {// las funcion isnan nos devuelve true si no es un numero
    Serial.println("Error leyendo el DHT11");
  }
  else {
  String mensajeDht = "{\"temp\":" + String(temperatura, 1) + ",\"hum\":" + String(humedad, 1) + "}";
  mqtt.publish("esp32c6/dht11", mensajeDht.c_str());
  }

}

// ======== LDR ================

if (millis() - UltimaLectura >= 500) {// si el tiempo transcurrido actual menos el tiempo trancurrido de la ultima lectura es mayor o igual a 500
  UltimaLectura = millis();// guarda el tiempo transcurrido de esta lectura

  int valor = analogRead(PIN_LDR);// lee el voltaje del pin analógico, el ADC convierte los voltajes de 0v a 3.3v a valores digitales de 0 a 4095

String mensajeLdr = "{\"luz\":" + String(valor) + "}"; // los \ indican que el caracter " es especial, para que no lo tome
mqtt.publish("esp32c6/ldr", mensajeLdr.c_str());// el metodo .c_str() devuelve un apuntador (const char*) a las letras que guarda el String

  Serial.print("LDR (0-4095): ");
  Serial.println(valor); //imprimimos el valor 
 } 
}