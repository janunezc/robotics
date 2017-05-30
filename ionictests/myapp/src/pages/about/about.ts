import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import { BLE } from '@ionic-native/ble';

@Component({
  selector: 'page-about',
  templateUrl: 'about.html'
})
export class AboutPage {

  constructor(public navCtrl: NavController, private ble: BLE) {
    this['myrssi'] = [];  
    this['myCount'] = 0;
    
    this['myrssi'].push(myCount + ": Ready! Calling ble.enable()")
    this['myCount']++;
    ble.enable();
    
    this['myrssi'].push(myCount + ": ble.enable() succeeded!");
    this['myCount']++;
    setInterval(() => {
      this['myrssi'].push(myCount + ": Interval Called! ble.scan()...");
      this['myCount']++;
      ble.scan([],500).subscribe(data=>{
        this['myrssi'].push(myCount + ": Scan Subscriber: " + data['name'] + ' | ' + data['rssi']);
        this['myCount']++;
        if(data['name']=="Blynk"){
          this['myrssi'].push(myCount + ": Blynk Found!!!");
          this['myCount']++;
        }
      });
    },1000);
    
    
    
  }

}
