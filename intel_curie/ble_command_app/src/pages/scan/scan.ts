import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import { BLE } from '@ionic-native/ble';
import { ApplicationRef } from '@angular/core';

@Component({
  selector: 'page-scan',
  templateUrl: 'scan.html'
})
export class ScanPage {

  public messages = [];

  public BLE;
  public ComandoTXT = "";
  public IntervalHandle;
  private AppRef;

  public Constants = {
      DEVICE_NAME: "COMANDO LED",
      CMD_FIND_DEVICE:"BUSCAR DISPOSITIVOS",
      CMD_STOP_SCAN:"Buscando... Clic para PARAR"
    };

  constructor(public navCtrl: NavController, private ble: BLE, private applicationRef : ApplicationRef /*Librería ApplicationRef de Angular*/) {
    this.AppRef = applicationRef;
    this.SetMessage("Constructor: INICIO!");
    this.BLE = ble;
    this.ComandoTXT = this.Constants.CMD_FIND_DEVICE;
    this.SetMessage("Constructor: FINAL!");
  }

  public Command(){
    this.SetMessage("Command() INICIO! " + this.ComandoTXT);
    if (this.ComandoTXT==this.Constants.CMD_FIND_DEVICE){
      this.SetMessage("Comando: FIND DEVICE");
      this.scan();
    } else if (this.ComandoTXT===this.Constants.CMD_STOP_SCAN){
      this.SetMessage("Comando: STOP SCAN...");
      clearInterval(this.IntervalHandle);
      this.ComandoTXT = this.Constants.CMD_FIND_DEVICE;
    }
  }

  public scan(){
      this.SetMessage("SCAN: Begin!");
      this.ComandoTXT = this.Constants.CMD_STOP_SCAN;

      let ble = this.BLE;

      this.SetMessage("SCAN: BLE ENABLE!");
      ble.enable();

      this.SetMessage("SCAN: Setting Interval...");
      this.IntervalHandle = setInterval(() => { // SET AN INTERVAL THAT RUNS EVERY 3 Seconds
        this.SetMessage("INTERVAL: BLE SCAN...");
        ble.scan([], 2 /*seconds (0) */).subscribe(data=>{ //DO SCAN DURING 1 SECOND
          this.SetMessage("SCAN SUBSCRIBER: " + data['id'] + ' | ' + data['name'] + ' | ' + data['rssi']);
          if(data['name']==this.Constants.DEVICE_NAME){
            this.SetMessage("SCAN SUBSCRIBER: DEVICE FOUND!");
          }
        });
      },2100);//END OF INTERVAL DEFINITION
    }

  public SetMessage(message){
    var count = this.messages.length;
    message = count + ': ' + message;
    this.messages.unshift(message);
    this.AppRef.tick();
  }
}
