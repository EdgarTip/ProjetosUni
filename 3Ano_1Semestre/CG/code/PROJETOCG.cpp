/* ===================================================================================
	Departamento Eng. Informatica - FCTUC
	Computacao Grafica - 2021/22
	................................................ Edgar Filipe Ferreira Duarte, 2019216077, edgarduarte@student.dei.uc.pt
	Projeto- Construção de um comando
======================================================================================= */


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL\glut.h>

#include "RgbImage.h"

//--------------------------------- Definir cores
#define scalex 24
#define scaley 24
#define BLUE     0.0, 0.0, 1.0, 1.0
#define RED		 1.0, 0.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define PI		 3.14159

//Variables

float centrox = 0;
float rotacao = 0;
float a = 0;
int right = 1;
GLfloat tam = 0.5;
GLint  msec = 10;
GLUquadric* qobj;
//Size of the game plane
float fGlass_size = 8;
float fGlass_height = 4;


//Joycon animation variables
float joycon_animation = 0;
int on_console = 1;
float animation_step = 0.05f;

//Rotation of the joycons variables
float rotation_angle = 0;
float rotation_step = 5;
int rotation_animation = 0;

//Pacman
float pacman_size = 1;
int pac_man_timer = 20 * msec;
int time_passed_pacman = 0;
GLint pacman_form = 0;

//Time a button is down after pressed
int button_pressed_time = 10 * msec;
int is_pressed_q = 0, is_pressed_x = 0, is_pressed_e = 0, is_pressed_z = 0;
int time_passed_q = 0, time_passed_x = 0, time_passed_e = 0, time_passed_z = 0;

int is_pressed_7 = 0, is_pressed_9 = 0, is_pressed_1 = 0, is_pressed_3 = 0;
int time_passed_7 = 0, time_passed_9 = 0, time_passed_1 = 0, time_passed_3 = 0;

//Sticks Rotation
int stick_pressed_time = 12 * msec;
//Controlled by arrows
int right_stick_rotate = 0, right_stick_x_rotation = 0, right_stick_y_rotation = 0;
int time_passed_right_stick = 0;
//Controlled by wasd
int left_stick_rotate = 0, left_stick_x_rotation = 0, left_stick_y_rotation = 0;
int time_passed_left_stick = 0;


//ANIMATION ON G
int time_passed_animation = 0, animation = 0, fase = 0, wacky_1 = 0;
float initial_value_f1 = 0, initial_value_f2 = 0;
int first_pass_third = 1, first_pass_two_thirds = 1;


int malha = 1;
int transp = 0;
int coef = 0.1 * 128;

static GLfloat vertices[] = {
	//…………………………………………………………………………………………………… x=tam (Frente)
		-tam,  -tam,  tam,	// 0 
		 tam,  -tam,  tam,	// 1 
		 tam,   tam,  tam,	// 2 
		-tam,   tam,  tam,	// 3 
//…………………………………………………… Direita
		 tam,  -tam,  tam,	// 4 
		 tam,  -tam,  -tam,	// 5 
		 tam,  tam,   -tam,	// 6 
		 tam,  tam,   tam,	// 7 
//……………………………………………………… (Cima
		-tam,  tam,  tam,	// 8 
		 tam,  tam,  tam,	// 9 
		 tam,  tam, -tam,	// 10 
		-tam,  tam, -tam,	// 11 
//……………………………………………………… (Tras)
		 tam,  tam, -tam,   // 12
		 tam,  -tam,-tam,   // 13
		-tam,  -tam,-tam,   // 14
		-tam, tam, -tam,     // 15

//……………………………………………………… (Baixo)
		-tam, -tam, tam,	//16
		-tam, -tam, -tam,	//17
		tam, -tam, -tam,	//18
		tam, -tam, tam,		//19

//……………………………………………………… (Esquerda)
		-tam, tam, tam,     //20
		-tam, tam, -tam,    //21
		-tam, -tam, -tam,   //22
		-tam, -tam, tam,    //23

//……………………………………………………… (Front Glass)
		-fGlass_size / 2, -fGlass_height / 2, tam + 0.01,
		fGlass_size / 2, -fGlass_height / 2, tam + 0.01,
		fGlass_size / 2, fGlass_height / 2, tam + 0.01,
		-fGlass_size / 2, fGlass_height / 2, tam + 0.01,
};

