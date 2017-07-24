import { Component } from '@angular/core';

import { LEDPage } from '../led/led';
import { ScanPage } from '../scan/scan';
import { ContactPage } from '../contact/contact';
import { HomePage } from '../home/home';

@Component({
  templateUrl: 'tabs.html'
})
export class TabsPage {

  tab1Root = HomePage;
  tab2Root = ScanPage;
  tab3Root = ContactPage;
  tab4Root = LEDPage;
  constructor() {

  }
}
