#pragma once

#include <stdio.h>
#include <graphics.h>
#pragma warning (disable :4996)
#include <windows.h>


#define SIZE 700 //������С
#define NUM  60 //���ִ�С
#define ROW 9
#define COL 9
#define NUM2 30


void Game();

void Load();//ͼƬ����
void Show();//����չʾ

int Input(char *str);
void Word();
bool JudgeNum(char sudu[][COL], int row, int col, char ch);