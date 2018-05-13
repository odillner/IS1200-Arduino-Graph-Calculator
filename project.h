/* variables */

extern int enable_polling;

/* nav */

extern int args[3];        /*input values*/
extern int nav_save;       /*saves menu selection*/

/* graph */

extern int zoom;      

extern uint8_t image_array[4][128];    /*contains graph image*/
extern int graph_array[128];           /*graph data*/
/* storage */

extern char textbuffer[4][16];     

/* constants */

extern const uint8_t const font[128*8];

/* func */

/* input*/

int getbtns(void);
int getspecificbtn(int);

/* display */

void display_image(int x, const uint8_t *data);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);

/* nav */

void btn_press(int btn);
void init_nav_arr(void);    /* initialize menu items */

/* graph */

void display_graph(void);   

/* math */

void display_math(void);
void num2char(char* c, int i);  /* converts int into char */
