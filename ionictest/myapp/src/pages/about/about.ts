import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import { BLE } from '@ionic-native/ble';

@Component({
  selector: 'page-about',
  templateUrl: 'about.html'
})
export class AboutPage {

  constructor(public navCtrl: NavController, private ble: BLE) {
    this['myrssi'] = 'Trying ble.enable()';  
    
    ble.enable();
    
    this['myrssi'] += '<p>ble.enable() succeeded!</p>';
    
    setInterval(() => {
      this['myrssi'] += '<p> Interval! </p>';
      ble.scan([],500).subscribe(data=>{
        this['myrssi'] += "<p>" + data['name'] + ' | ' + data['rssi'] + '</p>';
        if(data['name']=="Blynk"){
          this['myrssi'] += "<p> Blynk FOUND!!!! </p>";
        }
      });
    },1000);
    
    
    
  }

}
