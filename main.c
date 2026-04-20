#include "main.h"

int shapes[7][4][4] = {
    { {0,0,0,0},
      {1,1,1,1},
      {0,0,0,0},
      {0,0,0,0} },

      { {0,0,0,0},
      {0,1,1,0},
      {0,1,1,0},
      {0,0,0,0} },

      { {1,0,0,0},
      {1,1,1,0},
      {0,0,0,0},
      {0,0,0,0} },

      { {0,1,1,0},
      {1,1,0,0},
      {0,0,0,0},
      {0,0,0,0} },

      { {1,1,0,0},
      {0,1,1,0},
      {0,0,0,0},
      {0,0,0,0} },

      { {0,0,1,0},
      {1,1,1,0},
      {0,0,0,0},
      {0,0,0,0} },

      { {0,1,0,0},
      {1,1,1,0},
      {0,0,0,0},
      {0,0,0,0} },

};

int main()
{
    drawBoard();
    return 0;
}

// 커서를 원하는 좌표로 이동하는 함수
void gotoxy(int x, int y)
{
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    // 반환 값은 bool이지만, 테트리스라서 상관 X
}
//콘솔 텍스트 색상 변경
void setColor(int color)
{   
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
// 도화지깔기
void drawBoard()
{
    int i, j;

    // 1. 윗 테두리
    gotoxy(BOARD_X, BOARD_Y);
    for (j = 0; j < BOARD_WIDTH + 2; j++)
        printf("#");

    // 2. 중간 행들
    for (i = 0; i < BOARD_HEIGHT; i++)
    {
        gotoxy(BOARD_X, BOARD_Y + 1 + i);
        printf("#");   // 왼쪽 벽

        for (j = 0; j < BOARD_WIDTH; j++)
        {
            if (board[i][j] == 0)
                printf(" ");   // 빈 칸
            else
                printf("■");  // 블록
        }

        printf("#");   // 오른쪽 벽
    }

    // 3. 아랫 테두리
    gotoxy(BOARD_X, BOARD_Y + 1 + BOARD_HEIGHT);
    for (j = 0; j < BOARD_WIDTH + 2; j++)
        printf("#");
}

// 테트리스 피스 그리기
void drawPiece(Tetromino *piece)
{
    for(int i = 0; i < 4; i++)
    {
        for (int j = 0 ; j < 4; j++)
        {
            if(piece->shape[i][j] == 1)
            {
                board[piece->y + i][piece->x + j] = piece->color;
            }
        }
    }
}
// 테트리스 피스 지우기
void erasePiece(Tetromino *piece)
{
    for(int i = 0; i < 4; i++)
    {
        for (int j = 0 ; j < 4; j++)
        {
            if(piece->shape[i][j] == 1)
            {
                board[piece->y + i][piece->x + j] = 0;
            }
        }
    }
}

// 이동 가능 여부 확인 — 가능하면 1, 불가능하면 0
int canMove(Tetromino *piece, int dx, int dy)
{
    for(int i = 0 ; i < 4 ; i ++)
    {
        for (int j = 0 ; j < 4; j++)
        {
            if(piece->shape[i][j] == 1)
            {
                if(piece->y + i + dy < 0 || piece->y + i + dy >= BOARD_HEIGHT
                || piece->x + j + dx < 0 || piece->x + j + dx >=BOARD_WIDTH)
                {
                    return 0;
                }
                else if( board[piece->y + i + dy][piece->x + j + dx] != 0)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

// 실제 이동
void movePiece(Tetromino *piece, int dx, int dy)
{
    erasePiece(piece);
    piece->x = piece->x + dx;
    piece->y = piece->y + dy;
    drawPiece(piece);
}

