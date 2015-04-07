// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Introduction à OpenGL
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <GL/glut.h>

using namespace std;

// App parameters
static const unsigned int DEFAULT_SCREENWIDTH = 1024;
static const unsigned int DEFAULT_SCREENHEIGHT = 768;
static string appTitle ("Informatique Graphique & Realite Virtuelle - Travaux Pratiques - Introduction a OpenGL");
static GLint window;
static unsigned int screenWidth;
static unsigned int screenHeight;

// Camera parameters
static float fovAngle;
static float aspectRatio;
static float nearPlane;
static float farPlane;
static float camPhi; // Expressing the camera position in polar coordinate, in the frame of the target
static float camTheta;
static float camDist2Target;
static float camTargetX;
static float camTargetY;
static float camTargetZ;

GLuint texture;

void polar2Cartesian (float phi, float theta, float d, float & x, float & y, float & z) {
    x = d*sin (theta) * cos (phi);
    y = d*cos (theta);
    z = d*sin (theta) * sin (phi);
}

void printUsage () {
	std::cerr << std::endl // send a line break to the standard error output
		 << appTitle << std::endl
         << "Author : Tamy Boubekeur" << std::endl << std::endl
         << "Usage : ./main [<file.off>]" << std::endl
         << "Cammandes clavier :" << std::endl 
         << "------------------" << std::endl
         << " ?: Print help" << std::endl
		 << " w: Toggle wireframe mode" << std::endl
         << " <drag>+<left button>: rotate model" << std::endl 
         << " <drag>+<right button>: move model" << std::endl
         << " <drag>+<middle button>: zoom" << std::endl
         << " q, <esc>: Quit" << std::endl << std::endl; 
}

void genCheckerboard (unsigned int width, unsigned int height, unsigned char * image) {
    /*
    for (unsigned int i = 0 ; i < width*height ; i++) {
        if (i%2 == 0) {
            image[i] = 255;
        }
        else if (i%2 == 1) {
            image [i + height*width] = 255;
        }
    }*/
    for (unsigned int i = 0 ; i < 3 * width * height ; i++) {
        if (i%6 == 1) {
            image[i] = 255;
        }
        else if (i%6 == 5) {
            image[i] = 255;
        }
    }
}


