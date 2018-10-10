#include <cstdlib>
#include <stdio.h>

#include "igvInterfaz.h"

extern igvInterfaz interfaz; // los callbacks deben ser estaticos y se requiere este objeto para acceder desde
							 // ellos a las variables de la clase

// Metodos constructores -----------------------------------

igvInterfaz::igvInterfaz() {}

igvInterfaz::~igvInterfaz() {}


// Metodos publicos ----------------------------------------

void igvInterfaz::crear_mundo(void) {
	// crear cámaras
	interfaz.camara.set(IGV_PARALELA, igvPunto3D(3.0, 2.0, 4), igvPunto3D(0, 0, 0), igvPunto3D(0, 1.0, 0),
		-1 * 3, 1 * 3, -1 * 3, 1 * 3, -1 * 3, 200);
}

void igvInterfaz::configura_entorno(int argc, char** argv,
	int _ancho_ventana, int _alto_ventana,
	int _pos_X, int _pos_Y,
	string _titulo) {
	// inicialización de las variables de la interfaz																	
	ancho_ventana = _ancho_ventana;
	alto_ventana = _alto_ventana;

	// inicialización de la ventana de visualización
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(_ancho_ventana, _alto_ventana);
	glutInitWindowPosition(_pos_X, _pos_Y);
	glutCreateWindow(_titulo.c_str());

	glEnable(GL_DEPTH_TEST); // activa el ocultamiento de superficies por z-buffer
	glClearColor(1.0, 1.0, 1.0, 0.0); // establece el color de fondo de la ventana

	glEnable(GL_LIGHTING); // activa la iluminacion de la escena
	glEnable(GL_NORMALIZE); // normaliza los vectores normales para calculo iluminacion

	crear_mundo(); // crea el mundo a visualizar en la ventana
}

void igvInterfaz::inicia_bucle_visualizacion() {
	glutMainLoop(); // inicia el bucle de visualizacion de OpenGL
}

void igvInterfaz::set_glutKeyboardFunc(unsigned char key, int x, int y) {

	/* IMPORTANTE: en la implementación de este método hay que cambiar convenientemente el estado
	   de los objetos de la aplicación, pero no hacer llamadas directas a funciones de OpenGL */

	switch (key) {
	case 'p': // cambia el tipo de proyección de paralela a perspectiva y viceversa
		if (interfaz.escena.get_Perspectiva() == false) {
			interfaz.escena.set_Perspectiva(true);
		}
		else {
			interfaz.escena.set_Perspectiva(false);
		}

		interfaz.set_vistas();
		
		break;

	case 'v': // cambia la posición de la cámara para mostrar las vistas planta, perfil, alzado o perspectiva
		if (interfaz.escena.get_Vista() != 4) {
			interfaz.escena.set_Vista(interfaz.escena.get_Vista() + 1);
		}
		else {
			interfaz.escena.set_Vista(1);
		}

		interfaz.set_vistas();
		break;
	case '+': // zoom in
		interfaz.camara.zoom(-0.05);
		interfaz.camara.aplicar();
		break;
	case '-': // zoom out
		interfaz.camara.zoom(0.05);
		interfaz.camara.aplicar();
		break;

	case 'c': // incrementar la distancia del plano cercano
		interfaz.escena.set_corte(interfaz.escena.get_corte() + 0.2);
		
		interfaz.set_vistas();

		break;

	case 'C': // decrementar la distancia del plano cercano
		interfaz.escena.set_corte(interfaz.escena.get_corte() - 0.2);

		interfaz.set_vistas();

		break;

	case '9': // cambiar a formato 16:9 con deformación
		if (interfaz.escena.get_formato() == false) {
			interfaz.escena.set_formato(true);
		}
		else {
			interfaz.escena.set_formato(false);
		}

		interfaz.set_glutDisplayFunc();
		break;

	case 'e': // activa/desactiva la visualizacion de los ejes
		interfaz.escena.set_ejes(interfaz.escena.get_ejes() ? false : true);
		break;

	case 27: // tecla de escape para SALIR
		exit(1);
		break;
	}
	glutPostRedisplay(); // renueva el contenido de la ventana de vision y redibuja la escena
}

