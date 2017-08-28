#include <LiquidCrystal.h>

LiquidCrystal lcd(5, 6, 9, 10, 11 , 12);

void setup() {
  pinMode(2, INPUT_PULLUP);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  mensajeLCD("MEDIDOR NERF 1.0", 0);

}

unsigned long tiempoConOscuridad_Inicio = 0;
bool contandoOscuridad = false;
unsigned long tiempoConOscuridad = 0;
unsigned long now = 0;

void loop() {
  //mensajeLCD(String(micros()));
  //return;

  if (!hayLuz()) {
    if (!contandoOscuridad) {
      contandoOscuridad = true;
      tiempoConOscuridad_Inicio = micros();
    } 
    
    now = micros();
    tiempoConOscuridad = (now - tiempoConOscuridad_Inicio);

  } else {
    contandoOscuridad = false;
  }

  mensajeLCD("" + String(now),0);
  mensajeLCD("" + String(tiempoConOscuridad_Inicio) + "." + String(tiempoConOscuridad),1);

  //generarMensaje(tiempoConOscuridad);  
  
}


void generarMensaje(int tiempoConOscuridad) {
  String mensaje = "O:" + String(tiempoConOscuridad) + "us." ;
  mensajeLCD(mensaje,0);
}

bool hayLuz() {
  if (digitalRead(2) == LOW) return true;
  else return false;
}

void mensajeLCD(String mensaje, int row) {
  lcd.setCursor(0, row);
  lcd.print(mensaje + "          ");
}

void mensajeLCD(String mensaje) {
  mensajeLCD(mensaje, 1);
}

