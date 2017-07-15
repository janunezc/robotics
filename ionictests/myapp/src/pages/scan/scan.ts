import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import { BLE } from '@ionic-native/ble';

@Component({
  selector: 'page-scan',
  templateUrl: 'scan.html'
})
export class ScanPage {

  constructor(public navCtrl: NavController, private ble: BLE) {
    this['myCount'] = 0;
    this['messages'] = [];
    this.setMessage("Constructor: Begin!");
    this['ble'] = ble;
    this['ComandoTXT'] = "START SCAN";
    this['intervalHandle'];
  }
  
  public Command(){
    if (this['ComandoTXT']==="START SCAN"){
      this.setMessage("Command: START SCAN");
      this.scan();
    } else if (this['ComandoTXT']==="Scanning... CLICK TO STOP"){
      this.setMessage("Command: STOPING SCAN...");
      clearInterval(this['intervalHandle']);
      this['ComandoTXT'] = "START SCAN";
    }
      
  }
  
  public scan(){
      this.setMessage("SCAN: Begin!");
      this['ComandoTXT'] = "Scanning... CLICK TO STOP";

      let  ble = this['ble'];
      
      this.setMessage("SCAN: BLE ENABLE!");
      ble.enable();
      
      this.setMessage("SCAN: Setting Interval...");
      this['intervalHandle'] = setInterval(() => { // SET AN INTERVAL THAT RUNS EVERY 3 Seconds
        this.setMessage("INTERVAL: BLE SCAN...");
        //https://ionicframework.com/docs/native/ble/ 
        ble.scan([], 2 /*seconds (0) */).subscribe(data=>{ //DO SCAN DURING 1 SECOND
          this.setMessage("SCAN SUBSCRIBER: " + data['id'] + ' | ' + data['name'] + ' | ' + data['rssi']);
          if(data['name']=="Blynk"){
            this.setMessage("SCAN SUBSCRIBER: BLYNK FOUND!");
          }
        });
      },2100);//END OF INTERVAL DEFINITION
    }  

  public setMessage(message){
    this['myCount'] ++;
    message = this['myCount'] + ': ' + message;
    this['messages'].unshift(message);
  }
}
