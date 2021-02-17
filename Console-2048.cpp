#include<iostream>
#include<iomanip>
#include<ctime>     // for srand
#include<windows.h> // cls & sleep
#include<conio.h>   // keys event
#include<stdio.h>   // files io

using namespace std;

FILE* fl;
const int boardSize = 4;
int **board, SCORE, BEST, KE;
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
        
        KE = -1;
        GAME = true;
        WIN = false;
        for (int i = 0; i < boardSize; i++)
            for (int j = 0; j < boardSize; j++)
                board[i][j] = 0;
        board[Random(0, 3)][Random(0, 3)] = Random(1,2) * 2;
        
        DrawFrames();
        
        // GAME CYCLE
        while (GAME)
        {
            KE = KeyDown();
            if (KE > -1)
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
        char restart;
        cout << endl << "R to restart "; cin >> restart;
        if (restart == 'r' || restart == 'R')
            GAME = true;
        else
            break;
    }

    for (int i = 0; i < boardSize; i++)
        delete[] board[i];
    delete[] board;
    board = nullptr;

    return 0;
}

int Random(int rmin, int rmax)
{
    return rand() % (rmax - rmin + 1) + rmin;
}

void DrawFrames()
{
    system("cls");

    TextColor(0, 13);
    cout << setw(12) << "2048" << endl;
    TextColor(0, 15);

    if (WIN)
    {
        TextColor(0, 13);
        cout << setw(16) << "YOU ARE WIN!" << endl;
        TextColor(0, 15);
    }
    else
        cout << endl;

    for (int i = 0; i < boardSize; i++)
    {
        cout << "  ";
        for (int j = 0; j < boardSize; j++)
        {
            switch (board[i][j])
            {
                case 0:
                    if ((i + j) % 2)
                        TextColor(7, 7);
                    else
                        TextColor(8, 8);
                    break;
                case 2:  TextColor(14, 0); break;
                case 4:  TextColor(6, 0); break;
                case 8:  TextColor(12, 0); break;
                case 16: TextColor(4, 0);  break;
                case 32: TextColor(9, 0); break;
                case 64: TextColor(1, 0); break;
                case 128: TextColor(2, 0); break;
                case 256: TextColor(10, 0); break;
                case 512: TextColor(3, 0); break;
                case 1024: TextColor(11, 0); break;
                case 2048: TextColor(13, 0); break;
                default: TextColor(0, 7); break;
            }
            cout << setw(4) << board[i][j];
        }
        
        TextColor(0, 7);

        if (i == 1)
        {
            cout << "    SCORE: " << SCORE;
        }
        if (i == 2)
        {
            cout << "    BEST: " << BEST;
        }
        cout << endl;
    }
    
    
}

int KeyDown()
{
    int KeyEvent = -1;
    if (_kbhit())
        if (_getch() == 224)
            switch (_getch())
            {
                case 72: KeyEvent = 3; break;
                case 75: KeyEvent = 0; break;
                case 77: KeyEvent = 2; break;
                case 80: KeyEvent = 1; break;
            }
    return KeyEvent;
}

int Logic()
{
    for (int i = 0; i < KE; i++)
        BoardRotate();
    
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = boardSize - 2; j > -1; j--)
        {
            if (board[i][j] == 0 && board[i][j + 1] > 0)
            {
                for (int l = j; l < boardSize; l++)
                {
                    board[i][l] = board[i][l+1];
                }
                board[i][boardSize-1] = 0;
            }
        }

        for (int j = 0; j < boardSize-1; j++)
        {
            if (board[i][j] == board[i][j+1])
            {
                for (int l = j; l < boardSize-1; l++)
                {
                    board[i][l] = board[i][l+1];
                }
                board[i][j] *= 2;
                SCORE += board[i][j];
                board[i][boardSize-1] = 0;

                if (board[i][j] == 2048)
                    WIN = true;
                if (SCORE > BEST)
                    BEST = SCORE;
            }
        }
    }

    for (int i = KE; i < 4; i++)
        BoardRotate();

    int k = 0;
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            if (board[i][j]) k++;
        }
        
    }
    if (k == 16)
    {
        return 1;
    }

    int ri, rj;
    while (true)
    {
        ri = Random(0, 3);
        rj = Random(0, 3);
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