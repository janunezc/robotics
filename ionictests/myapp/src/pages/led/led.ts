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
    this['messages'] = [];
    this['ComandoTXT'] = "Buscando...";
    this.setMessage("Beginning constructor... ble.enable()...");
    ble.enable();
    
    this.setMessage("Constructor... this.encontrar()...");
    this.encontrar();  
  }
  
  public encontrar(){
    this.setMessage("encontrar: this.ble.scan...");
    //this['dispositivosEncontrados'] = [];
    //this.ble.scan(["19B10010-E8F2-537E-4F6C-D104768A1214"],950).subscribe(deviceData=>{
    this.ble.scan([],1000).subscribe(deviceData=>{
      this.setMessage("Found something!");
      let deviceMessage = deviceData.id + ":" + deviceData.name + "(" + deviceData.rssi + ")";
      this.setMessage(deviceMessage);
      
      if(deviceData.name == "Blynk"){
        this.setMessage("BLINK FOUND!");
        this["CommandTXT"] = "Cambiar LED";
        
        this['dispositivo'] = deviceData;  
      } else {
        setTimeout(()=>{this.encontrar();},500);
      }
      
    }, error=>{
      setTimeout(()=>{this.encontrar();},1000);
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
    this.ble.write(id,"19B10010-E8F2-537E-4F6C-D104768A1214","19B10011-E8F2-537E-4F6C-D104768A1214",this.valor[0].buffer).then(()=>{
      this.valor[0] = (this.valor[0]+1)%2;
      this.ble.disconnect(id);
    },(error)=>{
      this.encontrar();  
    });
  }
  
*/
  public setMessage(message){
    this['myCount'] ++;
    message = this['myCount'] + ':' + message;
    this['messages'].unshift(message);
  }

}
