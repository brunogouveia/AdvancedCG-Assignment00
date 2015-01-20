/*
 *  Basic Shader Example
 *
 *  Fixed pipeline
 *  Shader replicating fixed pipeline
 *  Move vertex and change color
 *
 *  Key bindings:
 *  m/M        Cycle through shaders
 *  o/O        Cycle through objects
 *  p/P        Toggle between orthogonal & perspective projection
 *  a          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx239.h"
int mx;
int my;
float dx = 0.0f;
float dy = 0.0f;
float dz = 0.0f;
const float step = 0.1;
int axes=1;       //  Display axes
int mode=0;       //  Shader mode
int move=1;       //  Move light
int proj=0;       //  Projection type
int obj=0;        //  Object
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
int model;        //  Model display list
int shader[] = {0, 0, 0, 0}; //  Shader program
char* text[] = {"No Shader","Vertex + Fragment Shaders", "Vertex Shader", "Fragment Shader"};

/*
 *  Draw a cube
 */
static void Cube(void)
{
   //  Front
   glColor3f(1,0,0);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,+1);
   glTexCoord2f(0,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,+1);
   glEnd();
   //  Back
   glColor3f(0,0,1);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   glColor3f(1,1,0);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   glColor3f(0,1,0);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   glColor3f(0,1,1);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glColor3f(1,0,1);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=2.0;  //  Length of axes

   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (proj)
   {
      glTranslatef(dx, dy, dz);
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glTranslatef(dx, dy, dz);
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   //  Select shader (0 => no shader)
   glUseProgram(shader[mode]);

   //  If is fragment only, we need to pass the dimension of the window in order to compute the right color for each pixel
   if (mode == 3)
   {
      int idX = glGetUniformLocation(shader[3],"xSize");
      int idY = glGetUniformLocation(shader[3],"ySize");
      if (idX>=0 && idY>=0) {
         glUniform1f(idX, glutGet(GLUT_WINDOW_WIDTH));
         glUniform1f(idY, glutGet(GLUT_WINDOW_HEIGHT));
      }
   }

   //  Draw the model, teapot or cube
   glColor3f(1,1,0);
   if (obj==2)
      glCallList(model);
   else if (obj==1)
      glutSolidTeapot(1.0);
   else
      Cube();

   //  No shader for what follows
   glUseProgram(0);

   //  Draw axes - no lighting from here on
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f Projection=%s %s",
     th,ph,dim,proj?"Perspective":"Orthogonal",text[mode]);

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int xch,int ych)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(proj?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int xch,int ych)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0'){
      th = ph = 0;
      dx = 0;
      dy = 0;
      dz = 0;
   }
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle projection type
   else if (ch == 'p' || ch == 'P')
      proj = 1-proj;
   //  Toggle objects
   else if (ch == 'o' || ch == 'O')
      obj = (obj+1)%3;
   //  Cycle modes
   else if (ch == 'm' || ch == 'M')
      mode = (mode + 1) % 4;
   else if (ch == 'a' || ch == 'a')
      dx -= step;
   else if (ch == 'd' || ch == 'D')
      dx += step;
   else if (ch == 'w' || ch == 'W')
      dz += step;
   else if (ch == 's' || ch == 'S')
      dz -= step;
   else if (ch == 'z' || ch == 'Z')
      dy -= step;
   else if (ch == 'q' || ch == 'Q')
      dy += step;

   //  Reproject
   Project(proj?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 * Callback function responsible for handling of mouse clicks.
 */
void mouseCallback(int button, int state, int x, int y) {
   mx = x;
   my = y;
}

/*
 * Callback function responsible for handling of mouse motion.
 */
void motionCallback(int x, int y) {
   th += (x-mx);
   ph += (y-my);
   mx = x;
   my = y;
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(proj?fov:0,asp,dim);
}


/*
 *  GLUT calls this routine every 50ms
 */
void timer(int k)
{
   glutPostRedisplay();
   glutTimerFunc(50,timer,0);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Bruno H. Gouveia - Assignment 01");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
   if (!GLEW_VERSION_2_0) Fatal("OpenGL 2.0 not supported\n");
#endif
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutMouseFunc(mouseCallback);
   glutMotionFunc(motionCallback);
   timer(1);
   //  Load object
   model = LoadOBJ("tyra.obj");
   //  Create Shader Programs
   shader[1] = CreateShaderProg("shaders/basic.vert","shaders/basic.frag");
   shader[2] = CreateShaderProg("shaders/NDCtoRGB.vert", NULL);
   shader[3] = CreateShaderProg(NULL, "shaders/NDCtoRGB.frag");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
