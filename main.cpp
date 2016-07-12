#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <cstring>
#include <SOIL/SOIL.h>

#define NUM_PARTICLES    1000     /* Number of particles  */
#define NUM_DEBRIS       100       /* Number of debris     */
#define PAUSE              1
#define QUIT               2

#define TEXTURE_PATH "img1.png"
#define WIDTH 256
#define HEIGHT 256
GLuint texture;
GLuint LoadTextureRAW(const char * filename, int wrap);

static float DayOfYear[8] = {0.0,100.0,190.0,300.0,40.0,150.0,250.0,360.0};
static float AnimateIncrement = 24.0;  // Time step for animation (hours)

int count=0,poscount=0,cont=0;
float pos=-0.2;
int event=0;

void newExplosion();
struct particleData
{
  float   position[3];
  float   speed[3];
  float   color[3];
};
typedef struct particleData    particleData;
float radius=0.0,sredcol=1.0,sgreencol=1.0,sbluecol=0.0;

struct debrisData
{
  float   position[3];
  float   speed[3];

  float   color[3];
  float   scale[3];
};
typedef struct debrisData    debrisData;

particleData     particles[NUM_PARTICLES];
debrisData       debris[NUM_DEBRIS];
int              fuel = 0;                /* "fuel" of the explosion */
float            angle = 0.0;             /* camera rotation angle */
int      wantPause = 0;       /* Pause flag */

//timer function to end the 1st event
void myTimer(int value)
{
    event++;
    newExplosion();
    glutPostRedisplay();

}

//timer function to end the 2nd event
void myTimer1(int value)
{

    event++;
    glutPostRedisplay();

}

//timer function to end the 3rd event
void myTimer2(int value)
{

    event++;
    glutPostRedisplay();
}

//to recompute the speed of particles

void
newSpeed (float dest[3])
{
  float    x;
  float    y;
  float    z;

  x = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
  y = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
  z = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;

  dest[0] = x;
  dest[1] = y;
  dest[2] = z;
}

