/* SimUniversity program skeleton */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define DELAYTIME 70000000
#define file "playboard.dat"
#define cfile "college.dat"
#define CHECKER 20
#define PLAYER 4
#define DICE_RANGE 6
#define DEPT_COST 100   

/* typedef structures */
typedef struct{
	char name[4];	/* 3 chars for dept name, remaining one for '\0' */
	
	char ctype;	/* stores the type of checker - 'd'/'c'/'g' */
	
	int owner;	/* only for type 'd': 0 means A, 1 means B  */
			/* and so on. -1 means no one owns it       */
	                
	int course;	/* only for type 'd': stores number of courses.  */

	int x,y; /* x -- x-coordinate of the checker 
			  * y -- y-coordinate of the checker */

} Dept;

typedef struct{
	int HP;		/* stores the HP (energy) of the player */
	int current;	/* stores the current position of the player */
} Player;

typedef struct{
	char event[31];
	int HPchange;
} College;

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

/* function prototypes */
void delay()
{
    int i;

    for (i=0;i<DELAYTIME;i++) {};
}

void PrintMaze(char maze[80][25], int, int);
void ClearMessageBoard(void);
void ClearPreviousPosition(Dept [], Player [], int);
void Move(Dept [], Player [], College [], int, int, int);
void PrintHP(Player []);

int main(void)
{
	FILE *fp, *cfp; 
	/* fp -- a file pointer is used to read the content of the "file"
	 * cfp -- a file pointer is used to read the content of the "cfile"
	 */
	int i, j, row, column, eventnum, dice, turn=0, round=0;
	/* i,j -- use as the counters
	 * row -- number of the row in the "file"
	 * column -- number of the column in the "file"
	 * eventnum -- number of the events of each college
	 * dice -- value of the dice thrown (1-6)
	 * turn -- 0 = A, 1 = B, 2 = C, 3 = D
	 * round -- counter
	 */

	char maze[80][25], dummy;
	/* maze[][] -- stores the maze of the SimUniversity
	 * dummy -- stores the "newline" charater
	 */
	
    Dept department[CHECKER]; 
    Player player[PLAYER];
	College college[40];
	/* department[] -- array of structure which stores the name of the department
	 *				   or college and type, owner, number of course offered and the 
	 *				   coordinates of the checker
     * player[] -- array of structure which stores the HP(energy) and the currrent
	 *             position of the player
	 * college[] -- array of sturcture which stores the college events of 4 colleges
	 *              and stores the points(positive or negative(maybe zero)) according to the good 
	 *				or bad luck messages
	 */

    /* START OF GIVEN CODE - SECTION2 */
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
/* END OF GIVEN CODE - SECTION2 */

	/* open the "file" and "cfile" using fp and cfp respectively */
	fp = fopen(file, "r"); 
	cfp = fopen(cfile, "r");

	/* if the fp or cfp is equal to "NULL" charater, the files can't be opened */
	if (fp == NULL)
		return -1;

	if (cfp == NULL)
		return -1;

	fscanf(fp,"%d", &row); /* read the number of row in "file" */
	fscanf(fp,"%d", &column); /* read the number of column in "file" */
	fscanf(fp,"%c", &dummy); /* read the "newline" character */

	/* read the maze of SimUniversity in "file" */
	for (j=0; j<row; j++)
	{
		for (i=0; i<column; i++)
			fscanf(fp, "%c", &maze[i][j]);

		fscanf(fp,"%c", &dummy);
	}

	/* read the information of each checker */
	for (i=0; i<CHECKER; i++)
	{
		fscanf(fp, "%s %c %d %d", department[i].name, &department[i].ctype,
			   &department[i].y, &department[i].x);
		
		fscanf(fp,"%c", &dummy);

		department[i].owner = -1;
		department[i].course = 0;
	}
	
	fscanf(cfp,"%d", &eventnum); /* read the number of college events in "cfile" */
	fscanf(cfp,"%c", &dummy); /* read the "newline" charater */

	/* read the messages of college events and the corresponding points gained or lost in "cfile" */
	for (i=0; i<eventnum*4; i++)
	{
		fgets(college[i].event, 30, cfp);
		fscanf(cfp, "%d", &college[i].HPchange);

		fscanf(cfp,"%c", &dummy);
	}
	
	/* Initiatization : set each player HP = 1000 and the starting position at "0" which is "Go" checker */
	for (i=0; i<PLAYER; i++)
	{
		player[i].HP = 1000;
		player[i].current = 0;		
	}
	
	/* Print the maze of the SimUniversity */
	PrintMaze(maze, row, column);

	srand(time(NULL)); /* use the time as a seed to choose the random number sequence */

	/* If there is no player with negative energy, the game continues. If there is one player with negative energy, the game stops */
	while ((player[0].HP>=0)&&(player[1].HP>=0)&&(player[2].HP>=0)&&(player[3].HP>=0))
	{		
		delay();

		ClearMessageBoard(); /* Clear the message board each time */
		
		turn = round%4; /* each player plays one by one */
		dice = rand()%DICE_RANGE +1; /* "throw" a dice using the random number */
		
		ClearPreviousPosition(department, player, turn); /* clear the previous position of the player */

		gotoxy(0,0);
		
		player[turn].current += dice; /* trace the position of the player in the maze */
		
		/* if the player finishes one loop, the next position is adjusted by deducting 20 */
		if (player[turn].current >=20)
		{
			player[turn].current -= 20;
			player[turn].HP += 200;
		}
		
		/* movement of the player */
		Move(department, player, college, turn, dice, eventnum);
		
		delay();

		round++; /* as a counter */
	}

	gotoxy(0,23);

	/* close the files */
	fclose(fp); 
	fclose(cfp);

    return(0);
}

