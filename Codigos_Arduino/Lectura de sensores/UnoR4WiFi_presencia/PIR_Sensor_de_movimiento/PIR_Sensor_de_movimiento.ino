const int PIN_PIR = 2;// pin digital donde esta conectado el sesnor

int estadoAnterior = LOW;// incia con un estado
unsigned long inicioMovimiento = 0;// variable para guardar el tiempo

void setup() {
  Serial.begin(9600);// el PIR necesita 30-60 s al encender
  pinMode(PIN_PIR, INPUT);

  Serial.println("PIR: calentando 60 s, no te muevas frente al sensor...");
  delay(60000);
  Serial.println("PIR listo.");
}

void loop() {
  int estado = digitalRead(PIN_PIR);//leemos el estado del pin 2

  if (estado != estadoAnterior) { // si estado es diferente del anterior
    if (estado == HIGH) {// si hubo un movimiento el pin se pone en HIGH
      inicioMovimiento = millis();// empieza a contar cuanto tiempo el sensor deja el pin en HIGH, esto depende de como este el potenciometro del sensor 
      Serial.println("MOVIMIENTO detectado");
    } else {// si el sensor ya dejo de poner el el pin HIGH entonces esta en LOW
      float duracion = (millis() - inicioMovimiento) / 1000.0;// milesegundos actales menos los milesegundos en que el pin estuvo en HIGH, dividimos el resultado entre 100.0 para convertilos a segundos 
      Serial.print("Sin movimiento. El pin "+ String(PIN_PIR)+" estuvo en HIGH ");
      Serial.print(duracion, 1);//imprimimos los segundos con un decimal
      Serial.println(" s");
    }
    estadoAnterior = estado;// actualizamos el estado
  }
}

/*
===== POTENCIÓMETROS =====
El sensor tiene dos potenciometros uno es para decidir cuanto tiempo pone el pin digital en HIGH y el otro ajusta la distancia máxima a la que el sensor detecta
movimiento (de ~3 m a ~7 m)

===== JUMPER DE MODO (H / L) =====
el sensor tiene una header de 3 pines con un jumper amarillo, con pines H y L, el pin del medio el que esta en medio de L y H es un pin del chip del sensor para saber en que modo esta y 
para saber el modo tenemos que puentear ese pin con L o H

Modo L:
cuando se pone el jumper del lado de L y hay movimiento pone el pin en HIGH, el temporizador corre(el que pusimos con el potenciometro) y mientras corre se ignora el movimiento. Cuando se acaba el tiempoo, pone el pin en lOW
y el sensor vuelve a estar listo para detectar.
pero cuando baja hay otro temporizador que pone el chip del sensor, asi que es normal que cuando cuando el pin se pone en LOW no detecte otro movimiento todavia

Modo H:
cuando se pone el jumper del lado de H y hay movimiento pone el pin en HIGH y el temporizador corre(el que pusimos con el potenciometro) y si vuelve a detectar un movimiento reinicia el temporizador que se puso con el potenciometro, cuando deja de detectase movimiento
pone el pin en LOW, aqui tambien el chip del sensor pone el temporizador osea que tambien es normal que aya retardo entre movimientos 

Sin el jumper amarillo:
Sin el jumper amarillo, el modo queda indefinido: la entrada del chip puede tomar H o L
por defecto (sin saber cuál) o quedar como entrada flotante, y entonces el modo puede
cambiar con ruido eléctrico. Por eso el jumper se deja siempre puesto en algun modo

===== CALENTAMIENTO =====
El PIR necesita un tiempo de calentamiento al encenderse de 30 a 60 s (por eso el delay de 60000 de setup)
Durante ese tiempo se estabiliza con el calor del ambiente, si no se hace esto puede no detectar movimientos reales y aveces puede detectar movimientos irreales, esto solo pasa al inicio
asta que el sensor caliente
*/

/*
Lectura de sensores/
├── UnoR4WiFi_presencia/UnoR4WiFi_presencia.ino
├── NanoESP32_presencia/NanoESP32_presencia.ino
└── ESP32C6_ambiente/ESP32C6_ambiente.ino
*/