#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <graphics.h>

#define MAX 20
#define ROW 20
#define LINE 20



void Game();
void GetCoordinate(int *row, int *line, char *move);
#endif