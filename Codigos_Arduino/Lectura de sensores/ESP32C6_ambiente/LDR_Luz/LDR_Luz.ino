#define PIN_LDR 0 // GPIO0 (entrada analogica)

void setup() {
  Serial.begin(9600);// inicializa la comunicacion serial a 9600 baudios
}

void loop() {
  int valor = analogRead(PIN_LDR);// lee el voltaje del pin analógico, el ADC convierte los voltajes de 0v a 3.3v a valores digitales de 0 a 4095

  Serial.print("LDR (0-4095): ");
  Serial.println(valor); //imprimimos el valor 

  delay(500);                          // espera medio segundo entre lecturas
}
/*
MUCHA LUZ:
el LDR baja su resistencia le toca poco voltaje del reparto
casi todo el voltaje cae en la resistencia fija
el pin lee un voltaje alto, valor del ADC alto (hacia 4095)

POCA LUZ (oscuro):
el LDR sube mucho su resistencia se queda con casi todo el voltaje
a la resistencia fija le llega muy poco
el pin lee un voltaje bajo, valor del ADC bajo (hacia 0)
*/