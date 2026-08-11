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
    SetConsoleOutputCP(65001);  // 콘솔 출력 UTF-8 설정 (한글/특수문자 깨짐 방지)

    srand(time(NULL));   // 랜덤 시드 초기화 (매 실행마다 다른 블록 생성)

    Tetromino current;
    memcpy(current.shape, shapes[0], sizeof(shapes[0])); // 초기 블록: I 블록 복사
    current.color = 9;   // 블록 색상 설정 (파란색)
    current.x = 3;       // 시작 위치 (가로 중앙 근처)
    current.y = 0;       // 시작 위치 (맨 위)
    
    drawPiece(&current); // 현재 블록 화면에 출력
    drawBoard();         // 보드 상태 출력

    DWORD lastTime = GetTickCount(); // 마지막 시간 저장 (자동 낙하용)

    while(1)
    {
        // ---------------------------
        // 1. 키보드 입력 처리
        // ---------------------------
        if(kbhit())  // 키 입력이 있으면
        {
            int key = getch();

            if(key == 224)  // 방향키 입력 (확장 키)
            {
                key = getch(); // 실제 방향키 값 읽기
                erasePiece(&current);   // 이동 전 현재 블록 지우기

                // 왼쪽 이동
                if(key == 75 && canMove(&current, -1, 0)) current.x--;
                // 오른쪽 이동
                else if(key == 77 && canMove(&current,  1, 0)) current.x++;
                // 아래 이동 (소프트 드롭)
                else if(key == 80 && canMove(&current,  0, 1)) current.y++;

                drawPiece(&current);    // 이동 후 다시 그리기
            }
            // 스페이스바 → 회전
            else if(key == 32)
            {
                erasePiece(&current);   // 회전 전 지우기
                rotatePiece(&current);  // 블록 회전
                drawPiece(&current);    // 다시 출력
            }

            drawBoard(); // 보드 갱신
        }

        // ---------------------------
        // 2. 시간 기반 자동 낙하 처리
        // ---------------------------
        if(GetTickCount() - lastTime > 500) // 0.5초마다 실행
        {
            lastTime = GetTickCount(); // 시간 갱신
            erasePiece(&current);      // 이동 전 블록 제거

            if(canMove(&current, 0, 1)) // 아래로 이동 가능하면
            {
                current.y++;           // 한 칸 아래로 이동
                drawPiece(&current);
            }
            else // 더 이상 내려갈 수 없으면
            {
                drawPiece(&current);   // 현재 위치에 고정

                clearLines();          // 꽉 찬 줄 제거

                // ---------------------------
                // 새로운 블록 생성
                // ---------------------------
                memcpy(current.shape, shapes[rand() % 7], sizeof(shapes[0])); // 랜덤 블록
                current.color = (rand() % 6) + 9; // 랜덤 색상
                current.x = 3;
                current.y = 0;

                // 생성 위치에 블록이 이미 있으면 게임오버
                if(!canMove(&current, 0, 0))
                {
                    drawBoard();
                    gotoxy(5, 10);
                    printf("GAME OVER");
                    return 0;
                }

                drawPiece(&current); // 새 블록 출력
            }

            drawBoard(); // 보드 갱신
        }

        // ---------------------------
        // 3. (별도 처리 없음 - 루프 반복)
        // ---------------------------
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