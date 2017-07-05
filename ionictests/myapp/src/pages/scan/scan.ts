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
    
    this['myrssi'].unshift(this['myCount'] + "Ready! Calling ble.enable()")
    this['myCount']++;
    ble.enable();
    
    this['myrssi'].unshift(this['myCount'] + "ble.enable() succeeded!");
    this['myCount']++;
    setInterval(() => {
      this['myrssi'].unshift(this['myCount'] + "Interval Called! ble.scan()...");
      this['myCount']++;
      ble.scan([],500).subscribe(data=>{
        this['myrssi'].unshift(this['myCount'] + "Scan Subscriber: " + data['name'] + ' | ' + data['rssi']);
        this['myCount']++;
        if(data['name']=="Blynk"){
          this['myrssi'].unshift(this['myCount'] + "Blynk Found!!!");
          this['myCount']++;
        }
      });
    },1000);
    
    
    
  }

}
