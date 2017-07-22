import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import { BLE } from '@ionic-native/ble';
import { ApplicationRef } from '@angular/core'

@Component({
  selector: 'page-led',
  templateUrl: 'led.html'
})


export class LEDPage {

  /**
   * Constants that we will use around the controller
   */
  public constants  = {
      CMD_FIND_ANGU:"FIND ANGU",
      CMD_STOP_SCAN:"Scanning... (Click to Stop)",
      CMD_TOGGLE_LED:"CAMBIAR LED",
      ON:"ON",
      OFF:"OFF"
    };

  /**
   * Constructor for the controller. Everything begins here.
   */
  constructor(public navCtrl: NavController, private ble: BLE, private applicationRef : ApplicationRef ) {
    this['messages'] = [];
    this['value'] = "ON FIRST TIME";
    this['service_id'] = "db938b80-f010-44b6-8aa9-1835adf9419a";
    this['characteristic_id'] = "9906064e-9bbe-4eba-b415-bbd223f7d3d9";
    this['ble'] = ble;
    this['ComandoTXT'] = this.constants.CMD_FIND_ANGU;
    this['targetDevice'] = {};
    this.setMessage("Constructor: Begin!");
    this["applicationRef"] = applicationRef;
  }

  /**
   * This is triggered when the button is clicked.
   */
  public Command(){
    this.setMessage("COMMAND RECEIVED!");
    this.setMessage(this["ComandoTXT"]);
    if(this['ComandoTXT']===this.constants.CMD_FIND_ANGU) {
      this['ComandoTXT'] = this.constants.CMD_STOP_SCAN;
      this.findANGU();
    } else if (this['ComandoTXT']===this.constants.CMD_STOP_SCAN) {
      this.setMessage("Command: STOPING SCAN...");
      clearInterval(this['intervalHandle']);
      this['ComandoTXT'] = this.constants.CMD_FIND_ANGU;
    } else if(this['ComandoTXT']=== this.constants.CMD_TOGGLE_LED){
      this.setMessage("ComandoTXT = CAMBIAR LED: Calling txData()");
      this.txData();
    }
  }

  /**
   * This searches for ANGU in the AIR using SCAN technique.
   */
  public findANGU(){
      this.setMessage("SCAN: Begin!");
      let  ble = this['ble'];

      this.setMessage("SCAN: BLE ENABLE!");
      ble.enable();

      this.setMessage("SCAN: Setting Interval...");
      this['intervalHandle'] = setInterval(() => { // SET AN INTERVAL THAT RUNS EVERY 3 Seconds
        this.setMessage("INTERVAL: BLE SCAN...");
        //https://ionicframework.com/docs/native/ble/
        ble.scan([], 2 /*seconds (0) */).subscribe( data => { //DO SCAN DURING 1 SECOND
          this.setMessage("SCAN SUBSCRIBER: " + data['id'] + ' | ' + data['name'] + ' | ' + data['rssi']);
          if(data['name']=="COMANDO LED"){
            this.setMessage("SCAN SUBSCRIBER: ANGU FOUND! STOPPED SCANNING!");
            clearInterval(this["intervalHandle"]);
            this["targetDevice"] = data;
            this["ComandoTXT"] = this.constants.CMD_TOGGLE_LED;
          }
          this.applicationRef.tick();
        });
      },2100);//END OF INTERVAL DEFINITION
    }

  /**
   * This transmits data to the selected ANGU device
   */
  public txData(){
    this.setMessage("txData: BEGIN! Doing ble connect...");

    let id = this['targetDevice'].id;
    this.setMessage(id);

    this.ble.connect(id).subscribe(datos=>{
      this.setMessage("BLE CONNECT SUBSCRIBE: BEGIN. Doing ble write..." + this["value"]);

      this.ble.write(this['targetDevice'].id, this['service_id'],this['characteristic_id'], this.stringToBytes(this["value"]) ).then(()=>{
        this.setMessage("BLE WRITE THEN:" + this["value"]);
        if(this["value"]==this.constants.ON){
          this["value"] = this.constants.OFF;
        } else {
          this["value"] = this.constants.ON;
        }
        this.ble.disconnect(id);
        this.applicationRef.tick();
      },(error)=>{
        this.setMessage("BLE Write ERROR!");
        this.setMessage(error);
        this.ble.disconnect(id);
      });
    },error=>{
      this.setMessage("BLE Connect ERROR!");
      this.setMessage(error.message);
    });
  }

  /**
   * Adds message to the history on screen
   */
  public setMessage(message){
    var count = this['messages'].length;
    message = count + ':' + message;
    this['messages'].unshift(message);
    this.applicationRef.tick();
  }

  // ASCII only
  public stringToBytes(string) {
     let array = new Uint8Array(string.length);
     for (let i = 0, l = string.length; i < l; i++) {
         array[i] = string.charCodeAt(i);
      }
      return array.buffer;
  }

  // ASCII only
  public bytesToString(buffer) {
      return String.fromCharCode.apply(null, new Uint8Array(buffer));
  }
}
