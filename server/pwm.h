// Header file for PWMserver.c


// Pinout On RPi
 /*----------*\
 1|	*	*	|2
 3|	*	*	|4
 5|	*	*	|6
 7|	*	*	|8
 9|	*	*	|10
11|	LM	RM	|12
13|	*	*	|14
15|	LF	LR	|16
17|	*	RF	|18
19|	*	*	|20
21|	*	RR	|22
23|	*	*	|24
25|	*	*	|26
 \*----------*/

// Defines pin names for convenience

#define LM_PIN RPI_GPIO_P1_11	//PWM for Left Motors
#define RM_PIN RPI_GPIO_P1_12	//PWM for Right Motors
#define LF_PIN RPI_GPIO_P1_15	//Left Forward
#define LR_PIN RPI_GPIO_P1_16	//Left Reverse
#define RF_PIN RPI_GPIO_P1_18	//Right Forward
#define RR_PIN RPI_GPIO_P1_22	//Right Reverse

// Function takes a boolean value for moveLeft and the delay
// times for the events associated with the PWM output

int createDualPWM( char moveLeft, unsigned int initDelay, unsigned int intermDelay, unsigned int finalDelay );