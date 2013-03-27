RPi-TankBot
===========

### About ###

Software written in C with a PHP websockets sever to turn your Raspberry Pi into a raging tank bot.

I am still in development stage, so be warned, it may not work fully. I will do my best to have the tank bot fully working with demos ASAP.

### Usage ###

** Software **

* Install `mjpg-streamer` following this [tutorial](http://wolfpaulus.com/journal/embedded/raspberrypi_webcam)
* Install the [BCM2835 library](http://www.airspayce.com/mikem/bcm2835/) for compiling the `PWMserver`
* Apache and PHP are necessary to run the websockets server

** Hardware **

This software can be used to interface with any hardware; however, it can only generate PWM on a maximum of 2 pins simultaneously.

** Build **

* place the `server` in `/var/` (this is the default directory, if you use another you will need to alter `build.sh`)
* add the contents of the `www` folder in `/var/www`
* run: `chmod -x build.sh`
* compile PWMserver using `sudo build.sh`
* start the necessary programs `sudo start.sh`

### Change Log ###

2013 03 26 - **r1**

* Initial Commit
