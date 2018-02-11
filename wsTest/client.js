

const myPort = 8080;
var counter = 0;

if (typeof(window) !== 'undefined') {

  var connection = new WebSocket('ws://localhost:' + myPort);

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
      connection.send("this is an awesome message. - from Client " + counter);
      counter++;
  }



}
