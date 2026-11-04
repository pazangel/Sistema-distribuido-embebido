// pin digital del microfono
#define pin_MIC 3

// pin digital del KY-002
#define pin_KY 2

//variable global para guardar el ultimo valor del sensor de vibracion
int valorAnteriorKY = 0;

// variable global para guardar el ultimo valor del microfono
int valorAnteriorMIC= 0;

// variable para guardar el tiempo de la ultima lectura
unsigned long UltimoValorLeido = 0;  

void setup() {
  Serial.begin(9600);
  //configuracion del pin del microfono
  pinMode(pin_MIC, INPUT);

  //configuracion del pin del KY-002
  pinMode(pin_KY, INPUT);

}

void loop() {
  // ====================== sesnor de vibracion ===================
if(millis() - UltimoValorLeido >= 50){ // si el tiempo actual menos el tiempo transcurrido del ultimo valor leido es mayor o igual a 50
  UltimoValorLeido = millis();// guarda el tiempo transcurrido de este valor leido

  int valor = digitalRead(pin_KY);

  if (valor != valorAnteriorKY) {// si valor es diferente del ultimo valor guardado
    valorAnteriorKY = valor;// actualizamos el valor anterior con el nuevo valor
    if (valor == 1) {// si valor es 1
    Serial.println("[KY-002]: Hubo vibracion: " + String(valor)+" ");
    }
    else {
    Serial.println("[KY-002]: No hubo vibracion: " + String(valor)+" ");
    }
    
  }
}

// ====== microfono ===============
  int valor = digitalRead(pin_MIC);

  if (valor != valorAnteriorMIC) {   // si valor es diferente del ultimo valor guardado
    valorAnteriorMIC = valor;        // actualizamos el valor anterior con el nuevo valor
    if (valor == 1) {   // si el valor es 1
      Serial.println("[LM393]: Ruido detectado: " + String(valor));
    }
    else {
      Serial.println("[LM393]: Silencio: " + String(valor));
    }
  }

}
