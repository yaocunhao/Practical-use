#pragma once

#include <stdio.h>
#include <graphics.h>
#pragma warning (disable :4996)
#include <windows.h>


#define SIZE 700 //背景大小
#define NUM  60 //数字大小
#define ROW 9
#define COL 9
#define NUM2 30


void Game();

void Load();//图片加载
void Show();//画面展示

int Input(char *str);
void Word();
bool JudgeNum(char sudu[][COL], int row, int col, char ch);