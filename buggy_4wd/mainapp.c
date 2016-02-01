#define GPIO_MOT_A1	12
#define GPIO_MOT_A2	14
#define GPIO_MOT_B1	16
#define GPIO_MOT_B2	15

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#include "lib_gpio/BBBiolib.h"
#include "lib_gpio/BBBiolib_PWMSS.h"

void GpioSetup(void);
unsigned char EndOfApp;

unsigned char arcValue;

// ***************************************************************************
// ---------------------------------------------------------------------------
// STARTUP
// ---------------------------------------------------------------------------
// ***************************************************************************

int main(void) {

	system("clear");
	printf("\nALGOID - 4WD WHEELER TEST MULTITASK - 01/02/2016\n");
	printf("------------------------------------------------\n\n");


	// ************   DEBUG 1er message non traité correctement, a corriger. -> ENVOIE UN MESSAGE BIDON
	sleep(1);

	if(iolib_init()) printf("\n ERREUR BBBIOLIB");
	else printf("\n BBBIOLIB OK");

	//usleep(100000);
	GpioSetup();			// Initialisation Port IO


	float PWM_HZ =20000.0f ;	/* 20KHz*/
	float duty_A = 10.0f ; 	/* 20% Duty cycle for PWM 0_A output */
	float duty_B = 10.0f ;	/* 50% Duty cycle for PWM 0_B output*/

	BBBIO_PWMSS_Setting(BBBIO_PWMSS1, PWM_HZ ,duty_B , duty_B);
	printf("PWM Demo setting ....\n");

	printf("PWM %d enable\n", BBBIO_PWMSS1);
	BBBIO_ehrPWM_Enable(BBBIO_PWMSS1);

	// ---------------------------------------------------------------------------
	// MAIN LOOP
	// ---------------------------------------------------------------------------
	while(!EndOfApp){
		int ch;

		//ch = getchar();
		ch = mygetch();

		printf("%d", ch);

		// FERME L'APPLICATION
		if(ch=='q')
			EndOfApp=1;

		if(ch=='e'){
			duty_A+=10.0;
			duty_B+=10.0;
			if(duty_B>100.0) duty_B=100.0;
			BBBIO_PWMSS_Setting(BBBIO_PWMSS1, PWM_HZ ,duty_A , duty_B);
			BBBIO_ehrPWM_Enable(BBBIO_PWMSS1);
		}

		if(ch=='w'){
					duty_A-=10.0;
					duty_B-=10.0;
					if(duty_B<5.0) duty_B=5.0;
					BBBIO_PWMSS_Setting(BBBIO_PWMSS1, PWM_HZ ,duty_A , duty_B);
					BBBIO_ehrPWM_Enable(BBBIO_PWMSS1);
				}

		if(ch=='g'){
			system ("shutdown -h 0");
			EndOfApp=1;
		}

		//Avancer
		if(ch==65){
			BBBIO_PWMSS_Setting(BBBIO_PWMSS1, PWM_HZ ,duty_A , duty_B);
			BBBIO_ehrPWM_Enable(BBBIO_PWMSS1);
			pin_low(8,GPIO_MOT_A1);
			pin_high(8,GPIO_MOT_A2);
			pin_high(8,GPIO_MOT_B1);
			pin_low(8,GPIO_MOT_B2);

		}

		// Reculer
		if(ch==66){
			BBBIO_PWMSS_Setting(BBBIO_PWMSS1, PWM_HZ ,duty_A , duty_B);
			BBBIO_ehrPWM_Enable(BBBIO_PWMSS1);
			pin_high(8,GPIO_MOT_A1);
			pin_low(8,GPIO_MOT_A2);
			pin_low(8,GPIO_MOT_B1);
			pin_high(8,GPIO_MOT_B2);
		}
// moin vittes
		//correction vitesse virage
		if(ch==36){
			arcValue+=5;
		}
// Plus vite
		if(ch==69){
			arcValue-=5;
		}

		//arc gauche
		if(ch==68){
			BBBIO_PWMSS_Setting(BBBIO_PWMSS1, PWM_HZ ,duty_A+arcValue , duty_B-arcValue);
			BBBIO_ehrPWM_Enable(BBBIO_PWMSS1);
			pin_low(8,GPIO_MOT_A1);
			pin_high(8,GPIO_MOT_A2);
			pin_high(8,GPIO_MOT_B1);
			pin_low(8,GPIO_MOT_B2);
		}

		//arc droit
		if(ch==67){
			BBBIO_PWMSS_Setting(BBBIO_PWMSS1, PWM_HZ ,duty_A-arcValue, duty_B+arcValue);
			BBBIO_ehrPWM_Enable(BBBIO_PWMSS1);
			pin_low(8,GPIO_MOT_A1);
			pin_high(8,GPIO_MOT_A2);
			pin_high(8,GPIO_MOT_B1);
			pin_low(8,GPIO_MOT_B2);
		}




		// spin gauche
		if(ch==44){
			duty_B=duty_A;
			BBBIO_PWMSS_Setting(BBBIO_PWMSS1, PWM_HZ ,duty_A , duty_B);
			BBBIO_ehrPWM_Enable(BBBIO_PWMSS1);
			pin_low(8,GPIO_MOT_A1);
			pin_high(8,GPIO_MOT_A2);
			pin_low(8,GPIO_MOT_B1);
			pin_high(8,GPIO_MOT_B2);
		}

		//spin droite
		if(ch==45){
			duty_B=duty_A;
			BBBIO_PWMSS_Setting(BBBIO_PWMSS1, PWM_HZ ,duty_A , duty_B);
			BBBIO_ehrPWM_Enable(BBBIO_PWMSS1);
			pin_high(8,GPIO_MOT_A1);
			pin_low(8,GPIO_MOT_A2);
			pin_high(8,GPIO_MOT_B1);
			pin_low(8,GPIO_MOT_B2);
		}

		//stop
		if(ch==10){
			pin_low(8,GPIO_MOT_A1);
			pin_low(8,GPIO_MOT_A2);
			pin_low(8,GPIO_MOT_B1);
			pin_low(8,GPIO_MOT_B2);
		}

	}
 /*Ralentit le thread d'ecriture... */

	BBBIO_ehrPWM_Disable(BBBIO_PWMSS0);
	BBBIO_ehrPWM_Disable(BBBIO_PWMSS1);
    iolib_free();
	usleep(10000);
		printf("\n# Fin d' application, bye bye !\n");
		return (0);
}

		// ---------------------------------------------------------------------------
		// INITIALISATION DES ENTREES/SORTIES DU SYSTEM
		// ---------------------------------------------------------------------------
		void GpioSetup(void){
			//iolib_init();	// Ouverture des GPIO
			//iolib_setdir(8,15, BBBIO_DIR_OUT);
			iolib_setdir(8,GPIO_MOT_A1, BBBIO_DIR_OUT);
			iolib_setdir(8,GPIO_MOT_A2, BBBIO_DIR_OUT);
			iolib_setdir(8,GPIO_MOT_B1, BBBIO_DIR_OUT);
			iolib_setdir(8,GPIO_MOT_B2, BBBIO_DIR_OUT);

			//iolib_setdir(8,GPIO_BUZZER, BBBIO_DIR_OUT);			// P8.x en sortie pour buzzer
			//pin_low(8,GPIO_BUZZER);								// Dï¿½sactive gpio buzzer
		}

		int mygetch(void)
		{
		    struct termios oldt,newt;
		    int ch;
		    tcgetattr( STDIN_FILENO, &oldt );
		    newt = oldt;
		    newt.c_lflag &= ~( ICANON | ECHO );
		    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
		    ch = getchar();
		    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
		    return ch;
		}
