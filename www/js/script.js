//init variables
var Server, accelerationX, accelerationY, accelerationZ, turnLeft, scaledY;

var motorData	= "0:0:0:0:";
var ROTATION_LIMIT	= 5;
var leftMotor		= 0,
	leftMotorDir	= 0,
	rightMotor 		= 0,
	rightMotorDir	= 0,
	throttle 		= 0;
	leftMotorValue	= 0;
	rightMotorValue = 0;

//note to self: don't forget to add a buffer between speed changes to prolong life possibly using (target-current)*.1

function send( text ) {
	//Server.send( 'message', text );	//commented for debugging purposes
	console.log(text);
}

function translate( rawX, rawY, throttle ) {
	turnLeft = (rawY > 0 && rawX < 0 ) || (rawY < 0 && rawX > 0);	//returns 1 if the device is tilted left

	//scales the Y acceleration value to be a range within ROTATION_LIMIT m/s^2 (default 5)
	if ((rawY <= ROTATION_LIMIT && rawY >= 0) || (rawY >= -ROTATION_LIMIT && rawY < 0)) {
		scaledY = (rawY / ROTATION_LIMIT) * 100;
	}
	else if (rawY < 0) {
		scaledY = -100;
	}
	else if (rawY > 0) {
		scaledY = 100;
	}

	if ((turnLeft && (scaledY > 0)) || !turnLeft && (scaledY < 0)) {
		scaledY = -scaledY;
	} 

	document.getElementById("scaledY").innerHTML = Math.floor(scaledY);

	if (throttle) {

	}
	else {
		if (scaledY >= 0) {					//turn right
			leftMotor 		= scaledY;
			leftMotorDir 	= 1;
			rightMotor 		= scaledY;
			rightMotorDir 	= 0;
		}
		else {								//turn left
			leftMotor 		= scaledY;
			leftMotorDir 	= 0;
			rightMotor 		= scaledY;
			rightMotorDir 	= 1;
		}
	}

}

$(document).ready(function() {
	console.log('Connecting...');
	Server = new FancyWebSocket('ws://141.114.220.4:8080');

	if (window.DeviceOrientationEvent != undefined) {
		window.ondevicemotion = function (e) {
			accelerationX = e.accelerationIncludingGravity.x;
			accelerationY = e.accelerationIncludingGravity.y;
			accelerationZ = e.accelerationIncludingGravity.z;

			translate(accelerationX, accelerationY, throttle);	//translate into motor values

			document.getElementById("accelerationX").innerHTML = Math.floor(accelerationX);
			document.getElementById("accelerationY").innerHTML = Math.floor(accelerationY);
			document.getElementById("accelerationZ").innerHTML = Math.floor(accelerationZ);

			var landscapeOrientation = window.innerWidth / window.innerHeight > 1;	//returns 1 if device is horizontal
			if (landscapeOrientation) {
				document.getElementById("orientation").innerHTML = "Horizontal";
			} else {
				document.getElementById("orientation").innerHTML = "Vertical";
			}
		}
		window.ondeviceorientation = function (e) {
			document.getElementById("orientationAlpha").innerHTML = Math.floor(e.alpha);
			document.getElementById("orientationBeta").innerHTML = Math.floor(e.beta);
			document.getElementById("orientationGamma").innerHTML = Math.floor(e.gamma);
		}
	}
	else {
		console.log("To use your iOS device you must have iOS firmware 4.2 or later.")
	}

	//send motor data to the server
	setTimeout( function() {
		send( leftMotor + ":" + rightMotor + ":" + leftMotorDir + ":" + rightMotorDir + ":" );
	}, 10); //set to 10ms because this is the max allowed by PWMserver


	//Let the user know we're connected
	Server.bind('open', function() {
		console.log( "Connected." );
	});

	//OH NOES! Disconnection occurred.
	Server.bind('close', function( data ) {
		console.log( "Disconnected." );
	});

	//Log any messages sent from server
	Server.bind('message', function( payload ) {
		console.log( payload );
	});

	Server.connect();
});

//send motor data to the server
setTimeout( function() {
	motorData = leftMotor + ":" + rightMotor + ":" + leftMotorDir + ":" + rightMotorDir + ":";
	send(motorData);
}, 10); //set to 10ms because this is the max allowed by PWMserver