static GLfloat normais[] = {
	//…………………………………………………………………………………………………… x=tam (Frente)
	  0.0,  0.0,  1.0,
	  0.0,  0.0,  1.0,
	  0.0,  0.0,  1.0,
	  0.0,  0.0,  1.0,
	  //…………………………………………………………………………………………………… x=tam (Direita)
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		//…………………………………………………………………………………………………… y=tam (Cima)
	   0.0,  1.0,  0.0,
	   0.0,  1.0,  0.0,
	   0.0,  1.0,  0.0,
	   0.0,  1.0,  0.0,

	   //…………………………………………………………………………………………………… z=tam (Tras)
	   0.0, 0.0, -1.0,
	   0.0, 0.0, -1.0,
	   0.0, 0.0, -1.0,
	   0.0, 0.0, -1.0,

	   //…………………………………………………………………………………………………… y=tam (Baixo)
	   0.0,  -1.0,  0.0,
	   0.0,  -1.0,  0.0,
	   0.0,  -1.0,  0.0,
	   0.0,  -1.0,  0.0,

	   //…………………………………………………………………………………………………… x=tam (Esquerda)
	 -1.0,  0.0,  0.0,
	 -1.0,  0.0,  0.0,
	 -1.0,  0.0,  0.0,
	 -1.0,  0.0,  0.0,


};
static GLuint   frente[] = { 0, 1,  2,  3 };
static GLuint  direita[] = { 4, 5,  6,  7 };
static GLuint     cima[] = { 8, 9, 10, 11 };
static GLuint     tras[] = { 12, 13, 14, 15 };
static GLuint    baixo[] = { 16, 17, 18, 19 };
static GLuint esquerda[] = { 20, 21, 22, 23 };
static GLuint fGlass[] = { 24, 25, 26, 27 };

//------------------------------------------------------------ Sistema Coordenadas + objectos
GLint		wScreen = 800, hScreen = 600;		//.. janela
GLfloat		xC = 10.0, yC = 10.0, zC = 10.0;	//.. Mundo
GLboolean   frenteVisivel = 1;

//------------------------------------------------------------ Observer 
GLfloat  rVisao = 15, aVisao = 0.5 * PI, incVisao = 0.05, aVisao2 = 0;
GLfloat  obsP[] = { rVisao * cos(aVisao), 3.0, rVisao * sin(aVisao) };
GLfloat  angZoom = 45;
GLfloat  incZoom = 3;


static GLfloat texturas[] = {
0, 0,
1, 0,
1, 1,
0, 1,
0, 0,
1, 0,
1, 1,
0, 1,
0, 0,
1, 0,
1, 1,
0, 1 };
RgbImage imag;
GLuint   texture[6];
GLuint texture2[1];

void initMaterials(int material);
char Materiais[18][30] = {
	"Esmerald",  "Jade",  "obsidian",    "Pearl",        "Ruby",
	"Turquoise", "Brass", "Bronze",      "Chrome",       "Copper",
	"Gold",      "Silver","blackPlastic","cyankPlastic", "greenPlastic",
	"redPlastic", "whitePlastic","yellowPlastic" };

GLint   Dia = 1;     //:::   'D'  
GLfloat intensidadeDia = 0.3;
GLfloat luzGlobalCorAmb[4] = { intensidadeDia, intensidadeDia,intensidadeDia, 0 };   // 

//---------------------------------------------------- Luz pontual no TETO (eixo Y)
GLint   ligaTeto = 1;		 //:::   'T'  
GLfloat intensidadeT = 0.1;  //:::   'I'  
GLint   luzR = 1;		 	 //:::   'R'  
GLint   luzG = 1;			 //:::   'G'  
GLint   luzB = 1;			 //:::   'B'  
GLint x = 0;
GLint y = 0;
GLint z = 0;
GLfloat localPos[4] = { 0, 1, 2.0, 1.0 };
GLfloat localCorAmb[4] = { 0, 0, 0, 1.0 };
GLfloat localCorDif[4] = { luzR, luzG, luzB, 1.0 };
GLfloat localCorEsp[4] = { luzR, luzG, luzB, 1.0 };

GLfloat Pos1[] = { -2.0f, 0.0f,  1.5f, 1.0f };   // Foco 1
GLfloat Pos2[] = { 2.0f, 0.0f,  1.5f, 1.0f };   // Foco 2 

//………………………………………………………………………………………………………………………………………………………………………………………… A variar no programa
bool 		Focos[] = { 1,1 };		//.. Dois Focos ligados ou desligados
GLfloat		aberturaFoco = 10.0;		//.. angulo inicial do foco
GLfloat		anguloINC = 3.0;		//.. incremento
GLfloat		anguloMIN = 3.0;		//.. minimo
GLfloat		anguloMAX = 70.0;		//.. maximo

//…………………………………………………………………………………………………………………………………
GLint	  dim = 64;   //numero divisoes da grelha



