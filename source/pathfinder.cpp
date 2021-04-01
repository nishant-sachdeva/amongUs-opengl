#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <cmath>
#include <cstdlib>

#include "pathfinder.h"

void PathFinder::set_dest( Direction new_dest ){
	this->init_dest = Dest;
	this->Dest = new_dest;
	rolling_status = 0;
	ismoving =true;
}

PathFinder::PathFinder(int x_position, int y_position, int maze_width, int maze_height)
{
	old_x = current_x = 20.0 + 10.0 * x_position;
	old_y = current_y = 20.0 + 10.0 * y_position;
	recursion_stack = new int[maze_width * maze_height * 4];	// size of worst case
	stack_top = -1;

	/* initialzing status factor */
	ismoving = false;
	walk_status = 0;
	eye_status = 0;
	rolling_status = 0;
	goal_ceremony_status = 0;
	degree_7 = sin(7 * atan(-1) / 180);	// sin( 7 * PI / 180)

	lists();
	init_dest = Dest = RIGHT;
}

void PathFinder::lists(){
	glNewList( Arm, GL_COMPILE );
		glBegin( GL_POLYGON );
			glEdgeFlag( GL_TRUE );
			glVertex2f( 0, 0 );
			glVertex2f( 15, -10 );
			glVertex2f( 25, -25 );
			//glVertex2f( 0, 0 );
		glEnd();
	glEndList();

	glNewList( Leg, GL_COMPILE );
		glBegin( GL_POLYGON );
			glEdgeFlag( GL_TRUE );
			glVertex2f( 0, 0 );
			glVertex2f( 7, -28.5 );
			glVertex2f( 0, -28.5 );
			//glVertex2f( 0, 0 );
		glEnd();
	glEndList();

	glNewList( Eye, GL_COMPILE );
		glColor3f( 1, 1, 1 );
		glBegin( GL_POLYGON );
			glEdgeFlag( GL_TRUE );
			glVertex2f( 0, 0 );
			glVertex2f( 10, -3 );
			glVertex2f( 8, -10 );
			glVertex2f( 2, -9 );
			//glVertex2f( 0, 0 );
		glEnd();

		glBegin( GL_POLYGON );
			glColor3f( 0, 0, 0 );
			glVertex2f( 6, -1.8 );
			glVertex2f( 8, -2.4 );
			glVertex2f( 8, -8 );
			glVertex2f( 6, -8 );
			//glVertex2f( 8, -2.4 );
		glEnd();
	glEndList();

	glNewList( Body, GL_COMPILE );
		glBegin( GL_POLYGON );
			glEdgeFlag( GL_TRUE );
			glVertex2f( 0, 0 );
			glVertex2f( 40, 0  );
			glVertex2f( 30, 40 );
			glVertex2f( 10, 40 );
			//glVertex2f( 0, 0 );
		glEnd();
	glEndList();

}

void PathFinder::Move()
{
	double movingfactor = 28.5 * fabs(sin(degree_7 * walk_status) - sin(degree_7 * (walk_status - 1)));
	// length of leg * abs_value( sin(7_deg * walk_status) - sin(7_deg * (walk_status-1)))
	// movement of one frame of the animation

	if( rolling_status == ROLL_FACT ){
		switch(Dest) {
		case UP:
			current_y += movingfactor;//( walk_status >= 0 )? walk_status/3.0 : -walk_status/3.0;
			break;
		case DOWN:
			current_y -= movingfactor;//( walk_status >= 0 )? walk_status/3.0 : -walk_status/3.0;
			break;
		case LEFT:
			current_x -= movingfactor;//( walk_status >= 0 )? walk_status/3.0 : -walk_status/3.0;
			break;
		case RIGHT:
			current_x += movingfactor;//( walk_status >= 0 )? walk_status/3.0 : -walk_status/3.0;
			break;
		}
	}

	if(abs(old_x - current_x) >= 10.0) {
		current_x = old_x + ((Dest == RIGHT) ? 10 : -10);
		old_x = current_x;
		ismoving = false;
	}
	else if(abs(old_y - current_y) >= 10.0) {
		current_y = old_y + ((Dest == UP) ? 10 : -10);
		old_y = current_y;
		ismoving = false;
	}

	if(rolling_status == ROLL_FACT) {	// if it is rotating then do not move the leg, arm and eye
		walk_status++;
		if( walk_status > 5 )
			walk_status = -4;
		eye_status++;
		if( eye_status >= 30 )
			eye_status = 0;
	}
}

