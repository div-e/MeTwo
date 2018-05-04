const myPort = 8080;
var counter = 0;


var connection = new WebSocket('ws://localhost:' + myPort);

connection.onopen = function() {
  console.log("client says: connection established!");
}

connection.onmessage = function(message) {
  console.log("message from server: " + message.data);
};

if( /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent) ) {
  var joystick	= new VirtualJoystick({
  container	: document.body,
  strokeStyle	: 'cyan',
  limitStickTravel: true,
  stickRadius	: 120
  });
  joystick.addEventListener('touchStartValidation', function(event){
    var touch	= event.changedTouches[0];
    if( touch.pageX < window.innerWidth/2 )	return false;
    return true
  });

  // one on the right of the screen
  var joystick	= new VirtualJoystick({
    container	: document.body,
    strokeStyle	: 'orange',
    limitStickTravel: true,
    stickRadius	: 120
  });

  joystick.addEventListener('touchStartValidation', function(event){
    var touch	= event.changedTouches[0];
    if( touch.pageX >= window.innerWidth/2 )	return false;
    return true
  });
} else {
  console.log("touchscreen is",
    VirtualJoystick.touchScreenAvailable() ? "available" : "not available");

  var joystick	= new VirtualJoystick({
    container	: document.getElementById('container'),
    mouseSupport	: true,
  });

  joystick.addEventListener('touchStart', function(){
    console.log('down')
  });

  joystick.addEventListener('touchEnd', function(){
    console.log('up')
  });

  setInterval(function(){
    connection.send(' dx:' + joystick.deltaX() + ' dy:' + joystick.deltaY());
  }, 1/30 * 1000);
}
