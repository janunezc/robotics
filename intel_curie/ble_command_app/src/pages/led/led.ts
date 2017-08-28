import {Component} from '@angular/core';
import {NavController} from 'ionic-angular';
import {BLE} from '@ionic-native/ble';
import {ApplicationRef} from '@angular/core';

@Component({
    selector: 'page-led',
    templateUrl: 'led.html'
})

export class LEDPage {

    public messages = []; //Histórico de ejecución
    public ejecutandoComando = false;
    public BLE;
    public ComandoTXT = "";
    public Value = "";
    public TargetDevice;
    private appRef;

    /**
     * CONSTANTES que serán usadas a lo largo del controlador
     */
    public constants = {
        DEVICE_NAME: "COMANDO LED",
        CMD_FIND_DEVICE: "FIND DEVICE",
        CMD_STOP_SCAN: "Escaneando... (Clic para parar)",
        CMD_TOGGLE_LED: "CAMBIAR LED",
        ON: "ON",
        OFF: "OFF"
    };

    /**
     * Constructor para el controlador. Todo comienza acá...
     */
    constructor(public navCtrl: NavController, private ble: BLE /*Librería BLE nativa*/, private applicationRef: ApplicationRef /*Librería ApplicationRef de Angular*/) {
        this.appRef = applicationRef; //Referencia a la aplicación ANGULAR
        this.SetMessage("Constructor: Iniciado!");
        this.BLE = ble; //Acceso a la librería BLE Nativa
        this.ComandoTXT = this.constants.CMD_FIND_DEVICE;
        this.Value = "ON"; //VALOR PARA ENVIAR
        this.TargetDevice = {}; //Dispositivo al cual conectarse. Inicialmente vacío... se llenará cuando se encuentre el dispositivo en el escaneo.
        this['service_id'] = "db938b80-f010-44b6-8aa9-1835adf9419a"; //Identificador del servicio BLE a conectarse
        this['characteristic_id'] = "9906064e-9bbe-4eba-b415-bbd223f7d3d9"; //Identificador de la característica BLE a conectarse
        this.SetMessage("Constructor: Finalizado!");
    }

    /**
     * This is triggered when the button is clicked.
     */
    public Command() {
        this.ejecutandoComando = true;
        this.SetMessage("Command() - Comando recibido!");
        this.SetMessage(this["ComandoTXT"]);
        this.SetMessage("Interpretando comando...");

        if (this['ComandoTXT'] === this.constants.CMD_FIND_DEVICE) {
            this.SetMessage("Ejecutando Comando: FIND DEVICE");
            this['ComandoTXT'] = this.constants.CMD_STOP_SCAN;
            this.findDevice();

        } else if (this['ComandoTXT'] === this.constants.CMD_STOP_SCAN) {
            this.SetMessage("Ejecutando Comando: STOP SCAN");
            clearInterval(this['intervalHandle']);
            this['ComandoTXT'] = this.constants.CMD_FIND_DEVICE;
            this.ejecutandoComando = false;

        } else if (this['ComandoTXT'] === this.constants.CMD_TOGGLE_LED) {
            this.SetMessage("Ejecutando Comando: TOGGLE LED: " + this.Value);
            this.SetMessage("Acá es donde enviamos un valor al dispositivo BLE");
            this.txData();
        }
    }

    /**
     * This searches for DEVICE in the AIR using SCAN technique.
     */
    private findDevice() {
        this.SetMessage("FindDevice() - INICIO!");
        let ble = this.BLE;

        this.SetMessage("FindDevice() - ENABLE!");
        ble.enable();

        this.SetMessage("FindDevice() - SET INTERVAL!");
        this['intervalHandle'] = setInterval(() => { // PROGRAMAR UN TIMER DE 2.1 SEGUNDOS
            this.SetMessage("INTERVAL: INICIO! LLAMANDO A BLE SCAN...");
            ble.scan([], 2 /*seconds (0) */).subscribe(data => { //REALIZAR SCAN BLE POR 2 SEGUNDOS
                this.SetMessage("BLE SCAN CALLBACK: " + data['id'] + ' | ' + data['name'] + ' | ' + data['rssi']);
                if (data['name'] == this.constants.DEVICE_NAME) {
                    this.SetMessage(this.constants.DEVICE_NAME + " HA SIDO ENCONTRADO!!");
                    clearInterval(this["intervalHandle"]);
                    this.TargetDevice = data;
                    this["ComandoTXT"] = this.constants.CMD_TOGGLE_LED;
                    this.ejecutandoComando = false;
                }
                this.appRef.tick();
            });
        }, 2100);//FIN DE LA DEFINICIÓN DEL TIMER
        this.ejecutandoComando = false;
    }

    /**
     * Transmitir datos al dispositivo BLE
     */
    private txData() {
        this.SetMessage("txData(): INICIO! Llamando a BLE CONNECT...");

        let id = this.TargetDevice.id;
        this.SetMessage("ID DE DISPOSITIVO: " + id);

        this.ble.connect(id).subscribe(datos => {
            this.SetMessage("BLE CONNECT CALLBACK: INICIO!. Llamando a BLE WRITE..." + this.Value);

            this.ble.write(this.TargetDevice.id, this['service_id'], this['characteristic_id'], this.StringToBytes(this.Value)).then(() => {
                this.SetMessage("BLE WRITE CALLBACK: INICIO! Cambiando valor... " + this.Value);
                if (this.Value == this.constants.ON) {
                    this.Value = this.constants.OFF;
                } else {
                    this.Value = this.constants.ON;
                }
                this.SetMessage("Nuevo valor... " + this.Value);

                this.SetMessage("Llamando a BLE DISCONNECT...");
                this.ble.disconnect(id);
                this.ejecutandoComando = false;
                this.appRef.tick();
            }, (error) => {
                this.SetMessage("BLE Write ERROR!");
                this.SetMessage(error);
                this.SetMessage("Llamando a BLE DISCONNECT...");
                this.ble.disconnect(id);
                this.ejecutandoComando = false;
                this.appRef.tick();
            });
        }, error => {
            this.SetMessage("BLE Connect ERROR!");
            this.SetMessage(error.message);
            this.ejecutandoComando = false;
            this.appRef.tick();
        });
    }

    /**
     * Agrega mensajes de ejecución al histórico
     */
    public SetMessage(message) {
        var count = this['messages'].length;
        message = count + ':' + message;
        this['messages'].unshift(message);

        /*
          ESTA LINEA ES IMPORTANTE PARA REFRESCAR LA PANTALLA CUANDO SE GENERAN EVENTOS
          FUERA DEL CONTROL DE ANGULAR. POR EJEMPLO CUANDO SE LLAMA A UN CALLBACK EN
          UN PROCESO DE BLE SCAN.
        */
        this.appRef.tick();
    }

    // ASCII only. Convierte el valor a escribir a un arreglo de BYTES
    public StringToBytes(string) {
        let array = new Uint8Array(string.length);
        for (let i = 0, l = string.length; i < l; i++) {
            array[i] = string.charCodeAt(i);
        }
        return array.buffer;
    }

    // ASCII only
    public BytesToString(buffer) {
        return String.fromCharCode.apply(null, new Uint8Array(buffer));
    }
}
