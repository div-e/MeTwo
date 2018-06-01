

const myPort = 8080;
var counter = 0;

// If JS is executed in a browser window: 
if (typeof(window) !== 'undefined') {

  var connection = new WebSocket('ws://192.168.0.101:'+myPort);
  connection.onopen = function() {
    console.log("client says: connection established!");
  }

  connection.onmessage = function(message) {
    console.log("message from server: " + message.data);
  };

  window.onkeydown = keyDownHandler;

  // called when a key is pressed.
  function keyDownHandler(e) {
      if (e.repeat) {
          return;
      }
      e = e || window.event; 

      var charCode = e.keyCode || e.which;
      var str = String.fromCharCode(charCode); 
      connection.send("Client: " + str);
  }



}
