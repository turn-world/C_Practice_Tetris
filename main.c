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
    SetConsoleOutputCP(65001);  // UTF-8

    srand(time(NULL));   // 랜덤 시드

    Tetromino current;
    memcpy(current.shape, shapes[0], sizeof(shapes[0])); // I블록
    current.color = 9;   // 파란색
    current.x = 3;       // 가로 중앙 근처
    current.y = 0;       // 맨 위에서 시작
    drawPiece(&current);
    drawBoard();

    DWORD lastTime = GetTickCount();

    while(1)
    {
        // 1. 입력 처리
        if(kbhit())
        {
        int key = getch();
            if(key == 224)
            {
                key = getch();
                erasePiece(&current);   // 먼저 지우고
                if(key == 75 && canMove(&current, -1, 0)) current.x--;
                else if(key == 77 && canMove(&current,  1, 0)) current.x++;
                else if(key == 80 && canMove(&current,  0, 1)) current.y++;
                drawPiece(&current);    // 다시 그리기
            }
            else if(key == 32)
            {
                erasePiece(&current);
                rotatePiece(&current);
                drawPiece(&current);
            }
            drawBoard();
        }
        // 2. 시간 처리 (블록 자동 낙하)
        if(GetTickCount() - lastTime > 500)
        {
            lastTime = GetTickCount();
            erasePiece(&current);   // 추가
        if(canMove(&current, 0, 1))
        {
            current.y++;
            drawPiece(&current);
        }
        else
        {
            drawPiece(&current);  // 고정
            clearLines();
                // 새 블록 생성
            memcpy(current.shape, shapes[rand() % 7], sizeof(shapes[0]));
            current.color = (rand() % 6) + 9;
            current.x = 3;
            current.y = 0;

                // 새 블록도 못 놓으면 게임오버
                if(!canMove(&current, 0, 0))
                {
                drawBoard();
                gotoxy(5, 10);
                printf("GAME OVER");
                return 0;
                }
            drawPiece(&current);
        }
            drawBoard();
        }
        // 3. 화면 갱신
    }

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

void rotatePiece(Tetromino *piece)
{
    int temp[4][4] = {0};

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            temp[j][3-i] = piece->shape[i][j];
        
    // 임시 구조체에 회전 결과 + 현재 위치 넣기
    Tetromino tmp = *piece;          // piece 전체 복사
    memcpy(tmp.shape, temp, sizeof(temp));

    // 제자리(dx=0, dy=0)에서 충돌 검사
    if(canMove(&tmp, 0, 0))
        memcpy(piece->shape, temp, sizeof(temp));  // 가능하면 적용
    // 불가능하면 아무것도 안 함 (원래 shape 유지)

    
}

void clearLines()
{
    for(int i = BOARD_HEIGHT - 1; i >= 0; i--)
    {
        // 이 행이 꽉 찼는지 확인
        int full = 1;
        for(int j = 0; j < BOARD_WIDTH; j++)
            if(board[i][j] == 0) { full = 0; break; }

        if(full)
        {
        // i번째 행부터 위로 올라가면서 전부 한 칸씩 내리기
        for(int k = i; k > 0; k--)
            for(int j = 0; j < BOARD_WIDTH; j++)
                board[k][j] = board[k-1][j];

        // 맨 위 행(0번째) 초기화
        for(int j = 0; j < BOARD_WIDTH; j++)
            board[0][j] = 0;

        i++;  // 같은 행 다시 검사 ✓
        }
    }
}