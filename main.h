#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define BOARD_WIDTH  10
#define BOARD_HEIGHT 20
#define BOARD_X      2    // 보드가 콘솔에 그려지는 시작 x좌표
#define BOARD_Y      1    // 보드가 콘솔에 그려지는 시작 y좌표

static int board[BOARD_HEIGHT][BOARD_WIDTH];

typedef struct {
    int shape[4][4];  // 블록 모양
    int color;        // 블록 색상
    int x, y;         // 현재 보드 위치
} Tetromino;

extern int shapes[7][4][4];
// int shapes[7][4][4]의 경우는  실제 메모리 할당
// extern int shapes[7][4][4];의 경우는 어딘가에 있고, 메모리 할당 X

void gotoxy(int x, int y);
void setColor(int color);
void drawBoard();
void drawPiece(Tetromino *piece);
void erasePiece(Tetromino *piece);

// 이동 가능 여부 확인 — 가능하면 1, 불가능하면 0
int canMove(Tetromino *piece, int dx, int dy);

// 실제 이동
void movePiece(Tetromino *piece, int dx, int dy);
// 회전
void rotatePiece(Tetromino *piece);
// 줄 삭제
void clearLines();