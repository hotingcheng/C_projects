#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* Symbolic constant */
#define DELAYTIME 5000000
#define MAX_SNAKE 100      
#define pathfile "path2.dat"
#define ROW 23
#define COLUMN 75

/*
 * SECTION1
 * This section contains definitions and functions which
 * are about screen handling features.
 */

#ifdef	WIN32
#   include <windows.h> 
#   include <wincon.h>
#   include <winuser.h>
#   include <time.h>
#else 
#   include <dos.h>
#   include <conio.h>
#endif

#ifdef	WIN32
HANDLE hStdout, hStdin; 
 
void gotoxy( int x, int y)
{
  COORD pos;

  if ((x <=79) && (x >= 0))
    pos.X  = x;
  else
    pos.X = 0;
  if ((y <=24) && (x >= 0))
    pos.Y = y;
  else
    pos.Y = 0;
  SetConsoleCursorPosition( hStdout, pos );    
}

#endif

void delay()
{
    int i;

    for (i=0;i<DELAYTIME;i++) {};
}
/* END OF SECTION1 */


/* Function Prototypes */
void PrintMaze(char [COLUMN][ROW], int, int);
int right(char [COLUMN][ROW], int, int);
int left(char [COLUMN][ROW], int, int);
int up(char [COLUMN][ROW], int, int);
int down(char [COLUMN][ROW], int, int);

int main(void)
{
	FILE *fp; /* declare file pointer */
    int row, column, i, j, x, y;
		/* row -- number of row of maze 
		 * column -- number of column of maze
		 * x -- x-coordinate of position 'C'
		 * y -- y-coordinate of position 'C'
		 */
	char location[COLUMN][ROW], dummy; 
		/* location -- content of the maze in which the bean determines the path of PacMan */

   /* SECTION2 */
#ifdef WIN32
    /* Get handles to STDIN and STDOUT.  */

    hStdin = GetStdHandle(STD_INPUT_HANDLE); 
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    if (hStdin == INVALID_HANDLE_VALUE || hStdout == INVALID_HANDLE_VALUE) 
    {
      printf("Error in initializing console!\n");
      exit(1);
    }
#endif
/* END OF SECTION2 */
	
	fp = fopen(pathfile, "r"); /* open file */

	fscanf(fp, "%d", &row);  /* read the value of number of row */
	fscanf(fp, "%d", &column);  /* read the value of number of column */
	fscanf(fp, "%c", &dummy); /* read the newline character */
	
	/* read the content of the maze */
	for (j=0; j<row; j++)
	{		
		for (i=0; i<column; i++)
		{
			fscanf(fp, "%c", &location[i][j]);

			/* locate the position of 'C' where is the starting point of PacMan */
			if (location[i][j] == 'C')
			{
				x = i; 
				y = j;
			}
		}
		fscanf(fp, "%c", &dummy);
	}
		
	gotoxy(0,0); /* set console cursor position at (0,0) */

	PrintMaze(location, row, column); /* Call the function which prints the maze */
		
	gotoxy(x,y); /* set console cursor at 'C' located position  */

	/* print the form of PacMan according to different directions */
	if (location[x+1][y] == '.')
		printf("%c", '<');
	else if (location[x-1][y] == '.')
		 	 printf("%c", '>');
	else if (location[x][y+1] == '.')
			 printf("%c", '^');
	else if (location[x][y-1] == '.')
			 printf("%c", 'v');		   

	/* search the surroundings of PacMan whether there is a bean there */
	while ((location[x+1][y] == '.')||(location[x-1][y] == '.')||
		   (location[x][y+1] == '.')||(location[x][y-1] == '.'))
	{
		if (location[x+1][y] == '.')
			x = right(location,x,y); /* a bean is at the right side of PacMan */
		else if (location[x-1][y] == '.')
				 x = left(location,x,y); /* a bean is at the left side of PacMan */
		else if (location[x][y+1] == '.')
				 y = down(location,x,y); /* a bean is at the bottom side of PacMan */
		else if (location[x][y-1] == '.')
				 y = up(location,x,y); /* a bean is at the top side of PacMan */
	}
		
	fclose(fp); /* close the file */
		
    return(0);
}


/* A function which prints the maze */
void PrintMaze(char array[COLUMN][ROW], int row, int column)
{
	int i, j;

	for (j=0; j<row; j++)
	{
		for (i=0; i<column; i++)
			printf("%c", array[i][j]);
		
		printf("\n");
	}
}

/* If a bean is on the right side of PacMan, then PacMan eats it and moves to the right */
int right(char array[COLUMN][ROW], int x, int y)
{
	delay();
	gotoxy(x,y); /* set console cursor position at the bean being eaten */
	printf(" "); /* clear the previous position */
	array[x][y] = ' ';
	x = x+1; /* move to right position */
	gotoxy(x,y); /* set console cursor position at the next position */
	printf("%c", '<'); /* print the PacMan */
	gotoxy(0,23);

	return  x;
}

/* If a bean is on the left side of PacMan, then PacMan eats it and moves to the left */
int left(char array[COLUMN][ROW], int x, int y)
{
	delay();
	gotoxy(x,y); /* set console cursor position at the bean being eaten */
	printf(" "); /* clear the previous position */
	array[x][y] = ' ';
	x = x-1; /* move to left position */
	gotoxy(x,y); /* set console cursor position at the next position */
	printf("%c", '>'); /* print the PacMan */
	gotoxy(0,23);

	return  x;
}

/* If a bean is on the top side of PacMan, then PacMan eats it and moves up */
int up(char array[COLUMN][ROW], int x, int y)
{
	delay();
	gotoxy(x,y); /* set console cursor position at the bean being eaten */
	printf(" "); /* clear the previous position */
	array[x][y] = ' ';
	y = y-1; /* move to upper position */
	gotoxy(x,y); /* set console cursor position at the next position */
	printf("%c", 'v'); /* print the PacMan */
	gotoxy(0,23);

	return  y;
}

/* If a bean is on the bottom side of PacMan, then PacMan eats it and moves down */
int down(char array[COLUMN][ROW], int x, int y)
{
	delay();
	gotoxy(x,y); /* set console cursor position at the bean being eaten */
	printf(" "); /* clear the previous position */
	array[x][y] = ' ';
	y =	y+1; /* move to lower position */
	gotoxy(x,y); /* set console cursor position at the next position */
	printf("%c", '^'); /* print the PacMan */
	gotoxy(0,23);

	return  y;
}
