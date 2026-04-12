/*
 * fractviewer.cxx [from agviewer.c  (version 1.0)]
 *
 * AGV: a glut viewer. Routines for viewing a 3d scene w/ glut
 *
 * See agv_example.c and agviewer.h comments within for more info.
 *
 * I welcome any feedback or improved versions!
 *
 * Philip Winston - 4/11/95
 * pwinston@hmc.edu
 * http://www.cs.hmc.edu/people/pwinston
 */

#include <config.h>

#if HAVE_GL && HAVE_GL_GLU_H
#  include <FL/glut.H>
#  include <FL/glu.h>

#  include <stdio.h>
#  include <stdlib.h>
#  include <math.h>
#  include <sys/types.h>
#  include <time.h>
#  if !defined(_WIN32)
#    include <sys/time.h>
#  endif // !_WIN32

#  include "fracviewer.h"

/* Some <math.h> files do not define M_PI... */
#ifndef M_PI
#define M_PI 3.14159265
#endif

/***************************************************************/
/************************** SETTINGS ***************************/
/***************************************************************/

   /* Initial polar movement settings */
constexpr float kInitPolarAz = 0.0f;
constexpr float kInitPolarEl = 30.0f;
constexpr float kInitDist    = 4.0f;
constexpr float kInitAzSpin  = 0.5f;
constexpr float kInitElSpin  = 0.0f;

  /* Initial flying movement settings */
constexpr float kInitEx   = 0.0f;
constexpr float kInitEy   = -2.0f;
constexpr float kInitEz   = -2.0f;
constexpr float kInitMove = 0.01f;
constexpr float kMinMove  = 0.001f;

  /* Start in this mode */
constexpr int kInitMode = POLAR;

  /* Controls:  */

  /* map 0-9 to an EyeMove value when number key is hit in FLYING mode */
#define SPEEDFUNCTION(x) ((x)*(x)*0.001f)

  /* Multiply EyeMove by (1+-kMoveFraction) when +/- hit in FLYING mode */
constexpr float kMoveFraction = 0.25f;

  /* What to multiply number of pixels mouse moved by to get rotation amount */
constexpr float kElSens = 0.5f;
constexpr float kAzSens = 0.5f;

  /* What to multiply number of pixels mouse moved by for movement amounts */
constexpr float kDistSens = 0.01f;
constexpr float kESens    = 0.01f;

  /* Minimum spin to allow in polar (lower forced to zero) */
constexpr float kMinAzSpin = 0.1f;
constexpr float kMinElSpin = 0.1f;

  /* Factors used in computing dAz and dEl (which determine AzSpin, ElSpin) */
constexpr float kSlowDAz = 0.90f;
constexpr float kSlowDEl = 0.90f;
constexpr float kPrevDAz = 0.80f;
constexpr float kPrevDEl = 0.80f;
constexpr float kCurDAz  = 0.20f;
constexpr float kCurDEl  = 0.20f;

/***************************************************************/
/************************** GLOBALS ****************************/
/***************************************************************/

int     MoveMode = kInitMode;  /* FLYING or POLAR mode? */

GLfloat Ex = kInitEx,             /* flying parameters */
        Ey = kInitEy,
        Ez = kInitEz,
        EyeMove = kInitMove,

        EyeDist = kInitDist,      /* polar params */
        AzSpin  = kInitAzSpin,
        ElSpin  = kInitElSpin,

        EyeAz = kInitPolarAz,    /* used by both */
        EyeEl = kInitPolarEl;

int agvMoving;    /* Currently moving?  */

int downx, downy,   /* for tracking mouse position */
    lastx, lasty,
    downb = -1;     /* and button status */

GLfloat downDist, downEl, downAz, /* for saving state of things */
        downEx, downEy, downEz,   /* when button is pressed */
        downEyeMove;

GLfloat dAz, dEl, lastAz, lastEl;  /* to calculate spinning w/ polar motion */
int     AdjustingAzEl = 0;

int AllowIdle, RedisplayWindow;
   /* If AllowIdle is 1 it means AGV will install its own idle which
    * will update the viewpoint as needed and send glutPostRedisplay() to the
    * window RedisplayWindow which was set in agvInit().  AllowIdle of 0
    * means AGV won't install an idle funciton, and something like
    * "if (agvMoving) agvMove()" should exist at the end of the running
    * idle function.
    */