void initTexturas()
{


	

	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	imag.LoadBmpFile("pewd.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());


	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	imag.LoadBmpFile("images.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	imag.LoadBmpFile("colorida.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	glGenTextures(1, &texture[3]);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	imag.LoadBmpFile("sky.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());


	//……………………………………………………………………………………………………………………………  Textura
	glGenTextures(1, &texture[4]);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	imag.LoadBmpFile("zelda.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	

}

//…………………………………………………………………………………………………………………………………………… LUZES
void initLights(void) {
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);

	glLightfv(GL_LIGHT0, GL_POSITION, localPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);
	glDisable(GL_LIGHT0);


}

void iluminacao() {




	glLightfv(GL_LIGHT0, GL_POSITION, localPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);
	glEnable(GL_LIGHT0);


}


void updateLuz() {

	localPos[0] = x;
	localCorDif[0] = luzR * intensidadeT;
	localCorDif[1] = luzG * intensidadeT;
	localCorDif[2] = luzB * intensidadeT;
	localCorEsp[0] = luzR * intensidadeT;
	localCorEsp[1] = luzG * intensidadeT;
	localCorEsp[2] = luzB * intensidadeT;
	glLightfv(GL_LIGHT0, GL_POSITION, localPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);


}

void defineFocos() {
	GLfloat Foco_direccao[] = { 0, 0, -1, 0 };	//……… -Z
	GLfloat Foco1_cor[] = { 0, 1,  0, 1 };	//……… Cor da luz 1
	GLfloat Foco2_cor[] = { 1, 0,  0, 1 };	//……… Cor da luz 2
	GLfloat Foco_ak = 1.0;
	GLfloat Foco_al = 0.05f;
	GLfloat Foco_aq = 0.0f;
	GLfloat Foco_Expon = 2.0;		// Foco, SPOT_Exponent
	//……………………………………………………………………………………………………………………………Foco Esquerda
	glLightfv(GL_LIGHT1, GL_POSITION, Pos1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Foco1_cor);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, Foco_ak);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, Foco_al);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, Foco_aq);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, aberturaFoco);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, Foco_direccao);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, Foco_Expon);

	//……………………………………………………………………………………………………………………………Foco Direita
	glLightfv(GL_LIGHT2, GL_POSITION, Pos2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, Foco2_cor);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, Foco_ak);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, Foco_al);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, Foco_aq);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, aberturaFoco);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, Foco_direccao);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, Foco_Expon);
}


//================================================================================
//=========================================================================== INIT
void init(void)
{
	glClearColor(0.3, 0.5, 0.5, 1.0);		//………………………………………………………………………………Apagar

	glEnable(GL_DEPTH_TEST);	//………………………………………………………………………………Profundidade
	glShadeModel(GL_SMOOTH);	//………………………………………………………………………………Interpolacao de cores	
	initTexturas();
	glEnable(GL_TEXTURE_2D);
	




	qobj = gluNewQuadric();
	gluQuadricNormals(qobj, GLU_SMOOTH);

	glVertexPointer(3, GL_FLOAT, 0, vertices); //………………………………………Vertex arrays
	glNormalPointer(GL_FLOAT, 0, normais);


	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, texturas);   // coordenadas textura
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	
	initLights();
	defineFocos();
	initMaterials(3);


}

void drawTriangle() {


	glBegin(GL_TRIANGLES);


	glVertex3f(1, 1, 0);
	glVertex3f(-1, 1, 0);
	glVertex3f(0, 0, 0);
	glEnd();

}

void drawSquare(int text) {
	float L = 0.5;
	glEnable(GL_TEXTURE_2D);
	if (text == 1)glBindTexture(GL_TEXTURE_2D, texture[0]);
	else glBindTexture(GL_TEXTURE_2D, texture[3]);


	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);



	glEnd();
	glDisable(GL_TEXTURE_2D);
}


void drawChao() {
	float L = 1;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glPushMatrix();
	glTranslatef(0, -3, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);    glVertex3i(-L * xC, 0, L * xC); //A 
	glTexCoord2f(1.0f, 0.0f); 	 glVertex3i(L * xC, 0, L * xC);  //B
	glTexCoord2f(1.0f, 1.0f);    glVertex3i(L * xC, 0, -L * xC); //C
	glTexCoord2f(0.0f, 1.0f);  	 glVertex3i(-L * xC, 0, -L * xC); //D
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
//Código adaptado de: https://community.khronos.org/t/half-sphere/49408/3
void drawHalfSphere(GLfloat r) {
	int i, j;
	GLfloat v[scalex * scaley][3];

	for (i = 0; i < scalex; ++i) {
		for (j = 0; j < scaley; ++j) {
			v[i * scaley + j][0] = r * cos(j * 2 * PI / scaley) * cos(i * PI / (2 * scalex));
			v[i * scaley + j][1] = r * sin(i * PI / (2 * scalex));
			v[i * scaley + j][2] = r * sin(j * 2 * PI / scaley) * cos(i * PI / (2 * scalex));
		}
	}

	glBegin(GL_QUADS);
	for (i = 0; i < scalex - 1; ++i) {
		for (j = 0; j < scaley; ++j) {
			glVertex3fv(v[(i + 1) * scaley + j]);
			glVertex3fv(v[(i + 1) * scaley + (j + 1) % scaley]);
			glVertex3fv(v[i * scaley + (j + 1) % scaley]);
			glVertex3fv(v[i * scaley + j]);



		}
	}
	glEnd();
}


void drawCube(int with_text) {
	if (with_text == 1) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
	}
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
	glNormal3f(1, 0, 0);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
	if (with_text)glDisable(GL_TEXTURE_2D);
}

