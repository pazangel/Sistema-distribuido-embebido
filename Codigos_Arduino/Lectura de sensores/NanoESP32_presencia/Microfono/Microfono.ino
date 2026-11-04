// pin digital del módulo de sonido (OUT), conectado a D3
#define pin_MIC 3

// variable global para guardar el ultimo valor
int valorAnterior = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pin_MIC, INPUT);

}

void loop() {
  int valor = digitalRead(pin_MIC);

  if (valor != valorAnterior) {   // si valor es diferente del ultimo valor guardado
    valorAnterior = valor;        // actualizamos el valor anterior con el nuevo valor
    if (valor == 1) {   // si el valor es 1
      Serial.println("Ruido detectado: " + String(valor));
    }
    else {
      Serial.println("Silencio: " + String(valor));
    }
  }

}

/*
============= Qué es el módulo de sonido (LM393) ========================

El micrófono electret capta el sonido y el comparador LM393 lo compara con un umbral
que se ajusta con el potenciómetro azul. Si el sonido supera el umbral, cambia la señal en OUT.
Solo dice si hay ruido o no, no mide cuánto sonido hay.
*/