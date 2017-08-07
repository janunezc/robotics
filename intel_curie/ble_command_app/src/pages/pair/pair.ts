import {Component} from '@angular/core';
import {NavController} from 'ionic-angular';
import {BLE} from '@ionic-native/ble';
import {ApplicationRef} from '@angular/core';

@Component({
    selector: 'page-pair',
    templateUrl: 'pair.html'
})

export class PairPage {

    public messages = []; //Histórico de ejecución
    public ejecutandoComando = false;
    public BLE;
    public ComandoTXT = "";
    public Value = "";
    public TargetDevice;
    public Devices = [];
    private appRef;
    public IntervalHandle;
    public CurrentDevice = {};

    /**
     * CONSTANTES que serán usadas a lo largo del controlador
     */
    public Constants = {
        CMD_SCAN: "ESCANEAR",
        CMD_STOP_SCAN: "Escaneando... (Tap para parar)",
        CMD_MANAGE_DEVICE: "ADM DISPOSITIVO"
    };

    public OpModes = {
      EDIT_DEVICE: "EDIT_DEVICE",
      LIST_DEVICES: "LIST_DEVICES"
    }

    public OpMode = this.OpModes.LIST_DEVICES;

    /**
     * Constructor para el controlador. Todo comienza acá...
     */
    constructor(public navCtrl: NavController, private ble: BLE /*Librería BLE nativa*/, private applicationRef: ApplicationRef /*Librería ApplicationRef de Angular*/) {
        this.appRef = applicationRef; //Referencia a la aplicación ANGULAR
        this.SetMessage("Constructor: Iniciado!");
        this.BLE = ble; //Acceso a la librería BLE Nativa
        this.ComandoTXT = this.Constants.CMD_SCAN;
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

        if (this['ComandoTXT'] === this.Constants.CMD_SCAN) {
            this.SetMessage("SCAN no definido todavía...");
            this['ComandoTXT'] = this.Constants.CMD_STOP_SCAN;
            this.scan();
            this.ejecutandoComando = false;

        } else if (this['ComandoTXT'] === this.Constants.CMD_STOP_SCAN) {
            this.SetMessage("Ejecutando Comando: STOP SCAN");
            clearInterval(this.IntervalHandle);
            this['ComandoTXT'] = this.Constants.CMD_SCAN;
            this.ejecutandoComando = false;

        }
    }

    public EditDevice(device){
      this.OpMode = this.OpModes.EDIT_DEVICE;
      this.CurrentDevice = device;
    }
    private scan(){
      this.SetMessage("scan() Begin!");
      this.ComandoTXT = this.Constants.CMD_STOP_SCAN;

      let ble = this.BLE;

      this.SetMessage("scan() ble.enable()");
      ble.enable();

      this.SetMessage("scan() Setting Interval...");
      this.IntervalHandle = setInterval(() => { // SET AN INTERVAL THAT RUNS EVERY 3 Seconds
        this.SetMessage("scan() ble.scan()");
        ble.scan([], 2 /*seconds (0) */).subscribe(data=>{ //DO SCAN DURING 1 SECOND
          this.SetMessage("scan() ble.scan callback: " + data['id'] + ' | ' + data['name'] + ' | ' + data['rssi']);
          this.addDevice(data);
        });
      },2100);//END OF INTERVAL DEFINITION
    }

    private addDevice(deviceToAdd){
      for (let device of this.Devices) {
        if(device.id == deviceToAdd.id && device.name == deviceToAdd.name) return;
      }
      this.Devices.unshift(deviceToAdd);
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