/* Print the Maze of the SimUniveristy */
void PrintMaze(char maze[80][25], int row, int column)
{
	int i, j;

	gotoxy(0,0);

	for (j=0; j<row; j++)
	{
		for (i=0; i<column; i++)
			printf("%c", maze[i][j]);
	
		printf("\n");
	}
}

/* Clear the meassage board */
void ClearMessageBoard(void)
{
	int i;

	gotoxy(17,9);
	for (i=0; i<30; i++)
		printf(" ");

	gotoxy(17,10);
	for (i=0; i<30; i++)
		printf(" ");

	gotoxy(17,11);
	for (i=0; i<30; i++)
		printf(" ");
		
	gotoxy(17,12);
	for (i=0; i<30; i++)
		printf(" ");
}

/* clear the previous position of the player when it is its turn */
void ClearPreviousPosition(Dept department[], Player player [], int turn)
{
	if (turn == 0)
	{
		gotoxy(department[player[turn].current].x + 2,department[player[turn].current].y + 2);
		printf(" ");
	}
	else if (turn == 1)
		{
			gotoxy(department[player[turn].current].x + 3,department[player[turn].current].y + 2);
			printf(" ");
		}
	else if (turn == 2)
		{
			gotoxy(department[player[turn].current].x + 4,department[player[turn].current].y + 2);
			printf(" ");
		}
	else if (turn == 3)
		{
			gotoxy(department[player[turn].current].x + 5,department[player[turn].current].y + 2);
			printf(" ");
		}
}