//Create a new explosion.
void
newExplosion (void)
{
  int    i;

  for (i = 0; i < NUM_PARTICLES; i++)
    {
      particles[i].position[0] = 0.0;
      particles[i].position[1] = 0.0;
      particles[i].position[2] = 0.0;

      particles[i].color[0] = 1.0;
      particles[i].color[1] = 1.0;
      particles[i].color[2] = 0.5;

      newSpeed (particles[i].speed);
    }

  for (i = 0; i < NUM_DEBRIS; i++)
    {
      debris[i].position[0] = 0.0;
      debris[i].position[1] = 0.0;
      debris[i].position[2] = 0.0;

      debris[i].scale[0] = (2.0 *
			    ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
      debris[i].scale[1] = (2.0 *
			    ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
      debris[i].scale[2] = (2.0 *
			    ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;

      newSpeed (debris[i].speed);

    }

  fuel = 100;
}

//to initialize display specifications of the 3rd event
void picinit()
{

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glDisable(GL_LIGHTING);
    glClearColor(0, 0, 0, 1);
    glLoadIdentity();
    gluOrtho2D(-.05, .05, -.05, .05);
    glEnable(GL_TEXTURE_2D);
    texture = SOIL_load_OGL_texture(TEXTURE_PATH,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    // check for an error during the load process
    if(texture == 0)
    printf("SOIL loading error: '%s'\n", SOIL_last_result());
}

//to initialize display specifications for the 4th event
void solarInit(void)
{
    glShadeModel( GL_FLAT );
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
}

//to animate the movement of a solar system
static void Animate(void)
{
           // Request a re-draw for animation purposes
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    int i;
    for(i=0;i<8;i++)
    {
            DayOfYear[i] += AnimateIncrement/24.0;
            DayOfYear[i] = DayOfYear[i] - ((int)((DayOfYear[i])/365)*365);
    }
    glLoadIdentity();
    // Back off eight units to be able to view from the origin.
    glTranslatef ( 0.0, 0.0, -8.0 );
    // Rotate the plane of the elliptic
    // (rotate the model's plane about the x axis by fifteen degrees)
    glRotatef(30.0, 1.0, 0.0, 0.0 );
    // Draw the sun
    glColor3f( 0.8, 0.6, 0.0 );
    glutSolidSphere( 1.0, 150, 150 );
    // Draw mercury
        // First position it around the sun
        //              Use DayOfYear to determine its position
    glPushMatrix();                                             // Save matrix state
    glRotatef( 360.0*DayOfYear[0]/365.0, 0.0, 5.0, 0.0 );
    glTranslatef( 1.5, 0.0, 0.0 );
    glColor3f( 0.5, 0.0, 0.0 );
    glutSolidSphere( 0.2, 150, 150);
    glPopMatrix();

    //venus
    glPushMatrix();                                             // Save matrix state
    glRotatef( 360.0*DayOfYear[1]/365.0, 0.0, 5.0, 0.0 );
    glTranslatef( 1.7, 0.0, 0.0 );
    glColor3f( 1.0, 0.25, 0.0 );
    glutSolidSphere( 0.4, 150, 150);
    glPopMatrix();                                              // Restore matrix state

    //earth
    glPushMatrix();                                             // Save matrix state
    glRotatef( 360.0*DayOfYear[2]/365.0, 0.0, 5.0, 0.0 );
    glTranslatef( 2.4, 0.0, 0.0 );
    glColor3f( 0.2, 0.2, 1.0 );
    glutSolidSphere( 0.5, 150, 150);
    glPopMatrix();                                              // Restore matrix state

    //mars
    glPushMatrix();                                             // Save matrix state
    glRotatef( 360.0*DayOfYear[3]/365.0, 0.0, 5.0, 0.0 );
    glTranslatef( 3.0, 0.0, 0.0 );
    glColor3f( 1.0, 0.25, 0.0 );
    glutSolidSphere( 0.4, 150, 150);
    glPopMatrix();

    //jupiter
    glPushMatrix();                                             // Save matrix state
    glRotatef( 360.0*DayOfYear[4]/365.0, 0.0, 5.0, 0.0 );
    glTranslatef( 3.7, 0.0, 0.0 );
    glColor3f( 0.64, 0.164, 0.164 );
    glutSolidSphere( 0.7, 150, 150);
    glPopMatrix();                                              // Restore matrix state

    //saturn
    glPushMatrix();                                             // Save matrix state
    glRotatef( 360.0*DayOfYear[5]/365.0, 0.0, 5.0, 0.0 );
    glTranslatef( 4.5, 0.0, 0.0 );
    glColor3f( 0.36, 0.25, 0.20 );
    glutSolidSphere( 0.5, 150, 150);
    glPopMatrix();                                              // Restore matrix state


    //uranus                                                // Restore matrix state
    glPushMatrix();                                             // Save matrix state
    glRotatef( 360.0*DayOfYear[6]/365.0, 0.0, 5.0, 0.0 );
    glTranslatef( 5.0, 0.0, 0.0 );
    glColor3f( 0.41, 0.13, 0.55 );
    glutSolidSphere( 0.35, 150, 150);
    glPopMatrix();

    //neptune
    glPushMatrix();                                             // Save matrix state
    glRotatef( 360.0*DayOfYear[7]/365.0, 0.0, 5.0, 0.0 );
    glTranslatef( 5.8, 0.0, 0.0 );
    glColor3f( 0.25, 0.25, 0.43 );
    glutSolidSphere( 0.2, 150, 150);
    glPopMatrix();                                              // Restore matrix state

    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();

}



void
display (void)
{
    int    i;
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity ();

  /* Place the camera */
  glTranslatef (0.0, 0.0, -10.0);
  glRotatef (angle, 0.0, 1.0, 0.0);

    //begin 1st event
    if(event==0)
    {
        if (fuel == 0 )
        {

            glColor3f(sredcol,sgreencol,sbluecol);
            glTranslatef(pos,0,0);
        glutSolidSphere(radius,20,20);
        }
    }
    if(event==1)
    {
        if (fuel > 0)
        {

            glPushMatrix ();
            glDisable (GL_LIGHTING);
            glDisable (GL_DEPTH_TEST);

            glBegin (GL_POINTS);

            for (i = 0; i < NUM_PARTICLES; i++)
            {
                glColor3fv (particles[i].color);
                glVertex3fv (particles[i].position);
            }

            glEnd ();

            glPopMatrix ();

            glEnable (GL_LIGHTING);
            glEnable (GL_LIGHT0);
            glEnable (GL_DEPTH_TEST);

            glNormal3f (0.0, 0.0, 1.0);

            for (i = 0; i < NUM_DEBRIS; i++)
            {

                glColor3f(0.7,.7,.7);
                glPushMatrix ();

                glTranslatef (debris[i].position[0],
                debris[i].position[1],
                debris[i].position[2]);


                glScalef (debris[i].scale[0],
                debris[i].scale[1],
                debris[i].scale[2]);


                glBegin (GL_TRIANGLES);
                glVertex3f (0.0, 0.5, 0.0);
                glVertex3f (-0.25, 0.0, 0.0);
                glVertex3f (0.25, 0.0, 0.0);
                glEnd ();

                glPopMatrix ();

        }
    }

    }

    if(event==2)
    {

            if(cont==0){

                picinit();
                cont++;
            }

            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0, 0, 0, 1);
            glRotatef(-88 ,0,1,0);
            gluOrtho2D(-.25,.25,-.25,.25);
            glBegin( GL_QUADS );
            glTexCoord2f(0.0,0.0);
            glVertex2d(-1, -1);
            glTexCoord2d(1.0,0.0);
            glVertex2d(1, -1);
            glTexCoord2d(1.0,1.0);
            glVertex2d(1, 1);
            glTexCoord2d(0.0,1.0);
            glVertex2d(-1, 1);
            glEnd();
            glFlush();

    }



    if(event==3)
    {

        solarInit();
        glDisable(GL_TEXTURE_2D);
        Animate();
    }

    glutSwapBuffers ();
}

void
keyboard (unsigned char    key,
	  int              x,
	  int              y)
{
  switch (key)
    {
    case ' ':
      newExplosion ();
      event=1;
      break;

    case 27:
      exit (0);
      break;

    case 'p':
      wantPause = 1 - wantPause;
      break;
    }
}


//Update animation variables.
 int status=0;
void
idle (void)
{
    int    i;
    if(!status)
    {
        if(radius>2.0)
        {

            status=1;
        }
        else
        {
            poscount++;
            if((poscount%2)!=0)
                pos=0;
            else
                pos=-0.2;
                radius=radius+0.002;
                sgreencol-=0.0008;
        }

    }

    //update variables for the 2nd event
    if(event==1)
    {
        if (!wantPause)
        {
            if (fuel > 0)
            {
                for (i = 0; i < NUM_PARTICLES; i++)
                {
                    particles[i].position[0] += particles[i].speed[0] * 0.2;
                    particles[i].position[1] += particles[i].speed[1] * 0.2;
                    particles[i].position[2] += particles[i].speed[2] * 0.2;

                    particles[i].color[0] -= 1.0 / 500.0;
                    if (particles[i].color[0] < 0.0)
                    {
                        particles[i].color[0] = 0.0;
                    }

                    particles[i].color[1] -= 1.0 / 100.0;
                    if (particles[i].color[1] < 0.0)
                    {
                        particles[i].color[1] = 0.0;
                    }

                    particles[i].color[2] -= 1.0 / 50.0;
                    if (particles[i].color[2] < 0.0)
                    {
                        particles[i].color[2] = 0.0;
                    }


        }

        for (i = 0; i < NUM_DEBRIS; i++)
	    {
	      debris[i].position[0] += debris[i].speed[0] * 0.1;
	      debris[i].position[1] += debris[i].speed[1] * 0.1;
	      debris[i].position[2] += debris[i].speed[2] * 0.1;

	    }

	  --fuel;
	  if(fuel==0)
	  fuel++;
	  if(!fuel)
	  exit(0);
	}

      angle += 0.3;  /* Always continue to rotate the camera */
    }
}

    glutPostRedisplay ();
}

//function to reshape the window
void
reshape (int    w,
	 int    h)
{
  glViewport (0.0, 0.0, (GLfloat) w, (GLfloat) h);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (45.0, (GLfloat) w / (GLfloat) h, 0.1, 100.0);

  glMatrixMode (GL_MODELVIEW);
}

//menu function
void
menuSelect (int value)
{
  switch (value)
    {
        //pause the animation
    case PAUSE:
      wantPause = 1 - wantPause;
      break;
        //exit animation
    case QUIT:
      exit (0);
      break;
    }
}


int
main (int     argc,
      char   *argv[])
{
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
  glutCreateWindow ("Explosion demo");
  glutKeyboardFunc (keyboard);
  glutIdleFunc (idle);
  glutDisplayFunc (display);
  glutReshapeFunc (reshape);
  srand (time (NULL));

  glutCreateMenu (menuSelect);
  glutAddMenuEntry ("Pause", PAUSE);
  glutAddMenuEntry ("Quit", QUIT);
  glutAttachMenu (GLUT_RIGHT_BUTTON);
  glutTimerFunc(10000,myTimer,0);
  glutTimerFunc(15000,myTimer1,0);
  glutTimerFunc(20000,myTimer2,0);
  glutMainLoop ();

  return 0;
}

