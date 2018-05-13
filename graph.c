#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"  /* Declrations of project functions and variables */

int pow(int a, int b){
  int i = 0;
  int c = 1;

  for (i=0;i<b;i++){
    c *= a;
  }
  return c;
}

/*fills graph array*/

void gen_lin_graph(const int a, const int b){
  int i = 0;

  for (i=0;i<128;i++){
    graph_array[i] = a*((i-64))+b;
  }
  return;
}

void gen_exp_graph(const int a, const int b){
  int i = 0;

  for (i=0;i<128;i++){
    graph_array[i] = a*(pow(b,((i-64))));
  }
  return;
}

void gen_pow_graph(const int a, const int b){
  int i = 0;

 for (i=0;i<128;i++){
    graph_array[i] = (pow((i-64),a))+b;
  }
  return;
}

void gen_quad_graph(const int a, const int b, const int c){
  int i = 0;

  for (i=0;i<128;i++){
    graph_array[i] = a*2+b*((i-64))+c;
  }
  return;
}

uint8_t getpixels(int y){   /*get which pixels should be drawn*/
  int i = 0;
  uint8_t count = 1;
  uint8_t pixels = 255;

  for (i=0;i<8;i++){
    if (y==(7-i)){
      pixels = pixels - count;
    }
    count = count + count;
  }
  
  return pixels;
}

void draw_graph(){
  int i = 0;
  int j = 0;

  for (i=0;i<4;i++){
    int k = 0;

    for (k=0;k<32;k++){
      int y = (graph_array[j]*10)/zoom;
      int quadrant = 0;
      int m = 0;
      int n = 8;
  
      if (y<0){

        if (y<-n){  /*places point in proper image array*/
          quadrant = 3;
          y += n*2;
        } else {
          quadrant = 2;
          y += n;
        }

      } else {

        if (y<n){
          quadrant = 1;
        } else {
          quadrant = 0;
          y -= n;
        }

      }

      for (m=0;m<4;m++){

        if (quadrant==m){
          image_array[i][k+m*32] &= getpixels(y);    /*draws x,y*/
        } else {
          image_array[i][k+m*32] &= 255;            /*clears rest*/
        }

      }

      j++;
    }
  }

  return;

}

void draw_origo(){
    int i = 0;

    for (i=0;i<4;i++){
        int k = 0;

        for (k=0;k<32;k++){
            int m = 0;

            for (m=0;m<4;m++) {

                if (i==2 && k==0){
                    image_array[i][m*32] = 0; /*draw x=0*/
                } else {

                    if (m==1){
                        image_array[i][k+m*32] = 127; /*draw y=0*/
                    } else {
                        image_array[i][k+m*32] = 255; /*clear the rest*/
                    }
                }
            }
        }
    }

    return;
}

void display_image_array(){
    int i = 0;

    for (i=0;i<4;i++){
        display_image(32*i, image_array[i]); /*displays array containing images*/
    }

    return;
}

void display_graph(){

    draw_origo();   /* draw axles unto image array*/

    switch (nav_save) {
        case 0 :
            gen_lin_graph(args[0],args[1]);
            break;
        case 1 :
            gen_exp_graph(args[0],args[1]);
            break;
        case 2 :
            gen_pow_graph(args[0],args[1]);
            break;
        case 3 :
            gen_quad_graph(args[0],args[1],args[2]);
            break;
    }

    draw_graph();

    display_image_array();

    return;
}