void PathFinder::Draw()
{
	glTranslatef( 30, 50, 0 );

	// draw body
	double rotateAngle = 0;
	switch (init_dest) {
	case LEFT:
		rotateAngle = 180.0;
		break;
	case UP:
		rotateAngle = 90.0;
		break;
	case DOWN:
		rotateAngle = -90.0;
		break;
	}
	if( rolling_status < ROLL_FACT ){
		switch (init_dest) {
		case LEFT:
			if( Dest == RIGHT )
				rotateAngle += 180.0/ROLL_FACT*rolling_status;
			else if( Dest == UP )
				rotateAngle += -90.0/ROLL_FACT*rolling_status;
			else if( Dest == DOWN )
				rotateAngle += 90.0/ROLL_FACT*rolling_status;
			break;
		case RIGHT:
			if( Dest == LEFT )
				rotateAngle += 180.0/ROLL_FACT*rolling_status;
			else if( Dest == UP )
				rotateAngle += 90.0/ROLL_FACT*rolling_status;
			else if( Dest == DOWN )
				rotateAngle += -90.0/ROLL_FACT*rolling_status;
			break;
		case UP:
			if( Dest == DOWN )
				rotateAngle += 180.0/ROLL_FACT*rolling_status;
			else if( Dest == LEFT )
				rotateAngle += 90.0/ROLL_FACT*rolling_status;
			else if( Dest == RIGHT )
				rotateAngle += -90.0/ROLL_FACT*rolling_status;
			break;
		case DOWN:
			if( Dest == LEFT )
				rotateAngle += -90.0/ROLL_FACT*rolling_status;
			else if( Dest == UP )
				rotateAngle += 180.0/ROLL_FACT*rolling_status;
			else if( Dest == RIGHT )
				rotateAngle += 90.0/ROLL_FACT*rolling_status;
			break;
		}
	}
	glTranslatef(20,15,0);
	glRotatef(rotateAngle, 0, 0, 1);
	glTranslatef(-20, -15, 0);
	glColor3f( bodyColorR, bodyColorG, bodyColorB );
	glCallList( Body );

	glPushMatrix();

	/* draw eye */
	glTranslatef( 23, 30, 0 );
	if( eye_status >= 24 ) glScalef( 1, ( 30 - eye_status -1 )/6.0, 1);
	glCallList( Eye );

	/* draw arms */
	glPopMatrix();
	glPushMatrix();
	glColor3f( 0.0, 0.0, 0.0 );
	glTranslatef( 35, 20, 0 );
	if( get_goal == true) glRotatef( 5 * goal_ceremony_status, 0, 0, 1);
	else glRotatef(abs(walk_status * 5), 0, 0, 1);
		//if( walk_status >= 0 ) glRotatef( 5*walk_status, 0, 0, 1 );
		//else glRotatef( walk_status * -5 , 0, 0, 1 );
	glCallList( Arm );

	glPopMatrix();
	glPushMatrix();
	glTranslatef( 5, 20, 0 );
	glRotatef( 180, 0 ,1, 0 );
	if( get_goal == true) glRotatef( -5 * goal_ceremony_status , 0, 0, 1);
	else glRotatef(abs(walk_status * 5), 0, 0, 1);
		//if( walk_status >= 0 ) glRotatef( 5*walk_status, 0, 0, 1 );
		//else glRotatef( walk_status * -5, 0, 0, 1 );
	glCallList( Arm );

	/* draw legs */
	glPopMatrix();
	glPushMatrix();
	glTranslatef( 32, 0, 0 );
	if( get_goal == false) glRotatef( abs(7 * walk_status), 0, 0, 1 );
		//if( walk_status >= 0 ) glRotatef( 7 * walk_status, 0, 0, 1 );
		//else glRotatef( walk_status * -7 , 0, 0, 1 );
	glCallList( Leg );

	glPopMatrix();
	glPushMatrix();
	glTranslatef( 8, 0, 0 );
	glRotatef( 180, 0, 1, 0 );
	if( get_goal == false) glRotatef( abs(7 * walk_status), 0, 0, 1 );
		//if( walk_status >= 0 ) glRotatef( 7 * walk_status, 0, 0, 1 );
		//else glRotatef( walk_status * -7 , 0, 0, 1 );
	glCallList( Leg );
	glPopMatrix();
}

void PathFinder::UpdateStatus() {
	if (ismoving) Move();
	if (rolling_status < ROLL_FACT) {
		rolling_status++;
		if (init_dest == Dest) rolling_status = ROLL_FACT;
		if (rolling_status == ROLL_FACT) init_dest = Dest;
	}
	if(get_goal == true) goal_ceremony_status++;
}
