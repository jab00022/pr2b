#include <cstdlib>
#include <stdio.h>

#include "igvEscena3D.h"

// Metodos constructores 

igvEscena3D::igvEscena3D() { ejes = true;
							perspectiva = false;
							vista = 4;
							corte = 1;
							formato = false;
}

igvEscena3D::~igvEscena3D() {}


// Metodos publicos 

void pintar_ejes(void) {
	GLfloat rojo[] = { 1,0,0,1.0 };
	GLfloat verde[] = { 0,1,0,1.0 };
	GLfloat azul[] = { 0,0,1,1.0 };

	glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
	glBegin(GL_LINES);
	glVertex3f(1000, 0, 0);
	glVertex3f(-1000, 0, 0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_EMISSION, verde);
	glBegin(GL_LINES);
	glVertex3f(0, 1000, 0);
	glVertex3f(0, -1000, 0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_EMISSION, azul);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 1000);
	glVertex3f(0, 0, -1000);
	glEnd();
}

void pintar_tubo() {
	GLUquadricObj *tubo;
	GLfloat color_tubo[] = { 0,0,0.5 };

	glMaterialfv(GL_FRONT, GL_EMISSION, color_tubo);

	tubo = gluNewQuadric();
	gluQuadricDrawStyle(tubo, GLU_FILL);

	glPushMatrix();
	glTranslatef(0, 0, -0.5);
	gluCylinder(tubo, 0.25, 0.25, 1, 20, 20);
	glPopMatrix();

	gluDeleteQuadric(tubo);
}

void igvEscena3D::visualizar(void) {
	// crear luces
	GLfloat luz0[] = { 10,8,9,1 }; // luz puntual
	glLightfv(GL_LIGHT0, GL_POSITION, luz0);
	glEnable(GL_LIGHT0);

	// crear el modelo
	glPushMatrix(); // guarda la matriz de modelado

	  // se pintan los ejes
	if (ejes) pintar_ejes();

	// se pintan los objetos de la escena
	GLfloat color_cubo[] = { 0,0.25,0 };
	glMaterialfv(GL_FRONT, GL_EMISSION, color_cubo);

	glPushMatrix();
	glScalef(1, 2, 4);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glRotatef(45, 1, 0, 0);
	glScalef(1, 1, 4.5);
	pintar_tubo();
	glPopMatrix();
	glPushMatrix();
	glRotatef(-45, 1, 0, 0);
	glScalef(1, 1, 4.5);
	pintar_tubo();
	glPopMatrix();

	glPopMatrix(); // restaura la matriz de modelado

}

