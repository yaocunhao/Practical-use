#ifndef _GAME_H_
#define _GAME_H_

#include <stdio.h>
#include <windows.h>
#pragma warning (disable :4996)

#define ROW 10
#define LINE 10

void SlectMap(char arr[][LINE], char[][ROW][LINE],int select);
void Game();
void Retain(char retarr[][LINE], char arr[][LINE]);
void Save(char retarr[][LINE], char arr[][LINE], int *row, int *line, int *retrow, int *retline);
#endif