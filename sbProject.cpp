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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"



float year = 0, day = 0, revolution = 0, earthDist = 3.0, moonDist = 0.9;
static int view = 15, x = 0, velocity = 1;
//x é a variável que rege quem sofrerá a mudança de massa (x = 0 -> weight sun increase; x = 1 -> weight sun decrease; x = 2 -> weight earth increase; x = 3 -> weight earth decrease)

unsigned char* loadImg(void){

    int width, height, channels;
    unsigned char *img = stbi_load("stb_image/textures/sun.jpg", &width, &height, &channels, 0);
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

    return img;
}

void init(void)
{
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };
   GLfloat light_position[] = { 1.0, 0.0, 0.0, 0.0 };

   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_position);

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

    //unsigned char* img = loadImg();
    //glEnable(GL_TEXTURE_2D);
        //glBindTexture(GL_TEXTURE_2D, img);
        glTranslatef(0,0,-4);
        glutSolidSphere(1.5, 20, 16);   /* draw sun */
    //glDisable(GL_TEXTURE_2D);

}

void drawEarth(){

    glRotatef ((GLfloat) year, 0.0, 1.0, 0.0);
    glTranslatef (earthDist, 0.0, 0.0);
    glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);
    glutSolidSphere(0.6, 20, 16);    /* draw earth */

}

void drawMoon(){

    glTranslatef (moonDist, 0.0, 0.0);
    ///glRotatef ((GLfloat) revolution, 0.0, 1.0, 0.0);
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

   //incremento na translação da lua (em torno da terra)
   //revolution = revolution + 3;
   //if(revolution > 360) revolution = revolution - 360;
}

void desenhoOrbita(GLdouble tamanho){
    //Orbita da terra/
    GLUquadric *disk;
    disk = gluNewQuadric();
    glPushMatrix();
    glColor3f(1.0, 0.0, 1.0);
    glRotatef(89, 1, 0, 0);
    glTranslatef(0, 0, 0);
    gluDisk(disk, tamanho-0.1, tamanho, 600, 600);
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
         earthDist = earthDist + 1;
         break;
      case 1:
      	 earthDist = earthDist - 1;
      	 break;
      case 2:
      	 moonDist = moonDist + 0.1;
         break;
      case 3:
         moonDist = moonDist - 0.1;
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
          if(view == 0){
            view = 15;
          }else{
            view = 0;
          }
          break;
      case 'a':
      	  gravityChange(0);
      	  break;
      case 's':
      	  gravityChange(1);
      	  break;
      case 'd':
      	  gravityChange(2);
      	  break;                            //tratar excessões -> velocity negativa, terra entrando no sol...
      case 'f':
      	  gravityChange(3);
      	  break;
      case 'h':
          velocity = velocity + 2;
          break;
      case 'l':
          velocity = velocity - 2;
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
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutIdleFunc(display);
   glutMainLoop();
   return 0;
}