void drawEixos()
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo
	initMaterials(16);   // whitw
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(10, 0, 0);
	glEnd();
	initMaterials(3);   // whitw
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 10, 0);
	glEnd();
	initMaterials(6);   // whitw
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 10);
	glEnd();

}

void DesenhaCirculo()
{
	glBegin(GL_POLYGON);                        // Middle circle
	double radius = 0.4;
	double ori_x = 0.0;                         // the origin or center of circle
	double ori_y = 0.0;
	for (int i = 0; i <= 300; i++) {
		double angle = 2 * PI * i / 300;
		double x = cos(angle) * radius;
		double y = sin(angle) * radius;
		glVertex2d(ori_x + x, ori_y + y);
	}
	glEnd();
}

void drawCilinder(int sides, float radius, float height) {


	float step = 360 / sides;


	for (int i = 0; i < sides; i++) {
		float angle = 2.0 * i * PI / sides;
		float x = radius * cos(angle);
		float z = radius * sin(angle);
		float y = height / 2;

		float angle2 = 2.0 * PI * (i + 1) / sides;
		float x_2 = radius * cos(angle2);
		float z_2 = radius * sin(angle2);

		glNormal3f(0, 0, 1);
		glBegin(GL_QUADS);
		glVertex3f(x_2, -y, z_2);
		glVertex3f(x, -y, z);
		glVertex3f(x, y, z);
		glVertex3f(x_2, y, z_2);
		glEnd();

	}




	//Bottom
	glNormal3f(0, -1, 0);
	glBegin(GL_POLYGON);
	
	for (int j = 0; j < sides; j++) {
		float angle = 2.0 * j * PI / sides;
		float angle2 = (j + 1) * 2.0 * PI / sides;

		glVertex3f(radius * cos(angle), -height / 2, radius * sin(angle));
	}
	glEnd();

	//Top
	glNormal3f(0, 1, 0);
	glBegin(GL_POLYGON);
	
	for (int j = sides; j > 0; j--) {
		float angle = 2.0 * j * PI / sides;
		float angle2 = (j + 1) * 2.0 * PI / sides;

		glVertex3f(radius * cos(angle), height / 2, radius * sin(angle));
	}
	glEnd();



}


