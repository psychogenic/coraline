webpackJsonp([0],{103:function(n,t,e){"use strict";e.d(t,"a",function(){return p});var o=e(0),i=e(31),a=e(41),r=e(101),c=e(102),s=e(199),l=this&&this.__decorate||function(n,t,e,o){var i,a=arguments.length,r=a<3?t:null===o?o=Object.getOwnPropertyDescriptor(t,e):o;if("object"==typeof Reflect&&"function"==typeof Reflect.decorate)r=Reflect.decorate(n,t,e,o);else for(var c=n.length-1;c>=0;c--)(i=n[c])&&(r=(a<3?i(r):a>3?i(t,e,r):i(t,e))||r);return a>3&&r&&Object.defineProperty(t,e,r),r},d=this&&this.__metadata||function(n,t){if("object"==typeof Reflect&&"function"==typeof Reflect.metadata)return Reflect.metadata(n,t)},p=function(){function n(n,t,e,o,i,a,r){this.navCtrl=n,this.navParams=t,this.dialogs=e,this.browser=o,this.device=i,this.ble=a,this.appref=r,this.iapLocation="",this.BLEDevicesCount=0,this.locEraseTimeout=0,this.notifOutput="<-- click one...",this.pluginsAvailable={Device:!0,Notification:!0,InAppBrowser:!0,BLE:!1,SQLitePlugin:!1},this.isScanning=!1,this.BLEDevices=new Array}return n.prototype.ionViewDidEnter=function(){var n=this;setTimeout(function(){["Device","Notification","InAppBrowser","BLE","SQLitePlugin"].forEach(function(t){nativebridge.pluginAvailable(t,function(){console.log(t+" plugin IS available..."),n.pluginsAvailable[t]=!0},function(){console.log(t+" plugin not available...")})})},100)},n.prototype.loadZip=function(){var n="https://coraline.psychogenic.com/app/demo.zip";this.iapLocation=n,this.browser.create(n),this.scheduleLocationErase()},n.prototype.loadImg=function(){var n="https://psychogenic.net/wp-content/uploads/2015/11/cropped-psychologosq.png";this.iapLocation=n;var t=this.browser.create(n,"img",{height:200,width:200,scrollbars:"no"});setTimeout(function(){t.hide(),setTimeout(function(){t.show(),setTimeout(function(){t.close()},1e3)},1500)},1500),this.scheduleLocationErase()},n.prototype.scheduleLocationErase=function(){var n=this;this.locEraseTimeout&&clearTimeout(this.locEraseTimeout),this.locEraseTimeout=setTimeout(function(){n.iapLocation=""},3500)},n.prototype.loadSite=function(){var n="https://flyingcarsandstuff.com/";this.iapLocation=n,this.browser.create(n,"site",{height:800,width:600,scrollbar:"yes"}),this.scheduleLocationErase()},n.prototype.loadExternal=function(n){var t={ionic:"https://ionicframework.com/",cordova:"https://cordova.apache.org/",coraline:"https://coraline.psychogenic.com/",psychogenic:"https://psychogenic.com",flyingcars:"https://flyingcarsandstuff.com/"};this.browser.create(n in t?t[n]:n,"external")},n.prototype.showAlert=function(){var n=this;this.dialogs.alert("Oh my, not sure what just happened\nPlease let us know...").then(function(){n.notifOutput="Un homme averti en vaut deux"})},n.prototype.showConfirm=function(){var n=this,t=["Yep","Nope","Maybe"];this.dialogs.confirm("Are you totally, absolutely\ncertain about your choice?","Confirm it",t).then(function(e){n.notifOutput=e?"You said: "+t[e-1]:"fuggedabout it..."})},n.prototype.showPrompt=function(){var n=this,t=["Super","Duper","Ok","Meh"];this.dialogs.prompt("Please give me your opinion on the matter...","Prompting you",t).then(function(e){n.notifOutput=e.buttonIndex?t[e.buttonIndex-1]+": "+e.input1:"Fine, be that way."})},n.prototype.stopScan=function(){var n=this;this.isScanning&&this.ble.stopScan().then(function(){n.isScanning=!1}).catch(function(){n.isScanning=!1})},n.prototype.startScan=function(){var n=this;this.isScanning||(this.BLEDevicesCount=0,this.BLEDevices.splice(0,this.BLEDevices.length),this.isScanning=!0,this.ble.startScan([]).subscribe(function(t){n.BLEDevicesCount+=1,console.log("Gots a device from scan..."),n.BLEDevices.push(t),n.appref.tick()}),setTimeout(function(){n.isScanning&&(n.ble.stopScan(),n.isScanning=!1)},4e3))},n=l([Object(o.m)({selector:"page-plugins",template:'\x3c!--\n  Generated template for the PluginsPage page.\n\n  See http://ionicframework.com/docs/components/#navigation for more info on\n  Ionic pages and navigation.\n--\x3e\n<ion-header>\n\n  <ion-navbar>\n\n    <button ion-button color="secondary" menuToggle>\n            <ion-icon secondary name="menu"></ion-icon>\n          </button>\n    <ion-title>Coraline Plugins</ion-title>\n  </ion-navbar>\n\n</ion-header>\n\n\n<ion-content padding>\n\n\n\n  <div class="cardsparent">\n    <div class="cardscontainer">\n\n      <ion-card>\n        <ion-card-content>\n          <div class="pluginimg">\n            <img src="assets/imgs/plugins.png" />\n          </div>\n          <h2>\n            Plugins</h2>\n\n            There are a few nice features\n            when you package your app for Coraline,\n            but the most interesting is <strong>access to plugins</strong>.\n\n          <div class="centimgcontainer">\n            <div class="pluginsbanner">\n              <img src="assets/imgs/pluginsbanner.png" />\n            </div>\n          </div>\n\n            A couple of these come pre-packaged with Coraline, while others can be installed independently.\n\n          <ion-list>\n            <ion-item>\n              Plugins give you access to the world beyond the browser.\n            </ion-item>\n            <ion-item>\n              Plugins respect the cordova/ionic API, so they can be used as drop-in replacements when deploying to Linux.\n            </ion-item>\n          </ion-list>\n\n        </ion-card-content>\n      </ion-card>\n\n\n\n\n      <ion-card>\n        <ion-card-content>\n\n          <div class="plugincardimg">\n            <img src="assets/imgs/plugble.png" />\n          </div>\n          <h2>Bluetooth Low Energy</h2> The <strong>BLE</strong> plugin provides access to bluetooth low energy devices. It behaves like\n          <a (click)="loadExternal(\'https://github.com/don/cordova-plugin-ble-central\')">\n                     cordova-plugin-ble-central</a>\n          <div *ngIf="! pluginsAvailable[\'BLE\']">\n            The BLE plugin is not yet installed...\n          </div>\n          <div *ngIf="pluginsAvailable[\'BLE\']">\n            <ion-grid>\n\n              <ion-row>\n                <ion-col>\n                  <button *ngIf="!isScanning" ion-button full secondary (click)="startScan()">\n                        Start BLE Scan</button>\n                  <button *ngIf="isScanning" ion-button full secondary (click)="stopScan()">\n                        Stop Scan</button>\n\n                  <span *ngIf="BLEDevicesCount">\n                          Found {{BLEDevicesCount}}</span>\n                </ion-col>\n                <ion-col>\n                  <div class="bleresults">\n\n                    <ion-list>\n                      <ion-item *ngFor="let dev of BLEDevices">\n                        <span *ngIf="dev.name">{{dev.name}}</span>\n                        <span *ngIf="! dev.name">No name set</span> ({{dev.id}})\n                      </ion-item>\n                    </ion-list>\n\n                  </div>\n                </ion-col>\n\n              </ion-row>\n\n            </ion-grid>\n          </div>\n        </ion-card-content>\n      </ion-card>\n\n\n\n\n\n\n\n\n\n\n      <ion-card>\n\n        <ion-card-content>\n\n          <div class="plugincardimg">\n            <img src="assets/imgs/plugnotif.png" />\n          </div>\n          <h2>Notifications</h2> The <strong>Notification</strong> plugin gives your app the ability to open various notification dialogs. It behaves like\n          <a (click)="loadExternal(\'https://cordova.apache.org/docs/en/latest/reference/cordova-plugin-dialogs/\')">\n               cordova-plugin-dialogs</a>\n          <div *ngIf="! pluginsAvailable[\'Notification\']">\n            The Notification plugin is not yet installed...\n          </div>\n          <div *ngIf="pluginsAvailable[\'Notification\']">\n            <ion-grid>\n\n              <ion-row>\n                <ion-col>\n\n                  <ion-list>\n                    <ion-item>\n                      <button ion-button full secondary (click)="showAlert()">Alert Me</button>\n                    </ion-item>\n                    <ion-item>\n                      <button ion-button full secondary (click)="showConfirm()">Sure \'bout that?</button>\n                    </ion-item>\n\n                    <ion-item>\n                      <button ion-button full secondary (click)="showPrompt()">Gimme Text</button>\n                    </ion-item>\n                  </ion-list>\n                </ion-col>\n                <ion-col>\n                  {{notifOutput}}\n                </ion-col>\n\n              </ion-row>\n\n            </ion-grid>\n          </div>\n        </ion-card-content>\n      </ion-card>\n\n\n\n\n      <ion-card>\n        <ion-card-content>\n\n          <div class="plugincardimg">\n            <img src="assets/imgs/plugbrowser.png" />\n          </div>\n          <h2>In-App Browser</h2> The <strong>InAppBrowser</strong> plugin gives your app the ability to load, display or download, remote content, either in native windows or the default system browser. It behaves (mostly like)\n          <a (click)="loadExternal(\'https://cordova.apache.org/docs/en/latest/reference/cordova-plugin-inappbrowser/index.html\')">\n                     cordova-plugin-inappbrowser</a>.\n          <div *ngIf="! pluginsAvailable[\'InAppBrowser\']">\n            The InAppBrowser plugin is not yet installed...\n          </div>\n          <div *ngIf="pluginsAvailable[\'InAppBrowser\']">\n            <ion-grid>\n              <ion-row>\n                <ion-col>\n                  <button ion-button secondary full (click)="loadZip()">Download Zip</button> Open file chooser and download\n                </ion-col>\n\n                <ion-col>\n                  <button ion-button secondary full (click)="loadImg()">Show Image</button> Show a remote image, and control the window\n                </ion-col>\n\n                <ion-col>\n                  <button ion-button secondary full (click)="loadSite()">Browser\n                    Window</button> Show a remote site in window\n                </ion-col>\n\n                <ion-col>\n                  <button ion-button secondary full (click)="loadExternal(\'coraline\')">Load External</button> Launch the system browser\n                </ion-col>\n              </ion-row>\n            </ion-grid>\n          </div>\n          <div *ngIf="iapLocation">\n            Location: {{iapLocation}}\n          </div>\n        </ion-card-content>\n      </ion-card>\n\n\n\n      <ion-card>\n        <ion-card-content>\n\n          <div class="plugincardimg">\n            <img src="assets/imgs/plugsqlite.png" />\n          </div>\n          <h2>SQLite</h2> The <strong>SQLite</strong> plugin gives your app access to SQLite backend for databases.\n          <div *ngIf="! pluginsAvailable[\'SQLitePlugin\']">\n            The SQLite plugin is not yet installed...\n          </div>\n          <div *ngIf="pluginsAvailable[\'SQLitePlugin\']">\n            SQLite plugin is available (demo to be done, though...)\n          </div>\n        </ion-card-content>\n      </ion-card>\n\n\n\n\n\n      <ion-card>\n        <ion-card-content>\n\n          <div class="plugincardimg">\n            <img src="assets/imgs/plugdevice.png" />\n          </div>\n          <h2>Device</h2> The <strong>Device</strong> plugin provides information about the device on which the app is running. It behaves like <a (click)="loadExternal(\'https://cordova.apache.org/docs/en/latest/reference/cordova-plugin-device/\')">\n          cordova-plugin-device</a>\n\n          <div *ngIf="! pluginsAvailable[\'Device\']">\n            The Device plugin is not yet installed...\n          </div>\n          <div *ngIf="pluginsAvailable[\'Device\']">\n            <ion-list>\n              <ion-item>\n                Platform: {{device.platform}}\n              </ion-item>\n\n              <ion-item>\n                Version: {{device.version}}\n              </ion-item>\n              <ion-item>Model/Manuf: {{device.model}} {{device.manufacturer}}\n              </ion-item>\n\n              <ion-item>\n                UUID: {{device.uuid}}\n              </ion-item>\n            </ion-list>\n          </div>\n        </ion-card-content>\n      </ion-card>\n\n    </div>\n  </div>\n\n</ion-content>\n'}),d("design:paramtypes",[i.e,i.f,r.a,a.a,c.a,s.a,o.f])],n)}()},114:function(n,t){function e(n){return Promise.resolve().then(function(){throw new Error("Cannot find module '"+n+"'.")})}e.keys=function(){return[]},e.resolve=e,n.exports=e,e.id=114},155:function(n,t){function e(n){return Promise.resolve().then(function(){throw new Error("Cannot find module '"+n+"'.")})}e.keys=function(){return[]},e.resolve=e,n.exports=e,e.id=155},198:function(n,t,e){"use strict";e.d(t,"a",function(){return p});var o=e(0),i=e(31),a=e(41),r=e(101),c=e(102),s=e(103),l=this&&this.__decorate||function(n,t,e,o){var i,a=arguments.length,r=a<3?t:null===o?o=Object.getOwnPropertyDescriptor(t,e):o;if("object"==typeof Reflect&&"function"==typeof Reflect.decorate)r=Reflect.decorate(n,t,e,o);else for(var c=n.length-1;c>=0;c--)(i=n[c])&&(r=(a<3?i(r):a>3?i(t,e,r):i(t,e))||r);return a>3&&r&&Object.defineProperty(t,e,r),r},d=this&&this.__metadata||function(n,t){if("object"==typeof Reflect&&"function"==typeof Reflect.metadata)return Reflect.metadata(n,t)},p=function(){function n(n,t,e,o){this.navCtrl=n,this.dialogs=t,this.browser=e,this.device=o}return n.prototype.goToPlugins=function(){this.navCtrl.setRoot(s.a)},n.prototype.loadSite=function(){this.loadExternal("flyingcars")},n.prototype.loadExternal=function(n){var t={ionic:"https://ionicframework.com/",cordova:"https://cordova.apache.org/",coraline:"https://coraline.psychogenic.com/",psychogenic:"https://psychogenic.com",flyingcars:"https://flyingcarsandstuff.com/"};n in t&&this.browser.create(t[n],"external")},n=l([Object(o.m)({selector:"page-home",template:'<ion-header>\n  <ion-navbar>\n    <button ion-button color="secondary" menuToggle>\n      <ion-icon name="menu"></ion-icon>\n    </button>\n    <ion-title>Coraline</ion-title>\n  </ion-navbar>\n</ion-header>\n\n<ion-content padding>\n  <div class="headerimgcontainer">\n    <div class="headerimg">\n      <img src="assets/imgs/logo.png" />\n    </div>\n  </div>\n\n  <div class="cardsparent">\n\n    <div class="cardscontainer">\n      <ion-card>\n        <ion-card-content>\n          <strong>Coraline</strong> is a cordova-like container that provides an easy way to run\n          <em>apps based on &quot;web-tech&quot;</em> \n          (<strong><a (click)="loadExternal(\'cordova\');">cordova</a></strong>,\n          <strong><a (click)="loadExternal(\'ionic\');">ionic</a></strong> or plain vanilla <em>DHTML</em>) natively under <strong>Linux</strong>, and aims to allow support for Linux clients with no (or minimal) changes to Android/windows/iOS code.\n\n          <div class="centimgcontainer">\n            <div class="coralineapps">\n              <img src="assets/imgs/coraline-apps.png" />\n            </div>\n          </div>\n        </ion-card-content>\n      </ion-card>\n\n      <ion-card>\n        <ion-card-content>\n          <div class="pluginimg">\n            <img src="assets/imgs/plugins.png" />\n          </div>\n          <h2>Plugins: Access native system functionality through javascript.</h2>\n          <p>\n            With <strong>plugins</strong>, your app has access to a host of information and functionality that breaks your app out of the browser.\n          </p>\n\n          <div class="centimgcontainer">\n            <div class="pluginsbanner">\n              <img src="assets/imgs/pluginsbanner.png" />\n            </div>\n          </div>\n          <p>\n            <ion-grid>\n              <ion-row>\n                <ion-col>\n                  <button full ion-button secondary (click)="goToPlugins()">Try Plugins</button>\n                </ion-col>\n                <ion-col>\n                  <button full menuToggle ion-button secondary>Find out more</button>\n                </ion-col>\n              </ion-row>\n            </ion-grid>\n          </p>\n        </ion-card-content>\n      </ion-card>\n    </div>\n  </div>\n\n</ion-content>\n'}),d("design:paramtypes",[i.e,r.a,a.a,c.a])],n)}()},200:function(n,t,e){"use strict";e.d(t,"a",function(){return s});var o=e(0),i=e(31),a=e(41),r=this&&this.__decorate||function(n,t,e,o){var i,a=arguments.length,r=a<3?t:null===o?o=Object.getOwnPropertyDescriptor(t,e):o;if("object"==typeof Reflect&&"function"==typeof Reflect.decorate)r=Reflect.decorate(n,t,e,o);else for(var c=n.length-1;c>=0;c--)(i=n[c])&&(r=(a<3?i(r):a>3?i(t,e,r):i(t,e))||r);return a>3&&r&&Object.defineProperty(t,e,r),r},c=this&&this.__metadata||function(n,t){if("object"==typeof Reflect&&"function"==typeof Reflect.metadata)return Reflect.metadata(n,t)},s=function(){function n(n,t,e){this.navCtrl=n,this.navParams=t,this.browser=e}return n.prototype.ionViewDidLoad=function(){console.log("ionViewDidLoad AboutPage")},n.prototype.gplSite=function(){this.externalSite("https://www.gnu.org/licenses/gpl-3.0.en.html")},n.prototype.homeSite=function(){this.externalSite("https://coraline.psychogenic.com")},n.prototype.psychoSite=function(){this.externalSite("https://psychogenic.com")},n.prototype.externalSite=function(n){this.browser.create(n,"external")},n=r([Object(o.m)({selector:"page-about",template:'\x3c!--\n  Generated template for the AboutPage page.\n\n  See http://ionicframework.com/docs/components/#navigation for more info on\n  Ionic pages and navigation.\n--\x3e\n<ion-header>\n\n  <ion-navbar>\n\n    <button ion-button  color="secondary"  menuToggle>\n        <ion-icon  name="menu"></ion-icon>\n      </button>\n\n    <ion-title>About Coraline</ion-title>\n  </ion-navbar>\n\n</ion-header>\n\n\n<ion-content padding>\n\n  <div class="headerimgcontainer">\n    <div class="headerimg">\n      <img src="assets/imgs/logo.png" />\n    </div>\n  </div>\n\n\n  <div class="cardsparent">\n    <div class="cardscontainer">\n\n      <ion-card>\n        <ion-card-content>\n            <a (click)="homeSite()"><strong>Coraline</strong></a> was created by Pat Deegan with support from\n            <a (click)="psychoSite()"><strong>psychogenic</strong></a>.\n          <br />\n\n          It is\n\n            <div class="centimgcontainer">\n                <div class="copynotice">\n                  <strong>Coraline v1.0.7</strong> <br />\n                Copyright &copy; 2017 Pat Deegan, psychogenic.com\n                </div>\n              <div class="copyimg">\n                <img src="assets/imgs/psychologo.png" />\n              </div>\n            </div>\n          and released under the terms of the\n          <a (click)="gplSite()">GNU General Public License v3.0</a>.\n          <br />\n            More information is available through the project site at\n            <a (click)="homeSite()">https://coraline.psychogenic.com</a>.\n\n        </ion-card-content>\n      </ion-card>\n\n\n      <ion-card>\n        <ion-card-content>\n          To do its job, Coraline relies on a host of awesome third party software. This includes:\n          <ion-list>\n            <ion-item>\n              <h2><a (click)="externalSite(\'https://github.com/zserge/webview\')">Webview</a></h2>\n              <p>A tiny cross-platform webview library for C/C++/Golang library, by Serge Zaitsev, to build modern cross-platform GUIs.</p>\n            </ion-item>\n\n            <ion-item>\n              <h2><a (click)="externalSite(\'https://github.com/nlohmann/json\')">JSON for Modern C++</a></h2>\n              <p>A spectacular C++ integration of JSON, by\n                <a (click)="externalSite(\'http://nlohmann.me\')">Niels Lohmann</a></p>\n            </ion-item>\n\n            <ion-item>\n              <h2><a (click)="externalSite(\'https://github.com/Cheedoong/xml2json\')">XML2JSON</a></h2>\n              <p>A header-only C++ library converts XML to JSON by Alan Zhuang</p>\n            </ion-item>\n\n            <ion-item>\n              <h2><a (click)="externalSite(\'https://www.gtk.org/\')">GTK+</a></h2>\n              <p>The GIMP Toolkit, along with all the coolness under that (GDK, glib, etc)</p>\n            </ion-item>\n\n            <ion-item>\n              <h2><a (click)="externalSite(\'https://webkitgtk.org/\')">WebKitGTK+</a></h2>\n              <p>WebKitGTK+ is a full-featured port of the WebKit rendering engine</p>\n            </ion-item>\n\n          </ion-list>\n        </ion-card-content>\n      </ion-card>\n    </div>\n  </div>\n\n\n\n</ion-content>\n'}),c("design:paramtypes",[i.e,i.f,a.a])],n)}()},201:function(n,t,e){"use strict";e.d(t,"a",function(){return s});var o=e(0),i=e(31),a=e(41),r=this&&this.__decorate||function(n,t,e,o){var i,a=arguments.length,r=a<3?t:null===o?o=Object.getOwnPropertyDescriptor(t,e):o;if("object"==typeof Reflect&&"function"==typeof Reflect.decorate)r=Reflect.decorate(n,t,e,o);else for(var c=n.length-1;c>=0;c--)(i=n[c])&&(r=(a<3?i(r):a>3?i(t,e,r):i(t,e))||r);return a>3&&r&&Object.defineProperty(t,e,r),r},c=this&&this.__metadata||function(n,t){if("object"==typeof Reflect&&"function"==typeof Reflect.metadata)return Reflect.metadata(n,t)},s=function(){function n(n,t,e){this.navCtrl=n,this.navParams=t,this.browser=e}return n.prototype.loadDevSection=function(){this.browser.create("https://coraline.psychogenic.com/developers/","external")},n=r([Object(o.m)({selector:"page-developers",template:'\x3c!--\n  Generated template for the DevelopersPage page.\n\n  See http://ionicframework.com/docs/components/#navigation for more info on\n  Ionic pages and navigation.\n--\x3e\n<ion-header>\n\n  <ion-navbar>\n\n    <button color="secondary" ion-button secondary menuToggle>\n            <ion-icon  name="menu"></ion-icon>\n          </button>\n    <ion-title>Developers</ion-title>\n  </ion-navbar>\n\n</ion-header>\n\n\n<ion-content padding>\n\n  <div class="cardsparent">\n    <div class="cardscontainer">\n\n      <ion-card>\n        <ion-card-content>\n\n            <div class="develimg">\n              <img src="assets/imgs/develico.png" />\n            </div>\n            <h2>Rolling your own</h2>\n          You can easily distribute <strong>your own apps</strong> to run using coraline and its plugins.\n          <br /> At its core, Coraline is a webkit renderer, you can point it to any HTML page on the filesystem and it will display it:\n<pre>\n  $ coraline /path/to/index.html\n</pre>\nOf greater interest is leveraging the power of the plugin system\nto boost your app’s capabilities. Though most any <em>web tech</em>\nsystem or framework should work, Coraline is geared\ntowards compatibility with <strong>Cordova</strong> and <strong>Ionic</strong>.\n        </ion-card-content>\n      </ion-card>\n\n      <ion-card>\n        <ion-card-content>\n          Access the Developer Quick Start and deployment guides on the\n          <a (click)="loadDevSection()">developer section\n          of the project site</a> for details on creating and\n          distributing your apps to Linux clients.\n        </ion-card-content>\n      </ion-card>\n    </div>\n  </div>\n</ion-content>\n'}),c("design:paramtypes",[i.e,i.f,a.a])],n)}()},202:function(n,t,e){"use strict";Object.defineProperty(t,"__esModule",{value:!0});var o=e(203),i=e(226);Object(o.a)().bootstrapModule(i.a)},226:function(n,t,e){"use strict";e.d(t,"a",function(){return y});var o=e(30),i=e(0),a=e(31),r=e(269),c=e(198),s=e(200),l=e(103),d=e(201),p=e(195),u=e(197),g=e(199),h=e(278),f=e(41),m=e(101),v=e(102),b=this&&this.__decorate||function(n,t,e,o){var i,a=arguments.length,r=a<3?t:null===o?o=Object.getOwnPropertyDescriptor(t,e):o;if("object"==typeof Reflect&&"function"==typeof Reflect.decorate)r=Reflect.decorate(n,t,e,o);else for(var c=n.length-1;c>=0;c--)(i=n[c])&&(r=(a<3?i(r):a>3?i(t,e,r):i(t,e))||r);return a>3&&r&&Object.defineProperty(t,e,r),r},y=function(){function n(){}return n=b([Object(i.I)({declarations:[r.a,c.a,s.a,l.a,d.a],imports:[o.a,a.c.forRoot(r.a,{},{links:[]})],bootstrap:[a.a],entryComponents:[r.a,c.a,s.a,l.a,d.a],providers:[p.a,u.a,{provide:i.u,useClass:a.b},{provide:h.a,useClass:h.a},{provide:f.a,useClass:f.a},{provide:m.a,useClass:m.a},{provide:v.a,useClass:v.a},{provide:g.a,useClass:g.a}]})],n)}()},269:function(n,t,e){"use strict";e.d(t,"a",function(){return g});var o=e(0),i=e(31),a=e(195),r=e(197),c=e(198),s=e(200),l=e(103),d=e(201),p=this&&this.__decorate||function(n,t,e,o){var i,a=arguments.length,r=a<3?t:null===o?o=Object.getOwnPropertyDescriptor(t,e):o;if("object"==typeof Reflect&&"function"==typeof Reflect.decorate)r=Reflect.decorate(n,t,e,o);else for(var c=n.length-1;c>=0;c--)(i=n[c])&&(r=(a<3?i(r):a>3?i(t,e,r):i(t,e))||r);return a>3&&r&&Object.defineProperty(t,e,r),r},u=this&&this.__metadata||function(n,t){if("object"==typeof Reflect&&"function"==typeof Reflect.metadata)return Reflect.metadata(n,t)},g=function(){function n(n,t,e){this.platform=n,this.statusBar=t,this.splashScreen=e,this.rootPage=c.a,this.initializeApp(),this.pages=[{title:"Coraline",component:c.a,icon:"home"},{title:"Plugins",component:l.a,icon:"outlet"},{title:"Developers",component:d.a,icon:"code"},{title:"About",component:s.a,icon:"information-circle"}]}return n.prototype.initializeApp=function(){var n=this;this.platform.ready().then(function(){n.statusBar.styleDefault(),n.splashScreen.hide()})},n.prototype.openPage=function(n){this.nav.setRoot(n.component)},p([Object(o._8)(i.d),u("design:type",i.d)],n.prototype,"nav",void 0),n=p([Object(o.m)({template:'<ion-menu [content]="content">\n  <ion-header>\n    <ion-toolbar>\n      <ion-title>Menu</ion-title>\n    </ion-toolbar>\n  </ion-header>\n\n  <ion-content>\n    <ion-list>\n      <button menuClose ion-item *ngFor="let p of pages" (click)="openPage(p)">\n        <ion-icon  color="secondary" name="{{p.icon}}"></ion-icon>\n        <span class="menuitem">{{p.title}}</span>\n      </button>\n    </ion-list>\n  </ion-content>\n\n</ion-menu>\n\n\x3c!-- Disable swipe-to-go-back because it\'s poor UX to combine STGB with side menus --\x3e\n<ion-nav [root]="rootPage" #content swipeBackEnabled="false"></ion-nav>\n'}),u("design:paramtypes",[i.g,a.a,r.a])],n)}()}},[202]);
