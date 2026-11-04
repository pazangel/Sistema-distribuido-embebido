#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085); // clase y objeto para controlar al sensor y un ID para identificar el sensor puede ser cualquier numero solo que sea distinto de otro sensor
/*
la clase Adafruit_BMP085_Unified ya tiene la direccion fisica internamente por lo que ya no hay que decirle a wire a que direccion debe conectarse para que mande los datos
aun asi ocupa un id para identificar a ese sensor
*/ 

//pines I2C
#define PIN_SDA 6
#define PIN_SCL 7

void setup() {
  Serial.begin(9600);

  Serial.println("Iniciando BMP180...");
  Wire.setPins(PIN_SDA, PIN_SCL);// guarda los pines para despues inicializar la comunicacion I2C
  //Wire.begin(PIN_SDA, PIN_SCL);

  if (!bmp.begin()) { // si el sensor no responde correctamente o no se inicia, bmp.begin internamente tiene a wire.begin este toma los pines que le asignamos para inicializar la comunicacion I2C
    Serial.println("No se detecta el BMP180, revisa conexiones!");
    while (1); // while true se queda en un bucle bacio despues de mostrar el mensaje
  }
}

void loop() {
  sensors_event_t event; // estructura sensors_event_t de la libreria <Adafruit_Sensor.h>, la variable event es de tipo de dato compuesto y ella guarda todo lo que contiene la misma estructura
  bmp.getEvent(&event); //  getEvent pide al sensor los datos atraves de wire y los guarda en event, es decir en la primera variable llamada pressure
  /*
  cuando obtenemos la direccion de memoria de una variable que guarda la estructura, la direccion que obtenemos la dirección donde comienza toda la estructura, 
  es decir la de la primera variable 

  para acceder al contenido de una variable de una estructura es nombre_de_la_variable_de_la_esctructura.nombre_de_la_variable_ala_que_queremos_acceder
  */

  if (event.pressure) { // Si la lectura de presión es válida (no cero)
    // Temperatura
    float temperature; ///event.temperature NO existe en esta librería se necesita varible extra
    bmp.getTemperature(&temperature);// le estamos diciendo que mida la temperatura y almacena el valor en la varibale temperature, geTemoperature ocuapa la direccion de memoria de la variable donde se va a guardar el valor
    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.print(" °C   |    ");

    // Presión
    Serial.print("Presion: ");
    Serial.print(event.pressure); //Dentro de event, la presión queda en event.pressure, accedemos al valor con event.pressure
    Serial.println(" hectopascal");
  }

  delay(1000);
}
/*
=============== Diferencia entre Wire.setPins(sda, scl) y Wire.begin(sda, scl) ============================================

+------------------------+----------------------------------------------------+
| Función                | Qué hace                                           |
+------------------------+----------------------------------------------------+
| Wire.setPins(sda, scl) | Solo guarda los pines. No inicia el bus.           |
+------------------------+----------------------------------------------------+
| Wire.begin(sda, scl)   | Guarda los pines y además inicia el bus I2C ahora. |
+------------------------+----------------------------------------------------+
*/