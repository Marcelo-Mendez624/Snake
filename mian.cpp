#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include <windows.h>
#include <cstdlib>


bool gameOver;
const int Width = 20;
const int Height = 10;
int x, y, FruitX, FruitY, Score, HighScore;
int TailX[100], TailY[100];
int nTail;

enum eDirection 
{ 
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN 
};
eDirection Dir;


void Draw() {
    system("cls");
    if (Score > HighScore)
        HighScore = Score;
       
    std::cout << "High score: " << HighScore << "\n";
        

    for (int i = 0; i < Width + 2; i++)
        std::cout << "#";
    std::cout << std::endl;

    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            if (j == 0)
                std::cout << "#";
            if (i == y && j == x)
                std::cout << "X";
            else if (i == FruitY && j == FruitX)
                std::cout << "O";
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (TailX[k] == j && TailY[k] == i) {
                        std::cout << "x";
                        print = true;
                    }
                }
                if (!print)
                    std::cout << " ";
            }
            if (j == Width - 1)
                std::cout << "#";
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < Width + 2; i++)
        std::cout << "#";
    std::cout << std::endl;
    std::cout << "Score:" << Score << std::endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            if(Dir != RIGHT)
                Dir = LEFT;
            break;
        case 'd':
            if (Dir != LEFT)
                Dir = RIGHT;
            break;
        case 'w':
            if (Dir != DOWN)
                Dir = UP;
            break;
        case 's':
            if (Dir != UP)
                Dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
        

    }
}

void Logic() {
    int prevX = TailX[0];
    int prevY = TailY[0];
    int prev2X, prev2Y;
    TailX[0] = x;
    TailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = TailX[i];
        prev2Y = TailY[i];
        TailX[i] = prevX;
        TailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (Dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }


    if(x >= Width) // check if snake hit right wall
        x = 0;
    else if(x < 0) // check if snake hit left wall
        x = Width - 1;

    if(y >= Height) // check if snake hit top wall
        y = 0; 
    else if(y < 0) // check if snake hit down wall
        y = Height - 1;

    for (int i = 0; i < nTail; i++)
        if (TailX[i] == x && TailY[i] == y)
            gameOver = true;

    if (x == FruitX && y == FruitY) {
        Score += 10;
        FruitX = rand() % Width;
        FruitY = rand() % Height;
        nTail++;
    }
}

void WriteMaxScore()
{
    std::ofstream MaxScore;
    MaxScore.open("MaxScore.txt");

    if (!MaxScore.is_open())
    {
        std::cout << "Faild to write MaxScore";
        return;
    }

    std::string High = std::to_string(HighScore);

    MaxScore << High;
    MaxScore.close();
}

int ReadMaxScore()
{
    std::ifstream MaxScore;
    MaxScore.open("MaxScore.txt");

    if (!MaxScore.is_open())
    {
        std::cout << "Faild to load MaxScore";
        return 0;
    }
        
    std::string FileMaxScore;

    MaxScore >> FileMaxScore;
    int Max = 0;

    if (FileMaxScore.empty()) return 0;
    
    Max = std::stoi(FileMaxScore);   

    MaxScore.close();


    return Max;
}



void Setup() {
    gameOver = false;
    Dir = RIGHT;
    x = Width / 2;
    y = Height / 2;
    FruitX = rand() % Width;
    FruitY = rand() % Height;

    HighScore = ReadMaxScore();

    std::ofstream ScoreFile;
    ScoreFile.open("MaxScore.txt");

    Score = 0;
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(1); 
    }
    std::cout << "Game Over";

    WriteMaxScore();
    return 0;
}