//Draws the scene
void drawScene() {



	//PacMan
	/*
	glPushMatrix();
	glTranslatef(-3, 0, 0.52);

	//Rotation so that pacman open and close animation looks good (or else only bottom mouth moves)
	if (!pacman_form) glRotatef(-10, 0, 0, 1);

	glScalef(0.3, 0.3, 0.3);

	//Max mouth size
	float mouthSize = 140;

	//Amount of drawn triangles (degrees of the cirlce that will be filled in
	int amount = 360 - mouthSize - 40 * pacman_form;
	for (int i = 0; i < amount; i++) {
		glRotatef(1, 0, 0, 1);
		glColor3f(1.0, 0.5, 0.0);
		drawTriangle();
	}*/


	glPopMatrix();

	//drawChao();
	//Yellow Squares (pacman food)
	/*
	initMaterials(3);
	glPushMatrix();
	glTranslated(-2, 0, 0.52);
	glScalef(0.3, 0.3, 0.3);
	drawSquare(2);
	glPopMatrix();

	glPushMatrix();

	glTranslated(0, 0, 0.52);
	glScalef(0.3, 0.3, 0.3);
	initMaterials(4);
	drawSquare(1);
	glPopMatrix();
	initMaterials(2);
	glPushMatrix();

	glTranslated(2, 0, 0.52);
	glScalef(0.3, 0.3, 0.3);
	drawSquare(4);
	glPopMatrix();
	*/

	//=================================================================
	//Right joycon (Red one)

	defineFocos();
	//Right Side
	glPushMatrix();

	initMaterials(7);
	glTranslatef(6, 0, 0);
	glTranslatef(0, joycon_animation * 5, 0);
	glRotatef(-rotation_angle, 0, 0, 1);


	glPushMatrix();
	glScalef(2, 5, 1);
	drawCube(1);
	glPopMatrix();

	//Right Stick
	glPushMatrix();

	if (transp) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_COLOR);
	}
	initMaterials(14);
	glTranslatef(0, -1, 0.8);

	if (right_stick_rotate) {
		glRotatef(30, right_stick_x_rotation, right_stick_y_rotation, 0);
	}
	glRotatef(90, 1, 0, 0);
	drawHalfSphere(0.5);

	glPushMatrix();
	initMaterials(16);
	glScalef(0.3, 0.9, 0.3);
	drawCilinder(18, 0.6, 1);
	glPopMatrix();
	glPopMatrix();

	if (transp) glDisable(GL_BLEND);

	//Right Side Buttons
	//Green
	glPushMatrix();
	initMaterials(15);
	glTranslatef(0, 1.5, 0.5 - is_pressed_7 * 0.2);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(90, 1, 0, 0);
	drawCilinder(18, 0.6, 1);

	glPopMatrix();

	//White
	glPushMatrix();
	initMaterials(15);
	glTranslatef(0, 0.5, 0.5 - is_pressed_3 * 0.2);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(90, 1, 0, 0);
	drawCilinder(18, 0.6, 1);
	glPopMatrix();

	//Cyan
	glPushMatrix();
	initMaterials(14);
	glTranslatef(-0.5, 1, 0.5 - is_pressed_1 * 0.2);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(90, 1, 0, 0);
	drawCilinder(18, 0.6, 1);
	glPopMatrix();

	//Yellow
	glPushMatrix();
	initMaterials(14);
	glTranslatef(0.5, 1, 0.5 - is_pressed_9 * 0.2);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(90, 1, 0, 0);
	drawCilinder(18, 0.6, 1);
	glPopMatrix();

	glPopMatrix();

	//=================================================================
	//Left joycon (Blue one)		

		//Left Side

	glPushMatrix();
	initMaterials(6);
	glTranslatef(0, joycon_animation * 5, 0);
	glTranslatef(-6, 0, 0);
	glRotatef(rotation_angle, 0, 0, 1);

	glPushMatrix();
	glScalef(2, 5, 1);
	drawCube(1);
	glPopMatrix();

	//Left Stick
	if (transp) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	glPushMatrix();
	initMaterials(9);
	glTranslatef(0, 1, 0.8);

	if (left_stick_rotate) {
		glRotatef(30, left_stick_x_rotation, left_stick_y_rotation, 0);
	}
	glRotatef(90, 1, 0, 0);
	drawHalfSphere(0.5);

	glPushMatrix();
	initMaterials(16);
	glScalef(0.3, 0.9, 0.3);
	drawCilinder(18, 0.6, 1);
	glPopMatrix();
	glPopMatrix();

	if(transp) glDisable(GL_BLEND);

	//Left Side Buttons
	//Green
	glPushMatrix();
	initMaterials(13);
	glTranslatef(0, -1.5, 0.5 - is_pressed_z * 0.2);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(-90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	drawCilinder(3, 0.6, 1);
	glPopMatrix();

	//White
	glPushMatrix();
	initMaterials(13);
	glTranslatef(0, -0.5, 0.5 - is_pressed_e * 0.2);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	drawCilinder(3, 0.6, 1);
	glPopMatrix();

	//Cyan
	glPushMatrix();
	initMaterials(17);
	glTranslatef(0.5, -1, 0.5 - is_pressed_x * 0.2);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(90, 1, 0, 0);
	drawCilinder(3, 0.6, 1);
	glPopMatrix();

	//Yellow
	glPushMatrix();
	initMaterials(17);
	glTranslatef(-0.5, -1, 0.5 - is_pressed_q * 0.2);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(180, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	drawCilinder(3, 0.6, 1);
	glPopMatrix();

	glPopMatrix();






	glPopMatrix();

	//Front view
	glPushMatrix();
	initMaterials(2);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, fGlass);
	glPopMatrix();



	//Console
	glPushMatrix();
	GLfloat  jadeAmb[] = { 0.135 ,0.2225 ,0.1575 };
	GLfloat  jadeDif[] = { 0.54 ,0.89 ,0.63 };
	GLfloat  jadeSpec[] = { 0.316228 ,0.316228 ,0.316228 };
	GLint   jadeCoef = coef; 

	

	glMaterialfv(GL_FRONT, GL_AMBIENT, jadeAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, jadeDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, jadeSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, jadeCoef);
	
	glScalef(10, 5, 1);
	drawCube(0);
	glPopMatrix();


	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);
	//----------------------------------------------- Textura - caracol
	glBindTexture(GL_TEXTURE_2D, texture[4]);

	//----------------------------------------------- Dsenha malha poligonos
	if (malha) {
		glPushMatrix();
		glTranslatef(-4.0, -2.0, 0);  // meio do poligono 



		glNormal3f(0, 0, 1);          //normal 

		int				i, j;
		float			med_dim = (float)dim / 2;
		glBegin(GL_QUADS);
		for (i = 0; i < 2 * dim; i++)
			for (j = 0; j < 4 * dim; j++) {
				glTexCoord2f((float)j / (4 * dim), (float)i / (2 * dim));
				glVertex3d((float)j / med_dim, (float)i / med_dim, 0.52);
				glTexCoord2f((float)(j + 1) / (4 * dim), (float)i / (2 * dim));
				glVertex3d((float)(j + 1) / med_dim, (float)i / med_dim, 0.52);
				glTexCoord2f((float)(j + 1) / (4 * dim), (float)(i + 1) / (2 * dim));
				glVertex3d((float)(j + 1) / med_dim, (float)(i + 1) / med_dim, 0.52);
				glTexCoord2f((float)j / (4 * dim), (float)(i + 1) / (2 * dim));
				glVertex3d((float)j / med_dim, (float)(i + 1) / med_dim, 0.52);
			}
		glEnd();
		glPopMatrix();
	}


}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_LIGHTING);
	glViewport(0, 0, wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angZoom, (float)wScreen / hScreen, 0.1, 3.0 * zC);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	gluLookAt(obsP[0], obsP[1], obsP[2], 0, 0, 0, 0, 1, 0);

	iluminacao();
	drawEixos();
	drawScene();


	glutSwapBuffers();
}

