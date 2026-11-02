#include <ld2410.h>

ld2410 radar;  // objeto de la libreria

#define PIN_ld2410 3  // pin digital

void setup() {
  Serial.begin(9600);

  pinMode(PIN_ld2410, INPUT);// configuramos el pin digital como entrada

  Serial1.begin(256000);  //incializamos la comunicacion serial con el LD2410
  Serial.println("Iniciando radar LD2410...");

  if (radar.begin(Serial1)) {// verifica si el LD2410 responde por serial1, radar.begin siempre ocupa el objeto de los puertos seriales, true si el radar respondió y false si no
    Serial.println("Radar conectado.");
  } else {
    Serial.println("Sin respuesta del radar. Revisa TX/RX, GND y el divisor.");
  }
}

void loop() {
  radar.read();// toma el string que llega por serial y saca cada dato del string y los guarda en variables internas para luego acceder a su valor

  if (!radar.isConnected()) { // isConnected() dice si el radar sigue mandando datos con regularidad (a diferencia de radar.begin(Serial1), que solo comprueba al arrancar),Si la funcion isConnected() devuelve false, avisamos y no imprimimos valores viejos.
    Serial.println("Radar sin datos...");
    return;
  }


if (digitalRead(PIN_ld2410)== HIGH ){ // si en el pin digital hay un 1 quiere decir que hay presencia serca, si hay un 0 quiere decir que no hay presencia
Serial.print("pin digital - Se a detectado a alguien |");
}
else{
  Serial.print("pin digital - no hay nadie |");
}

if (radar.presenceDetected()) { // esta funcion hace lo mismo que el pin digital, 1 si hay presencia, 0 si no hay presencia
  Serial.print(" UART - se ha detectado a alguien");
} else {
  Serial.print(" UART - no hay nadie");
}

if(radar.movingTargetDetected()) {// devueleve true si esta una persona moviendose, false si no hay una persona moviendose
    Serial.print(" | MOVIMIENTO ");
    Serial.print(radar.movingTargetDistance());// esta funcion nos devuelve a cuántos cm está esa persona
    Serial.print(" cm, intensidad de la señal ");
    Serial.print(radar.movingTargetEnergy());// esta funcion devuelve qué tan fuerte regresó la señal o el eco de la persona quieta en una escala de 0 al 100
  }
  
  else if (radar.stationaryTargetDetected()) {// esta funcion nos devuleve true si hay una persona quieta, ya sea senta o parada o false si no hay una persona quieta
    Serial.print(" | QUIETO ");
    Serial.print(radar.stationaryTargetDistance());// esta funcion nos devuelve a cuántos cm está esa persona
    Serial.print(" cm, intensidad de la señal ");
    Serial.print(radar.stationaryTargetEnergy());// esta funcion devuelve qué tan fuerte regresó la señal o el eco de la persona quieta en una escala de 0 al 100
  }
  
  Serial.println();

  delay(500);
}

/*
  ===== PRINCIPIO DE FUNCIONAMIENTO =====
  El LD2410C es un radar de presencia que trabaja con microondas de 24 GHz (la misma
  familia de ondas que WiFi y Bluetooth, que usan 2.4 GHz). Emite ondas, escucha el eco
  que rebota y con eso calcula distancia y movimiento.

  ===== COMPARACION CON EL ULTRASONICO (HC-SR04) =====
  Parecido: los dos emiten una onda, esperan el eco y miden.
  Diferencias:
  - Onda: el HC-SR04 usa sonido (40 kHz) y el LD2410 usa microondas (24 GHz).
  - Distancia: el HC-SR04 mide el tiempo que tarda el eco. Con microondas ese tiempo
    dura nanosegundos y no se puede medir directo, asi que el LD2410 emite una frecuencia
    que sube con el tiempo (funciona como un reloj). El eco regresa con la frecuencia que
    tenia al salir y el radar ya esta emitiendo una mas alta: la diferencia indica cuanto
    tardo el eco, y a mayor diferencia, mayor distancia.
  - Cuarto vacio: el HC-SR04 reporta la distancia a la pared. El LD2410 no reporta nada,
    porque ignora lo que no cambia.
  - Persona quieta: el LD2410 la detecta por micro-movimientos (respiracion). El HC-SR04
    solo ve una distancia y no sabe si es una persona.
*/