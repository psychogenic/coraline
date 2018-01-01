/*
 * NativeBridge.cpp
 *
 *  Created on: Dec 4, 2017
 *      
 *        Copyright (C) 2017 Pat Deegan, https://psychogenic.com
 *  
 *  This file is part of Coraline [https://coraline.psychogenic.com/]
 *
 *  Coraline is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Coraline is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Coraline.  If not, see <http://www.gnu.org/licenses/>.
 *  
 */

#include "coraline/builtin/NativeBridge.h"
#include "coraline/webview/Registry.h"
namespace Coraline {
namespace Plugin {

NativeBridge::NativeBridge(const Context & ctx) : Base(ctx) {


}

NativeBridge::~NativeBridge() {
}

ClientCode NativeBridge::clientSideSupport() {
	return ClientCode(
	"var AAA=function(){return function(d,a){this.successCb=d;this.errorCb=a}}(),NativeBridge=function(){function d(){this.cbcounter=2;this.callbacks={};this.lifecycleEvents={deviceready:new Event(\"deviceready\")}}d.prototype.notifyDeviceReady=function(){document.dispatchEvent(this.lifecycleEvents.deviceready)};d.prototype.addCallbacks=function(a,b){var c=this.cbcounter;this.callbacks[c]=new AAA(a,b);this.cbcounter+=2;return c};d.prototype.triggerCallback=function(a,b){return this.triggerCallbackNoRemove(a, b)?(delete this.callbacks[a],!0):!1};d.prototype.triggerCallbackNoRemove=function(a,b){var c=!1;a%2&&(c=!0,--a);return a in this.callbacks?(c?this.callbacks[a].errorCb(b):this.callbacks[a].successCb(b),!0):!1};d.prototype.convertArrayBufferToNative=function(a,b){var c=[],d;d=b?a:new Uint8Array(a);for(var e=0;e<d.length;e++)c[e]=d[e];return c};d.prototype.convertToNative=function(a){if(ArrayBuffer.isView(a))return this.convertArrayBufferToNative(a,!0);if(\"[object ArrayBuffer]\"==Object.prototype.toString.call(a))return this.convertArrayBufferToNative(a, !1);if(Array.isArray(a)){for(var b=0;b<a.length;b++)a[b]=this.convertToNative(a[b]);return a}if(\"object\"==typeof a)for(var c=Object.keys(a),b=0;b<c.length;b++)a[c[b]]=this.convertToNative(a[c[b]]);return a};d.prototype.pluginAvailable=function(a,b,c){a?this.exec(b,c,\"NativeBridge\",\"pluginAvailable\",[a]):c&&c()};d.prototype.exec=function(a,b,c,d,e){a||(a=function(a){});b||(b=function(a){});if(!c||!d)return!1;e||(e=[]);for(var f=0;f<e.length;f++)e[f]=this.convertToNative(e[f]);a=this.addCallbacks(a, b);c=JSON.stringify({cb:[a,a+1],dest:[c,d],args:e});window.external.invoke_(c)};d.prototype.terminateGUI=function(){window.external.shutdown_(\"ho\")};return d}(),nativebridge=new NativeBridge,cordova=nativebridge;"
	);
}

AboutString NativeBridge::about() {
	return "Coraline NativeBridge Plugin, Copyright (C) 2017 Pat Deegan, psychogenic.com";
}
AboutString NativeBridge::usage() {
	return AboutString(
			"Coraline NativeBridge Plugin, Copyright (C) 2017 Pat Deegan, psychogenic.com\n"
			"Used to provide cordova-like access to native plugins, usage is internal."
	);

}

void NativeBridge::notifyDeviceReady() {
	executeJS("nativebridge.notifyDeviceReady();");

}

void NativeBridge::injectCode() {
	executeJS(clientSideSupport());
}

void NativeBridge::registerAllMethods() {
	PLUGINREGMETH(pluginAvailable);
}


bool NativeBridge::pluginAvailable(const StandardCallbackIDs & callbacks,
		const ArgsList & args) {

	if (args.size() < 1) {
		return false;
	}

	if (! args[0].is_string()) {
		return false;
	}

	if (Registry::getInstance()->has(args[0])) {
		reportSuccess(callbacks);
	} else {
		reportError(callbacks);
	}

	return true;

}

} /* namespace Plugin */
} /* namespace Coraline */