//Updates the lookat angle
void disp() {

	if (!animation) {
		obsP[0] = rVisao * cos(aVisao);
		obsP[2] = rVisao * sin(aVisao);
	}

	else {
		obsP[0] = rVisao * cos(aVisao);
		obsP[1] = rVisao * sin(aVisao2);
		obsP[2] = rVisao * sin(aVisao);
		if (wacky_1) obsP[2] = rVisao * (0.8 * sin(aVisao) + cos(aVisao2));


	}

	glutPostRedisplay();
}

//Variables that change with time, to create the ilusion of an animation
void Timer(int value)
{
	//Pacman animations
	if (time_passed_pacman < pac_man_timer) {
		time_passed_pacman += msec;
	}
	else {
		if (pacman_form) pacman_form = 0;
		else pacman_form = 1;
		time_passed_pacman = 0;
	}


	//See if left side buttons are being pressed
	if (is_pressed_q && time_passed_q < button_pressed_time) {
		time_passed_q += msec;
	}
	else if (is_pressed_q) {
		is_pressed_q = 0;
	}

	if (is_pressed_x && time_passed_x < button_pressed_time) {
		time_passed_x += msec;
	}
	else if (is_pressed_x) {
		is_pressed_x = 0;
	}

	if (is_pressed_e && time_passed_e < button_pressed_time) {
		time_passed_e += msec;
	}
	else if (is_pressed_e) {
		is_pressed_e = 0;
	}

	if (is_pressed_z && time_passed_z < button_pressed_time) {
		time_passed_z += msec;
	}
	else if (is_pressed_z) {
		is_pressed_z = 0;
	}

	//See if right side buttons are being pressed
	if (is_pressed_1 && time_passed_1 < button_pressed_time) {
		time_passed_1 += msec;
	}
	else if (is_pressed_1) {
		is_pressed_1 = 0;
	}

	if (is_pressed_3 && time_passed_3 < button_pressed_time) {
		time_passed_3 += msec;
	}
	else if (is_pressed_3) {
		is_pressed_3 = 0;
	}

	if (is_pressed_7 && time_passed_7 < button_pressed_time) {
		time_passed_7 += msec;
	}
	else if (is_pressed_7) {
		is_pressed_7 = 0;
	}

	if (is_pressed_9 && time_passed_9 < button_pressed_time) {
		time_passed_9 += msec;
	}
	else if (is_pressed_9) {
		is_pressed_9 = 0;
	}

	//See if right side stick is being pressed
	if (right_stick_rotate && time_passed_right_stick < stick_pressed_time) {
		time_passed_right_stick += msec;
	}
	else if (right_stick_rotate) {
		right_stick_rotate = 0;
	}

	//See if left side stick is being pressed
	if (left_stick_rotate && time_passed_left_stick < stick_pressed_time) {
		time_passed_left_stick += msec;
	}
	else if (left_stick_rotate) {
		left_stick_rotate = 0;
	}

	//See if controllers need to be taken out of the console
	if (on_console && joycon_animation > 0) {
		joycon_animation -= animation_step;

		if (joycon_animation < 0) joycon_animation = 0;
	}
	else if (!on_console && joycon_animation < 1) {
		joycon_animation += animation_step;

		if (joycon_animation > 1) joycon_animation = 1;

	}

	//See if the controllers are being rotated
	if (!on_console && rotation_animation) {
		rotation_angle += rotation_step;
	}

	if (!rotation_animation) rotation_angle = 0;

	//Lookat animation
	if (animation) {
		if (fase == 1) {
			if (initial_value_f1 == 0) {
				initial_value_f1 = aVisao;
			}
			wacky_1 = 1;
			aVisao = (aVisao - 0.03);

			//First circle, around the right controller (1/3 of the way)
			if (initial_value_f1 - aVisao >= (0.6666 * PI) && first_pass_third) {
				aVisao = initial_value_f1 - 0.6666 * PI;
				first_pass_third = 0;
				fase = 2;
			}

			//Second circle, around the left controller (1/3 of the way)
			if (initial_value_f1 - aVisao >= 1.33333333 * PI && first_pass_two_thirds) {
				aVisao = initial_value_f1 - 1.3333333 * PI;
				first_pass_two_thirds = 0;
				fase = 2;
			}

			//end (3/3 of the way)
			if (initial_value_f1 - aVisao >= 2 * PI) {
				aVisao = initial_value_f1 + 2 * PI;
				first_pass_third = 1;
				first_pass_two_thirds = 1;
				initial_value_f1 = 0;
				wacky_1 = 0;
				rVisao = 15;
				aVisao = 0.5 * PI;
				obsP[0] = rVisao * cos(0.5 * PI);
				obsP[1] = 3.0;
				obsP[2] = rVisao * sin(0.5 * PI);
				fase = 0;
				animation = 0;
			}
			disp();
		}
		//Circles around controllers
		if (fase == 2) {

			if (initial_value_f2 == 0) {
				initial_value_f2 = aVisao2;
			}

			if (first_pass_third) aVisao2 = aVisao2 + 0.03;
			else aVisao2 = aVisao2 - 0.03;


			if (initial_value_f2 - aVisao2 >= 2 * PI || initial_value_f2 - aVisao2 <= -2 * PI) {
				aVisao2 = initial_value_f2 + 2 * PI;
				initial_value_f2 = 0;
				fase = 1;
			}
			disp();

		}

	}

	glutPostRedisplay();
	glutTimerFunc(msec, Timer, 1);
}




