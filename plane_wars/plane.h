#ifndef _PLANE_H_
#define _PLANE_H_


#include <stdio.h>
#include <windows.h>
#include <time.h>

#define ROW 20
#define LINE 20


void Showmap(char str[][ROW]);//打印地图
void game();//调用开始
void Move(char str[][ROW], int row, int line);//游戏运行
int Buttle(char str[][LINE], int row, int line);//子弹生成
void FadeButtle(char str[][LINE], int row, int line, int size);//子弹消除
void Plan(char str[][LINE]);//敌机
void FeoButtle(char str[][LINE], int foerow, int foeline);//敌机子弹
int Judge(char str[][LINE], int row, int line);//判定函数
#endif