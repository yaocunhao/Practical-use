#ifndef _PLANE_H_
#define _PLANE_H_


#include <stdio.h>
#include <windows.h>
#include <time.h>

#define ROW 20
#define LINE 20


void Showmap(char str[][ROW]);//��ӡ��ͼ
void game();//���ÿ�ʼ
void Move(char str[][ROW], int row, int line);//��Ϸ����
int Buttle(char str[][LINE], int row, int line);//�ӵ�����
void FadeButtle(char str[][LINE], int row, int line, int size);//�ӵ�����
void Plan(char str[][LINE]);//�л�
void FeoButtle(char str[][LINE], int foerow, int foeline);//�л��ӵ�
int Judge(char str[][LINE], int row, int line);//�ж�����
#endif