//Events
void keyboard(unsigned char key, int x, int y) {


	switch (key) {

		//Take the controllers out of the console
	case 'b':
	case 'B':
		if (on_console) {
			on_console = 0;
			rotation_animation = 0;
		}
		else {
			on_console = 1;
			rotation_animation = 0;
		}

		break;

		//Rotate the controllers (when out of the console)
	case 'R':
	case 'r':
		if (rotation_animation) rotation_animation = 0;
		else rotation_animation = 1;
		glutPostRedisplay();
		break;

		//Left side buttons
	case 'Q':
	case 'q':
		is_pressed_q = 1;
		time_passed_q = 0;
		glutPostRedisplay();
		break;

	case 'X':
	case 'x':
		is_pressed_x = 1;
		time_passed_x = 0;
		glutPostRedisplay();
		break;


	case 'e':
	case 'E':
		is_pressed_e = 1;
		time_passed_e = 0;
		glutPostRedisplay();
		break;

	case 'z':
	case 'Z':
		is_pressed_z = 1;
		time_passed_z = 0;
		glutPostRedisplay();
		break;

		//Left side stick 
	case 'a':
	case 'A':
		left_stick_rotate = 1;
		left_stick_x_rotation = 0;
		left_stick_y_rotation = -1;
		time_passed_left_stick = 0;
		break;

	case 'w':
	case 'W':
		left_stick_rotate = 1;
		left_stick_x_rotation = -1;
		left_stick_y_rotation = 0;
		time_passed_left_stick = 0;
		break;

	case 's':
	case 'S':
		left_stick_rotate = 1;
		left_stick_x_rotation = 1;
		left_stick_y_rotation = 0;
		time_passed_left_stick = 0;
		break;

	case 'd':
	case 'D':
		glutPostRedisplay();
		left_stick_rotate = 1;
		left_stick_x_rotation = 0;
		left_stick_y_rotation = 1;
		time_passed_left_stick = 0;
		break;

		//Move away camera
	case 't':
	case 'T':
		rVisao = rVisao++;
		obsP[0] = rVisao * cos(aVisao);
		obsP[2] = rVisao * sin(aVisao);
		glutPostRedisplay();
		break;

		//Get closer camera
	case 'y':
	case 'Y':

		rVisao = rVisao--;
		obsP[0] = rVisao * cos(aVisao);
		obsP[2] = rVisao * sin(aVisao);
		glutPostRedisplay();
		break;

		//Starts sick animation :)
	case 'g':
	case 'G':
		if (!animation) {
			rVisao = 15;
			aVisao = 0.5 * PI;
			aVisao2 = 0;
			obsP[0] = rVisao * cos(0.5 * PI);
			obsP[1] = 3.0;
			obsP[2] = rVisao * sin(0.5 * PI);


			animation = 1;
			fase = 1;
		}
		break;
		//Reset camera button
	case 'h':
	case 'H':
		first_pass_third = 1;
		first_pass_two_thirds = 1;
		initial_value_f1 = 0;
		initial_value_f2 = 0;
		animation = 0;
		wacky_1 = 0;
		rVisao = 15;
		aVisao = 0.5 * PI;
		obsP[0] = rVisao * cos(0.5 * PI);
		obsP[1] = 3.0;
		obsP[2] = rVisao * sin(0.5 * PI);

		break;

		//Move camera to the left
	case 'j':
	case 'J':
		if (!animation) {
			aVisao = (aVisao + 0.05);
			disp();
		}
		break;

		//Move camera to the right
	case 'l':
	case 'L':
		if (!animation) {
			aVisao = (aVisao - 0.05);
			disp();
		}
		break;

	case 'I':
	case 'i':
		intensidadeT = intensidadeT + 0.1;
		if (intensidadeT > 4.1) intensidadeT = 0.1;
		updateLuz();
		glutPostRedisplay();
		break;


		//Right Side buttons
	case '1':
		is_pressed_1 = 1;
		time_passed_1 = 0;
		glutPostRedisplay();
		break;

	case '3':
		is_pressed_3 = 1;
		time_passed_3 = 0;
		glutPostRedisplay();
		break;

	case '7':
		is_pressed_7 = 1;
		time_passed_7 = 0;
		glutPostRedisplay();
		break;

	case '9':
		is_pressed_9 = 1;
		time_passed_9 = 0;
		glutPostRedisplay();
		break;

	case '2':
		aberturaFoco += 2;
		if (aberturaFoco >100) {
			aberturaFoco = 5;
		}
		defineFocos();

		break;

	case '4':
		if (malha) malha = 0;
		else malha = 1;

		glutPostRedisplay();
		break;

	case '5':
		if (transp) transp = 0;
		else transp = 1;

		glutPostRedisplay();
		break;

	case '6':
		if (coef < 0.1 * 128 ) coef += 0.01 * 128;
		else coef = 0.01 * 128;

		glutPostRedisplay();
		break;


	case 'v':
	case 'V':
		luzR = 0;
		luzG = 1;
		luzB = 0;
		updateLuz();
		break;

	case 'c':
	case 'C':
		luzR = 0;
		luzG = 0;
		luzB = 1;
		updateLuz();
		break;

	case 'p':
	case 'P':
		luzR = 1;
		luzG = 0;
		luzB = 0;
		updateLuz();
		break;

	case 'u':
	case 'U':
		luzR = 1;
		luzG = 1;
		luzB = 1;
		updateLuz();
		break;


		//Escape
	case 27:
		exit(0);
		break;
	}

}


