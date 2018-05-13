#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"  /* Declrations of project functions and variables */

/*	LAB CODE */
void mipslabmain(void){
    /* Set up peripheral bus clock */
    /* OSCCONbits.PBDIV = 1; */
    OSCCONCLR = 0x100000; /* clear PBDIV bit 1 */
	OSCCONSET = 0x080000; /* set PBDIV bit 0 */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
    SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;
	
	display_init();

	return;
}
/*	LAB CODE END */

int getbtns(void){
    return (PORTD>>5)&(0x7);
}
int getspecificbtn(int index){
	int i = 0;

	i = (getbtns()>>index)&(0x1);

	return i;
}


void init(void){
	mipslabmain();

	/* buttons init */
	/* switches init */
	TRISDSET = (0xFE0);
	PORTDCLR = (0xFE0);


  	/*init timer2 and interrupts*/

  	T2CON = 0x0;                /*clear config*/
  	TMR2 = 0x0;                 /*clear timer*/
  	PR2 =  30472;          

  	IFSCLR(0) = 0x0100;         /*clear interrupt flag*/
  	IECCLR(0) = 0x0100;         /*clear enable interrupts*/
  	IPCCLR(2) = 0x001f;         /*clear timer2 priority field*/ 
  	IPCSET(2) = 0x0010;         /*set priority to 4*/
  	IPCSET(2) = 0x0000;         /*set subpriority to 0*/
  	IECSET(0) = 0x0100;         /*enable timer2 interrupts*/

  	T2CONSET = 0x8070;          /*start timer and enable prescaling 1:256*/

	return;
}
void timeout(int t){	/*pauses main loop*/
    int timeout = 0;

    while (timeout<t){
       if (((IFS(0)>>8)&0x1)==1) {
            IFSCLR(0) = 0x0100;
            timeout++;
        }
    }

    return;
}
void polling(void){
    int i = 0;
    int btn = -1;
    int locked = 0;


    while (enable_polling==1){

        if (locked==0){
            for (i=0;i<3;i++){
                if (getspecificbtn(i)==1){ /*locks selection in*/
                    PORTDCLR = (0xFE0);
                    locked = 1;	
                    btn = i;
                }
            }
        }

        if (((IFS(0)>>8)&0x1)==1) {
            IFSCLR(0) = 0x0100;
            
            if (locked==1){

                if (btn>-1){
                    btn_press(btn);
                }

                timeout(2);
                btn = -1;
                locked = 0;
            }
        }
    }
    return;
}


int main(void){
	init();
    init_nav_arr();

    while (1) {
        polling();
        enable_polling = 1;
    }
    
    return 0;
}