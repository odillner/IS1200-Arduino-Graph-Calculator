#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"  /* Declrations of project functions and variables */

void num2char(char* c, int num){
    int i = 0;
    
    for (i=0;i<4;i++){
        c[i]= 0x20;
    }
    if (num<0){
        c[0] = 0x2d;
        num = num * -1;
    }

    if (num>9){
        int dmp = 0;
        if (num>99){ 
            dmp = num/100;
            int dmp2 = (num-100*dmp)/10;
            int dmp3 = (num-100*dmp)-10*dmp2;

            c[1] = "0123456789"[dmp];
            c[2] = "0123456789"[dmp2];  
            c[3] = "0123456789"[dmp3];          
        } else {
            dmp = num/10;

            c[1] = "0123456789"[dmp];
            c[2] = "0123456789"[num-10*dmp];
        }
    } else {
        c[1] = "0123456789"[num];
    }
}
void display_math() {
    int i = 0;
    int dmp = 0;
    char c[16];
    char c2[4];

    num2char(c2,args[0]);

    for (i=0;i<4;i++){
        if (c2[i]) {
            c[i]=c2[i];
        } else {
            c[i]=0x20;
        }
    }

    switch (nav_save) {
        case 0 :
            c[4] = 0x2b;
            dmp = args[0]+args[1];
            break;
        case 1 :
            c[4] = 0x2d;      
            dmp = args[0]-args[1];    
            break;
        case 2 :
            c[4] = 0x2a;
            dmp = args[0]*args[1];         
            break;
        case 3 :
            c[4] = 0x2f;
            dmp = args[0]/args[1];
            break;
    }
    num2char(c2,args[1]);

    for (i=0;i<4;i++){
        if (c2[i]) {
            c[i+5]=c2[i];
        } else {
            c[i+5]=0x20;
        }
    }

    c[9] = 0x3d;

    num2char(c2,dmp);

    for (i=0;i<4;i++){
        c[10+i] = c2[i];
    }

    display_string(0,c);


    char* cdmp = " ";

    display_string(1,cdmp);
    display_string(2,cdmp);
    display_string(3,cdmp);
    display_update();
}