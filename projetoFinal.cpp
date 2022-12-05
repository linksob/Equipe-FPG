/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>


float year = 0, day = 0, revolution = 0, earthDist = 3.0, moonDist = 0.9;
static int view = 15, x = 0, velocity = 1;
//x é a variável que rege quem sofrerá a mudança de massa (x = 0 -> weight sun increase; x = 1 -> weight sun decrease; x = 2 -> weight earth increase; x = 3 -> weight earth decrease)


void init(void)
{
   float lightposition[] = {0, 0, 0, 1};
   glLightfv(GL_LIGHT0, GL_POSITION, lightposition);


   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);

}

void reshape(int w,int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void pointOfView(){
    gluLookAt (0, view, 3, 0.0, 0.0, 0.0, 0, 1.0, 0); //posição da câmera
}

void drawSun(){
    //Propriedades do material
    GLfloat mat_colorSun[] = {1.0, 1.0, 0.0, 0.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_colorSun);

    //Transformações geométricas
    glTranslatef(0,0,-4);
    glutSolidSphere(1.5, 20, 16);   /* draw sun */
}

void drawEarth(){
    //Propriedades do material
    GLfloat mat_colorEarth[] = {0.0, 0.0, 1.0, 0.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_colorEarth);

    //Transformações geométricas
    glRotatef ((GLfloat) year, 0.0, 1.0, 0.0);
    glTranslatef (earthDist, 0.0, 0.0);
    glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);
    glutSolidSphere(0.6, 20, 16);    /* draw earth */
}

void drawMoon(){
    //Propriedades do material
    GLfloat mat_colorMoon[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_colorMoon);

    //Transformações geométricas
    glTranslatef (moonDist, 0.0, 0.0);
    glutSolidSphere(0.2, 20, 16);  /* draw moon */
    glPopMatrix();
}

void incrementation(){
   //incremento na translação da terra (em torno do sol
   year = year + velocity*0.1;
   if(year > 360) year = year - 360;

   //incremento na rotação da terra
   day = day + velocity*0.5;
   if(day > 360) day = day - 360;
}

void desenhoOrbita(GLdouble tamanho){
    //Orbita da terra/
    //Propriedades do material
    GLfloat mat_colorOrbit[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_colorOrbit);

    GLUquadric *disk;
    disk = gluNewQuadric();
    glPushMatrix();
    glColor3f(1.0, 0.0, 1.0);
    glRotatef(89, 1, 0, 0);
    glTranslatef(0, 0, 0);
    gluDisk(disk, tamanho-0.05, tamanho, 600, 600);
    glPopMatrix();
    gluDeleteQuadric(disk);
}

void display(void)
{
   	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa cada posição anterior
   	glClearColor (0.0,0.0,0.0,1.0);
   	glLoadIdentity();
   	pointOfView();
   	drawSun();
   	desenhoOrbita(earthDist);
   	drawEarth();
   	drawMoon();
   	incrementation();
   	glutSwapBuffers(); //swap the buffers
}


void gravityChange(int x){

   switch (x) {
      case 0:
         earthDist = earthDist*(1.333);
         break;
      case 1:
         if(earthDist >= 3){
            earthDist = earthDist*(0.75);
         }
      	 break;
      case 2:
         if(moonDist < 1.2)
      	 moonDist = moonDist + 0.1;
         break;
      case 3:
         if(moonDist >= 0.9){
            moonDist = moonDist - 0.1;
         }
         break;
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 'v':
      case 'V':
          if(view == 0){
            view = 15;
          }else{
            view = 0;
          }
          break;
      case 'a':
      case 'A':
      	  gravityChange(0);
      	  break;
      case 's':
      case 'S':
      	  gravityChange(1);
      	  break;
      case 'd':
      case 'D':
      	  gravityChange(2);
      	  break;                            //tratar excessões -> velocity negativa, terra entrando no sol...
      case 'f':
      case 'F':
      	  gravityChange(3);
      	  break;
      case 'h':
      case 'H':
          velocity = velocity + 2;
          break;
      case 'l':
      case 'L':
          if(velocity > 2){
             velocity = velocity - 2;
          }
          break;
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (1000, 1000);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutIdleFunc(display);
   glutMainLoop();
   return 0;
}
