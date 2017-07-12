import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import { BLE } from '@ionic-native/ble';

@Component({
  selector: 'page-led',
  templateUrl: 'led.html'
})
export class LEDPage {

  constructor(public navCtrl: NavController, private ble: BLE) {
    this['myCount'] = 0;
    this['valor'] = 0;
    this['messages'] = [];
    this['service_id'] = "19B10010-E8F2-537E-4F6C-D104768A1214";
    this.setMessage("Constructor: Begin!");
    this['ble'] = ble;
    this['ComandoTXT'] = "FIND BLYNK";
    this['targetDevice'] = {};
  }
  
  public Command(){
    if(this['ComandoTXT']==="FIND BLYNK") {
      this.findBlynk();
    }
    
    if(this['ComandoTXT']=== "CAMBIAR LED"){
      //this.txData();
      this.setMessage("CAMBIAR LED txData DISABLEd");
    }
  }
  
  public findBlynk(){
      this.setMessage("SCAN: Begin!");
      this['ComandoTXT'] = "Scanning...";

      let  ble = this['ble'];
      
      this.setMessage("SCAN: BLE ENABLE!");
      ble.enable();
      
      this.setMessage("SCAN: Setting Interval...");
      this['intervalHandle'] = setInterval(() => { // SET AN INTERVAL THAT RUNS EVERY 3 Seconds
        this.setMessage("INTERVAL: BLE SCAN...");
        //https://ionicframework.com/docs/native/ble/ 
        ble.scan([], 2 /*seconds (0) */).subscribe( data => { //DO SCAN DURING 1 SECOND
          this.setMessage("SCAN SUBSCRIBER: " + data['id'] + ' | ' + data['name'] + ' | ' + data['rssi']);
          if(data['name']=="Blynk"){
            this.setMessage("SCAN SUBSCRIBER: BLYNK FOUND! STOPPED SCANNING!");
            clearInterval(this["intervalHandle"]);
            this["targetDevice"] = data;
            this["ComandoTXT"] = "CAMBIAR LED";

            let idx = 0;
            while(idx<10000){
              this["test"] = idx;
              idx ++;
            }
          }
        });
      },2100);//END OF INTERVAL DEFINITION
    }  
  
  public setMessage(message){
    this['myCount'] ++;
    message = this['myCount'] + ':' + message;
    this['messages'].unshift(message);
  }

}
