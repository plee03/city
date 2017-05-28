#include <cstdlib>
#include <cstdio>
#include <iostream>

/* Grid */
#include <vector>
#include <string>
#include <random>
#include <cmath>
#include <iomanip>
#include "CityObject.h"
#include "BuildingObject.h"
#include "StreetObject.h"
#include "grid.h"
#include "draw.h"

/* Rendering */ 
#include <GL/glui.h>
#include "Camera.h"

#define MOVEMENT_UNIT 0.1

/* Global Variables */
std::vector<CityObject> grid; /* 2-D array of grid for city */
std::vector<BuildingObject> buildings; /* Array of buildings */
std::vector<StreetObject> streets; /* Array of streets */

/** These are the live variables passed into GLUI ***/
int  toFill = 1;
int	 rotX = 0;
int	 rotY = 0;
int	 rotZ = 0;
int  scale = 50;
int  gsl = 100; // default grid size
int inception = 0;

float camRotU = 0.0;
float camRotV = 0.0;
float camRotW = 0.0;
int  viewAngle = 50;
float eyeX = 0;
float eyeY = 2.25;
float eyeZ = 2;
float lookX = 0.0;
float lookY = 0.0;
float lookZ = -2;
float clipNear = 0.001;
float clipFar = 30;
int in_x_lowerbound = 0;
int in_x_upperbound = 0;
int in_y_lowerbound = 0;
int in_y_upperbound = 0;

int  main_window;

/** these are the global variables used for rendering **/
Camera* camera = new Camera();

/***************************************** myGlutIdle() ***********/

void myGlutIdle(void)
{
	Vector look(lookX, lookY, lookZ);
	look.normalize();

	if (in_x_lowerbound) {
		look = look - (0.01 * camera->GetU());
	} 

	if (in_x_upperbound) {
		// camRotV++;
		look = look + (0.01 * camera->GetU());
	} 

	if (in_y_upperbound) {
		camRotU+= 0.5;
	}

	if (in_y_lowerbound) {
		camRotU -= 0.5;
	}
	look.normalize();

	lookX = look[0];
	lookY = look[1];
	lookZ = look[2];

	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	glutPostRedisplay();
}


/**************************************** myGlutReshape() *************/

void myGlutReshape(int x, int y)
{
	float xy_aspect;

	xy_aspect = (float)x / (float)y;
	glViewport(0, 0, x, y);

	camera->SetScreenSize(x, y);

	glutPostRedisplay();
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->SetViewAngle(viewAngle);
	camera->SetNearPlane(clipNear);
	camera->SetFarPlane(clipFar);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	Matrix projection = camera->GetProjectionMatrix();
	glMultMatrixd(projection.unpack());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Point eyeP(eyeX, eyeY, eyeZ);
	Vector lookV(lookX, lookY, lookZ);
	Vector upV(0, 1, 0);
	camera->Orient(eyeP, lookV, upV);
	camera->RotateV(camRotV);
	camera->RotateU(camRotU);
	camera->RotateW(camRotW);
	Matrix modelView = camera->GetModelViewMatrix();
	glMultMatrixd(modelView.unpack());

	//rotate object
	glRotatef(rotX, 1.0, 0.0, 0.0);
	glRotatef(rotY, 0.0, 1.0, 0.0);
	glRotatef(rotZ, 0.0, 0.0, 1.0);

	/* drawing the axes */ 
 	 glDisable(GL_LIGHTING);
	// glBegin(GL_LINES);
	// glColor3f(1.0, 0.0, 0.0);
	// glVertex3f(0, 0, 0); glVertex3f(1.0, 0, 0);
	// glColor3f(0.0, 1.0, 0.0);
	// glVertex3f(0, 0, 0); glVertex3f(0.0, 1.0, 0);
	// glColor3f(0.0, 0.0, 1.0);
	// glVertex3f(0, 0, 0); glVertex3f(0, 0, 1.0);
	// glEnd();

	//scale object
	glScalef(scale / 50.0, scale / 50.0, scale / 50.0);
	draw(gsl, buildings, streets, toFill, inception);

	camera->RotateV(-camRotV);
	camera->RotateU(-camRotU);
	camera->RotateW(-camRotW);

	glutSwapBuffers();
}

