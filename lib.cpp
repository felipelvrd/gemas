#include "lib.h"

#include <cstdio>
#include <cstring>
using namespace std;

GLfloat anguloCuboX = 0.0f;
GLfloat anguloCuboY = 0.0f;
GLfloat anguloEsfera = 0.0f;

GLint ancho=400;
GLint alto=400;

int hazPerspectiva = 0;
char puntaje[10];
int mtz[8][8];
char miSegundo[2];
int mtzx;
int mtzy;
int xr=0;
char miMinuto[2];//no ponga los char seguidos porque tira el error de ayer
int yr=0;
int as=0;
char timpo[6];
float ESPAI=0.2;
bool hrztl=true;
char nombre[10];
int cont=0;
bool validar=false;
int cont2=0;
bool jugar=false;
bool correTiempo=false;
char timpop[6];

struct lpuntos{
	char nombre[11];
	char tiempo[7];
	char puntos[11];
} lista[5];

//------------------------------------------------------------------------------
void puntajeReset(){
	for (int i=0;i<10;i++)
		puntaje[i]='0';
}
//------------------------------------------------------------------------------
bool mayor(char p1[10], char p2[10]){
	for (int i=0;i<10;i++)
		if(p1[i]>p2[i])
			return true;
	return false;
}
//------------------------------------------------------------------------------
void puntajeUp(){
	puntaje[9]++;
	for(int i = 9;i!=0;i--)
	if(puntaje[i]>57){
		puntaje[i-1]++;
		puntaje[i]='0';
	}
}
//------------------------------------------------------------------------------
void TIni(){
	miSegundo[0]='0';
	miSegundo[1]='0';
}
//-----------------------------------------------------------------------------
void TiempoDown(char up[2]){
	if(correTiempo){
		up[1]--;
		if(up[1]<'0'){
			up[1]='9';
			up[0]--;
		}
		if(up[0]<'0'){
			up[1]='9';
			up[0]='5';
		}
	}
}
//------------------------------------------------------------------------------
void tiempo(int segundos){

	if(miSegundo[0]=='0' && miSegundo[1]=='0'){
		TiempoDown(miMinuto);
	}
	TiempoDown(miSegundo);
	timpo[0]=miMinuto[0];
	timpo[1]=miMinuto[1];
	timpo[2]=':';
	timpo[3]=miSegundo[0];
	timpo[4]=miSegundo[1];
	glutTimerFunc(1000, tiempo,0);
}
//------------------------------------------------------------------------------
void mostra_text(char cadena[], float x, float y) { //este muestra el texto
	int tmp;
	glLoadIdentity();	
	glLineWidth(3.0f);
	for(tmp=0; tmp < strlen(cadena); tmp++) {         
		x+=ESPAI;
		glRasterPos3f(x, y,-5.0f );
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,(int)cadena[tmp]);
	}
}
//------------------------------------------------------------------------------
void borrar(int x){
	for (int i=0;i<8;i++)
		for (int j=0;j<8;j++)
			if(mtz[i][j]>=100)
				mtz[i][j]=0;			
}
//------------------------------------------------------------------------------
void restaurar(){
	for(int i=0; i<8; i++)
		for (int j=0;j<8;j++)
			if(mtz[i][j]>=10 && mtz[i][j]<100)
				mtz[i][j]/=10;
}
//------------------------------------------------------------------------------
void ordenar(){
	for (int i=0;i<8;i++){
		int vec[8];
		for (int k=0;k<8;vec[k]=0,k++);
		int x=0;
		for (int j=0;j<8;j++)
			if(mtz[j][i]!=0){
				vec[x] = mtz[j][i];
				x++;
			}
		for (int k=0; k<8;k++)
			mtz[k][i]=vec[k];
	}
	llenarMtz();
}
//------------------------------------------------------------------------------
void imprimirTiempo(int i){
	printf("%s : %s \n", miMinuto,miSegundo);
	glutTimerFunc(1000,imprimirTiempo,1);
}
//------------------------------------------------------------------------------
void compare(){
	int p;
	int s;
	int t;
	for (int i=0;i<8;i++)
		for (int j=0;j<6;j++){
			bool x=true;
			p=mtz[i][j];
			s=mtz[i][j+1];
			t=mtz[i][j+2];
			if((p==s)&&(p==t)&&(p<100)&&(p!=0)){
				printf("%c",7);
				for (int k=0;k<8;k++){
					if(x && k+j<8)
						if(p==mtz[i][k+j]){
							mtz[i][j+k]*=100;
							puntajeUp();
							}
						else
							x=false;
				}
				glutTimerFunc(500, borrar, 1);
			}
		}
	for (int i=0;i<6;i++)
		for (int j=0;j<8;j++){
			bool x=true;
			p=mtz[i][j];
			s=mtz[i+1][j];
			t=mtz[i+2][j];
			if((p==s)&&(p==t)&&(p<100)&&(p!=0)){
				printf("%c",7);
				for (int k=0;k<8;k++){
					if( x  && k+i < 8)
						if(p==mtz[i+k][j]){
							mtz[i+k][j]*=100;
							puntajeUp();
						}
						else
							x=false;
				}
				glutTimerFunc(500, borrar, 1);
			}
		}
}
//------------------------------------------------------------------------------
void pasivo(int x,int y){
	int t=(x-28)/57;
	int z=(y-47)/48;
	t=7-t;
	if(hrztl){
		if(z<8 && z>=0 && t<8 && t>0 && x>28 && y>47){
			if(mtz[z][t]<10){
				mtz[z][t]*=10;
				if(mtz[z][t-1]<10)
					mtz[z][t-1]*=10;
			}
		}
	}
	else{
		if(z<7 && z>=0 && t<8 && t>=0 && x>28 && y>47){
			if(mtz[z][t]<10){
				mtz[z][t]*=10;
				if(mtz[z+1][t]<10)
					mtz[z+1][t]*=10;
			}
		}		
	}
	if(xr!=t || yr != z){
		restaurar();
		xr=t;
		yr=z;
	}
}
//------------------------------------------------------------------------------
bool si(){
	int p;
	int s;
	int t;
	bool x=false;
	for (int i=0;i<8;i++)
		for (int j=0;j<6;j++){
			p=mtz[i][j];
			s=mtz[i][j+1];
			t=mtz[i][j+2];
			if(p>=10)
				p/=10;
			if(s>=10)
				s/=10;
			if(t>=10)
				t/=10;
			if((p==s)&&(p==t)&&(p!=0))
				x=true;
		}
	for (int i=0;i<6;i++)
		for (int j=0;j<8;j++){
			p=mtz[i][j];
			s=mtz[i+1][j];
			t=mtz[i+2][j];
			if(p>=10)
				p/=10;
			if(s>=10)
				s/=10;
			if(t>=10)
				t/=10;
			if((p==s)&&(p==t)&&(p!=0))
				x=true;
		}
	return x;
}
//------------------------------------------------------------------------------
void vuelta(int i){
	int w=mtz[mtzx][mtzy];
	mtz[mtzx][mtzy]=mtz[mtzx][mtzy-1];
	mtz[mtzx][mtzy-1]=w;
}
//------------------------------------------------------------------------------
void vuelta2(int i){
	int w=mtz[mtzx][mtzy];
	mtz[mtzx][mtzy]=mtz[mtzx+1][mtzy];
	mtz[mtzx+1][mtzy]=w;
}
//------------------------------------------------------------------------------
void movimiento(int boton, int estado, int x, int y){
	if ((estado == GLUT_DOWN) && (boton== GLUT_LEFT_BUTTON))
	{
		int t=(x-28)/57;
		int z=(y-47)/48;
		t=7-t;
		if(hrztl)
			if(z<8 && z>=0 && t<8 && t>0 && x>28 && y>47)
				if(mtz[z][t]<100 && mtz[z][t-1]<100){
					mtzx=z;
					mtzy=t;
					vuelta(1);
					if(!si()){
						mtzx=z;
						mtzy=t;
						glutTimerFunc(400,vuelta,1);
					}
				}
				else;
			else;
		else
			if(z<7 && z>=0 && t<8 && t>=0 && x>28 && y>47)
					if(mtz[z][t]<100 && mtz[z+1][t]<100){
						mtzx=z;
						mtzy=t;
						vuelta2(1);
						if(!si()){
							mtzx=z;
							mtzy=t;
							glutTimerFunc(400,vuelta2,1);
						}
					}
		}
	if ((estado == GLUT_DOWN) && (boton== GLUT_RIGHT_BUTTON)){
		hrztl=!hrztl;
		pasivo(x+60,y);
		pasivo(x,y);
	}
}
//------------------------------------------------------------------------------
int xrand(int x,int y, int z){
	int t;
	do{
		t=(rand () % 5)+1;
	}while(t==x || t==y || t==z);
	return t;
}
//------------------------------------------------------------------------------
void llenarMtz(){
	srand(time(NULL));
	for (int i=0;i<8;i++)
		for (int j=0;j<8;j++){
			int temp=0;
			int x=0;
			if(j!=0)
				x=mtz[i][j-1];
			if(j!=7)
				temp=mtz[i][j+1];
			if(mtz[i][j]==0)
				if(i==0)
					mtz[i][j] = xrand(x,0,temp);
				else
					mtz[i][j] = xrand(x,mtz[i-1][j],temp);
		}
}
//------------------------------------------------------------------------------
void reshape_cb (int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(hazPerspectiva)
		gluPerspective(60.0f, (GLfloat)width/(GLfloat)height, 1.0f, 20.0f);
	else
		glOrtho(-4,10, -5, 5, 1, 10);
	glMatrixMode(GL_MODELVIEW);
	ancho = width;
	alto = height;
}
//------------------------------------------------------------------------------
void dibuja(double x, double y,int figura){
	glLoadIdentity();
	glTranslatef(x, y, -5.0f);
	glRotatef(anguloCuboX, 1.0f, 0.0f, 0.0f);
	glRotatef(anguloCuboY, 0.0f, 1.0f, 0.0f);
	switch(figura)
	{
	case 1:	glColor3f(0,1,1); glLineWidth(1);glutWireCube(0.5);break;
	case 2:	glColor3f(1,0,1); glLineWidth(1);glutWireCone(0.4,0.5,5,4);break;
	case 3:	glColor3f(0,1,0); glLineWidth(1);glutWireSphere(0.4,5,5);break;
	case 4:	glColor3f(1,0,0); glLineWidth(1);glScalef(0.4,0.4,0.3); glutWireIcosahedron();break;
	case 5:	glColor3f(1,1,1); glLineWidth(1);glScalef(0.2,0.2,0.3);glutWireDodecahedron();break;
	
	case 10:	glColor3f(0,1,1); glLineWidth(1);glutWireCube(0.65);break;
	case 20:	glColor3f(1,0,1); glLineWidth(1);glutWireCone(0.6,0.6,5,4);break;
	case 30:	glColor3f(0,1,0); glLineWidth(1);glutWireSphere(0.6,5,5);break;
	case 40:	glColor3f(1,0,0); glLineWidth(1);glScalef(0.5,0.5,0.4); glutWireIcosahedron();break;
	case 50:	glColor3f(1,1,1); glLineWidth(1);glScalef(0.3,0.3,0.4);glutWireDodecahedron();break;
	
	case 100:	glColor3f(0,1,1); glLineWidth(2);glutSolidCube(0.5);glColor3f(0,0,0);glutWireCube(0.5);break;
	case 200:	glColor3f(1,0,1); glLineWidth(2);glutSolidCone(0.4,0.5,5,4);glColor3f(0,0,0);glutWireCone(0.4,0.5,5,4);break;
	case 300:	glColor3f(0,1,0); glLineWidth(2);glutSolidSphere(0.4,5,5);glColor3f(0,0,0);glutWireSphere(0.4,5,5);break;
	case 400:	glColor3f(1,0,0); glLineWidth(2);glScalef(0.4,0.4,0.3); glutSolidIcosahedron();glColor3f(0,0,0);glutWireIcosahedron();break;
	case 500:	glColor3f(1,1,1); glLineWidth(2);glScalef(0.2,0.2,0.3);glutSolidDodecahedron();glColor3f(0,0,0);glutWireDodecahedron();break;
	}	
}
//------------------------------------------------------------------------------
void teclado(unsigned char key, int x, int y){
	if(!jugar){
		switch(key){
			case 8:
				cont--;
				nombre[cont]=' ';
				break;
			case 13:
				if(validar==true){
					jugar=true;
					strcpy(timpop,timpo);
					correTiempo=true;
				}
				if(validar==false)
					validar=true;
				break;
			default:    
				if(validar==false)
						nombre[cont]=key;
				else{ 
					miMinuto[cont2]=key; 
					cont2++;
				}
				cont++;
				break;
		}
	}
}
//------------------------------------------------------------------------------
void display_cb() {
	if(miSegundo[0]=='0' && miSegundo[1]=='0' && miMinuto[1]=='0' && miMinuto[0]=='0'){
		correTiempo=false;
		jugar=false;
		cont=0;
		cont2=0;
		validar=false;
		bool x=true;
		for(int i=0;i<5 && x;i++)
			if(mayor(puntaje,lista[i].puntos)){
				strcpy(lista[i].nombre,nombre);
				strcpy(lista[i].puntos,puntaje);
				strcpy(lista[i].tiempo,timpop);
				x=false;
			}
		for (int i=0; i<10;i++){
			nombre[i]=' ';
			puntaje[i]='0';
		}
		miMinuto[0]=' ';
		miMinuto[1]=' ';
	}
	if(jugar){
		compare();
		ordenar();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mostra_text(nombre,5.0f,3.0f);
		mostra_text(puntaje,7.0f,3.0f);
		mostra_text(timpo,5.0f,2.5f);
		glLoadIdentity();
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_LINES);
		glVertex3f(-3.5f, 3.9f, -5.0f); glVertex3f(4.5f, 3.9f, -5.0f);
		glVertex3f(-3.5f, 3.9f, -5.0f); glVertex3f(-3.5f, -4.0f, -5.0f);
		for(int w=1;w<9;w++){
			glVertex3f(-3.5f, (4.0f)-w, -5.0f); glVertex3f(4.5f, (4.0f)-w, -5.0f);
			glVertex3f((-3.5f)+w, 3.9f, -5.0f); glVertex3f((-3.5f)+w, -4.0f, -5.0f);
		}
		glEnd();
		for (int i=0;i<8;i++)
			for (int j=0;j<8;j++)
				if(mtz[i][j]!=0){
					dibuja((double)(4.0-j),((double)(3.5-i)),mtz[i][j]);
				}
		glFlush();
		glutSwapBuffers();
		anguloCuboX+=0.1f;
		anguloCuboY+=0.1f;	
	}
	else{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glColor3f(1.0f, 1.0f, 1.0f);
		mostra_text("D igite el nom bre: ",-3.0f,4.0f);
		mostra_text("D igite el tiempo en m inu tos: ",-3.0f,3.0f);
		mostra_text(nombre,0.7f,4.0f);
		mostra_text(miMinuto,3.5,3.0f);
		mostra_text("M ejores Tiem pos:",-3.0,2.0);
		for(int i=0;i<5;i++){
			mostra_text(lista[i].nombre, -3.0,1.0-float(i));
			mostra_text(lista[i].tiempo, -1.0,1.0-float(i));
			mostra_text(lista[i].puntos, 1.0,1.0-float(i));
		}
		glFlush();
		glutSwapBuffers();
	}
}
//------------------------------------------------------------------------------
void idle(){
	display_cb();
}
//------------------------------------------------------------------------------
void initialize() {
	glutInitDisplayMode (GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize (800,480);
	glutInitWindowPosition (100,100);
	glutCreateWindow ("Jewels 1.0");
	glClearColor(0,0,0,0);
	glEnable(GL_DEPTH_TEST);
	ancho = 400;
	alto = 400;
	glutDisplayFunc (display_cb);
	glutKeyboardFunc(teclado);
	glutReshapeFunc (reshape_cb);
	glutIdleFunc(idle);
}
//------------------------------------------------------------------------------
