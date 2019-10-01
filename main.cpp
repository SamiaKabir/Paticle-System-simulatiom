//
//  main.cpp
// Particle_system
//
//  Created by Samia Kabir on 8/28/19.
//  Copyright © 2019 Samia Kabir. All rights reserved.
//

#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>
#include<unistd.h>
#include <vector>
#include "Eigen/Dense"
#include <iostream>
#include <chrono>
#include <ctime>
#include "Particle.h"
#include <random>
#include "materials.h"

using namespace std;
using namespace Eigen;

double ballx,bally,ballz;

int rotateon;

double xmin, xmax, ymin, ymax, zmin, zmax;
double maxdiff;

int lastx, lasty;
int xchange, ychange;
float spin = 0.0;
float spinup = 0.0;


vector<Particle> all_particles;
Vector3d point;

void simulation_loop(int i);
void Generator(int i);

//all probabilistic generator and distributor
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(0);
uniform_real_distribution<double> distributionx(-130.0,120.0);
uniform_real_distribution<double> distributiony(70.0,75.0);
uniform_real_distribution<double> distributionz(90,-80.0);
uniform_real_distribution<double> distributionv(0.0,2.0);
uniform_real_distribution<double> distribution_r(0.2,0.35);
uniform_real_distribution<double> distribution_mass(0.01,0.001);

void pyramid();
GLint LoadGLTexture(const char *filename, int width, int height);
GLuint texture_snow;
GLUquadricObj *sphere = NULL;


void reshape(int w,int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
    //glOrtho(-25,25,-2,2,0.1,100);
    glMatrixMode(GL_MODELVIEW);
}





void display(void)
{
    
    
    GLfloat base_ambient[] = { 0.1, 0.1, 0.1 };
    GLfloat base_diffuse[] = { 0.4, 0.4, 0.4 };
    GLfloat base_specular[] = { 0.1, 0.1, 0.1 };
    GLfloat base_shininess[] = { 0.1 };
    
    GLfloat smallrgb[]={0.1,0,0};
    GLfloat smallrgb2[]={0.0,0.1,0.2};
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef(spinup, 1.0, 0.0, 0.0);
    glRotatef(spin, 0.0, 1.0, 0.0);
    
    //draw the object


     pyramid();
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, base_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, base_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, base_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, base_shininess);

    //draw the base
    glBegin(GL_QUADS);
    glMaterialfv(GL_FRONT, GL_AMBIENT, smallrgb2);
    glVertex3f(-100, 0, 90);
    glVertex3f(100, 0, 90);
    glVertex3f(100, 0, -80);
    glVertex3f(-100, 0, -80);
    glEnd();
    
    //glPopMatrix();

    
   // glPushMatrix();
    
   // draw the particles


    for(int i=0;i<all_particles.size();i++){
        all_particles.at(i).draw(sphere,texture_snow);
    }


    glPopMatrix();



    glutSwapBuffers();
    

}

void init(void)
{
    glClearColor(0.1, 0.2, 0.2, 0.5);
  
    glColor3f(1.0, 1.0, 1.0);
    // Enable Z-buffering, backface culling, and lighting
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_FLAT);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 1, 600);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Set eye point and lookat point
  //  gluLookAt(0,50, 200, 0, 0, 0, 0, 1, 0);
    
     gluLookAt(0,50, 120, 0, 0, 0, 0, 1, 0);
    // Set up lights
    GLfloat light0color[] = { 1,1,1 };
    GLfloat light0pos[] = { -150, 150, 150 };
    GLfloat light1color[] = { 1,1,1};
    GLfloat light1pos[] = { 150, 150, -150 };
    glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0color);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0color);
    glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1color);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1color);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1color);

    texture_snow=LoadGLTexture("snow.png", 500, 500);
    sphere = gluNewQuadric();
    
    gluQuadricDrawStyle(sphere, GLU_FILL);
    
    gluQuadricTexture(sphere, GL_TRUE);
    
    gluQuadricNormals(sphere, GLU_SMOOTH);

 

  }


GLint LoadGLTexture(const char *filename, int width, int height)
{
    GLuint texture;
    unsigned char *data;
    FILE *file;
    
    // open texture data
    file = fopen(filename,"r");
    if (file == NULL) return 0;
    texture = LoadGLTexture("snow.png", 420, 420);


    // allocate buffer
    data = (unsigned char*) malloc(width * height * 4);
    
    //read texture data
    fread(data, width * height * 4, 1, file);
    fclose(file);
    
    
    // enable textures
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // sample: specify texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set the active texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 data);


    
    return texture;
}