void myGlutKeyboard(unsigned char Key, int x, int y) {
	(void) x, (void) y;
	
	Vector look(lookX, lookY, lookZ);
	look.normalize();
	Vector eye(eyeX, eyeY, eyeZ);

	switch(Key)
	{
		case 27: /* esc key */
			exit(0);
			break;
		case 'f':
			toFill = !toFill;
			break;
		case 'n':
			clipNear += MOVEMENT_UNIT;
			break;
		case 'm':
			clipNear -= MOVEMENT_UNIT;
			break;
		case '=':
			scale++;
			break;
		case '-':
			scale--;
			break;
		case 'q':
			eyeY += MOVEMENT_UNIT;
			break;
		case 'e':
			eyeY -= MOVEMENT_UNIT;
			break;
		case 'w':
			eye = eye + (MOVEMENT_UNIT * look);
			eyeX = eye[0];
			eyeY = eye[1];
			eyeZ = eye[2];
			break;
		case 's':
			eye = eye - (MOVEMENT_UNIT * look);
			eyeX = eye[0];
			eyeY = eye[1];
			eyeZ = eye[2];
			break;
		case 'a':
			eye = eye - (MOVEMENT_UNIT * camera->GetU());
			eyeX = eye[0];
			eyeY = eye[1];
			eyeZ = eye[2];
			break;
		case 'd':
			eye = eye + (MOVEMENT_UNIT * camera->GetU());
			eyeX = eye[0];
			eyeY = eye[1];
			eyeZ = eye[2];
			break;
		case 'i':
			inception = !inception;
	}
}

void myGlutMouse(int x, int y) {
	int screenwidth = camera->GetScreenWidth();
	int screenheight = camera->GetScreenHeight();

	int center_x = screenwidth / 2;
	int center_y = screenheight / 2;

	int y_lowerbound = (screenheight - center_y) / 2;
	int y_upperbound = ((screenheight - center_y) / 2) + center_y;

	int x_lowerbound = (screenwidth - center_x) / 2;
	int x_upperbound = ((screenwidth - center_x) / 2) + center_x; 

	in_x_lowerbound = (x < x_lowerbound);
	in_x_upperbound = (x > x_upperbound);
	in_y_lowerbound = (y < y_lowerbound);
	in_y_upperbound = (y > y_upperbound);
}

void onExit()
{
	std::cout << "bye!" << std::endl;
}

/**************************************** main() ********************/

void callback_start(int id) {
	(void) id;
	grid.clear();
	buildings.clear();
	streets.clear();
	generateGrid(gsl, grid, buildings, streets);
}


int main(int argc, char* argv[])
{
	cout << "INSTRUCTIONS" << endl;
	cout << "------------------------------" << endl;
	cout << "n | near plane + " << endl;
	cout << "m | near plane -" << endl;
	cout << "f | fill" << endl;
	cout << "- | zoom in, scale smaller" << endl;
	cout << "= | zoom out, scale bigger" << endl;
	cout << "w, a, s, d | movement" << endl;
	cout << "q, e | up, down" << endl;
	cout << "esc | quit + " << endl;
	cout << "------------------------------" << endl;
	cout << endl;


	/* Grid */
	generateGrid(gsl, grid, buildings, streets);
	//printGrid(gsl, grid);

	/* Render */
	atexit(onExit);

	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1200, 750);

	main_window = glutCreateWindow("cities.");
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);
	GLUI_Master.set_glutKeyboardFunc(myGlutKeyboard);
	glutPassiveMotionFunc(myGlutMouse);


	/****************************************/
	/*       Set up OpenGL lighting         */
	/****************************************/


	    glClearColor (0.38, 0.38, 0.38, 0.0);
        glShadeModel (GL_SMOOTH);

        GLfloat light_pos0[] = {0.0f, 0.0f, 1.0f, 0.0f};
        GLfloat diffuse[] = {0.5f, 0.5f, 0.5f, 0.0f};
        GLfloat ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};

        glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv (GL_LIGHT0, GL_POSITION, light_pos0);

        glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable (GL_COLOR_MATERIAL);

        glEnable(GL_LIGHTING);
        glEnable (GL_LIGHT0);
        glEnable (GL_DEPTH_TEST);

	///****************************************/
	///*          Enable z-buferring          */
	///****************************************/

	glPolygonOffset(1, 1);

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	GLUI *glui = GLUI_Master.create_glui("GLUI");

	GLUI_Panel *generate_panel = glui->add_panel(" ");
	(new GLUI_Spinner(generate_panel, "Size: ", &gsl))
		->set_int_limits(10, 500);
	glui -> add_button("Start!", 0, callback_start);


	glui->set_main_gfx_window(main_window); 

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	glutMainLoop();

	return EXIT_SUCCESS;
}
