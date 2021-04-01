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

void Fire::set_dest( Direction new_dest ){
	this->init_dest = Dest;
	this->Dest = new_dest;
	rolling_status = 0;
	ismoving =true;
}

Fire::Fire(int x_position, int y_position, int maze_width, int maze_height, bool is_auto)
{
	old_x = current_x = 20.0 + 10.0 * x_position;
	old_y = current_y = 20.0 + 10.0 * y_position;
	recursion_stack = new int[maze_width * maze_height * 4];	// size of worst case
	stack_top = -1;

	auto_mode = is_auto; // this will if the enemy is in auto mode or not

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

void Fire::lists(){
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

}

void Fire::Move()
{
	double movingfactor = 28.5 * fabs(sin(degree_7 * walk_status) - sin(degree_7 * (walk_status - 1)));
	// length of leg * abs_value( sin(7_deg * walk_status) - sin(7_deg * (walk_status-1)))
	// movement of one frame of the animation

	if( rolling_status == ROLL_FACT ){
		switch(Dest) {
		case UP:
			current_y += movingfactor; //( walk_status >= 0 )? walk_status/3.0 : -walk_status/3.0;
			break;
		case DOWN:
			current_y -= movingfactor; //( walk_status >= 0 )? walk_status/3.0 : -walk_status/3.0;
			break;
		case LEFT:
			current_x -= movingfactor; //( walk_status >= 0 )? walk_status/3.0 : -walk_status/3.0;
			break;
		case RIGHT:
			current_x += movingfactor; //( walk_status >= 0 )? walk_status/3.0 : -walk_status/3.0;
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

void Fire::Draw()
{
	glTranslatef( 30, 50, 0 );
	
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

void Fire::UpdateStatus() {
	if (ismoving)
		Move();
	if (rolling_status < ROLL_FACT) {
		rolling_status++;
		if (init_dest == Dest) 
			rolling_status = ROLL_FACT;
		if (rolling_status == ROLL_FACT) 
			init_dest = Dest;
	}
	if(get_goal == true) 
		goal_ceremony_status++;
}
