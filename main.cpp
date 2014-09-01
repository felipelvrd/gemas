#include "lib.h"

#include <cstdio>
using namespace std;
int main(int argc, char **argv){
	TIni();
	puntajeReset();
	glutInit (&argc, argv);
	initialize();
	glutMouseFunc(movimiento);
	glutPassiveMotionFunc(pasivo);
	tiempo(1);
	imprimirTiempo(1);
	glutMainLoop();
	return 0;
}
