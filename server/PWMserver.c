/********************* INFO *********************\
 * This program will be run whenever the motor 	*
 * speed changes for either motor. The script	*
 * uses PDW to vary the speed of each motor.	*
 * The loop will create a loop and turn the 	*
 * motors on or off depending on the given DC   *
 * time. Pin is wired to the enable pin on the	*
 * L293D h-bridge driver.						*
\************************************************/

/************************ TO COMPILE ************************\
 * Run: 													*
 * 	(sudo) build.sh 	--builds and copys to /var/server/ 	*
 * Or:														*
 * 	gcc -o PWMserver -l rt PWMserver.c -l bcm2835 -std=c99	*
\************************************************************/


#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pwm.h"

// 
int main( int argc, char *argv[] ) {

	// initialie the bcm2835 library
	if ( !bcm2835_init() )
		return 1;

	// Set the pins to be outputs
	bcm2835_gpio_fsel( LM_PIN, BCM2835_GPIO_FSEL_OUTP );
	bcm2835_gpio_fsel( RM_PIN, BCM2835_GPIO_FSEL_OUTP );
	bcm2835_gpio_fsel( LF_PIN, BCM2835_GPIO_FSEL_OUTP );
	bcm2835_gpio_fsel( LR_PIN, BCM2835_GPIO_FSEL_OUTP );
	bcm2835_gpio_fsel( RF_PIN, BCM2835_GPIO_FSEL_OUTP );
	bcm2835_gpio_fsel( RR_PIN, BCM2835_GPIO_FSEL_OUTP );

	printf("Starting PWM server...\n");

	while ( 1 ) {

		unsigned int LeftMotorPWM, RightMotorPWM,
			LeftMotorDir, RightMotorDir;

		// get motor data
		FILE *fp;
		fp = fopen( "motorData", "rw" );

		// if there is an error reading file don't alter variables
		if ( fp != NULL ) {

			/************* motor_data.txt format ************\
			 * LeftDC:RightDC:LeftMotorDir:RightMotorDir:	*
			 * e.x. 75:30:1:0: [turn right]					*
			\************************************************/

			char buffer[30];
			fgets( buffer, 30, fp );
			char *search = ":";
			
			LeftMotorPWM	= atoi( strtok( buffer, search ) );	// first variable
			RightMotorPWM	= atoi( strtok( NULL, search ) );	// second variable
			LeftMotorDir	= atoi( strtok( NULL, search ) );	// third variable
			RightMotorDir 	= atoi( strtok( NULL, search ) );	// fourth variable

			fclose( fp );

		}
		else {
			printf("Error opening file 'motorData' in the current directory.\n");
		}

		// end get motor data

		unsigned int Left_TimeOn 	= LeftMotorPWM * 100;
		unsigned int Right_TimeOn 	= RightMotorPWM * 100;

		unsigned int Left_TimeOff 	= 10000 - Left_TimeOn;
		unsigned int Right_TimeOff 	= 10000 - Right_TimeOn;


		// set the direction of left motors
		if ( LeftMotorDir == 1 ) {
			bcm2835_gpio_write(LF_PIN, HIGH);
			bcm2835_gpio_write(LR_PIN, LOW);
		}
		else if ( LeftMotorDir == 0 ) {
			bcm2835_gpio_write(LF_PIN, LOW);
			bcm2835_gpio_write(LR_PIN, HIGH);
		}

		// sets the direction of right motors
		if ( RightMotorDir == 1 ) {
			bcm2835_gpio_write(RF_PIN, HIGH);
			bcm2835_gpio_write(RR_PIN, LOW);
		}
		else if ( RightMotorDir == 0 ) {
			bcm2835_gpio_write(RF_PIN, LOW);
			bcm2835_gpio_write(RR_PIN, HIGH);
		}

		// turn Right
		if ( Left_TimeOn < Right_TimeOn ) {
			createDualPWM( 0, Left_TimeOn, (Right_TimeOn - Left_TimeOn), Right_TimeOff );
		}

		// turn Left
		if ( Right_TimeOn < Left_TimeOn ) {
			createDualPWM( 1, Right_TimeOn, (Left_TimeOn - Right_TimeOn), Left_TimeOff );
		}

		// creates a single PWM for both motors
		if ( Left_TimeOn == Right_TimeOn ) {

			// check if motors != 0
			if ( Left_TimeOn != 0 ) {
				// turn both motors on
				bcm2835_gpio_write(LM_PIN, HIGH);
				bcm2835_gpio_write(RM_PIN, HIGH);
			}
			else {
				// set all I/O pins to LOW for possible shutdown
				// 
				bcm2835_gpio_write( LM_PIN, LOW );
				bcm2835_gpio_write( RM_PIN, LOW );
				bcm2835_gpio_write( LF_PIN, LOW );
				bcm2835_gpio_write( LR_PIN, LOW );
				bcm2835_gpio_write( RF_PIN, LOW );
				bcm2835_gpio_write( RR_PIN, LOW );
			}

			// initial delay
			bcm2835_delayMicroseconds( Left_TimeOn );

			// turn both motors off
			bcm2835_gpio_write(LM_PIN, LOW);
			bcm2835_gpio_write(RM_PIN, LOW);

			// final delay
			bcm2835_delayMicroseconds( Left_TimeOff );

		}

	}

	// creates independent PWM for each motor
	int createDualPWM( char moveLeft, unsigned int initDelay, unsigned int intermDelay, unsigned int finalDelay ) {

		// turn both motors on
		bcm2835_gpio_write(LM_PIN, HIGH);
		bcm2835_gpio_write(RM_PIN, HIGH);
		
		// initial delay
		bcm2835_delayMicroseconds( initDelay );
		
		// if moveLeft trun off RM_PIN
		if ( moveLeft == 0 ) {
			bcm2835_gpio_write(LM_PIN, LOW);
		}

		if ( moveLeft == 1 ) {
			bcm2835_gpio_write(RM_PIN, LOW);
		}
		
		// intermediate delay
		bcm2835_delayMicroseconds( intermDelay );

		// if moveLeft turn of LR_PIN
		if ( moveLeft == 0 ) {
			bcm2835_gpio_write(RM_PIN, LOW);
		}

		if ( moveLeft == 1 ) {
			bcm2835_gpio_write(LM_PIN, LOW);
		}
		
		// final delay
		bcm2835_delayMicroseconds( finalDelay );

		return 0;

	}

	// Resets GPIO pins if the script 
	bcm2835_gpio_write( LM_PIN, LOW );
	bcm2835_gpio_write( RM_PIN, LOW );
	bcm2835_gpio_write( LF_PIN, LOW );
	bcm2835_gpio_write( LR_PIN, LOW );
	bcm2835_gpio_write( RF_PIN, LOW );
	bcm2835_gpio_write( RR_PIN, LOW );

	printf("Quiting unexpectidly...");

	return 0;

}