#include <CurieBLE.h>
#define ledPin 3

//Definimos un servicio BLE
BLEService servicioBLE_Comandos("db938b80-f010-44b6-8aa9-1835adf9419a"); // create service

//Definimos una característica que pueda ser leida o escrita desde una central BLE, con capacidad de 20 bytes por mensaje.
BLECharacteristic caracteristicaBLE_Comandos("9906064e-9bbe-4eba-b415-bbd223f7d3d9", BLERead | BLEWrite, 20);

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); 
  delay(500); //Esperamos un poco de tiempo (500ms) por si se puede habilitar la terminal serial.
  parpadear(5,200);//Señalizamos con el LED que estamos iniciando el sketch
  
  BLE.begin(); //Inicializamos el sistema Bluetooth Low Energy (BLE) del Curie
  parpadear(2 ,100);//Señal de aviso de que la operación anterior fue exitosa
  
  BLE.setLocalName("COMANDO LED"); //Definimos un nombre para publicitar nuestro dispositivo
  BLE.setAdvertisedService(servicioBLE_Comandos);//Definimos el servicio que se va a publicitar
  servicioBLE_Comandos.addCharacteristic(caracteristicaBLE_Comandos); //Agregamos la característica al servicio
  BLE.addService(servicioBLE_Comandos); //Agregamos el servicio al dispositivo  
  BLE.advertise(); // Publicitamos el dispositivo
  parpadear(3 ,100);//Señal de aviso de que la operación anterior fue exitosa

  Serial.println("En este punto el dispositivo queda listo, publicitado y esperando conexiones...");
  parpadear(10,50);//Señal de aviso de que SETUP se concluyó satisfactoriamente
}

void loop() {
  BLE.poll(); // Obtenemos cualquier evento BLE que haya sido enviado al dispositivo

  if (caracteristicaBLE_Comandos.written()) { //Si hay datos, ejecutamos el comando
    leerYEjecutarComado();
  } // ... caso contrario simplemente se termina el flujo de loop()
}

/**
 * Este método lee el comando registrado en la característica BLE y lo ejecuta.
 */
void leerYEjecutarComado(){
    String valorDelComando = obtenerValorDelComando();

    if (valorDelComando.startsWith("ON")) {
      Serial.println("LED on");
      digitalWrite(ledPin, HIGH);
    } else {
      Serial.println("LED off");
      digitalWrite(ledPin, LOW);
    }
}

/**
 * Extraer el valor del comando en formato String
 */
String obtenerValorDelComando(){
    int longitudDelComando = caracteristicaBLE_Comandos.valueLength();
    Serial.print("Longitud: ");
    Serial.println(longitudDelComando);
      
    const byte* val = caracteristicaBLE_Comandos.value();
    
    String strValor = String((const char *)val).substring(0, longitudDelComando);
    Serial.println("|" + strValor + "|");

    return strValor;
}

void parpadear(int times, int milliseconds){
  for(int i=0; i<times; i++){
    digitalWrite(ledPin, HIGH);
    delay(milliseconds);
    digitalWrite(ledPin, LOW);
    delay(milliseconds);
  }
}
