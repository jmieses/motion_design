#include <iostream>
#include <vector>
#include <stdlib.h>
#include "Interaction.h"
#include "Point2D.h"
#include <GL/glut.h>
using namespace std;

#define TRUE 1
#define FALSE 0



//static variables being defined here; outside the class since these are class wide variables

Point2D Interaction::m_LDownPos;
Point2D Interaction::m_RDownPos;
Point2D Interaction::m_MDownPos;
int Interaction::m_LButtonDown;
int Interaction::m_RButtonDown;
int Interaction::m_MButtonDown;
double Interaction::m_xRotate;
double Interaction::m_yRotate;
double Interaction::m_zRotate;
double Interaction::m_xTrans;
double Interaction::m_yTrans;
double Interaction::m_zTrans;



void Interaction::mouseButtonEvents(int button, int state, int x, int y) 
{

switch( button )
     {
        case GLUT_LEFT_BUTTON:
      
           switch( state )
           {
              case GLUT_DOWN:
				m_LButtonDown = TRUE;
				m_LDownPos.setPx(x);
				m_LDownPos.setPy(y);
                break;
              case GLUT_UP:
				m_LButtonDown = FALSE;
                break;
           }
           break;

		case GLUT_RIGHT_BUTTON:
      
           switch( state )
           {
              case GLUT_DOWN:
				m_RButtonDown = TRUE;
				m_RDownPos.setPx(x);
				m_RDownPos.setPy(y);
                break;
              case GLUT_UP:
				m_RButtonDown = FALSE;
                break;
           }
           break;

		case GLUT_MIDDLE_BUTTON:

		   switch(state)
		   {
		   case GLUT_DOWN:
			   m_MButtonDown = TRUE;
			   m_MDownPos.setPx(x);
			   m_MDownPos.setPy(y);
			   break;
		   case GLUT_UP:
			   m_MButtonDown = FALSE;
			   break;
		   }
		   break;
		

		default:
			break;

     } //End of switch

	glutPostRedisplay();
}

void Interaction::mouseMotionEvents(int x, int y) 
{
  /* find out what action to perform */
	if (m_LButtonDown)
	{
		m_yRotate -= (m_LDownPos.getPx() - (double)x)/2.0;
		m_xRotate -= (m_LDownPos.getPy() - (double)y)/2.0;
			
		m_LDownPos.setPx(x);
		m_LDownPos.setPy(y);
		
		
	}

	if (m_RButtonDown)
	{
		m_xTrans -= (m_RDownPos.getPx() - (double)x)/10.0f; 	
		m_yTrans += (m_RDownPos.getPy() - (double)y)/10.0f;

		m_RDownPos.setPx(x);
		m_RDownPos.setPy(y);

		
	}

		if (m_MButtonDown)
	{
		m_zTrans -= (m_MDownPos.getPy() - (double)y)/10.0f; 	

		m_MDownPos.setPx(x);
		m_MDownPos.setPy(y);

		
	}

  glutPostRedisplay();
}

void Interaction::keyboard(unsigned char Key, int x, int y)
{
  switch(Key)
  {
  case 27: 
  case 'q':
    exit(0);
    break;
  };
  
  glutPostRedisplay();

}

