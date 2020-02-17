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
#include <math.h>
#include<bits/stdc++.h>
using namespace std;

vector<int>vec;

static int slices = 16;
static int stacks = 16;

 int x00 = 0, y00 = 0, flagup = 0, flying = 1, addy1 = 7, a = 80, b = 80,time1=0,time2=0,gtime=0,u=2, v=100, xmin = -320,ymin = -240,xmax = 319 ,ymax = 239, flag = 0, yprev = 0;

/* GLUT callback Handlers */

static void resize(int width, int height){
    const float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-320, 319, -240, 239, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

void mouse(int button, int state, int mousex, int mousey)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        int mx = mousex;
        int my = mousey;
        printf(" c1 %d %d\n",mx-320,-my+240);
        glutPostRedisplay();
        flag++;
        if (flag == 1) {
        x00 = mx-320;
        y00 = -my+240;
        if(y00 + b <= ymax)
            ymax = y00 + b;

        printf("%d %d %d\n",x00,y00,ymax);
        }
    }
    // if (flag == 2)
    // {
   
    // findzone(0,0,50,-10);
    // }
}

void draw4way(int xc, int yc, int x, int y){
    glVertex2i(xc+x, yc+y);
    glVertex2i(xc+x, yc-y);
    glVertex2i(xc-x, yc+y);
    glVertex2i(xc-x, yc-y);
}

void drawEllipseOb(int xc, int yc, int a, int b){
    int x=0, y=b, d=(4*b*b)-(4*a*a*b)+(a*a);
    draw4way(xc,yc,x,y);
    while(a*a*(2*y - 1) > 2*b*b*(x+1)){
        if(d<0){ //dE
            x++;
            d += 4*b*b*((2*x) + 3);
        }
        else{ // dSE
            x++,y--;
            d += 4*(b*b*((2*x)+3) + (a*a*(2-(2*y))));
        }
        draw4way(xc,yc,x,y);
    }
    while(y>0){
        if(d<0){ //dSE
            x++,y--;
            d += 4*(b*b*((2*x)+2) + (a*a*(3-(2*y))));
        }
        else{
            y--;
            d += 4*a*a*(3 -(2*y));
        }
        draw4way(xc,yc,x,y);
    }
}

static void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    glBegin(GL_LINES);
    glVertex2i(-320, 0);
    glVertex2i(319, 0);
    glVertex2i(0, -240);
    glVertex2i(0, 239);

    glEnd();
    glBegin(GL_POINTS);

    if(flag){

    if(flying){
        if(!flagup){
            y00 = (ymax - b) - ((u*gtime*gtime)/20);
            gtime++;
            vec.push_back(y00);
        }
        else{
            y00 = vec.back() + 5;
            printf("%d\n",y00);
            vec.pop_back();
            // y00 = -160 + ((u*gtime*gtime)/200);
            // gtime--;
        }
        //gtime ++;
        //printf("gtime %d\n",gtime);
    }

    drawEllipseOb(x00,y00,a,b);

    if(y00 + b > ymax){
        // flying = 0;
        // if(time1<20){
        //     addy1 = 241 - y00 - b + round(0.1*b);
        //     b -= round(0.1*b);
        //     time1 ++;
        //     // if(time1 == 19)
        //     //     b=80,addy1 = -10,time1=0;
        // }
        // else if(time2<20){
        //     addy1 = 241 - y00 - round(b/0.9);
        //     b = round(b/0.9);
        //     time2 ++;
        //     if(time2==20)
        //         time1 = 0, time2 = 0, b = 80, flying = 1, flagup = 0, gtime = 1;
        // }

        time1 = 0, time2 = 0, b = 80, flying = 1, flagup = 0, gtime = 0;

        //y00 += addy1 ;
        
        // flagdown = 0;
        // addy1 = -10;
    }
    else if(y00 - b < ymin){
        //printf("%d\n", gtime);
        flying = 0;
        //printf(" %d %d %d %d\n", y00,time1,time2,b);
        if(time1<10){
            //addy1 = -241 - y00 + b - 8;
            if(time1 == 0)
                yprev = y00;
            y00 -= 8;
            b -= 8;
            time1 ++;
            // if(y00<ymin){
            //     y00 = ymin - 1, time1 = 10;
            //     printf("1 %d %d %d %d\n",y00,time1,time2,b);
            // }
            // if(time1 == 19)
            //     b=80,addy1 = -10,time1=0;
        }
        else if(y00<=yprev){
            //addy1 = -241 - y00 + b + 8;
            y00 += 8;
            b += 8;
            time2 ++;
            if(b>80)
                b = 80;
            if(y00>yprev){
                time1 = 0, time2 = 0, b = 80, flying = 1, flagup = 1,gtime=0;
                //printf("1\n");
            }
            //printf("%d\n",y00);
            // if(time2==11)
            //     time1 = 0, time2 = 0, b = 80, flying = 1, flagup = 1, gtime = 0;
        }
        //y00 += addy1 ;
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
    glutMouseFunc(mouse);
    glutMainLoop();

    return EXIT_SUCCESS;
}