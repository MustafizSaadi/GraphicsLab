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

static int slices = 16;
static int stacks = 16;
int flag = 0, x00,y00,x11,y11,flag2=0;
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
        //printf(" c1 %d %d\n",mx-320,-my+240);
        glutPostRedisplay();
        flag++;
        if (flag == 1) {
        x00 = mx-320;
        y00 = -my+240;
        }
        else if (flag == 2)
        {
        x11 = mx-320;
        y11 = -my+240;
        }
    }
    // if (flag == 2)
    // {
   
    // findzone(0,0,50,-10);
    // }
}

void draw8way(int x,int y,int zone){
    switch(zone){
        case 0:
            glColor3d(1,1,1);
            glVertex2i(x, y);
            break;
        case 1:
            glColor3d(0,0,1);
            glVertex2i(y, x);
            break;
        case 2:
            glColor3d(0,1,0);
            glVertex2i(-y, x);
            break;
        case 3:
            glColor3d(1,0,0);
            glVertex2i(-x, y);
            break;
        case 4:
            glColor3d(0,1,1);
            glVertex2i(-x, -y);
            break;
        case 5:
            glColor3d(0.5,0.5,0);
            glVertex2i(-y, -x);
            break;
        case 6:
            glColor3d(1,0,1);
            glVertex2i(y, -x);
            break;
        case 7:
            glColor3d(0.7,0,0.7);
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
            if(flag2)
                printf("Zone 0 white\n");
            return drawline0(x0,y0,x1,y1,0);
        }
        else{
            if(flag2)
                printf("Zone 1 blue\n");
            return drawline0(y0,x0,y1,x1,1);
        }
    }
    if(dx<=0 && dy>0){
        if(abs(dx) > abs(dy)){
            if(flag2)
                printf("Zone 3 red\n");
            return drawline0(-x0,y0,-x1,y1,3);
        }
        else{
            if(flag2)
                printf("Zone 2 green\n");
            return drawline0(y0,-x0,y1,-x1,2);
        }
    }
    if(dx<0 && dy<=0){
        if(abs(dx) > abs(dy)){
            if(flag2)
                printf("Zone 4 sky blue\n");
            return drawline0(-x0,-y0,-x1,-y1,4);
        }
        else{
            if(flag2)
                printf("Zone 5 yellow\n");
            return drawline0(-y0,-x0,-y1,-x1,5);
        }

    }
    else{
        if(abs(dx) > abs(dy)){
            if(flag2)
                printf("Zone 7 violet\n");
            return drawline0(x0,-y0,x1,-y1,7);
        }
        else{
            if(flag2)
                printf("Zone 6 pink\n");
            return drawline0(-y0,x0,-y1,x1,6);
        }
    }


}

int a1=0,a2=0,b1=0,b2=0;

static void display(void){
    //int x0 = 0, y0 = 0, x1 = 70, y1 =-10;
   // FILE *fp;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    glBegin(GL_LINES);
    glVertex2i(-320, 0);
    glVertex2i(319, 0);
    glVertex2i(0, -240);
    glVertex2i(0, 239);
    glEnd();
    glBegin(GL_POINTS);

    // fp = fopen("data.txt","r");
    // glColor3d(1,0,0);
    // for(int i=0;i<4;i++){
    // fscanf(fp,"%d %d %d %d",&x0,&y0,&x1,&y1);
    if(flag==2){
        flag=0,flag2=1;
        a1=x00,a2 =x11, b1 = y00, b2 = y11;


    }
    //printf("%d %d %d %d\n",a1,a2,b1,b2);
    //glColor3d(1,0,0);
    zonefinding(a1,b1,a2,b2);
    flag2=0;
    //printf("hello");
    // }
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
    printf("zone 0  white\n");
    printf("zone 1  blue\n");
    printf("zone 2  green\n");
    printf("zone 3  red\n");
    printf("zone 4  sky blue\n");
    printf("zone 5  yellow\n");
    printf("zone 6  pink\n");
    printf("zone 7  violet\n");
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