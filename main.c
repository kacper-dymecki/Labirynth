/*
 * Copyright 2017 Kacper Dymecki

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "include/getch.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#define CLEARCONSOLE "cls"
#else
#define CLEARCONSOLE "clear"
#endif

#define TRUE 1
#define FALSE 0

#define DEBUG FALSE //zmienić na false by wyłączyć debuginfo

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"	
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

//Zmiana wielkości mapy
#define WIDTH 50
#define HEIGHT 25

struct player
{
	char name[17];
	char symbol;
	int playerPositionX;
	int playerPositionY;
};

char map[WIDTH + 1][HEIGHT + 1];

void clearScreen()
{
	system(CLEARCONSOLE);
}

FILE * file;

int loadOptions(struct player * Player)
{
	char Playername[17];
	char Playersymbol;
	if((file = fopen("data/options.ini", "r")) == NULL)
	{
		return FALSE;
	}
	fscanf(file, "%s %c", Playername, &Playersymbol);
	fclose(file);
	strcpy((*Player).name, Playername);
	(*Player).symbol = Playersymbol;
	return TRUE;
}
void writeOptions(struct player * Player)
{
	file = fopen("data/options.ini", "w");
	fprintf(file, "%s %c", (*Player).name, (*Player).symbol);
    	fclose(file);
}

void addToScoreboard(int score, struct player * Player)
{
    if(loadOptions(Player) == TRUE)
    {
        char buffer[20];
        file = fopen("data/leaderboard.txt", "a");
        sprintf(buffer,"%s %d", (*Player).name, score);
        fprintf(file, "%s\n", buffer);
        fclose(file);
    }
}

void readScoreboard()
{
    if((file = fopen("data/leaderboard.txt", "r")) != NULL)
    {
        char name[5][17];
        int score[5];
        int i = 0,j;
        while(fscanf(file, "%s %d", name[i % 5], &score[i % 5]) != EOF)
        {
            i++;
        }
	for(j = i % 5; j < 5; j++)
	{
		printf("%s %d\n", name[j], score[j]);
	}
	for(j = 0; j < i % 5; j++)
	{
		printf("%s %d\n", name[j], score[j]);		
	}	
        fclose(file);
    }
}

char randChar()
{
	if(rand() % 4 == 1)
	{
		return 'x';
	}
	else
	{
		return 32;
	}
}

void fillMap(struct player * Player)
{
	int i,j;
	int blockades = 0, maxBlockadeCount = ((WIDTH * HEIGHT) / 4);
	for(i = 0; i < WIDTH; i++)
	{
		for(j = 0; j < HEIGHT; j++)
		{
			if(i == (*Player).playerPositionX && j == (*Player).playerPositionY) map[i][j] = (*Player).symbol;
			else
			{
				if(blockades < maxBlockadeCount)
				{
					map[i][j] = randChar();
					if(map[i][j] == 'x') blockades++;
				}
				else map[i][j] = 32;
			}
		}
	}
	for(i = 0; i < WIDTH; i++)
	{
		if(i < HEIGHT)
		{
			map[0][i] = '|';
			map[WIDTH][i] = '|';
		}
		map[i][0] = '-';
		map[i][HEIGHT] = '-';
	}
	map[0][0] = '+';
	map[WIDTH][0] = '+';
	map[0][HEIGHT] = '+';
	map[WIDTH][HEIGHT] = '+';
}

void drawMap(struct player * Player)
{	
	int i = 0, j = 0;
	for(i = 0; i <= HEIGHT; i++)
	{
		for(j = 0; j <= WIDTH; j++)
		{
			if((map[j][i] == '*')  && (map[j][i] != (*Player).symbol))
			{
				printf("%s%c", KRED,map[j][i]);
			}
			else if(map[j][i] == (*Player).symbol)
			{
				printf("%s%c", KCYN,map[j][i]);
			}
			else
			{
				printf("%s%c", KNRM,map[j][i]);
			}
		}
		printf("\n");
	}
}

int pktX = 55, pktY = 55;
int moveChar(char move, struct player * Player)
{
	if(move == 'w')
	{
		if(map[(*Player).playerPositionX][(*Player).playerPositionY - 1] == 32 || map[(*Player).playerPositionX][(*Player).playerPositionY - 1] == '*')
		{
			(*Player).playerPositionY -= 1;
			if(map[(*Player).playerPositionX][(*Player).playerPositionY] == '*')
			{
				map[(*Player).playerPositionX][(*Player).playerPositionY] = (*Player).symbol;
				map[(*Player).playerPositionX][(*Player).playerPositionY + 1] = 32;
				return 1;					
			}
			else
			{
				map[(*Player).playerPositionX][(*Player).playerPositionY] = (*Player).symbol;
				map[(*Player).playerPositionX][(*Player).playerPositionY + 1] = 32;
				return 0;
			}
                }
                else return -1;
	}
	else if(move == 's')
	{
		if(map[(*Player).playerPositionX][(*Player).playerPositionY + 1] == 32 || map[(*Player).playerPositionX][(*Player).playerPositionY + 1] == '*')
                {
	                (*Player).playerPositionY += 1;
                        if(map[(*Player).playerPositionX][(*Player).playerPositionY] == '*')
                        {
				map[(*Player).playerPositionX][(*Player).playerPositionY] = (*Player).symbol;
				map[(*Player).playerPositionX][(*Player).playerPositionY - 1] = 32;
                                return 1;
                        }
			else
			{
				map[(*Player).playerPositionX][(*Player).playerPositionY] = (*Player).symbol;
				map[(*Player).playerPositionX][(*Player).playerPositionY - 1] = 32;
				return 0;
			}
                }
                else return -1;
	}
	else if(move == 'a')
	{
		if(map[(*Player).playerPositionX - 1][(*Player).playerPositionY] == 32 || map[(*Player).playerPositionX - 1][(*Player).playerPositionY] == '*')
                {
	                (*Player).playerPositionX -= 1;
                        if(map[(*Player).playerPositionX - 1][(*Player).playerPositionY] == '*')
                        {
				map[(*Player).playerPositionX][(*Player).playerPositionY] = (*Player).symbol;
				map[(*Player).playerPositionX + 1][(*Player).playerPositionY] = 32;
                                return 1;
                        }
			else
			{
				map[(*Player).playerPositionX][(*Player).playerPositionY] = (*Player).symbol;
				map[(*Player).playerPositionX + 1][(*Player).playerPositionY] = 32;
				return 0;
			}
                }
                else return -1;
	}
	else if(move == 'd')
	{
		if(map[(*Player).playerPositionX + 1][(*Player).playerPositionY] == 32 || map[(*Player).playerPositionX + 1][(*Player).playerPositionY] == '*')
                {
	                (*Player).playerPositionX += 1;
                        if(map[(*Player).playerPositionX][(*Player).playerPositionY] == '*')
                        {
				map[(*Player).playerPositionX][(*Player).playerPositionY] = (*Player).symbol;
				map[(*Player).playerPositionX - 1][(*Player).playerPositionY] = 32;
                                return 1;
                        }
			else
			{
				map[(*Player).playerPositionX][(*Player).playerPositionY] = (*Player).symbol;
				map[(*Player).playerPositionX - 1][(*Player).playerPositionY] = 32;
				return 0;
			}
                }
                else return -1;
	}
}


void spawnPoint(struct player * Player)
{
	int x,y;
	x = rand() % WIDTH;
	y = rand() % HEIGHT;
	if(map[x][y] != (*Player).symbol && map[x][y] != 'x' && map[x][y] != '|' && map[x][y] != '-' && map[x][y] != '+')
	{
		map[pktX][pktY] = 32;
		map[x][y] = '*';
		pktX = x;
		pktY = y;			
	}	
	else
	{
		spawnPoint(Player);
	}
}

void playGame(struct player * Player)
{
	int maxMoves = WIDTH * HEIGHT - (WIDTH * HEIGHT / 6);
	int score = 0;
	int moveCounter = 0;
	int moveCharValue = 0;	
	(*Player).playerPositionX = 1;
	(*Player).playerPositionY = 1;
	fillMap(Player);
	while(maxMoves > 0)
	{
		if((moveCounter % (WIDTH + HEIGHT)) == 0)
		{
			moveCounter = 0;
			spawnPoint(Player);
		}
		clearScreen();
		printf("Punkty : %d\nIlosc ruchow : %d\n", score, maxMoves);
		if(DEBUG == TRUE)
		{
			printf("PktX/PktY/MoveCharValue : %d/%d/%d\nPlayerPos : %d/%d\nMoveCounter : %d\n", pktX, pktY,moveCharValue, (*Player).playerPositionX, (*Player).playerPositionY, moveCounter);
		}		
		drawMap(Player);
		fflush(stdin);
		char move = getch();
		fflush(stdin);
		if(move == 'k')
		{
			break;
		}	
		else if(move == 'r')
		{
			moveCounter = 0;

			spawnPoint(Player);
			maxMoves = maxMoves - ((WIDTH * HEIGHT) / 10);
		}
		else
		{
			moveCounter++;
			moveCharValue = moveChar(move, Player);
			if(moveCharValue == 1 || (moveCharValue == 0 && (*Player).playerPositionX == pktX && (*Player).playerPositionY == pktY))
			{
				score++;
				maxMoves += moveCounter / 2;
				moveCounter = 0;
			spawnPoint(Player);
				map[(*Player).playerPositionX][(*Player).playerPositionY] = (*Player).symbol;	
			}
			if(moveCharValue == 0 || moveCharValue > 1)
			{
				maxMoves--;
			}
			if(moveCharValue == -1)
			{
				maxMoves = maxMoves - 5;
			}
		}
	}
        addToScoreboard(score, Player);
}

void helpMenu(struct player * Player)
{
	clearScreen();
	fflush(stdin);
	printf("Labirynt jest grą prostą.\nCelem gry jest zdobycie jak najwyższej liczby punktów w ograniczonej liczbie ruchów\n");
	printf("Aktualne ustawienia gry to :\n");
	printf("Rozmiar mapy : %dx%d\n", WIDTH, HEIGHT);
	printf("Maksymalna liczba ruchów : %d\n", (WIDTH * HEIGHT / 6));
	printf("Maksymalna liczba przeszkód : %d\n", (WIDTH * HEIGHT / 4));
	printf("Położenie punktu do zebrania zmienia się po 60 ruchach gracza\n");
	printf("Po zebraniu punktu, maksymalna liczba ruchów zwiększa się o : %d\n", (WIDTH + HEIGHT));
	printf("Za próbę wejścia w ścianę, gracz traci 5 ruchów z puli maksymalnych ruchów\n");
	printf("Sterowanie :\n W A S D - Ruch w Górę/Lewo/Dół/Prawo\n");
	printf("R - zmiana pozycji punktu, za co traci się 50 ruchów z puli maksymalnych ruchów\n");
	printf("K - koniec gry\n");
	printf("Znak gracza : %s%c\n%s", KCYN, (*Player).symbol, KWHT);
	printf("Znak punktu do zebrania : %s*\n%s", KRED, KWHT);
	printf("Wpisz dowolny ciąg znaków by wyjść do głównego menu");
	char temp[100];
	scanf("%s", temp);
	fflush(stdin);
	clearScreen();
}

void optionsMenu(struct player * Player)
{
    	clearScreen();
	char nickname[17];
	char symbol;
	printf("Podaj nazwe gracza : ");
	scanf("%s",nickname);
	fflush(stdin);
	printf("\nPodaj znak gracza : ");
	scanf(" %c",&symbol);
	fflush(stdin);
	strcpy((*Player).name, nickname);
	(*Player).symbol = symbol;
	int seed;
	printf("\nPodaj nowy seed : ");
	scanf("%d", &seed);
	srand(seed);
	writeOptions(Player);
}

int main()
{
	struct player Player;
	srand(time(NULL) * time(NULL));
	while(TRUE != FALSE)
	{
        	if(loadOptions(&Player) == FALSE)
       		{
       			strcpy(Player.name , "Default");
        		Player.symbol = '@';
        	}
		clearScreen();
		printf("%sNick: %s\nSymbol: %c\n", KWHT, Player.name, Player.symbol);
		printf("______________________\n");
		printf("|                    |\n");
		printf("|     1. Graj	     |\n");
		printf("|     2. Opcje	     |\n");
		printf("|     3. Pomoc       |\n");
		printf("|     4. Wyjdź	     |\n");
		printf("|____________________|\n");
		printf("\n\nNajnowsze wyniki\n");        	
		readScoreboard();
		int menuSwitch = 0;
		scanf("%d", &menuSwitch);
		switch(menuSwitch)
		{
			case 1:
			{
				playGame(&Player);
				break;	
			}
			case 2:
			{
				optionsMenu(&Player);
				break;	
			}
			case 3:
			{
				helpMenu(&Player);
				break;
			}
			case 4:
			{
				clearScreen();
				return 0;
				break;	
			}
			default:
			{
				break;		
			}
		}
		clearScreen();
	}
	clearScreen();
	return 0;	
}
