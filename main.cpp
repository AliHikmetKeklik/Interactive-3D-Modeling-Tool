#include <GL/glut.h>
#include <iostream>

int cx = 0, cy = 0, cz = 0, cn; // variables to store the current position of the cube
int lastMouseX, lastMouseY; // variables to store the last mouse position for camera rotation
float rotationX = 0, rotationY = 0; // variables to store the camera rotation angles

struct Quads {
	int x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
	float r, g, b;
	int state;
	int total;
}; Quads Q[100];

// function to add a new quad to the model
void addQuad() { 
	Q[0].state++; if (Q[0].state > 4) { Q[0].state = 1; }
	int st = Q[0].state; //store the current state in a local variable for convenience
	if (st == 1) { Q[0].total++; cn = Q[0].total; }
	if (st == 1) { Q[cn].x1 = cx; Q[cn].y1 = cy; Q[cn].z1 = cz; }
	if (st == 1 || st == 2) { Q[cn].x2 = cx; Q[cn].y2 = cy; Q[cn].z2 = cz; }
	if (st == 1 || st == 2 || st == 3) { Q[cn].x3 = cx; Q[cn].y3 = cy; Q[cn].z3 = cz; }
	if (st == 1 || st == 2 || st == 3 || st == 4) { Q[cn].x4 = cx; Q[cn].y4 = cy; Q[cn].z4 = cz; }
}

// function to draw all the quads in the model
void drawQuads() {
	for (int i = 0; i < Q[0].total + 1; i++) {
		glBegin(GL_QUADS);
		glColor3f(Q[i].r, Q[i].g, Q[i].b);			// The drawQuads function iterates
		glVertex3f(Q[i].x1, Q[i].y1, Q[i].z1);		// through all the quadrilaterals
		glVertex3f(Q[i].x2, Q[i].y2, Q[i].z2);		// in the Q arrayand renders them
		glVertex3f(Q[i].x3, Q[i].y3, Q[i].z3);		// using OpenGL commands.
		glVertex3f(Q[i].x4, Q[i].y4, Q[i].z4);
		glEnd();
	}
}

void theCube() {

	glPushMatrix();					// The theCube function pushes
	glColor3f(1, 1, 1);				// a new matrix onto the OpenGL matrix stack,
	glTranslatef(cx, cy, cz);		// translates it to the current coordinates(cx, cy, cz),
	glutSolidCube(0.4);				// and renders a solid cube.
	glPopMatrix();

}
void drawGrid() {
	for (int i = 0; i < 40; i++) {
		glPushMatrix();
		if (i < 20) { glTranslatef(0, 0, i); }
		if (i >= 20) { glTranslatef(i - 20, 0, 0); glRotatef(-90, 0, 1, 0); }

		glBegin(GL_LINES);				// The drawGrid function appears to be used 
		glColor3f(1, 1, 1);				// for rendering a grid of lines on the x - z plane.
		glLineWidth(1);					// It does this by rendering a set of lines in the x - direction 
		glVertex3f(0, -0.1, 0);			// and another set in the z - direction.
		glVertex3f(19, -0.1, 0);
		glEnd();
		glPopMatrix();
	}
	
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();				// The display function is the main rendering function.
									// It clears the colorand depth buffers and sets the perspectiveand modelview matrices.
	glTranslatef(-13, 0, -45);		// It may rotate the model by user using mouse
	glRotatef(40, 1, 1, 0);			// and then renders the grid, quadrilaterals, and cube.

	glTranslatef(10, 0, 10);
	glRotatef(rotationY, 0, 1, 0);  // Rotate around the Y axis
	glRotatef(rotationX, 1, 0, 0);  // Rotate around the X axis
	glTranslatef(-10, 0, -10);

	drawGrid();
	drawQuads();
	theCube();
	
	glutSwapBuffers();

}

void init() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();							// he init function sets up the rendering context
	gluPerspective(35, 1.0f, 0.1f, 1000);		// and enables the depth test.
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2, 0.2, 0.2, 1);

}

//The keyboard function handles keyboard input from the user.It appears to allow the user to
//move a cube along the x, y, and z axes using the WASD keys.It also allows the user to change the 
//current coordinates(cx, cy, cz) using the arrow keys.The function also appears to allow the user to 
//add a new quadrilateral to the Q array using the space bar.
void keyboard(unsigned char key, int x, int y) {
	if (key == 'w') { cz -= 1; } if (key == 's') { cz += 1; }   // forward / back
	if (key == 'a') { cx -= 1; } if (key == 'd') { cx += 1; }	// left    / right	
	if (key == 'q') { // up
		cy += 1; 
		std::cout << cy << std::endl;        // I wanted to write to the console 
	}										 // the position of cy
	if (key == 'z') { // down				 // So we can check and see if the cube is up or down 
		cy -= 1;                             // when its difficult to understand by looking to the grid.
		std::cout << cy << std::endl;
	}	      

	if (key == 32) { addQuad(); }								// spacebar

	if (key == 'r') { Q[cn].r = 1, Q[cn].g = 0, Q[cn].b = 0; }
	if (key == 'g') { Q[cn].r = 0, Q[cn].g = 1, Q[cn].b = 0; }
	if (key == 'b') { Q[cn].r = 0, Q[cn].g = 0, Q[cn].b = 1; }
	if (key == 'y') { Q[cn].r = 1, Q[cn].g = 1, Q[cn].b = 0; }
	if (key == 'c') { Q[cn].r = 0, Q[cn].g = 1, Q[cn].b = 1; }

	glutPostRedisplay();
}


void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		lastMouseX = x;
		lastMouseY = y;
	}
}

void mouseMove(int x, int y) {

	int dx = x - lastMouseX;
	int dy = y - lastMouseY;

	// Use the mouse movement to update the rotation angles
	rotationX += dy;
	rotationY += dx;

	// Save the current mouse position for the next movement
	lastMouseX = x;
	lastMouseY = y;

	// Redraw the scene
	glutPostRedisplay();
}
int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
	init();
	glutMainLoop();

	return 0;

}
