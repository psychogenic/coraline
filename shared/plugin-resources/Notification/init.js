(function(){function e(a){"string"===typeof a&&(a=a.split(","));return a}this.navigator.notification={alert:function(a,d,c,b){a="string"===typeof a?a:JSON.stringify(a);cordova.exec(d,null,"Notification","alert",[a,"string"===typeof c?c:"Alert",b&&"string"===typeof b?b:"OK"])},confirm:function(a,d,c,b){a="string"===typeof a?a:JSON.stringify(a);c="string"===typeof c?c:"Confirm";b=b||["OK","Cancel"];"string"===typeof b&&console.log("Notification.confirm(string, function, string, string) is deprecated.  Use Notification.confirm(string, function, string, array).");
b=e(b);cordova.exec(d,null,"Notification","confirm",[a,c,b])},prompt:function(a,d,c,b,f){a="string"===typeof a?a:JSON.stringify(a);c="string"===typeof c?c:"Prompt";b=b||["OK","Cancel"];"string"===typeof b&&console.log("Notification.prompt(string, function, string, string) is deprecated.  Use Notification.confirm(string, function, string, array).");b=e(b);cordova.exec(d,null,"Notification","prompt",[a,c,b,f||""])},beep:function(a){cordova.exec(null,null,"Notification","beep",[a||1])}}}).call(this);
