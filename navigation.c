#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"  /* Declrations of project functions and variables */

int toggle = 0;

int mode = 0;       /*Current screen*/
int nav_select = 0; /*Current menu item*/

char* navigation_array[6][4]; 	/*menus*/
int	nav_select_array[5];     /*menu selection bounds*/

int arg_amount = 0; /*amount of args to display*/

void toggle_nav_display(){
  int index = nav_select;
  if (mode>2){
    index++;
  }
  char* c = textbuffer[index];

  if (toggle==1){
    toggle = 0;
    c[0] = 0x20;
  } else {
    toggle = 1;
    c[0] = 0x23;
  }

  display_string(index,c);
  display_update();

  return;
}

void submit_query(){  /*submits results to graph/math functions */
  if (mode==3){
    mode = 7;
    display_math();
  } else {
    mode = 6;
    zoom = 10;
    display_graph();
  }
  return;
}
void display_arg(int index){  /*displays arguement in query */
  int i = 0;
  char c[16];
  char c2[4];
  int dmp = args[index];

  for (i=0;i<4;i++){
    c[i] = navigation_array[5][index][i];
  }

  num2char(c2,dmp);

  for (i=0;i<4;i++){
    c[i+4] = c2[i];
  }

  c[8] = 0;

  display_string(index+1,c);
  display_update();

  return;
}

void display_query(){ /*displays query for selected mode */
  nav_save = nav_select;
  arg_amount = 2;
  int i = 0;

  if (mode==4&&nav_select==3){
    arg_amount++;
  }

  display_string(0, navigation_array[mode][nav_select]);

  while (i<arg_amount){
    args[i] = 0;

    display_arg(i);
    i++;
  }

  if (i<3){
    char* c = " ";
    display_string(3,c);
  }
  display_update();
  
  return;
}

void display_mode_arr(){
  int i = 0;

  for (i=0;i<4;i++){
    display_string(i,navigation_array[mode][i]);  /*displays corresponding navigation menu*/
  }

  display_update();

  return;
}
void btn0(){
  if (mode<3){
    if (nav_select<nav_select_array[mode]){
      toggle_nav_display();

      nav_select++;

      toggle_nav_display();
    }
  } else if (mode<6) {
    args[nav_select] = args[nav_select] + 1;
    display_arg(nav_select);

    toggle = 0;
    toggle_nav_display();
  } else {
    if (mode==6){
      zoom++;
      display_graph();
    }
  }
  return;
}
void btn1(){
  if (mode<3){
    if (nav_select>0){
      toggle_nav_display();

      nav_select--;

      toggle_nav_display();
    }
  } else if (mode<6) {
    args[nav_select] = args[nav_select] - 1;
    display_arg(nav_select);

    toggle = 0;
    toggle_nav_display();
  } else {
    if (mode==6){
      if (zoom>1){
        zoom--;
        display_graph();
      }
    }
  }
  return;
}
void btn2(){
  switch (mode){
    case 0:
      mode = nav_select+1;
      display_mode_arr();

      nav_select = 0;
      toggle = 0;
      toggle_nav_display();

      break;
    case 1:
      mode = mode + 2;
      display_query();

      nav_select = 0;
      toggle = 0;
      toggle_nav_display();

      break;
    case 2:
      mode = mode + 2;
      display_query();

      nav_select = 0;
      toggle = 0;
      toggle_nav_display();

      break;
    case 3:
      if (nav_select<arg_amount-1){
        toggle_nav_display();

        nav_select++;

        toggle_nav_display();
      } else {
        submit_query();
      }
      break;
    case 4:
      if (nav_select<arg_amount-1){
        toggle_nav_display();

        nav_select++;

        toggle_nav_display();
      } else {
        submit_query();
      }
      break;
    case 6:
      mode = 0;
      display_mode_arr();

      nav_select = 0;
      break;
    case 7:
      mode = 0;
      display_mode_arr();

      nav_select = 0;
      break;
  }


  return;
}
void btn_press(int btn){
  if (btn==0){
    btn0();
  }
  if (btn==1){
    btn1();
  }
  if (btn==2){
    btn2();
  }

  return;
}
void init_nav_arr(void){
  mode = 0;

  /*main nav*/
  /*mode selection screen*/
  navigation_array[0][0] = " 1. Calc";
  navigation_array[0][1] = " 2. Graph"; 
  navigation_array[0][2] = " 3."; 
  navigation_array[0][3] = " 4."; 

  nav_select_array[0] = 1;

  /*calc selection screen*/
  navigation_array[1][0] = " 1. Add";
  navigation_array[1][1] = " 2. Sub"; 
  navigation_array[1][2] = " 3. Mult"; 
  navigation_array[1][3] = " 4. Div"; 

  nav_select_array[1] = 3;
  
  /*graph selection screen*/
  navigation_array[2][0] = " 1. Linear";
  navigation_array[2][1] = " 2. Exp"; 
  navigation_array[2][2] = " 3. Power"; 
  navigation_array[2][3] = " 4. Quad"; 

  nav_select_array[2] = 3;
  
  /*calc*/
  navigation_array[3][0] = "a+b";
  navigation_array[3][1] = "a-b"; 
  navigation_array[3][2] = "a*b"; 
  navigation_array[3][3] = "a/b"; 

  nav_select_array[3] = 2;

  /*graph*/
  navigation_array[4][0] = "y = a*x+b";
  navigation_array[4][1] = "y = a*b^x"; 
  navigation_array[4][2] = "y = x^a+b"; 
  navigation_array[4][3] = "y = a*2+b*x+c"; 

  nav_select_array[4] = 2;

  /*args*/
  navigation_array[5][0] = " a = ";
  navigation_array[5][1] = " b = "; 
  navigation_array[5][2] = " c = "; 

  display_mode_arr();
  toggle_nav_display();

  return;
}