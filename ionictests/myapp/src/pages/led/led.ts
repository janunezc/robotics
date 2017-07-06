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
            
            this.navCtrl.setRoot(this.navCtrl.getActive().component);
          }
        });
      },2100);//END OF INTERVAL DEFINITION
    }  
  
  /*
  public findBlynk(){
    this.setMessage("findBlynk: BEGIN...");
    this['ComandoTXT'] = "Buscando...";
    this.setMessage("findBlynk: BLE ENABLE...");
    this['ble'].enable();
    this.setMessage("findBlynk: encontrar()...");

    this.ble.scan([],500).subscribe(deviceData=>{
      this.setMessage("SUBSCRIBER: Found Device...");
      let deviceMessage = deviceData.id + ":" + deviceData.name + "(" + deviceData.rssi + ")";
      this.setMessage(deviceMessage);
      
      if(deviceData.name == "Blynk"){
        this.setMessage("BLYNK FOUND!");
        this["ComandoTXT"] = "CAMBIAR LED";
        this['dispositivo'] = deviceData;  
        
        this.ble.stopScan().then(() => {
          this.setMessage('SCANNING STOPPED!');
        });
        
      } else {
        //setTimeout(()=>{this.findBlynk();},500);
      }
    
    }, error=>{
      this.setMessage("BLE SCAN ERROR");
      this.setMessage(error.message);
      setTimeout(()=>{this.findBlynk();},1000);
    });
  }
  
  public txData(){
    this.setMessage("txData: BEGIN! Doing ble connect...");
    let id = this['deviceData'].id;
    this.ble.connect(id).subscribe(datos=>{
      this.setMessage("BLE CONNECT SUBSCRIBE: BEGIN. Doing ble write...");
      this.ble.write(id,this['service_id'],this['service_id'],this['valor'].buffer).then(()=>{
        this.setMessage("BLE WRITE THEN!");
        this['valor'] = (this['valor'] + 1) % 2;
        this.ble.disconnect(id);
      },(error)=>{
        this.setMessage("BLE Write ERROR!");
      });
    },error=>{
      this.setMessage("BLE Connect ERROR!");
    });
  }
  */
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

}
