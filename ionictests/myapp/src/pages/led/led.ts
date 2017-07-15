import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import { BLE } from '@ionic-native/ble';
import { BluetoothSerial } from '@ionic-native/bluetooth-serial';

@Component({
  selector: 'page-led',
  templateUrl: 'led.html'
})
export class LEDPage {

  constructor(public navCtrl: NavController, /*private ble: BLE , */ private ble: BluetoothSerial ) {
    this['myCount'] = 0;
    this['valor'] = 0;
    this['messages'] = [];
    this['service_id'] = "19B10010-E8F2-537E-4F6C-D104768A1214";
    this.setMessage("Constructor: Begin!");
    this['ble'] = ble;
    this['ComandoTXT'] = "FIND ANGU";
    this['targetDevice'] = {};
    ble.enable();
  }
  
  public Command(){
    if(this['ComandoTXT']==="FIND ANGU") {
      this.findANGU();
    }
    
    if(this['ComandoTXT']=== "CAMBIAR LED"){
      this.setMessage("ComandoTXT = CAMBIAR LED: Calling txData()");
      this.txData();
      
    }
  }
  
  public findANGU(){
      this.setMessage("SCAN: Begin!");
      this['ComandoTXT'] = "Scanning...";

      let  ble = this['ble'];
      
      this.setMessage("SCAN: BLE ENABLE!");
      ble.enable();
      
      this.setMessage("SCAN: Setting Interval...");
      this['intervalHandle'] = setInterval(() => { // SET AN INTERVAL THAT RUNS EVERY 3 Seconds
        this.setMessage("INTERVAL: BLE SCAN...");
        //https://ionicframework.com/docs/native/ble/ 
        //ble.scan([], 2 /*seconds (0) */).subscribe( data => { //DO SCAN DURING 1 SECOND
        //  this.setMessage("SCAN SUBSCRIBER: " + data['id'] + ' | ' + data['name'] + ' | ' + data['rssi']);
        //  if(data['name']=="ANGU"){
        //    this.setMessage("SCAN SUBSCRIBER: ANGU FOUND! STOPPED SCANNING!");
        //    clearInterval(this["intervalHandle"]);
       //     this["targetDevice"] = data;
        //    this["ComandoTXT"] = "CAMBIAR LED";
       //   }
       // });
      },2100);//END OF INTERVAL DEFINITION
    }  
    
  public txData(){
    this.setMessage("txData: BEGIN! Doing ble connect...");
    
    let id = this['targetDevice'].id;
    this.setMessage(id);
    
    this.ble.connect(id).subscribe(datos=>{
      this.setMessage("BLE CONNECT SUBSCRIBE: BEGIN. Doing ble write...");
      this['valor'] = "hello world";
      //this.ble.write(this['targetDevice'].id, this['targetDevice'].service_id,this['targetDevice'].service_id, this['valor'].buffer ).then(()=>{
      //  this.setMessage("BLE WRITE THEN!");
      //  this.ble.disconnect(id);
      //},(error)=>{
      //  this.setMessage("BLE Write ERROR!");
      //  this.setMessage(error);
      //});
    },error=>{
      this.setMessage("BLE Connect ERROR!");
      this.setMessage(error.message);
    });
  }
  
/*
  
  public conectar(id){
    this.ble.connect(id).subscribe(datos=>{
      this.escribir(id);
    },error=>{
      this.encontrar();  
    });
  }

  public escribir(id){

  }
  
*/
  public setMessage(message){
    this['myCount'] ++;
    message = this['myCount'] + ':' + message;
    this['messages'].unshift(message);
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