void reshapeFunc(GLint newWidth, GLint newHeight)
{
    if (newWidth > newHeight) // Keep a square viewport
        glViewport((newWidth - newHeight) / 2, 0, newHeight, newHeight);
    else
        glViewport(0, (newHeight - newWidth) / 2, newWidth, newWidth);
    init();
    glutPostRedisplay();
}

void rotateview(void)
{
    if (rotateon) {
        spin += xchange / 250.0;
        if (spin >= 360.0) spin -= 360.0;
        if (spin < 0.0) spin += 360.0;
        spinup -= ychange / 250.0;
        if (spinup > 89.0) spinup = 89.0;
        if (spinup < -89.0) spinup = -89.0;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                lastx = x;
                lasty = y;
                xchange = 0;
                ychange = 0;
                rotateon = 1;
            }
            else if (state == GLUT_UP) {
                xchange = 0;
                ychange = 0;
                rotateon = 0;
            }
            break;
            
        default:
            break;
    }
}


void mouseClick(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        point[0] =x /(float) 700;
        point[1] = y /(float) 700;
        point[2] = 0;
    }
    printf("%f %f \n", point[0],point[1]);
}

void motion(int x, int y)
{
    xchange = x - lastx;
    ychange = y - lasty;
}

void keyboard(unsigned char key, int x, int y){
    
    switch(key){
            
        case 'g':
            Generator(0);
            break;
        
        case 's':
            simulation_loop(0);
            break;
        
        case 'b':
            simulation_loop(0);
            Generator(0);
            break;
            
        case 'r':
            wind_value(4);
            break;
            
            
        case 'h':
            set_heat();
            break;
            
        default:
            break;
            
    }
    

}


void SpecialInput(int key, int x, int y){
    
    switch(key){
        case GLUT_KEY_UP:
            wind_value(2);
            break;
            
        case GLUT_KEY_DOWN:
            wind_value(3);
            break;
            
        case GLUT_KEY_LEFT:
            wind_value(0);
            break;
            
            
        case GLUT_KEY_RIGHT:
            wind_value(1);
            break;
            
        default:
            break;
    }
    
}

void pyramid()
{
    GLfloat smallrgb[]={0.1,0,0};
    glBegin(GL_TRIANGLE_STRIP);
    glMaterialfv(GL_FRONT, GL_AMBIENT, smallrgb);
    glColor3f(1, 1, 1); glVertex3f(0, 25, 0);
    glColor3f(1, 0, 0); glVertex3f(-30, 0, 30);
    glColor3f(0, 1, 0); glVertex3f(30, 0, 30);
    glColor3f(0, 0, 1); glVertex3f(0, 0, -30);
    glColor3f(1, 1, 1); glVertex3f(0, 25, 0);
    glColor3f(1, 0, 0); glVertex3f(-30, 0, 30);
    glEnd();
    
}


//generator

//push to all_particles
void Generator(int i){
 auto start = std::chrono::system_clock::now();
    for(int j=0;j<50;j++){
    
        double pos_x= distributionx(generator);
        double pos_y=distributiony(generator);
        double pos_z=distributionz(generator);
        double vel_y= distributionv(generator);
        double radius= distribution_r(generator);
        double mass= distribution_mass(generator);
        
        Vector3d X(pos_x,pos_y,pos_z);
        Vector3d V(0,vel_y,0);
        Particle p;
        p.position[0]=X[0]; p.position[1]=X[1];  p.position[2]=X[2];
        p.velocity[0]=V[0]; p.velocity[1]=V[1];  p.velocity[2]=V[2];
        p.mass=mass;
        p.radius=radius;
        all_particles.push_back(p);
        
    }

    
    auto end = std::chrono::system_clock::now();
    
    std::chrono::duration<double> elapsed_seconds = end-start;
    
    
    if(elapsed_seconds.count()<1){
        sleep((1- elapsed_seconds.count()));
        
    }
    glutTimerFunc(0, Generator,0);
}

void simulation_loop(int i)
{
  
   auto start2 = std::chrono::system_clock::now();
    
    //edit all particles
    
    
    for(int i=0;i<all_particles.size();i++){
        all_particles.at(i).collision();
    }
    
    printf("%d \n", all_particles.size());
    
    auto end2 = std::chrono::system_clock::now();
    
    std::chrono::duration<double> elapsed_seconds2 = end2-start2;
    
    
    
    if(elapsed_seconds2.count()<0.3333){
        sleep((0.3333- elapsed_seconds2.count()));
        
    }
    
    display();
    glutTimerFunc(0, simulation_loop,0);
    
}

int main(int argc, char** argv)
{
    GLint SubMenu1, SubMenu2, SubMenu3, SubMenu4;
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Particle System");
    init();
    rotateon = 0;
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(SpecialInput);
    glutIdleFunc(rotateview);
    glutReshapeFunc(reshapeFunc);
    generate_potential_field();
    glutMainLoop();
    return 0;
}

