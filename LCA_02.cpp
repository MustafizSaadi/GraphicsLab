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

static int slices = 16;
static int stacks = 16;
static int top = 8, bottom = 4, right = 2, left = 1;

int xmin = -100, xmax = 130, ymin = -100, ymax = 100, flag = 0, x00, y00, x11, y11, flag1 =0 , flagAC = 0, flagPart = 0, flagReg = 0; 

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
        if (flag % 2 == 1 ) {
        x00 = mx-320;
        y00 = -my+240;
        }
        else if (flag % 2 == 0)
        {
        x11 = mx-320;
        y11 = -my+240;
        flagAC=0,flagPart=0,flagReg=0;
        }
    }
    // if (flag == 2)
    // {
   
    // findzone(0,0,50,-10);
    // }
}

int makecode(int x,int y){
    int code=0;
    if(y>ymax)
        code += 8;
    else if(y<ymin)
        code += 4;
    if(x>xmax)
        code += 2;
    else if(x<xmin)
        code += 1;
    return code;
} 

void draw8way(int x,int y,int zone){
    switch(zone){
        case 0:
            //glColor3d(1,1,1);
            glVertex2i(x, y);
            break;
        case 1:
            //glColor3d(0,0,1);
            glVertex2i(y, x);
            break;
        case 2:
            //glColor3d(0,1,0);
            glVertex2i(-y, x);
            break;
        case 3:
            //glColor3d(1,0,0);
            glVertex2i(-x, y);
            break;
        case 4:
            //glColor3d(0,1,1);
            glVertex2i(-x, -y);
            break;
        case 5:
            //glColor3d(0.5,0.5,0);
            glVertex2i(-y, -x);
            break;
        case 6:
            //glColor3d(1,0,1);
            glVertex2i(y, -x);
            break;
        case 7:
            //glColor3d(0.7,0,0.7);
            glVertex2i(x, -y);
            break;

    }

}

void drawline0(int x0,int y0,int x1,int y1,int zone){
    int dx = x1-x0, dy = y1-y0;
    int x=x0,y=y0, d= 2*dy - dx, dE = 2*dy, dNE = 2*(dy-dx);
    draw8way(x, y,zone);
    while(x<x1){
        if(d<0) // east
            x++, d+= dE;
        else // north-east
            x++,y++, d+= dNE;
        draw8way(x, y,zone);
    }

}

void zonefinding(int x0,int y0,int x1,int y1 ){
    int dx = x1-x0, dy = y1-y0;
    if(dx>0 && dy>=0){
        if(abs(dx) > abs(dy)){
            // if(flag2)
            //     printf("Zone 0 white\n");
            return drawline0(x0,y0,x1,y1,0);
        }
        else{
            // if(flag2)
            //     printf("Zone 1 blue\n");
            return drawline0(y0,x0,y1,x1,1);
        }
    }
    if(dx<=0 && dy>0){
        if(abs(dx) > abs(dy)){
            // if(flag2)
            //     printf("Zone 3 red\n");
            return drawline0(-x0,y0,-x1,y1,3);
        }
        else{
            // if(flag2)
            //     printf("Zone 2 green\n");
            return drawline0(y0,-x0,y1,-x1,2);
        }
    }
    if(dx<0 && dy<=0){
        if(abs(dx) > abs(dy)){
            // if(flag2)
            //     printf("Zone 4 sky blue\n");
            return drawline0(-x0,-y0,-x1,-y1,4);
        }
        else{
            // if(flag2)
            //     printf("Zone 5 yellow\n");
            return drawline0(-y0,-x0,-y1,-x1,5);
        }

    }
    else{
        if(abs(dx) > abs(dy)){
            // if(flag2)
            //     printf("Zone 7 violet\n");
            return drawline0(x0,-y0,x1,-y1,7);
        }
        else{
            // if(flag2)
            //     printf("Zone 6 pink\n");
            return drawline0(-y0,x0,-y1,x1,6);
        }
    }


}

float symmetricRoundoff(float bound, float x0, float y0, float x1, float y1){
    return round((bound-y0)*((x1-x0)/(y1-y0)));
}

void cohenSutherland(int x0,int y0,int x1,int y1){
    int code0,code1,code,x,y;

    code0 = makecode(x0,y0);
    code1 = makecode(x1,y1);

    while(1){
        if((code0 | code1) == 0){ //fully accepted
            if(!flag1){
                glColor3d(1,0,0);
                // glVertex2i(x0, y0);
                // glVertex2i(x1, y1);
                zonefinding(x0,y0,x1,y1);
                // glColor3d(0,1,0);
                // glVertex2i(x0, y0);
                // glVertex2i(x1, y1);
                if(!flagAC){
                    flagAC=1;
                    printf("Accepted\n");
                }
                break;
            }
            else{
                glColor3d(0,1,0);
                zonefinding(x0,y0,x1,y1);
                glPointSize(10);
                glColor3d(0,0,1);
                if(!(x00 == x0 && y00 == y0))
                    glVertex2i(x0, y0);
                if(!(x11 == x1 && y11 == y1))
                    glVertex2i(x1, y1);
                if(!flagPart){
                    flagPart=1;
                    printf("Partially Accepted\n");
                }
                break;
            }
        }
        else if(code0 & code1){ // fully rejected
            if(!flagReg){
                flagReg=1;
                printf("Rejected\n");
            }
            break;
        }
        else{ //partially accepted
            flag1 = 1;
            if(code0) code = code0;
            else code = code1;

            if(code & top){
                y = ymax;
                x = x0 + symmetricRoundoff(ymax,x0,y0,x1,y1);
            }
            else if(code & bottom){
                y = ymin;
                x = x0 + symmetricRoundoff(ymin,x0,y0,x1,y1);
            }
            else if(code & right){
                x = xmax;
                y = y0 + symmetricRoundoff(xmax,y0,x0,y1,x1);
            }
            else{
                x = xmin;
                y = y0 + symmetricRoundoff(xmin,y0,x0,y1,x1);
            }

            if(code == code0){
                x0 = x, y0 = y;
                code0 = makecode(x0,y0);
                glVertex2i(x0, y0);
            }
            else{
                x1 = x,y1 = y;
                code1 = makecode(x1,y1);
                glVertex2i(x0, y0);
            }

        }
    }
}

static void display(void){
    int x = 10, y = 20;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    glPointSize(3);
    glBegin(GL_LINES);
    // glVertex2i(-320, 0);
    // glVertex2i(319, 0);
    // glVertex2i(0, -240);
    // glVertex2i(0, 239);
    glVertex2i(-320, 100); //upper horizontal line
    glVertex2i(319, 100); // upper horizontal line
    glVertex2i(-100, -240); // left vertical line
    glVertex2i(-100, 239); // left vertical line
    glVertex2i(-320, -100); // lower horizontal line
    glVertex2i(319, -100); // lower horizontal line
    glVertex2i(130, -240); // right vertical line
    glVertex2i(130, 239);  // right vertical line
    glEnd();
    glBegin(GL_POINTS);
    // glVertex2i(x, y+1);
    // glVertex2i(x, y);
        if(flag % 2 ==0 && flag!=0){
            //glVertex2i(x00, y00);
            //glVertex2i(x11, y11);
            flag1 = 0;
            zonefinding(x00,y00,x11,y11);
            cohenSutherland(x00,y00,x11,y11);
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