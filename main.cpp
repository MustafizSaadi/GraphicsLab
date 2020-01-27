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

static int slices = 16;
static int stacks = 16;

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

void drawline0(int x0,int y0,int x1,int y1){
    int dx = x1-x0, dy = y1-y0;
    int x=x0,y=y0, d= 2*dy - dx, dE = 2*dy, dNE = 2*(dy-dx);
    glVertex2i(x, y);
    while(x<x1){
        if(d<0) // east
            x++, d+= dE;
        else // north-east
            x++,y++, d+= dNE;
        glVertex2i(x, y);
    }

}

void drawline1(int x0,int y0,int x1,int y1){
    int dx = x1-x0, dy = y1-y0;
    int x=x0,y=y0, d= dy - 2*dx, dN = - 2*dx, dNE = 2*(dy-dx);
    glVertex2i(x, y);
    while(y<y1){
        if(d<0) // north-east
            x++, y++, d+= dNE;
        else // north
            y++, d+= dN;
        glVertex2i(x, y);
    }

}


void drawline2(int x0,int y0,int x1,int y1){
    int dx = x1-x0, dy = y1-y0;
    int x=x0,y=y0, d= -(2*dx + dy), dN = - 2*dx, dWN = -2*(dy+dx);
    glVertex2i(x, y);
    while(y<y1){
        if(d<0) // north
            y++, d+= dN;
        else // west-north
            x--,y++, d+= dWN;
        glVertex2i(x, y);
    }

}

void drawline3(int x0,int y0,int x1,int y1){
    int dx = x1-x0, dy = y1-y0;
    int x=x0,y=y0, d= -(dx + 2*dy), dW = - 2*dy, dWN = -2*(dy+dx);
    glVertex2i(x, y);
    while(x>x1){
        if(d<0) // west-north
            x--,y++, d+= dWN;
        else // west
            x--,d+= dW;
        glVertex2i(x, y);
    }

}

void drawline4(int x0,int y0,int x1,int y1){
    int dx = x1-x0, dy = y1-y0;
    int x=x0,y=y0, d= dx - 2*dy, dW = - 2*dy, dSW = 2*(dx-dy);
    glVertex2i(x, y);
    while(x>x1){
        if(d>0) // south-west
            x--,y--, d+= dSW;
        else // west
            x--,d+= dW;
        glVertex2i(x, y);
    }

}

void drawline5(int x0,int y0,int x1,int y1){
    int dx = x1-x0, dy = y1-y0;
    int x=x0,y=y0, d= 2*dx - dy, dS = 2*dx, dSW = 2*(dx-dy);
    glVertex2i(x, y);
    while(y>y1){
        if(d>0) // south
            y--, d+= dS;
        else // south-west
            x--,y--,d+= dSW;
        glVertex2i(x, y);
    }

}

void drawline6(int x0,int y0,int x1,int y1){
    int dx = x1-x0, dy = y1-y0;
    int x=x0,y=y0, d= dy + 2*dx, dS = 2*dx, dSE = 2*(dx+dy);
    glVertex2i(x, y);
    while(y>y1){
        if(d>0) // south-east
            x++,y--, d+= dSE;
        else // south
            y--,d+= dS;
        glVertex2i(x, y);
    }

}

void drawline7(int x0,int y0,int x1,int y1){
    int dx = x1-x0, dy = y1-y0;
    int x=x0,y=y0, d= 2*dy + dx, dE = 2*dy, dSE = 2*(dx+dy);
    glVertex2i(x, y);
    while(x<x1){
        if(d>0) // east
            x++,d+= dE;
        else // south-east
            x++,y--,d+= dSE;
        glVertex2i(x, y);
    }

}

void zonefinding(int x0,int y0,int x1,int y1 ){
    int dx = x1-x0, dy = y1-y0;
    if(dx>0 && dy>=0){
        if(abs(dx) > abs(dy))
            return drawline0(x0,y0,x1,y1);
        else
            return drawline1(x0,y0,x1,y1);
    }
    if(dx<=0 && dy>0){
        if(abs(dx) > abs(dy))
            return drawline3(x0,y0,x1,y1);
        else
            return drawline2(x0,y0,x1,y1);
    }
    if(dx<0 && dy<=0){
        if(abs(dx) > abs(dy))
            return drawline4(x0,y0,x1,y1);
        else
            return drawline5(x0,y0,x1,y1);

    }
    else{
        if(abs(dx) > abs(dy))
            return drawline7(x0,y0,x1,y1);
        else
            return drawline6(x0,y0,x1,y1);
    }


}


static void display(void){
    int x0 = 0, y0 = 0, x1 = -30, y1 =-1;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    glBegin(GL_LINES);
    glVertex2i(-320, 0);
    glVertex2i(319, 0);
    glVertex2i(0, -240);
    glVertex2i(0, 239);
    glEnd();
    glBegin(GL_POINTS);
    
    zonefinding(x0,y0,x1,y1);

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