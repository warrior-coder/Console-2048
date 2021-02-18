#include <iostream>
#include <iomanip>   // setw()
#include <ctime>     // random values for srand
#include <windows.h> // textcolor, sleep
#include <conio.h>   // keys event
#include <stdio.h>   // files io, system()

using namespace std;

enum ConsoleColor {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    LightMagenta = 13,
    Yellow = 14,
    White = 15
};
FILE* fl;
const int boardSize = 4;
int **board, SCORE, BEST, KeyEvent;
bool GAME, WIN;

void DrawFrames();
int KeyDown();
int Logic();
void BoardRotate();
int Random(int, int);
void TextColor(int, int);

int main()
{
    srand(time(NULL));
    system("mode con cols=36 lines=14");
    system("title 2048");


    board = new int* [boardSize];
    for (int i = 0; i < boardSize; i++)
        board[i] = new int[boardSize];

    while(true)
    {
        // SETUP
        SCORE = 0;
        
        fopen_s(&fl, "record.dat", "r");
        if (fl)
        {
            fscanf_s(fl, "%d", &BEST);
            fclose(fl);
        }
        else
            BEST = 0;
        
        KeyEvent = -1;
        GAME = true;
        WIN = false;
        for (int i = 0; i < boardSize; i++)
            for (int j = 0; j < boardSize; j++)
                board[i][j] = 0;

        board[Random(0, boardSize-1)][Random(0, boardSize-1)] = Random(1,2) * 2;
        
        DrawFrames();
        
        // GAME CYCLE
        while (GAME)
        {
            if (KeyDown() > -1)
            {
                if (Logic())
                {
                    TextColor(0, 13);
                    cout << endl << setw(14) << "GAME OVER" << endl;
                    TextColor(0, 15);
                    GAME = false;
                    break;
                }
                DrawFrames();
            }
        }

        if (BEST == SCORE)
        {
            fopen_s(&fl, "record.dat", "wt");
            fprintf(fl, "%d", BEST);
            fclose(fl);
        }

        // RESTART
        cout << endl << "Space - restart" << endl << "Esc - exit ";
        while (!GAME)
            if (_kbhit())
            {
                switch (_getch())
                {
                    case 32: GAME = true; break;
                    case 27: exit(0); break;
                }
            }
    }
}

int Random(int rmin, int rmax)
{
    return rand() % (rmax - rmin + 1) + rmin;
}

void DrawFrames()
{
    system("cls");

    TextColor(Black, LightMagenta);
    cout << setw(12) << "2048" << endl;
    TextColor(Black, White);

    if (WIN)
    {
        TextColor(Black, LightMagenta);
        cout << setw(16) << "YOU ARE WIN!";
        TextColor(Black, LightMagenta);
    }
    cout << endl;

    for (int i = 0; i < boardSize; i++)
    {
        cout << "   ";
        for (int j = 0; j < boardSize; j++)
        {
            switch (board[i][j])
            {
                case 0:
                    if ((i + j) % 2)
                        TextColor(DarkGray, DarkGray);
                    else
                        TextColor(LightGray, LightGray);
                    break;
                case 2: TextColor(Yellow, Black); break;
                case 4: TextColor(Brown, Black); break;
                case 8: TextColor(LightRed, Black); break;
                case 16: TextColor(Red, Black);  break;
                case 32: TextColor(LightBlue, Black); break;
                case 64: TextColor(Blue, Black); break;
                case 128: TextColor(LightGreen, Black); break;
                case 256: TextColor(Green, Black); break;
                case 512: TextColor(LightCyan, Black); break;
                case 1024: TextColor(Cyan, Black); break;
                case 2048: TextColor(LightMagenta, Black); break;
                default: TextColor(Magenta, Black); break;
            }
            cout << setw(4) << board[i][j];
        }
        
        TextColor(Black, White);

        if (i == 1) cout << "    SCORE: " << SCORE;
        else if (i == 2) cout << "    BEST: " << BEST;
        TextColor(Black, Black);
        cout << "X" << endl;
        TextColor(Black, White);

    }
}

int KeyDown()
{
    KeyEvent = -1;
    if (_kbhit())
        if (_getch() == 224)
            switch (_getch())
            {
                case 75: KeyEvent = 0; break;
                case 80: KeyEvent = 1; break;
                case 77: KeyEvent = 2; break;
                case 72: KeyEvent = 3; break;
            }
    return KeyEvent;
}

int Logic()
{
    for (int i = 0; i < KeyEvent; i++) BoardRotate();
    
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = boardSize-2; j > -1; j--)
        {
            if (board[i][j] == 0 && board[i][j + 1] > 0)
            {
                for (int l = j; l < boardSize; l++)
                    board[i][l] = board[i][l+1];
                board[i][boardSize-1] = 0;
            }
        }

        for (int j = 0; j < boardSize-1; j++)
        {
            if (board[i][j] == board[i][j+1])
            {
                for (int l = j; l < boardSize-1; l++)
                    board[i][l] = board[i][l+1];
                board[i][boardSize-1] = 0;

                SCORE += (board[i][j] *= 2);

                if (board[i][j] >= 2048) WIN = true;
                
            }
        }
    }
    if (SCORE > BEST) BEST = SCORE;
    for (int i = KeyEvent; i < 4; i++) BoardRotate();

    int k = 0;
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++)
            if (board[i][j]) k++;
    if (k == boardSize*boardSize) return 1;

    int ri, rj;
    while (true)
    {
        ri = Random(0, boardSize-1);
        rj = Random(0, boardSize-1);
        if (board[ri][rj] == 0)
        {
            board[ri][rj] = Random(1,2) * 2;
            return 0;
        }
    }
}

void BoardRotate()
{
    int tmp;
    for (int i = 0; i < boardSize-1; i++)
		for (int j = i + 1; j < boardSize; j++)
		{
			tmp = board[i][j];
            board[i][j] = board[j][i];
            board[j][i] = tmp;
		}
	for (int i = 0; i < boardSize; i++)
		for (int j = 0; j < boardSize/2; j++)
		{
			tmp = board[i][j];
            board[i][j] = board[i][boardSize-j-1];
            board[i][boardSize-j-1] = tmp;
		}
}

void TextColor(int bg, int txt)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bg << 4) | txt);
}