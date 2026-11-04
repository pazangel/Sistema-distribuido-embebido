#include <DHT.h>          

#define DHTPIN 20         // GPIO donde conectaste el pin S
#define DHTTYPE DHT11     // modelo del sensor (la librería también soporta DHT22)

DHT dht(DHTPIN, DHTTYPE); // objeto que representa alsensor, guarda los datos que le pasamos el pin y el modelo del sensor

void setup() {
  Serial.begin(9600);
  dht.begin(); // configura el pin como INPUT_PULLUP
}

void loop() {
  
  float humedad = dht.readHumidity(); // pide los 40 bits y devuelve el %
  float temperatura = dht.readTemperature(); // pide los 40 bits y devuelve en °C

  // Si la lectura de humedad o temperatura no es un numero 
  if (isnan(humedad) || isnan(temperatura)) {// las funcion isnan nos devuelve true si no es un numero
    Serial.println("Error leyendo el DHT11");
    return;// sal de loop() y vuelve a intentar
  }

  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.print(" %  |  Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" C");

  delay(2000);            // el DHT11 no admite más de 1 lectura por segundo
}

/*
====== cómo funciona internamente la librería =======
El DHT11 manda los datos con pulsos, parecido a un PWM, lo que importa es cuánto
dura cada pulso en HIGH. Si el HIGH es más corto que el LOW anterior, el bit
es 0, si es más largo, es 1.
La librería lee el pin digital (cuenta cuánto tiempo se queda en HIGH), decide cada
bit y lo va guardando en un buffer de 5 bytes (40 bits): humedad, humedad decimal,
temperatura, temperatura decimal y checksum.
El checksum es la suma de los primeros 4 bytes; si no coincide, la lectura se descarta
y devuelve NaN.

Ejemplo de una lectura: bits 0 0 0 1 1 0 0 0 ---> byte 00011000 = 24 (°C).
*/