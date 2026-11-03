#include <ld2410.h>

ld2410 radar;  // objeto de la libreria

#define PIN_ld2410 3  // pin digital
const int PIN_PIR = 2;// pin digital donde esta conectado el sesnor

int estadoAnterior = LOW;// incia con un estado
unsigned long inicioMovimiento = 0;// variable para guardar el tiempo

unsigned long ultimaImpresion = 0;

void setup() {
  Serial.begin(9600);

// ====== PIR ======
  pinMode(PIN_PIR, INPUT);
  /*
  Serial.println("PIR: calentando 60 s, no te muevas frente al sensor...");
  delay(60000);
  Serial.println("PIR listo.");
  */

// ===== ld2410 ========
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
// ==== PIR =========
if (millis() >= 60000) { // si han pasado 60 segundos desde que se encio la placa

  int estado = digitalRead(PIN_PIR);//leemos el estado del pin 2

  if (estado != estadoAnterior) { // si estado es diferente del anterior
    if (estado == HIGH) {// si hubo un movimiento el pin se pone en HIGH
      inicioMovimiento = millis();// empieza a contar cuanto tiempo el sensor deja el pin en HIGH, esto depende de como este el potenciometro del sensor 
      Serial.print("[PIR]: MOVIMIENTO detectado |");
    } else {// si el sensor ya dejo de poner el el pin HIGH entonces esta en LOW
      float duracion = (millis() - inicioMovimiento) / 1000.0;// milesegundos actales menos los milesegundos en que el pin estuvo en HIGH, dividimos el resultado entre 100.0 para convertilos a segundos 
      Serial.print("[PIR]: Sin movimiento. El pin "+ String(PIN_PIR)+" estuvo en HIGH ");
      Serial.print(duracion, 1);//imprimimos los segundos con un decimal
      Serial.print(" s |");
    }
    estadoAnterior = estado;// actualizamos el estado
  }
}
// ==== LD2410 =========
  radar.read();// toma el string que llega por serial y saca cada dato del string y los guarda en variables internas para luego acceder a su valor

if (millis() - ultimaImpresion >= 500) {// si el tiempo actual menos el tiempo de la ultima impresion es mayor o igual a 500
  ultimaImpresion = millis();// guardamos el tiempo actual como la hora de esta impresion 

  if (!radar.isConnected()) { // isConnected() dice si el radar sigue mandando datos con regularidad (a diferencia de radar.begin(Serial1), que solo comprueba al arrancar),Si la funcion isConnected() devuelve false, avisamos y no imprimimos valores viejos.
    Serial.println("Radar sin datos...");
    return;
  }


if (digitalRead(PIN_ld2410)== HIGH ){ // si en el pin digital hay un 1 quiere decir que hay presencia serca, si hay un 0 quiere decir que no hay presencia
Serial.print("[RADAR]: pin digital - Se a detectado a alguien |");
}
else{
  Serial.print("[RADAR]: pin digital - no hay nadie |");
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
  }
}

/*
  ===== MULTITAREA =====
  Se uso multitarea para que las esperas de un sensor no detengan al otro.

   Problema: con delay() la placa se queda esperando en esa linea y no ejecuta nada mas
  hasta que pasa el tiempo.
  - delay(60000) del PIR: el radar no podia iniciar ni leerse durante ese minuto.
  - delay(500) del radar: podia afectar en la deteccion de movimiento del PIR.

  Solucion: cambiar los delay() por millis(). Cada tarea tiene su propio reloj:
  - PIR: se ignora hasta que millis() llega a 60000 (calentamiento).
  - Radar: radar.read() se ejecuta en cada vuelta, y se imprime cada 500 ms
    comparando millis() con la hora de la ultima impresion.


    ===== OBSERVACION: PIR vs RADAR =====

El radar es mas preciso que el PIR
Ejemplo real:

[PIR]: Sin movimiento. El pin 2 estuvo en HIGH 1.3 s |[RADAR]: ... MOVIMIENTO 30 cm, intensidad de la senal 100

El PIR reacciona rapido pero tiene puntos ciegos y no ve presencia quieta.
El radar da mas informacion (distancia, intensidad, movimiento o quieto). Por eso se
usan los dos en el cluster de Presencia: cuando uno falla, el otro lo cubre.
*/