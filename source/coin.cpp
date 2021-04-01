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

Coin::Coin(int x_position, int y_position, int maze_width, int maze_height, bool is_auto)
{
	old_x = current_x = 20.0 + 10.0 * x_position;
	old_y = current_y = 20.0 + 10.0 * y_position;
	recursion_stack = new int[maze_width * maze_height * 4];	// size of worst case
	stack_top = -1;

	lists();
}

void Coin::lists(){
	glNewList( Eye, GL_COMPILE );

		glColor3f( 0.5f, 0.5f, 0.5f );
		glBegin( GL_POLYGON );
			glEdgeFlag( GL_TRUE );
			glVertex2f( 0, 0 );
			glVertex2f( 10, -3 );
			glVertex2f( 8, -10 );
			glVertex2f( 2, -9 );
		glEnd();

	glEndList();

}

void Coin::Draw()
{
	/* draw eye */
	glTranslatef( 23, 30, 0 );
	// if( eye_status >= 24 )
		glScalef( 1, 1, 1);
	glCallList( Eye );

}

