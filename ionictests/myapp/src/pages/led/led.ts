import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import { BLE } from '@ionic-native/ble';

@Component({
  selector: 'page-led',
  templateUrl: 'led.html'
})
export class LEDPage {

  public valor;
  constructor(public navCtrl: NavController, private ble: BLE) {
    this.valor = new Uint8Array(2);
    this.valor[0] = 1;
    this.valor[1] = 0;
    this['dispositivosEncontrados'] = [];  
    ble.enable();+
    this.encontrar();
  }
  
  public encontrar(){
    this['dispositivosEncontrados'] = [];
    this.ble.scan(["19B10010-E8F2-537E-4F6C-D104768A1214"],950).subscribe(deviceData=>{
        this['dispositivosEncontrados'].push(deviceData);
    });
  }
  
  public conectar(id){
    this.ble.connect(id).subscribe(datos=>{
      this.escribir(id);
    },error=>{
      this.encontrar();  
    });
  }

  public escribir(id){
    this.ble.write(id,"19B10010-E8F2-537E-4F6C-D104768A1214","19B10011-E8F2-537E-4F6C-D104768A1214",this.valor.buffer).then(()=>{
      this.valor[0] = (this.valor[0]+1)%2;
      this.ble.disconnect(id);
      this.encontrar();  
    },(error)=>{
      this.encontrar();  
    });
  }

}