void init () {  
	// OpenGL initialization
    glEnable (GL_LIGHTING);
    glCullFace (GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
    glEnable (GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
    glDepthFunc (GL_LESS); // Specify the depth test for the z-buffer
    glEnable (GL_DEPTH_TEST); // Enable the z-buffer in the rasterization
	glLineWidth (2.0); // Set the width of edges in GL_LINE polygon mode
    glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // Background color

    unsigned int height = 16;
    unsigned int width = 16;
    unsigned char * image = new unsigned char[height*width*3];
    genCheckerboard(width, height, image);
    glEnable (GL_TEXTURE_2D);
    glGenTextures (1, &texture);
    glBindTexture (GL_TEXTURE_2D, texture);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    delete[] image;

	// Camera initialization
	fovAngle = 45.f;
	nearPlane = 0.01;
	farPlane = 10.0;
    camPhi = M_PI/2.0;
    camTheta = M_PI/2.0;
	camDist2Target = 5.0;
	camTargetX = 0.0;
	camTargetY = 0.0;
	camTargetZ = 0.0;
}

void setupCamera () {
	glMatrixMode (GL_PROJECTION); // Set the projection matrix as current. All upcoming matrix manipulations will affect it.
	glLoadIdentity ();
	gluPerspective (fovAngle, aspectRatio, nearPlane, farPlane); // Set the current projection matrix with the camera intrinsics
	glMatrixMode (GL_MODELVIEW); // Set the modelview matrix as current. All upcoming matrix manipulations will affect it.
	glLoadIdentity ();
	float camPosX, camPosY, camPosZ;
    polar2Cartesian (camPhi, camTheta, camDist2Target, camPosX, camPosY, camPosZ);
	camPosX += camTargetX;
	camPosY += camTargetY;
	camPosZ += camTargetZ;
    gluLookAt (camPosX, camPosY, camPosZ, camTargetX, camTargetY, camTargetZ, 0.0, 1.0, 0.0); // Set up the current modelview matrix with camera transform
}

void reshape (int w, int h) {
    screenWidth = w;
	screenHeight = h;
	aspectRatio = static_cast<float>(w)/static_cast<float>(h);
	glViewport (0, 0, (GLint)w, (GLint)h); // Dimension of the drawing region in the window
	setupCamera ();
}

void glSphere(float x0, float y0, float z0, float radius) {

    glMatrixMode (GL_MODELVIEW);
    glPushMatrix ();
    glTranslatef (x0, y0, z0);
    glBegin (GL_TRIANGLES);
    float x, y, z, d;
    d=radius;
    for (float theta = 0 ; theta < M_PI ; theta=theta+0.1) {

        for (float phi = 0; phi < 2*M_PI ; phi=phi+0.1) {

            polar2Cartesian(phi, theta, d, x, y ,z);
            glTexCoord2f(theta/(M_PI), phi/(2*M_PI));
            glNormal3f (x, y, z);
            glVertex3f(x, y, z);

            polar2Cartesian(phi+0.1, theta+0.1, d, x, y ,z);
            glTexCoord2f(theta/(M_PI), phi/(2*M_PI));
            glNormal3f (x, y, z);
            glVertex3f(x, y, z);

            polar2Cartesian(phi+0.1, theta, d, x, y ,z);
            glTexCoord2f(theta/(M_PI), phi/(2*M_PI));
            glNormal3f (x, y, z);
            glVertex3f(x, y, z);

            polar2Cartesian(phi, theta+0.1, d, x, y ,z);
            glTexCoord2f(theta/(M_PI), phi/(2*M_PI));
            glNormal3f (x, y, z);
            glVertex3f(x, y, z);

            polar2Cartesian(phi+0.1, theta+0.1, d, x, y ,z);
            glTexCoord2f(theta/(M_PI), phi/(2*M_PI));
            glNormal3f (x, y, z);
            glVertex3f(x, y, z);

            polar2Cartesian(phi, theta, d, x, y ,z);
            glTexCoord2f(theta/(M_PI), phi/(2*M_PI));
            glNormal3f (x, y, z);
            glVertex3f(x, y, z);
        }
    }
    glEnd ();
    glPopMatrix ();
}

void glSphereWithMat(float x0, float y0, float z0, float radius, float difR, float difG, float difB, float specR, float specG, float specB, float shininess) {

    GLfloat material_color[4] = {difR, difG, difB, 1.0f};
    GLfloat material_specular[4] = {specR, specG, specB,1.0};
    glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
    glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glBindTexture (GL_TEXTURE_2D, texture);

    glSphere(x0, y0, z0, radius);
}

void display () {  
    setupCamera ();   
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Erase the color and z buffers.

    GLfloat light_position[4] = {10.0f, 10.0f, 10.0f, 1.0f};
    GLfloat color[4] = {1.0f, 1.0f, 0.9f, 1.0f};
    glLightfv (GL_LIGHT0, GL_POSITION, light_position);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, color);
    glLightfv (GL_LIGHT0, GL_SPECULAR, color);

    GLfloat light_position1[4] = {10.0f, 10.0f, 10.0f, 1.0f};
    GLfloat color1[4] = {1.0f, 0.9f, 0.8f, 1.0f};
    glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv (GL_LIGHT1, GL_SPECULAR, color1);

    GLfloat light_position2[4] = {-10.0f, 0.0f, -1.0f, 1.0f};
    GLfloat color2[4] = {0.0f, 0.1f, 0.3f, 1.0f};
    glLightfv (GL_LIGHT2, GL_POSITION, light_position2);
    glLightfv (GL_LIGHT2, GL_DIFFUSE, color2);
    glLightfv (GL_LIGHT2, GL_SPECULAR, color2);

    glEnable (GL_LIGHT0);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHT2);
	// Put your drawing code (glBegin, glVertex, glCallList, glDrawArray, etc) here
    /*glSphere(-1,-1,0,0.5);
    glSphere(0,-1,0,0.5);
    glSphere(1,-1,0,0.5);
    glSphere(-1,-1,1,0.5);
    glSphere(0,-1,1,0.5);
    glSphere(1,-1,1,0.5);
    glSphere(-1,-1,-1,0.5);
    glSphere(0,-1,-1,0.5);
    glSphere(1,-1,-1,0.5);

    glSphere(0.5,-0.5,0.5,0.5);
    glSphere(0.5,-0.5,-0.5,0.5);
    glSphere(-0.5,-0.5,0.5,0.5);
    glSphere(-0.5,-0.5,-0.5,0.5);*/

    //glSphere(0,0,0,0.5);

    glSphereWithMat(-2.5,-2,0,1,1,1,1,1,1,1,0.5);

    glFlush (); // Ensures any previous OpenGL call has been executed
    glutSwapBuffers ();  // swap the render buffer and the displayed (screen) one
}

void keyboard (unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
	case 'w':
		GLint mode[2];
		glGetIntegerv (GL_POLYGON_MODE, mode);
		glPolygonMode (GL_FRONT_AND_BACK, mode[1] ==  GL_FILL ? GL_LINE : GL_FILL);
        break;
    case '1':
        glEnable (GL_LIGHT0);
        break;
    case '2':
        glEnable (GL_LIGHT1);
        break;
    case '3':
        glEnable (GL_LIGHT2);
        break;
    case '4':
        glDisable (GL_LIGHT0);
        break;
    case '5':
        glDisable (GL_LIGHT1);
        break;
    case '6':
        glDisable (GL_LIGHT2);
        break;
    case 'q':
    case 27:
        exit (0);
        break;
     default:
        printUsage ();
        break;
    }
    glutPostRedisplay ();
}

void mouse (int button, int state, int x, int y) {
}

void motion (int x, int y) {
}

// This function is executed in an infinite loop. It updated the window title
// (frame-per-second, model size) and ask for rendering
void idle () {
}

int main (int argc, char ** argv) {
    glutInit (&argc, argv); // Initialize a glut app
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE); // Setup a RGBA framebuffer to display, with a depth buffer (z-buffer), in double buffer mode (fill a buffer then update the screen)
    glutInitWindowSize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT); // Set the window app size on screen
    window = glutCreateWindow (appTitle.c_str ()); // create the window
    init (); // Your initialization code (OpenGL states, geometry, material, lights, etc)
    glutReshapeFunc (reshape); // Callback function executed whenever glut need to setup the projection matrix
	glutDisplayFunc (display); // Callback function executed when the window app need to be redrawn
    glutKeyboardFunc (keyboard); // Callback function executed when the keyboard is used
    glutMouseFunc (mouse); // Callback function executed when a mouse button is clicked 
	glutMotionFunc (motion); // Callback function executed when the mouse move
	glutIdleFunc (idle); // Callback function executed continuously when no other event happens (good for background procesing or animation for instance).
    printUsage (); // By default, display the usage help of the program   
    glutMainLoop ();
    return 0;
}

