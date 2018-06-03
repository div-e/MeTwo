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
    limitStickTravel  : true
  });

  joystick.addEventListener('touchStart', function(){
    console.log('down');
  });

  joystick.addEventListener('touchEnd', function(){
    console.log('up');
  });

  var deployStatus = 0;
  document.getElementById("deployer").onclick = function() {
    if (deployStatus == 0) {
      deployStatus = 1;
    } else {
      deployStatus = 0;
    }
  }

  /* send key signals */
  var w = false;
  var a = false;
  var s = false;
  var d = false;

  /* set wasd booleans to true on key down */
  document.onkeydown = function (event) {
    if (event.keyCode == 87) {
      w = true;
    }
    if (event.keyCode == 65) {
      a = true;
    }
    if (event.keyCode == 83) {
      s = true;
    }
    if (event.keyCode == 68) {
      d = true;
    }
  }

  /* set wasd booleans to false on key up */
  document.onkeyup = function (event) {
    if (event.keyCode == 87) {
      w = false;
    }
    if (event.keyCode == 65) {
      a = false;
    }
    if (event.keyCode == 83) {
      s = false;
    }
    if (event.keyCode == 68) {
      d = false;
    }
  }
  setInterval(function(){

    /* calculate speed of left and right motors based on wasd keys */
    var leftMotor = 0;
    var rightMotor = 0;

    if (w) {
      leftMotor += 2;
      rightMotor += 2;
    }

    if (a) {
      leftMotor -= 1;
      rightMotor += 1;
    }

    if (s) {
      leftMotor -= 2;
      rightMotor -= 2;
    }

    if (d) {
      leftMotor += 1;
      rightMotor -= 1;
    }
    leftMotor += 4;
    rightMotor += 4;

    var joystickX = Math.round(joystick.deltaX() / 20) + 5;
    var joystickY = Math.round(joystick.deltaY() / 20) + 5;
    /* convert to json string */
    var json = {leftMotor:leftMotor, rightMotor:rightMotor,
                dx:joystickX, dy:joystickY, deployed:deployStatus};
    var jsonString = JSON.stringify(json);

    /* send json strings */
    connection.send(jsonString);
  }, 1/30 * 1000);
}