#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define TORAD(x) ((M_PI/180.0)*(x))
#define TODEG(x) ((180.0/M_PI)*(x))

/***************************************************************/
/************************ PROTOTYPES ***************************/
/***************************************************************/

  /*
   * these are functions meant for internal use only
   * the other prototypes are in agviewer.h
   */

void PolarLookFrom(GLfloat dist, GLfloat elevation, GLfloat azimuth);
void FlyLookFrom(GLfloat x, GLfloat y, GLfloat z,
                        GLfloat az, GLfloat el);
int  ConstrainEl(void);
void MoveOn(int v);
void SetMove(float newmove);
static void normalize(GLfloat v[3]);
void ncrossprod(float v1[3], float v2[3], float cp[3]);


/***************************************************************/
/************************ agvInit ******************************/
/***************************************************************/

void agvInit(int window)
{
  glutMouseFunc(agvHandleButton);
  glutMotionFunc(agvHandleMotion);
  glutKeyboardFunc(agvHandleKeys);
  RedisplayWindow = glutGetWindow();
  agvSetAllowIdle(window);
}

/***************************************************************/
/************************ VIEWPOINT STUFF **********************/
/***************************************************************/

  /*
   * viewing transformation modified from page 90 of red book
   */
void PolarLookFrom(GLfloat dist, GLfloat elevation, GLfloat azimuth)
{
  glTranslatef(0, 0, -dist);
  glRotatef(elevation, 1, 0, 0);
  glRotatef(azimuth, 0, 1, 0);

}

  /*
   * I took the idea of tracking eye position in absolute
   * coords and direction looking in Polar form from denis
   */
void FlyLookFrom(GLfloat x, GLfloat y, GLfloat z, GLfloat az, GLfloat el)
{
  float lookat[3], perp[3], up[3];

  lookat[0] = GLfloat(sin(TORAD(az))*cos(TORAD(el)));
  lookat[1] = GLfloat(sin(TORAD(el)));
  lookat[2] = GLfloat(-cos(TORAD(az))*cos(TORAD(el)));
  normalize(lookat);
  perp[0] = lookat[2];
  perp[1] = 0;
  perp[2] = -lookat[0];
  normalize(perp);
  ncrossprod(lookat, perp, up);
  gluLookAt(x, y, z,
            x+lookat[0], y+lookat[1], z+lookat[2],
            up[0], up[1], up[2]);
}

  /*
   * Call viewing transformation based on movement mode
   */
void agvViewTransform(void)
{
  switch (MoveMode) {
    case FLYING:
      FlyLookFrom(Ex, Ey, Ez, EyeAz, EyeEl);
      break;
    case POLAR:
      PolarLookFrom(EyeDist, EyeEl, EyeAz);
      break;
    }
}

  /*
   * keep them vertical; I think this makes a lot of things easier,
   * but maybe it wouldn't be too hard to adapt things to let you go
   * upside down
   */
int ConstrainEl(void)
{
  if (EyeEl <= -90) {
    EyeEl = -89.99f;
    return 1;
  } else if (EyeEl >= 90) {
    EyeEl = 89.99f;
    return 1;
  }
  return 0;
}

 /*
  * Idle Function - moves eyeposition
  */
void agvMove(void)
{
  switch (MoveMode)  {
    case FLYING:
      Ex += GLfloat(EyeMove*sin(TORAD(EyeAz))*cos(TORAD(EyeEl)));
      Ey += GLfloat(EyeMove*sin(TORAD(EyeEl)));
      Ez -= GLfloat(EyeMove*cos(TORAD(EyeAz))*cos(TORAD(EyeEl)));
      break;

    case POLAR:
      EyeEl += ElSpin;
      EyeAz += AzSpin;
      if (ConstrainEl()) {  /* weird spin thing to make things look     */
        ElSpin = -ElSpin;      /* look better when you are kept from going */
                               /* upside down while spinning - Isn't great */
        if (fabs(ElSpin) > fabs(AzSpin))
          AzSpin = GLfloat(fabs(ElSpin) * ((AzSpin > 0.0f) ? 1.0f : -1.0f));
      }
      break;
    }

  if (AdjustingAzEl) {
    dAz *= kSlowDAz;
    dEl *= kSlowDEl;
  }

  if (AllowIdle) {
    glutSetWindow(RedisplayWindow);
    glutPostRedisplay();
  }
}


  /*
   * Don't install agvMove as idle unless we will be updating the view
   * and we've been given a RedisplayWindow
   */
