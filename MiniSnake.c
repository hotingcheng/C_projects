/*
 * CSC1110 Assignment6 (Mini PacMan)
 *
 * Class: A
 * Student id: 00769352
 * Student name: Cheng Ho Ting
 * Email Addr: htcheng0@ie.cuhk.edu.hk
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* Symbolic constant */
#define DELAYTIME 5000000
#define MAX_SNAKE 100      
#define pathfile "path1.dat"
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
int right(char [COLUMN][ROW], int, int, int);
int left(char [COLUMN][ROW], int, int, int);
int up(char [COLUMN][ROW], int, int, int);
int down(char [COLUMN][ROW], int, int, int);

int main(void)
{
	FILE *fp; /* declare file pointer */
    int row, column, i, j, x, y, length, path_x, path_y, cnt=0;
		/* row -- number of row of maze 
		 * column -- number of column of maze
		 * x -- x-coordinate of position 'C'
		 * y -- y-coordinate of position 'C'
		 * length -- length of the snake
		 * cnt -- act as a counter
		 */
	char location[COLUMN][ROW], dummy;
		/* location -- content of the maze in which the bean determines the path ot the head of snake 
		 * dummy -- stores the 'newline' character
		 */

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

	printf("Input the length of snake: ");
	scanf("%d", &length);

	while (length > MAX_SNAKE)
	{
		printf("Invalid Input !\n"); 
		printf("Input the length of snake: ");
		scanf("%d", &length);
	}
	
	fp = fopen(pathfile, "r"); /* open file and choose the "read" mode */

	fscanf(fp, "%d", &row); /* read the value of number of row */
	fscanf(fp, "%d", &column); /* read the value of number of column */
	fscanf(fp, "%c", &dummy); /* read the newline character */
	
	/* read the content of the maze */
	for (j=0; j<row; j++)
	{		
		for (i=0; i<column; i++)
		{
			fscanf(fp, "%c", &location[i][j]);

			/* locate the position of 'C' where the snake appears*/
			if (location[i][j] == 'C')
			{
				/* the starting position of the snake */ 
				x = i;
				y = j;

				/* the starting position of the "invisible" snake */
				path_x = i;
				path_y = j;
			}
		}
		fscanf(fp, "%c", &dummy);
	}

	gotoxy(0,0); /* set console cursor position at (0,0) */

	PrintMaze(location, row, column); /* Call the function which prints the maze */
		
	gotoxy(x,y); /* set console cursor at 'C' located position  */

	/* print the form of head of snake according to different directions */
	if (location[x+1][y] == '.')
		printf("%c", '<');
	else if (location[x-1][y] == '.')
	         printf("%c", '>');
	else if (location[x][y+1] == '.')
	   	     printf("%c", '^');
	else if (location[x][y-1] == '.')
			 printf("%c", 'v');		   
	
	/* search the surroundings of head of snake whether there is a bean there */
	while ((location[x+1][y] == '.')||(location[x-1][y] == '.')||
		   (location[x][y+1] == '.')||(location[x][y-1] == '.')) 
	{
		/* This part of the program is to create an "invisible snake" to "eat" the tail 
		   of the snake such that only the required "length" of the snake is shown */
		if (cnt++ >= length)
		{
			gotoxy(path_x,path_y);
			printf(" ");
			gotoxy(0,23);
	
			if (location[path_x+1][path_y] == '\1')
			{ 
				location[path_x][path_y] = '\0';
				path_x = path_x+1;
			}
			else if (location[path_x-1][path_y] == '\1')
				{	
					location[path_x][path_y] = '\0';
					path_x = path_x-1;
				}
			else if (location[path_x][path_y+1] == '\1')
				{	
					location[path_x][path_y] = '\0';
					path_y = path_y+1;
				}
			else if (location[path_x][path_y-1] == '\1')
				{
					location[path_x][path_y] = '\0';
					path_y = path_y-1;
				}
		} 
	
		/* Search the bean around the head of snake */
		if (location[x+1][y] == '.')
			x = right(location, x, y, length); /* a bean is at the right side of the head of the snake */
		else if (location[x-1][y] == '.')
				x = left(location, x, y, length); /* a bean is at the left side of the head of the snake */
		else if (location[x][y+1] == '.')
				y = down(location, x, y, length); /* a bean is at the bottom side of the head of the snake */
		else if (location[x][y-1] == '.')
				y = up(location, x, y, length); /* a bean is at the top side of the head of the snake */
	}
	
	/* Clear the head of the snake at the exit */
	if (length != 1)
	{
		if ((location[x-1][y] == '\1')||(location[x+1][y] == '\1'))
		{
			delay();
			gotoxy(x,y);
			printf("-");
		}
		else if ((location[x][y-1] == '\1')||(location[x][y+1] == '\1'))
			{
				delay();
				gotoxy(x,y);
				printf("|");
			}
		gotoxy(0,23);
	}

	/* This part of the program is for the invisible snake to "eat" the remaining tail of the snake */
	for (i=0; i<length; i++) 
	{
		gotoxy(path_x,path_y);
		printf(" ");
		gotoxy(0,23);

		if (location[path_x+1][path_y] == '\1')
		{
			delay();
			location[path_x][path_y] = '\0';
			path_x = path_x+1;
		}
		else if (location[path_x-1][path_y] == '\1')
				{	
					delay();
					location[path_x][path_y] = '\0';
					path_x = path_x-1;
				}
		else if (location[path_x][path_y+1] == '\1')
				{	
					delay();
					location[path_x][path_y] = '\0';
					path_y = path_y+1;
				}
		else if (location[path_x][path_y-1] == '\1')
				{
					delay();
					location[path_x][path_y] = '\0';
					path_y = path_y-1;
				}
	}

	/* clear the last part of the snake */
	delay();
	gotoxy(x,y);
	printf(" ");
	
	gotoxy(0,23);

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

/* If a bean is on the right side of the head of the snake , then snake eats it and moves to the right */
int right(char array[COLUMN][ROW], int x, int y, int length)
{
	delay();
	gotoxy(x,y); /* set console cursor position at the bean being eaten */
	
	if (length == 1) /* if length is 1, there is the head only so no tail is printed */
		printf(" ");
			/* when the head of the snake changes its direction, the direction of the tail changes accordingly*/
	else if ((array[x][y-1]=='\1')||(array[x][y+1]=='\1'))
			printf("|");
		 /* the direction of the tail is determined by the direction of the head of the snake */
	else printf("-");
	
	array[x][y] = '\1';
	x = x+1; /* move to the right position */
	gotoxy(x,y); /* set console cursor position at the next position */
	printf("%c", '<'); /* print the head of snake */
	gotoxy(0,23);

	return  x;
}

/* If a bean is on the left side of the head of the snake, then snake eats it and moves to the left */
int left(char array[COLUMN][ROW], int x, int y, int length)
{
	delay();
	gotoxy(x,y); /* set console cursor position at the bean being eaten */
	
	if (length == 1) /* if length is 1, there is the head only so no tail is printed */
		printf(" ");
			/* when the head of the snake changes its direction, the direction of the tail changes accordingly*/
	else if ((array[x][y-1]=='\1')||(array[x][y+1]=='\1'))
			printf("|");
		 /* the direction of the tail is determined by the direction of the head of the snake */
	else printf("-");
	
	array[x][y] = '\1';
	x = x-1; /* move to the left position */
	gotoxy(x,y); /* set console cursor position at the next position */
	printf("%c", '>'); /* print the head of snake */
	gotoxy(0,23);

	return  x;
}

/* If a bean is on the top side of the head of the snake, then snake eats it and moves up */
int up(char array[COLUMN][ROW], int x, int y, int length)
{
	delay();
	gotoxy(x,y); /* set console cursor position at the bean being eaten */

	if (length == 1) /* if length is 1, there is the head only so no tail is printed */
		printf(" "); 
			/* when the head of the snake changes its direction, the direction of the tail changes accordingly*/
	else if ((array[x-1][y]=='\1')||(array[x+1][y]=='\1'))
			printf("-");
		 /* the direction of the tail is determined by the direction of the head of the snake */
	else printf("|");
	
	array[x][y] = '\1';
	y = y-1; /* move to the upper position */
	gotoxy(x,y); /* set console cursor position at the next position */
	printf("%c", 'v'); /* print the head of snake */
	gotoxy(0,23);

	return  y;
}

/* If a bean is on the bottom side of the head of the snake, then snake eats it and moves down */
int down(char array[COLUMN][ROW], int x, int y, int length)
{
	delay();
	gotoxy(x,y); /* set console cursor position at the bean being eaten */
	
	if (length == 1) /* if length is 1, there is the head only so no tail is printed */
		printf(" ");
	        /* when the head of the snake changes its direction, the direction of the tail changes accordingly*/
	else if ((array[x-1][y]=='\1')||(array[x+1][y]=='\1'))
			printf("-");
		 /* the direction of the tail is determined by the direction of the head of the snake */
	else printf("|");
	
	array[x][y] = '\1';
	y =	y+1; /* move to the lower position */
	gotoxy(x,y); /* set console cursor position at the next position */
	printf("%c", '^'); /* print the head of snake */
	gotoxy(0,23);

	return  y;
}
