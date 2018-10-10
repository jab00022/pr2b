#ifndef __IGVESCENA3D
#define __IGVESCENA3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <glut.h>
#include <string>
#endif

class igvEscena3D {
protected:
	// Atributos

	bool ejes;
	bool perspectiva;
	int vista;
	double corte;
	bool formato;

public:
	// Constructores por defecto y destructor
	igvEscena3D();
	~igvEscena3D();

	// Metodos estáticos

	// Métodos
	// método con las llamadas OpenGL para visualizar la escena
	void visualizar();

	bool get_ejes() { return ejes; };
	void set_ejes(bool _ejes) { ejes = _ejes; };

	bool get_Perspectiva() { return perspectiva; }
	void set_Perspectiva(bool _perspectiva) { perspectiva = _perspectiva; }

	int get_Vista() { return vista; }
	void set_Vista(int _vista) { vista = _vista; }

	double get_corte() { return corte; }
	void set_corte(double c) { corte = c; }

	bool get_formato() { return formato; }
	void set_formato(bool f) { formato = f; }
};

#endif
