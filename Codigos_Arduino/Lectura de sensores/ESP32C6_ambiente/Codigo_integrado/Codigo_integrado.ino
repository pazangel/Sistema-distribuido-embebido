#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <DHT.h>    

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085); // clase y objeto para controlar al sensor y un ID para identificar el sensor puede ser cualquier numero solo que sea distinto de otro sensor

unsigned long UltimaLecturaSensores = 0;

//pines I2C
#define PIN_SDA 6
#define PIN_SCL 7

//pin y modelo del DHT
#define DHTPIN 20         // Pin difital
#define DHTTYPE DHT11     // modelo del sensor (la librería también soporta DHT22)

//pin del LDR y tiempo de ultima lectura
#define PIN_LDR 0 // GPIO0 (entrada analogica)
unsigned long UltimaLectura = 0;   

DHT dht(DHTPIN, DHTTYPE); // objeto que representa alsensor, guarda los datos que le pasamos el pin y el modelo del sensor


void setup() {
  Serial.begin(9600);

  Serial.println("Iniciando BMP180...");
  Wire.setPins(PIN_SDA, PIN_SCL);// guarda los pines para despues inicializar la comunicacion I2C
  //Wire.begin(PIN_SDA, PIN_SCL);

  if (!bmp.begin()) { // si el sensor no responde correctamente o no se inicia, bmp.begin internamente tiene a wire.begin este toma los pines que le asignamos para inicializar la comunicacion I2C
    Serial.println("No se detecta el BMP180, revisa conexiones!");
    while (1); // while true se queda en un bucle bacio despues de mostrar el mensaje
  }

    dht.begin(); // configura el pin como INPUT_PULLUP
}

void loop() {
  // ====== BMP180 =================
  sensors_event_t event; // estructura sensors_event_t de la libreria <Adafruit_Sensor.h>, la variable event es de tipo de dato compuesto y ella guarda todo lo que contiene la misma estructura
  bmp.getEvent(&event); //  getEvent pide al sensor los datos atraves de wire y los guarda en event, es decir en la primera variable llamada pressure

if (millis()-UltimaLecturaSensores >= 2000) {// si el tiempo trancurrido actual menos el tiempo transcurrido de la ultima lectura de los sensores es mayor o igual a 2000
UltimaLecturaSensores = millis();// guarda el tiempo transcurrido de esta lectura

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
  }

// ======== DHT11 =======================
  float humedad = dht.readHumidity(); // pide los 40 bits y devuelve el %
  float temperatura = dht.readTemperature(); // pide los 40 bits y devuelve en °C

  // Si la lectura de humedad o temperatura no es un numero 
  if (isnan(humedad) || isnan(temperatura)) {// las funcion isnan nos devuelve true si no es un numero
    Serial.println("Error leyendo el DHT11");
    return;// sal de loop() y vuelve a intentar
  }

  Serial.print("[DHT11]: Humedad: ");
  Serial.print(humedad);
  Serial.print(" %  |  Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" C || ");
}

// ======== LDR ================

if (millis() - UltimaLectura >= 500) {// si el tiempo transcurrido actual menos el tiempo trancurrido de la ultima lectura es mayor o igual a 500
  UltimaLectura = millis();// guarda el tiempo transcurrido de esta lectura

  int valor = analogRead(PIN_LDR);// lee el voltaje del pin analógico, el ADC convierte los voltajes de 0v a 3.3v a valores digitales de 0 a 4095

  Serial.print("LDR (0-4095): ");
  Serial.println(valor); //imprimimos el valor 
 } 
}