const myPort = 8085;
var counter = 0;

var connection = new WebSocket('ws://192.168.0.201:' + myPort);//'ws://192.168.0.201:'+myPort);
var connectionOpen = false; 

connection.onopen = function() {
  console.log("client says: connection established!");
  connectionOpen = true;
}

connection.onmessage = function(message) {
  console.log("message from server: " + message.data);
};

connection.onclose = function() {
  connectionOpen = false;
}


/* send key signals */
var w = false;
var a = false;
var s = false;
var d = false;

/* motor speed global vars */
var leftMotor = 0;
var rightMotor = 0;

var leftMotor_prev = 0;
var rightMotor_prev = 0;

/* joystick global vars */
var joystickX = 5;
var joystickY = 5;

var joystickX_prev = 5;
var joystickY_prev = 5;


// Checks whether or not there was a change in drive motor speed or joystick.
function hasChange() {

  if(leftMotor_prev != leftMotor) {console.log("leftmotor"); return true;}
  if(rightMotor_prev != rightMotor) {console.log("rightmotor"); return true;}
  if(joystickX_prev != joystickX) {console.log("joyX"); return true;}
  if(joystickY_prev != joystickY) {console.log("joyY"); return true;}

  return false;
}

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

<<<<<<< HEAD
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
=======
  // Set wasd booleans to true on key down.
>>>>>>> 4f58a6aab126d227ed35bb4216cefacbb40256db
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

  // Set wasd booleans to false on key up.
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

    // Update the previous motor values.
    leftMotor_prev = leftMotor;
    rightMotor_prev = rightMotor;

    // Calculate speed of left and right motors based on wasd keys.
    leftMotor = 0;
    rightMotor = 0;

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
    // Offset the left and right motors by 4, so the range is 1 to 7, with 4 being the not moving state.
    leftMotor += 4;
    rightMotor += 4;

<<<<<<< HEAD
    var joystickX = Math.round(joystick.deltaX() / 20) + 5;
    var joystickY = Math.round(joystick.deltaY() / 20) + 5;
    /* convert to json string */
    var json = {leftMotor:leftMotor, rightMotor:rightMotor,
                dx:joystickX, dy:joystickY, deployed:deployStatus};
    var jsonString = JSON.stringify(json);
=======
    // Update joystick prev values.
    joystickX_prev = joystickX;
    joystickY_prev = joystickY;

    // Update joystick values.
    joystickX = Math.floor(joystick.deltaX() / 20) + 5;
    joystickY = Math.floor(joystick.deltaY() / 20) + 5;
>>>>>>> 4f58a6aab126d227ed35bb4216cefacbb40256db

    if(connectionOpen && hasChange()) {

      // Motor values 1 to 7. 4 is the middle value.
      // Joystick values 0 to 9. 5 is the middle value.
      var stringToSend = '' + leftMotor + rightMotor + joystickX + joystickY;


      // Send string.
      connection.send(stringToSend);
    }

  }, 1/30 * 1000);
}