/* Each player's movement: number of steps it walks, action taken(own a department/pay energy/add courses/leave) and/or college activities come across */ 
void Move(Dept department[], Player player[], College college[], int turn, int dice, int eventnum)
{
	int number; /* randomly choose the evnets */

	/* print the new position of the player in the stepped checker */
	switch (turn)
	{
		case 0: gotoxy(department[player[turn].current].x + 2,department[player[turn].current].y + 2);
				printf("A");
				gotoxy(25,10);
				printf("A walks: %d steps", dice);
				break;
		case 1: gotoxy(department[player[turn].current].x + 3,department[player[turn].current].y + 2);
				printf("B");
				gotoxy(25,10);
				printf("B walks: %d steps", dice);
				break;
		case 2: gotoxy(department[player[turn].current].x + 4,department[player[turn].current].y + 2);
				printf("C");
				gotoxy(25,10);
				printf("C walks: %d steps", dice);
				break;
		case 3: gotoxy(department[player[turn].current].x + 5,department[player[turn].current].y + 2);
				printf("D");
				gotoxy(25,10);
				printf("D walks: %d steps", dice);
				break;
	}
	
	/* if the player steps on the department checker */
	if (department[player[turn].current].ctype == 'd')
	{
		/* if no one owns this department */
		if (department[player[turn].current].owner == -1)
		{
			/* if the player has enough energy */
			if (player[turn].HP >= DEPT_COST)
			{
				department[player[turn].current].owner = turn; /* this department belongs to the player */
				player[turn].HP -= DEPT_COST; /* the player consumes 100 energy */
				department[player[turn].current].course++; /* no. of course increase by 1 */

				/* goto the top right corner of the checker and print 'A/B/C/D' to indicate the department owned by the player */
				gotoxy(department[player[turn].current].x + 7,department[player[turn].current].y);

				switch (turn)
				{
					/* goto the message board to print the message that the player owns this department */
					case 0:	printf("A");
							gotoxy(21,11);
							printf("A takes control of %s!", department[player[turn].current].name);
							break;
					case 1: printf("B");
							gotoxy(21,11);
							printf("B takes control of %s!", department[player[turn].current].name);
							break;
					case 2: printf("C");
							gotoxy(21,11);
							printf("C takes control of %s!", department[player[turn].current].name);
							break;
					case 3: printf("D");
							gotoxy(21,11);
							printf("D takes control of %s!", department[player[turn].current].name);
							break;
				}

				/* print the number of course offered by this department and shown in this checker */
				gotoxy(department[player[turn].current].x + 4,department[player[turn].current].y);
				printf("%d", department[player[turn].current].course);
			}
		}
		else if (department[player[turn].current].owner == turn) /* if this department is alreadly owned by the player */
			{
				/* if the player has enough energy */
				if (player[turn].HP >= DEPT_COST)
				{
					department[player[turn].current].course++; /* no. of course increase by 1 */
					player[turn].HP -= DEPT_COST; /* 'A' consumes 100 energy */

					/* goto the message board to print the message that the player adds a new course in this department */
					gotoxy(19,11);
					
					switch (turn)
					{
						case 0:	printf("A adds a new course in %s!", department[player[turn].current].name);
								break;
						case 1: printf("B adds a new course in %s!", department[player[turn].current].name);
								break;
						case 2: printf("C adds a new course in %s!", department[player[turn].current].name);
								break;
						case 3: printf("D adds a new course in %s!", department[player[turn].current].name);
								break;
					}
					
					/* print the number of course offered by this department and shown in this checker */
					gotoxy(department[player[turn].current].x + 4,department[player[turn].current].y);
					printf("%d", department[player[turn].current].course);
				}
			}
		else 
			{
				player[turn].HP -= DEPT_COST*department[player[turn].current].course ; /* pays the energy to the department owner according to no. of courses offered*/
				player[department[player[turn].current].owner].HP += DEPT_COST*department[player[turn].current].course; /* the department owner then increases energy by same amount of energy as 'A' loses */
				
				/* goto the message board to print the message that the player pays energy to department owner */
				gotoxy(23,11);

				switch (turn)
				{
					case 0:	if (department[player[turn].current].owner == 1)
								printf("A pays energy to B !");
							else if (department[player[turn].current].owner == 2)
									printf("A pays energy to C !");
							else if (department[player[turn].current].owner == 3)
									printf("A pays energy to D !");
							break;
					case 1: if (department[player[turn].current].owner == 0)
								printf("B pays energy to A !");
							else if (department[player[turn].current].owner == 2)
									printf("B pays energy to C !");
							else if (department[player[turn].current].owner == 3)
									printf("B pays energy to D !");
							break;
					case 2: if (department[player[turn].current].owner == 0)
								printf("C pays energy to A !");
							else if (department[player[turn].current].owner == 1)
									printf("C pays energy to B !");
							else if (department[player[turn].current].owner == 3)
									printf("C pays energy to D !");
							break;
					case 3: if (department[player[turn].current].owner == 0)
								printf("D pays energy to A !");
							else if (department[player[turn].current].owner == 1)
									printf("D pays energy to B !");
							else if (department[player[turn].current].owner == 2)
									printf("D pays energy to C !");
							break;
				}
			}
	}

	/* if the player steps on the college checker */
	if (department[player[turn].current].ctype == 'c')
	{
		/* check which college the player steps on */
		switch(department[player[turn].current].name[0])
		{
			/* 'N' = NA, 'U' = UC, 'C' = CC, 'S' = SC */
			case 'N': number = (rand()%eventnum); break;
			case 'U': number = (rand()%eventnum + eventnum); break;
			case 'C': number = (rand()%eventnum + eventnum*2); break;
			case 'S': number = (rand()%eventnum + eventnum*3); break;
		}
							
		/* goto message board to print the message of the college event */
		gotoxy(18,11);				

		switch (turn)
		{
			case 0: printf("A %s", college[number].event);
					break;
			case 1: printf("B %s", college[number].event);
					break;
			case 2: printf("C %s", college[number].event);
					break;
			case 3: printf("D %s", college[number].event);
					break;
		}

		/* goto message board to print the points gained or lost corresponded to the college event */
		gotoxy(25,12);

		if  (college[number].HPchange < 0)
			printf("Loses %d points", -college[number].HPchange);
		else printf("Gains %d points", college[number].HPchange);

		/* player then gains or loses the amount of energy */
		player[turn].HP += college[number].HPchange;
	}
	
	PrintHP(player);
}

/* print the new values of energy of four players */
void PrintHP(Player player[])
{	
	gotoxy(12,5);
	printf("    ");
	gotoxy(12,5);
	printf("%d", player[0].HP);

	gotoxy(12,15);
	printf("    ");
	gotoxy(12,15);
	printf("%d", player[1].HP);

	gotoxy(50,5);
	printf("    ");
	gotoxy(50,5);			
	printf("%d", player[2].HP);

	gotoxy(50,15);
	printf("    ");
	gotoxy(50,15);
	printf("%d", player[3].HP);

	gotoxy(0,0);
}
