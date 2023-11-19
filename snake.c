#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ESC 27
#define BOX_X_START 10
#define BOX_X_END 70
#define BOX_Y_START 10
#define BOX_Y_END 30
#define DOWN_ARROW "v"
#define UP_ARROW "^"
#define LEFT_ARROW "<"
#define RIGHT_ARROW ">"

int length = 7;
int direction = RIGHT;
int max_length = 60;
int score = 0;
int eaten = 0;

struct coordinate
{
    int x;
    int y;
};
// why we have to typedef sturcture in global to use... and not in int main?
typedef struct coordinate coordinate;

void initializeValues();
void makeSnake();
void moveSnake();
void Print();
void load();
void gotoxy(int x, int y);
void Boarder();
void GotoXY(int x, int y);
void delay();
void runGame();
void printHead();
int checkStrike();
void makeFood();
int is_Present(struct coordinate apnaFood);
coordinate getFoodCoordinates();
void end();
void foodEaten();
void print_score();

coordinate body[60];
coordinate head;
coordinate tail = {30, 20};
coordinate tailold = {29, 20};
coordinate food = {0, 0};
coordinate score_coordinates = {40, 5};

int main()
{
    // Print();
    // load();
    initializeValues();
    Boarder();
    makeFood();
    runGame();
    return 0;
}

// game engine
void runGame()
{
    while (!kbhit())
    {
        delay();
        moveSnake();
        makeSnake();
    }
    char a = getch();
    if (a == UP || a == DOWN || a == LEFT || a == RIGHT)
    {
        direction = a;
        runGame();
    }
    else if (a == ESC)
        exit(0);
    else
        runGame();
}

// move the snake according to body and direction
void moveSnake()
{
    // checkStrike - return 1 for game end
    //               - 2 for food eaten
    //               - 0 for nothing;
    if (checkStrike() == 1)
        end();
    else if (checkStrike() == 2)
        foodEaten();
    if (direction == UP)
        head.y--;
    else if (direction == DOWN)
        head.y++;
    else if (direction == LEFT)
        head.x--;
    else if (direction == RIGHT)
        head.x++;
    tailold = body[0];
    for (int i = 0; i < length - 1; i++)
    {
        body[i] = body[i + 1];
    }
    body[length - 1] = head;
}

// set head , tail and body of the snake
void initializeValues()
{
    body[0] = tail;
    for (int i = 1; i < length; i++)
    {
        body[i].x = tail.x + i;
        body[i].y = tail.y;
    }
    head = body[length - 1];
}

// draw the snake on the board
void makeSnake()
{
    for (int i = 0; i < length; i++)
    {
        struct coordinate currentBodyPart = body[i];
        GotoXY(currentBodyPart.x, currentBodyPart.y);
        printf("*");
    }
    printHead();
    GotoXY(tailold.x, tailold.y);
    printf(" ");
    gotoxy(0, 0);
}

// draw the head
void printHead()
{
    GotoXY(head.x, head.y);
    if (direction == RIGHT)
        printf(">");
    if (direction == LEFT)
        printf("<");
    if (direction == UP)
        printf("^");
    if (direction == DOWN)
        printf("v");
}

// welcome msg
void Print()
{
    GotoXY(10, 12);
    printf("\tWelcome to the mini Snake game.(press any key to continue)\n");
    getch();
    system("cls");
    printf("\tGame instructions:\n");
    printf("\n-> Use arrow keys to move the snake.\n\n-> You will be provided foods at the several coordinates of the screen which you have to eat. Everytime you eat a food the length of the snake will be increased by 1 element and thus the score.\n\n-> Here you are provided with three lives. Your life will decrease as you hit the wall or snake's body.\n\n-> YOu can pause the game in its middle by pressing any key. To continue the paused game press any other key once again\n\n-> If you want to exit press esc. \n");
    printf("\n\nPress any key to play game...");
    if (getch() == ESC)
        exit(0);
    system("cls");
}

// set cursor to x and y
void gotoxy(int x, int y)
{

    COORD c;
    c.X = x;
    c.Y = y;

    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// print the score
void print_score()
{
    GotoXY(score_coordinates.x, score_coordinates.y);
    printf("%d", score);
}

// clear the screen and make borders
void Boarder()
{
    system("cls");
    GotoXY(30, 5);
    printf("SCORE : ");
    print_score();
    int i;
    for (i = BOX_X_START; i <= BOX_X_END; i++)
    {
        GotoXY(i, 10);
        printf("-");
        GotoXY(i, 30);
        printf("!");
    }
    for (i = BOX_Y_START; i <= BOX_Y_END; i++)
    {
        GotoXY(10, i);
        printf("!");
        GotoXY(70, i);
        printf("!");
    }
}

// loading animation
void load()
{

    int row, col, r, c, q;
    gotoxy(36, 14);
    printf("loading...");
    gotoxy(30, 15);
    for (r = 1; r <= 20; r++)
    {
        for (q = 0; q <= 100000000; q++)
            ; // to display the character slowly

        printf("%c", 177); // ascii 117 is ▒
    }
    getch();
}

// gotoxy + fast output
void GotoXY(int x, int y)
{
    HANDLE a;
    COORD b;
    fflush(stdout);
    b.X = x;
    b.Y = y;
    a = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(a, b);
}

void delay()
{
    Sleep(70);
    // long double i;
    // for(i=0; i<=(10000000); i++);
}

int checkStrike()
{
    // return 1 for game end
    // 2 for food eaten
    // 0 for nothing
    if (head.x <= BOX_X_START || head.x >= BOX_X_END || head.y >= BOX_Y_END || head.y <= BOX_Y_START)
        return 1;
    for (int i = 0; i < length - 1; i++)
        if (body[i].x == head.x && body[i].y == head.y)
            return 1;
    if (head.x == food.x && head.y == food.y)
        return 2;
    return 0;
}

void makeFood()
{
    food = getFoodCoordinates();
    GotoXY(food.x, food.y);
    printf("F");
}

struct coordinate getFoodCoordinates()
{
    struct coordinate myFood;
    do
    {
        srand(time(NULL));
        myFood.x = rand() % (BOX_X_END - BOX_X_START - 1) + BOX_X_START + 1;
        myFood.y = rand() % (BOX_Y_END - BOX_Y_START - 1) + BOX_Y_START + 1;
    } while (is_Present(myFood));
    return myFood;
}

int is_Present(struct coordinate apnaFood)
{
    for (int i = 0; i < length; i++)
    {
        if (body[i].x == apnaFood.x && body[i].y == apnaFood.y)
            return 1;
    }
    return 0;
}

// end the game
void end()
{
    GotoXY(BOX_X_START + 10, BOX_Y_END + 3);
    printf("U died.... Your Score was %d", score);
    GotoXY(BOX_X_START + 10, BOX_Y_END + 6);
    exit(0);
};

// when food is eaten
void foodEaten()
{
    score++;
    print_score();
    makeFood();
    body[length].x = head.x;
    body[length].y = head.y;
    length++;
};
