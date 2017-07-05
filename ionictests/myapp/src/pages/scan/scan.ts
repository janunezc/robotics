import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import { BLE } from '@ionic-native/ble';

@Component({
  selector: 'page-scan',
  templateUrl: 'scan.html'
})
export class ScanPage {

  constructor(public navCtrl: NavController, private ble: BLE) {
    this['myrssi'] = [];  
    this['myCount'] = 0;
    
    this['myrssi'].push( "Ready! Calling ble.enable()")
    this['myCount']++;
    ble.enable();
    
    this['myrssi'].push("ble.enable() succeeded!");
    this['myCount']++;
    setInterval(() => {
      this['myrssi'].push("Interval Called! ble.scan()...");
      this['myCount']++;
      ble.scan([],500).subscribe(data=>{
        this['myrssi'].push("Scan Subscriber: " + data['name'] + ' | ' + data['rssi']);
        this['myCount']++;
        if(data['name']=="Blynk"){
          this['myrssi'].push("Blynk Found!!!");
          this['myCount']++;
        }
      });
    },1000);
    
    
    
  }

}