void MoveOn(int v)
{
  if (v && ((MoveMode == FLYING && EyeMove != 0) ||
             (MoveMode == POLAR &&
             (AzSpin != 0 || ElSpin != 0 || AdjustingAzEl)))) {
    agvMoving = 1;
    if (AllowIdle)
      glutIdleFunc(agvMove);
  } else {
    agvMoving = 0;
    if (AllowIdle)
      glutIdleFunc(NULL);
  }
}

  /*
   * set new redisplay window.  If <= 0 it means we are not to install
   * an idle function and will rely on whoever does install one to
   * put statement like "if (agvMoving) agvMove();" at end of it
   */
void agvSetAllowIdle(int allowidle)
{
  if ((AllowIdle = allowidle))
    MoveOn(1);
}


  /*
   * when moving to flying we stay in the same spot, moving to polar we
   * reset since we have to be looking at the origin (though a pivot from
   * current position to look at origin might be cooler)
   */
void agvSwitchMoveMode(int move)
{
  switch (move) {
    case FLYING:
      if (MoveMode == FLYING) return;
      Ex    = GLfloat(-EyeDist*sin(TORAD(EyeAz))*cos(TORAD(EyeEl)));
      Ey    = GLfloat( EyeDist*sin(TORAD(EyeEl)));
      Ez    = GLfloat( EyeDist*(cos(TORAD(EyeAz))*cos(TORAD(EyeEl))));
      EyeEl = -EyeEl;
      EyeMove = kInitMove;
      break;
    case POLAR:
      EyeDist = kInitDist;
      EyeAz   = kInitPolarAz;
      EyeEl   = kInitPolarEl;
      AzSpin  = kInitAzSpin;
      ElSpin  = kInitElSpin;
      break;
    }
  MoveMode = move;
  MoveOn(1);
  glutPostRedisplay();
}

/***************************************************************/
/*******************    MOUSE HANDLING   ***********************/
/***************************************************************/

void agvHandleButton(int button, int state, int x, int y)
{
 // deal with mouse wheel events, that fltk sends as buttons 3 or 4
 //if (button > GLUT_RIGHT_BUTTON)return;
 if ((state == GLUT_DOWN) && ((button == 3) || (button == 4))) {
    // attempt to process scrollwheel as zoom in/out
    float deltay = 0.25;
    if (button == 3) {
      deltay = (-0.25);
    }
    downb = -1;
    downDist = EyeDist;
    downEx = Ex;
    downEy = Ey;
    downEz = Ez;
    downEyeMove = EyeMove;
    EyeMove = 0;

    EyeDist = downDist + deltay;
    Ex = GLfloat(downEx - kESens*deltay*sin(TORAD(EyeAz))*cos(TORAD(EyeEl)));
    Ey = GLfloat(downEy - kESens*deltay*sin(TORAD(EyeEl)));
    Ez = GLfloat(downEz + kESens*deltay*cos(TORAD(EyeAz))*cos(TORAD(EyeEl)));

    EyeMove = downEyeMove;
    glutPostRedisplay();
    return;
 }
 else if (button > GLUT_RIGHT_BUTTON)return; // ignore any other button...

 if (state == GLUT_DOWN && downb == -1) {
    lastx = downx = x;
    lasty = downy = y;
    downb = button;

    switch (button) {
      case GLUT_LEFT_BUTTON:
        lastEl = downEl = EyeEl;
        lastAz = downAz = EyeAz;
        AzSpin = ElSpin = dAz = dEl = 0;
        AdjustingAzEl = 1;
        MoveOn(1);
        break;

      case GLUT_MIDDLE_BUTTON:
        downDist = EyeDist;
        downEx = Ex;
        downEy = Ey;
        downEz = Ez;
        downEyeMove = EyeMove;
        EyeMove = 0;
    }

  } else if (state == GLUT_UP && button == downb) {

    downb = -1;

    switch (button) {
      case GLUT_LEFT_BUTTON:
        if (MoveMode != FLYING) {
          AzSpin =  -dAz;
          if (AzSpin < kMinAzSpin && AzSpin > -kMinAzSpin)
            AzSpin = 0;
          ElSpin = -dEl;
          if (ElSpin < kMinElSpin && ElSpin > -kMinElSpin)
            ElSpin = 0;
        }
        AdjustingAzEl = 0;
        MoveOn(1);
        break;

      case GLUT_MIDDLE_BUTTON:
        EyeMove = downEyeMove;
      }
  }
}

 /*
  * change EyeEl and EyeAz and position when mouse is moved w/ button down
  */
