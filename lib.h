#include <GL/glut.h>
#include <ctime>
#include <cstdlib>

void reshape_cb (int width, int height);
void display_cb();
void idle();
void initialize() ;
void llenarMtz();
void movimiento(int boton, int estado, int x, int y);
void pasivo(int x,int y);
void tiempo(int segundos);
void puntajeReset();
void TIni();
void imprimirTiempo(int i);
