/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include<math.h>
#include <bits/stdc++.h>
using namespace std;

static int slices = 16;
static int stacks = 16;
/* GLUT callback Handlers */
struct st{
    float r,g,b;
};

struct ob
{
    float x, y, z;
    float r, g,b,ka,kd,ks;
    float radius;
    int shiny;
};

int xmin = -320, xmax = 319, ymin = -240, ymax = 239;

static void resize(int width, int height){
    const float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(xmin, xmax, ymin, ymax, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

double quad_solve(double A, double B, double d, string sign){
    if(sign == "plus"){
        return (-B + sqrt(d))/(2*A); 
    }
    if(sign == "minus"){
        return (-B - sqrt(d))/(2*A);
    }
}

double dist(double x, double y, double z){
    return sqrt((x*x) + (y*y) + (z*z));
}




static void display(void){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    // glBegin(GL_LINES);
    // glVertex2i(xmin, 0);
    // glVertex2i(xmax, 0);
    // glVertex2i(0, ymin);
    // glVertex2i(0, ymax);
    // glEnd();
    glBegin(GL_POINTS);
    int totalx = abs(xmin) + xmax + 1;
    int totaly = abs(ymin) + ymax + 1;
    int offsetx = abs(xmin);
    int offsety = abs(ymin);
    st pixel[totalx][totaly];

    double copx = 10, copy = 50, copz = 0, zp = -500, lsx = -200, lsy = 400, lsz =-100;
    double isr = 0.8, isg = 0.8, isb = 0.8, iar = 0.1, iag = 0.1, iab = 0.1;

    for (int i = xmin; i <= xmax; i++)
    {
        for (int j = ymin; j <= ymax; j++)
        {
            pixel[i+offsetx][j+offsety].r = 0, pixel[i+offsetx][j+offsety].g = 0, pixel[i+offsetx][j+offsety].b = 0;
        }
        
    }

    string file = "object.txt";
    ifstream input(file) ;
    // string data;
    int obnum;
    input>>obnum;
    //cout << obnum <<endl;
    ob object[obnum];
    for (int k = 0; k < obnum; k++)
    {
        input>> object[k].x>> object[k].y>> object[k].z>> object[k].radius>> object[k].r >> object[k].g >> object[k].b >> object[k].ka >> object[k].kd >> object[k].ks >> object[k].shiny;

        //cout << object[k].x<< object[k].y<< object[k].z<< object[k].radius<< object[k].r << object[k].g << object[k].b << endl;
    }

    for (int i = xmin; i <= xmax; i++)
    {
        for (int j = ymin; j <= ymax; j++)
        {
            double t= INT_MAX;
            int obcnt = -1;
            for (int k = 0; k < obnum; k++)
            {
               double A = pow(i-copx,2) + pow(j-copy,2) + pow(zp-copz,2);
               double B = 2*(((copx-object[k].x)*(i-copx)) + ((copy-object[k].y)*(j-copy)) + ((copz-object[k].z)*(zp-copz)));
               double C = pow(copx-object[k].x,2) + pow(copy-object[k].y,2) + pow(copz-object[k].z,2) - pow(object[k].radius,2);
               double d = (B*B) - (4*A*C);
               if(d >= 0){
                    double t1 = quad_solve(A,B,d,"plus");
                    double t2 = quad_solve(A,B,d,"minus"); 
                    if(t1 < t)
                        t = t1, obcnt = k;
                    if(t2 < t)
                        t = t2, obcnt = k;
               }
            }
            if(t!=INT_MAX){
                double px = copx + t*(i-copx);
                double py = copy + t*(j-copy);
                double pz = copz + t*(zp-copz);

                double nx = px - object[obcnt].x;
                double ny = py - object[obcnt].y;
                double nz = pz - object[obcnt].z;

                double lx = lsx - px;
                double ly = lsy - py;
                double lz = lsz - pz;

                double length1 = dist(lx,ly,lz);
                lx /= length1;
                ly /= length1;
                lz /= length1;

                double length2 = dist(nx,ny,nz);
                nx /= length2;
                ny /= length2;
                nz /= length2;

                double dot = ((nx*lx) + (ny*ly) +(nz*lz));

                double ir = (object[obcnt].ka*iar) + (object[obcnt].kd*isr*fmax(0,dot));
                double ig = (object[obcnt].ka*iag) + (object[obcnt].kd*isg*fmax(0,dot));
                double ib = (object[obcnt].ka*iab) + (object[obcnt].kd*isb*fmax(0,dot));
                if(obcnt == 0){
                    pixel[i+offsetx][j+offsety].r = object[obcnt].r * (ir);
                    pixel[i+offsetx][j+offsety].g = object[obcnt].g * (ig);
                    pixel[i+offsetx][j+offsety].b = object[obcnt].b * (ib);
                }
                else if(obcnt == 1){
                    double vx = copx - px;
                    double vy = copy - py;
                    double vz = copz - pz;

                    double lengthv = dist(vx,vy,vz);
                    vx/= lengthv;
                    vy/= lengthv;
                    vz/= lengthv;

                    double hx = (lx + vx)/2;
                    double hy = (ly + vy)/2;
                    double hz = (lz + vz)/2;

                    dot = ((nx*hx) + (ny*hy) +(nz*hz));

                    ir += object[obcnt].ks*isr*pow(fmax(0,dot),object[obcnt].shiny);
                    ig += object[obcnt].ks*isg*pow(fmax(0,dot),object[obcnt].shiny);
                    ib += object[obcnt].ks*isb*pow(fmax(0,dot),object[obcnt].shiny);

                    pixel[i+offsetx][j+offsety].r = object[obcnt].r * (ir);
                    pixel[i+offsetx][j+offsety].g = object[obcnt].g * (ig);
                    pixel[i+offsetx][j+offsety].b = object[obcnt].b * (ib);
                }
            }
            glColor3d(pixel[i+offsetx][j+offsety].r,pixel[i+offsetx][j+offsety].g,pixel[i+offsetx][j+offsety].b);
            glVertex2i(i, j);

        }
        
    }


    

    

    
    
    glEnd();
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Experiment 01");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutMainLoop();



    return EXIT_SUCCESS;
}