void agvHandleMotion(int x, int y)
{
  int deltax = x - downx, deltay = y - downy;

  switch (downb) {
    case GLUT_LEFT_BUTTON:
      EyeEl  = GLfloat(downEl + kElSens * deltay);
      ConstrainEl();
      EyeAz  = GLfloat(downAz + kAzSens * deltax);
      dAz    = GLfloat(kPrevDAz*dAz + kCurDAz*(lastAz - EyeAz));
      dEl    = GLfloat(kPrevDEl*dEl + kCurDEl*(lastEl - EyeEl));
      lastAz = EyeAz;
      lastEl = EyeEl;
      break;
    case GLUT_MIDDLE_BUTTON:
        EyeDist = GLfloat(downDist + kDistSens*deltay);
        Ex = GLfloat(downEx - kESens*deltay*sin(TORAD(EyeAz))*cos(TORAD(EyeEl)));
        Ey = GLfloat(downEy - kESens*deltay*sin(TORAD(EyeEl)));
        Ez = GLfloat(downEz + kESens*deltay*cos(TORAD(EyeAz))*cos(TORAD(EyeEl)));
      break;
  }
  glutPostRedisplay();
}

/***************************************************************/
/********************* KEYBOARD HANDLING ***********************/
/***************************************************************/

  /*
   * set EyeMove (current speed) for FLYING mode
   */
void SetMove(float newmove)
{
  if (newmove > kMinMove) {
    EyeMove = newmove;
    MoveOn(1);
  } else {
    EyeMove = 0;
    MoveOn(0);
  }
}

  /*
   * 0->9 set speed, +/- adjust current speed  -- in FLYING mode
   */
void agvHandleKeys(unsigned char key, int, int) {
  if (MoveMode != FLYING)
    return;

  if (key >= '0' && key <= '9')
    SetMove(SPEEDFUNCTION((key-'0')));
  else
    switch(key) {
      case '+':
        if (EyeMove == 0)
          SetMove(kMinMove);
         else
          SetMove(EyeMove *= (1 + kMoveFraction));
        break;
      case '-':
        SetMove(EyeMove *= (1 - kMoveFraction));
        break;
    }
}

/***************************************************************/
/*********************** VECTOR STUFF **************************/
/***************************************************************/

  /* normalizes v */
static void normalize(GLfloat v[3])
{
  GLfloat d = sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);

  if (d == 0)
    fprintf(stderr, "Zero length vector in normalize\n");
  else {
    v[0] /= d; v[1] /= d; v[2] /= d;
  }
}

  /* calculates a normalized crossproduct to v1, v2 */
void ncrossprod(float v1[3], float v2[3], float cp[3])
{
  cp[0] = v1[1]*v2[2] - v1[2]*v2[1];
  cp[1] = v1[2]*v2[0] - v1[0]*v2[2];
  cp[2] = v1[0]*v2[1] - v1[1]*v2[0];
  normalize(cp);
}

/***************************************************************/
/**************************** AXES *****************************/
/***************************************************************/


  /* draw axes -- was helpful to debug/design things */
void agvMakeAxesList(int displaylistnum)
{
  int i,j;
  GLfloat axes_ambuse[] =   { 0.5, 0.0, 0.0, 1.0 };
  GLfloat trans = -10;
  glNewList(displaylistnum, GL_COMPILE);
  glPushAttrib(GL_LIGHTING_BIT);
  glMatrixMode(GL_MODELVIEW);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, axes_ambuse);
    glBegin(GL_LINES);
      glVertex3f(15, 0, 0); glVertex3f(-15, 0, 0);
      glVertex3f(0, 15, 0); glVertex3f(0, -15, 0);
      glVertex3f(0, 0, 15); glVertex3f(0, 0, -15);
    glEnd();
    for (i = 0; i < 3; i++) {
      glPushMatrix();
        glTranslatef(trans*(i==0), trans*(i==1), trans*(i==2));
        for (j = 0; j < 21; j++) {
//          glutSolidCube(0.1);
          glTranslatef(i==0, i==1, i==2);
        }
      glPopMatrix();
    }
  glPopAttrib();
  glEndList();
}


#endif // HAVE_GL && HAVE_GL_GLU_H