//Right stick controls
void teclasNotAscii(int key, int x, int y) {

	if (key == GLUT_KEY_UP) {
		right_stick_rotate = 1;
		right_stick_x_rotation = -1;
		right_stick_y_rotation = 0;
		time_passed_right_stick = 0;
	}

	if (key == GLUT_KEY_DOWN) {
		right_stick_rotate = 1;
		right_stick_x_rotation = 1;
		right_stick_y_rotation = 0;
		time_passed_right_stick = 0;
	}


	if (key == GLUT_KEY_LEFT) {
		right_stick_rotate = 1;
		right_stick_x_rotation = 0;
		right_stick_y_rotation = -1;
		time_passed_right_stick = 0;
	}


	if (key == GLUT_KEY_RIGHT) {
		right_stick_rotate = 1;
		right_stick_x_rotation = 0;
		right_stick_y_rotation = 1;
		time_passed_right_stick = 0;

	}



}


//Main
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(300, 100);
	glutCreateWindow("Edgar Filipe Ferreira Duarte| |Nintendo switch Imitation| |Animação- G| |Tirar joycons- B| |Rodar Joycons (fora) - R| |Butões lado direito-'QEZX'|  |Butões lado esquerdo-'RTFG'|  |Stick esquerdo-'WASD'| |Stick direito-'Setas'|     |Observador:'JL'| ");

	init();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}