void igvInterfaz::set_glutReshapeFunc(int w, int h) {
	// dimensiona el viewport al nuevo ancho y alto de la ventana
	// guardamos valores nuevos de la ventana de visualizacion
	interfaz.set_ancho_ventana(w);
	interfaz.set_alto_ventana(h);

	// establece los parámetros de la cámara y de la proyección
	interfaz.camara.aplicar();
}

void igvInterfaz::set_glutDisplayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borra la ventana y el z-buffer

	// se establece el viewport
	if (interfaz.escena.get_formato() == false) {
		glViewport(0, 0, interfaz.get_ancho_ventana(), interfaz.get_alto_ventana());
	}
	else {
		glViewport(0, 100, 512,288); //Formato 16:9
	}

	//visualiza la escena
	interfaz.escena.visualizar();

	// refresca la ventana
	glutSwapBuffers(); // se utiliza, en vez de glFlush(), para evitar el parpadeo
}

void igvInterfaz::inicializa_callbacks() {
	glutKeyboardFunc(set_glutKeyboardFunc);
	glutReshapeFunc(set_glutReshapeFunc);
	glutDisplayFunc(set_glutDisplayFunc);
}

void igvInterfaz::set_vistas() {
	switch (interfaz.escena.get_Vista())
	{
	case 1: //En planta
		if (interfaz.escena.get_Perspectiva() == false) {
			interfaz.camara.set(IGV_PARALELA, igvPunto3D(0, 4.5, 0), igvPunto3D(0, 0, 0), igvPunto3D(1.0, 0, 0),
				-1 * 3, 1 * 3, -1 * 3, 1 * 3, 1 * interfaz.escena.get_corte(), 200);
		}
		else {
			interfaz.camara.set(IGV_PERSPECTIVA, igvPunto3D(0, 4.5, 0), igvPunto3D(0, 0, 0), igvPunto3D(1.0, 0, 0),
				65, 5 / 5, 1 * interfaz.escena.get_corte(), 200);
		}
		interfaz.camara.aplicar();
		break;
	case 2: //Alzado
		if (interfaz.escena.get_Perspectiva() == false) {
			interfaz.camara.set(IGV_PARALELA, igvPunto3D(4.0, 0, 0), igvPunto3D(0, 0, 0), igvPunto3D(0, 1.0, 0),
				-1 * 3, 1 * 3, -1 * 3, 1 * 3, 1 * interfaz.escena.get_corte(), 200);
		}
		else {
			interfaz.camara.set(IGV_PERSPECTIVA, igvPunto3D(4.0, 0, 0), igvPunto3D(0, 0, 0), igvPunto3D(0, 1.0, 0),
				65, 5 / 5, 1 * interfaz.escena.get_corte(), 200);
		}
		interfaz.camara.aplicar();
		break;
	case 3: //Perfil
		if (interfaz.escena.get_Perspectiva() == false) {
			interfaz.camara.set(IGV_PARALELA, igvPunto3D(0, 0, 4), igvPunto3D(0, 0, 0), igvPunto3D(0, 1.0, 0),
				-1 * 3, 1 * 3, -1 * 3, 1 * 3, 1 * interfaz.escena.get_corte(), 200);
		}
		else {
			interfaz.camara.set(IGV_PERSPECTIVA, igvPunto3D(0, 0, 4), igvPunto3D(0, 0, 0), igvPunto3D(0, 1.0, 0),
				100, 5 / 5, 1 * interfaz.escena.get_corte(), 200);
		}
		interfaz.camara.aplicar();
		break;
	case 4:
		if (interfaz.escena.get_Perspectiva() == true) {
			interfaz.camara.set(IGV_PERSPECTIVA, igvPunto3D(3.0, 2.0, 4), igvPunto3D(0, 0, 0), igvPunto3D(0, 1.0, 0),
				65, 1 / 1, 1 * interfaz.escena.get_corte(), 200);
		}
		else {
			interfaz.camara.set(IGV_PARALELA, igvPunto3D(3.0, 2.0, 4), igvPunto3D(0, 0, 0), igvPunto3D(0, 1.0, 0),
				-1 * 3, 1 * 3, -1 * 3, 1 * 3, 1 * interfaz.escena.get_corte(), 200);
		}

		interfaz.camara.aplicar();
		break;
	}
}
