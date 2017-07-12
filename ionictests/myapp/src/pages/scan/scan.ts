import { Component } from '@angular/core';
import {  NavController,NavParams,LoadingController } from 'ionic-angular';
import { BLE } from '@ionic-native/ble';
import { NgZone } from '@angular/core';


class Datos{
    public valores = [];
    public valoresViejos = [];
    constructor(datos){
      for (var llave in datos) {
        this.valores[llave] = datos[llave];
        this.valoresViejos[llave] = datos[llave];
      }
    }
    public cambio(){
      var a = false;;
      for(var llave in this.valores){
        if(this.valores[llave] != this.valoresViejos[llave]){
          a = true;
        }
        this.valoresViejos[llave] = this.valores[llave];
      }
      return a;
    }
};

@Component({
  selector: 'pagina-opciones',
  templateUrl: 'opciones.html'
})
export class PaginaOpciones {
  public UUID_UART_SERVICE = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
  public UUID_RX = "6e400003-b5a3-f393-e0a9-e50e24dcca9e";
  public UUID_TX = "6e400002-b5a3-f393-e0a9-e50e24dcca9e";
  private zone:any;
  private dispositivo:any;
  private conectando:boolean;
  private actualizador:any;

  private opcion: string = "1";
  private color: Datos;
  private olas: Datos;
  private arcoiris: Datos;
  private destellos: Datos;
  private respirar: Datos;

  constructor(private loadingCtrl: LoadingController, private navCtrl: NavController, private navParams: NavParams, private ble: BLE) {
    this.zone = new NgZone({ enableLongStackTrace: false });
    this.dispositivo = this.navParams.get('dispositivo');
    this.conectando = false;

    this.color = new Datos( {h:0,s:100,v:100});
    this.olas = new Datos({v:50,b:100});
    this.arcoiris = new Datos({v:50,b:100});
    this.destellos = new Datos({h:0,s:100,v:50,b:100});
    this.respirar = new Datos({h:0,s:100,v:50,b:100});

    this.start();

  }

  private start(){
    let loader = this.loadingCtrl.create({
      content: "Conectando con "+this.dispositivo.name,
    });
    loader.present();
    this.ble.connect(this.dispositivo.id).subscribe(peripheralData => {
      console.log("conectado");
      loader.dismiss();
      this.conectando = false;
      this.actualizador = setInterval(()=>{
        this.zone.run(()=>{
          this.actualizar();
        });
      },500);
    },peripheralData => {
      this.conectando = true;
      loader.dismiss();
      this.navCtrl.pop();
      console.log("Desconectado");
    });
  }

  private salir(){
    this.ble.disconnect(this.dispositivo.id);
    this.navCtrl.pop();
  }
  private apagar(){
    var datos:Uint8Array = new Uint8Array(6);
    datos[0] = "!".charCodeAt(0);
    datos[1] = "H".charCodeAt(0);
    datos[2] = 0;
    datos[3] = 0;
    datos[4] = 0;
    datos[5] = 0;
    this.enviar(datos);
  }
  private actualizar(){
      switch(this.opcion){
        case "1":
          if(this.color.cambio()){
            var datos:Uint8Array = new Uint8Array(6);
            datos[0] = "!".charCodeAt(0);
            datos[1] = "H".charCodeAt(0);
            datos[2] = this.color.valores['h'] >> 8;
            datos[3] = this.color.valores['h'];
            datos[4] = this.color.valores['s'];
            datos[5] = this.color.valores['v'];
            this.enviar(datos);
          }
          break;
        case "2":
          if(this.olas.cambio()){
            var datos:Uint8Array = new Uint8Array(4);
            datos[0] = "!".charCodeAt(0);
            datos[1] = "O".charCodeAt(0);
            datos[2] = this.olas.valores['v'];
            datos[3] = this.olas.valores['b'];
            this.enviar(datos);
          }
          break;
        case "3":
          if(this.arcoiris.cambio()){
            var datos:Uint8Array = new Uint8Array(4);
            datos[0] = "!".charCodeAt(0);
            datos[1] = "A".charCodeAt(0);
            datos[2] = this.arcoiris.valores['v'];
            datos[3] = this.arcoiris.valores['b'];
            this.enviar(datos);
          }
          break;
        case "4":
          if(this.destellos.cambio()){
            var datos:Uint8Array = new Uint8Array(7);
            datos[0] = "!".charCodeAt(0);
            datos[1] = "D".charCodeAt(0);
            datos[2] = this.destellos.valores['h']>>8;
            datos[3] = this.destellos.valores['h'];
            datos[4] = this.destellos.valores['s'];
            datos[5] = this.destellos.valores['v'];
            datos[6] = this.destellos.valores['b'];
            this.enviar(datos);
          }
          break;
          case "5":
            if(this.respirar.cambio()){
              var datos:Uint8Array = new Uint8Array(7);
              datos[0] = "!".charCodeAt(0);
              datos[1] = "R".charCodeAt(0);
              datos[2] = this.respirar.valores['h']>>8;
              datos[3] = this.respirar.valores['h'];
              datos[4] = this.respirar.valores['s'];
              datos[5] = this.respirar.valores['v'];
              datos[6] = this.respirar.valores['b'];
              this.enviar(datos);
            }
            break;
      }
  }

  private enviar(datos:Uint8Array){
    this.ble.write(this.dispositivo.id,this.UUID_UART_SERVICE,this.UUID_TX,datos.buffer).then(()=>{
      console.log("DONE");
    },()=>{
      console.log("UPS");
    });
  }

}