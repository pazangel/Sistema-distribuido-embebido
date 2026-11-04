// pin digital del KY-002
#define pin_KY 2

//variable global para guardar el ultimo valor 
int valorAnterior = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin_KY, INPUT);

}

void loop() {
  int valor = digitalRead(pin_KY);

  if (valor != valorAnterior) {// si valor es diferente del ultimo valor guardado
    valorAnterior = valor;// actualizamos el valor anterior con el nuevo valor
    if (valor == 1) {// si valor es 1
    Serial.println("Hubo vibracion: " + String(valor));
    }
    else {
    Serial.println("No hubo vibracion: " + String(valor));
    }
    
  }
delay(50);

}
/*
============= Qué es el KY-002 ========================

Dentro del cilindro negro hay un resorte con un contacto que en reposo no se tocan, cuando algo lo sacude o golpea, 
el resorte se mueve, toca el contacto durante unos milisegundos y cambia la señal.

lo malo de este sensor esque tiene demasiado ruido
*/