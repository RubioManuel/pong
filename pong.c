/*
 * PONG GAME USING RAYLIB WITH C
 * author: rxxbyy
 * date: june 8, 2022
 */
#include "raylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* Symbolic constants definition */
#define MAX_POINTS 5

/* Types and Structures definition */
typedef struct {
    const int width;
    const int height;
    char *title;
} Window;

typedef struct {
    int x;
    int y;
    int width;
    int height;
    int velocity;
    int lifes;
} Player;

typedef struct {
    int x;
    int y;
    int radius;
    int velocity;
    int direction;
} Ball;

/* Function prototypes */
void initPlayerOne(Player *player);
void initPlayerTwo(Player *player);
void initBall(Ball *ball);
void movePlayer(Player *player, int upKey, int downKey);
void ballDirection(Ball *ball);
void ballScreenCollision(Ball *ball, Player *playerOne, Player *playerTwo);
void playerBallCollision(Player *player, Ball *ball);
void showWinner(Player *p1, Player *p2);

/* Main functionalities of game */ 
int main(void)
{
    srand(time(NULL));

    Window mainWindow = {800, 600, "PONG"};

    InitWindow(mainWindow.width, mainWindow.height, mainWindow.title);
    SetTargetFPS(60);

    Player playerOne;
    Player playerTwo;

    Ball ball;

    initPlayerOne(&playerOne);
    initPlayerTwo(&playerTwo);
    initBall(&ball);

    /* Lifes counter to printf*/
    char p1lifes[1];
    char p2lifes[1];

    HideCursor();

    /* Title screen */ 
    BeginDrawing();
        ClearBackground(BLACK);

        DrawText("PONG", mainWindow.width / 2 - 100, mainWindow.height / 2 - 50, 64, WHITE);
    EndDrawing();

    WaitTime(3);

    /* Menu Screen */
    while (true) {
        BeginDrawing();
            ClearBackground(BLACK);

            DrawText("Start [SPACE]", mainWindow.width / 2 - 100, mainWindow.height / 2 - 50, 32, WHITE);
            DrawText("Exit [ESC]", mainWindow.width / 2 - 100, mainWindow.height / 2, 32, WHITE);

            ClearBackground(BLACK);
        EndDrawing();

        if (IsKeyPressed(KEY_SPACE)) break;
        if (IsKeyPressed(KEY_ESCAPE)) exit(0);
    }


    while(!WindowShouldClose() && (playerOne.lifes && playerTwo.lifes)) {
        /* Main drawing proccess */
        BeginDrawing();
            ClearBackground(BLACK);

            DrawRectangle(playerOne.x, playerOne.y, playerOne.width, playerOne.height, WHITE);
            DrawRectangle(playerTwo.x, playerTwo.y, playerTwo.width, playerTwo.height, WHITE);
            DrawCircle(ball.x, ball.y, ball.radius, RED);

            sprintf(p1lifes, "%d", playerOne.lifes);
            DrawText(p1lifes, 5, 10, 24, GREEN);

            sprintf(p2lifes, "%d", playerTwo.lifes);
            DrawText(p2lifes, 780, 10, 24, GREEN);
        EndDrawing();

        /* Detect keyboard logic and movement of players */
        movePlayer(&playerOne, KEY_W, KEY_S);
        movePlayer(&playerTwo, KEY_UP, KEY_DOWN);
        ballDirection(&ball);
        ballScreenCollision(&ball, &playerOne, &playerTwo);
        playerBallCollision(&playerOne, &ball);
        playerBallCollision(&playerTwo, &ball);
    }

    showWinner(&playerOne, &playerTwo);

    return 0;
}

/*
    INITIALIZERS
*/ 
void initPlayerOne(Player *player)
{
    player->x = 50;
    player->y = (GetScreenHeight() / 2) - 50;
    player->width = 20;
    player->height = 100;
    player->velocity = 1;
    player->lifes = MAX_POINTS;
}

void initPlayerTwo(Player *player)
{
    player->x = 700;
    player->y = (GetScreenHeight() / 2) - 50;
    player->width = 20;
    player->height = 100;
    player->velocity = 1;
    player->lifes = MAX_POINTS;
}

void initBall(Ball *ball)
{
    ball->x = GetScreenWidth() / 2;
    ball->y = GetScreenHeight() / 2;
    ball->radius = 10;
    ball->velocity = 1;
    ball->direction = 1 + rand() % 4;
}

/*
    LOGIC AND MOVEMENT
*/
void movePlayer(Player *player, int upKey, int downKey)
{
    if (IsKeyPressed(upKey) && player->y > 0) player->y -= player->velocity*50;

    if (IsKeyPressed(downKey) && player->y + player->height < GetScreenHeight())
        player->y += player->velocity*50;
}

void ballDirection(Ball *ball)
{
    int velMagnitude = 5;

    switch (ball->direction) {
        case 1:
            ball->x -= ball->velocity * velMagnitude;
            ball->y -= ball->velocity * velMagnitude;

            break;
        
        case 2:
            ball->x += ball->velocity * velMagnitude;
            ball->y -= ball->velocity * velMagnitude;

            break;

        case 3:
            ball->x += ball->velocity * velMagnitude;
            ball->y += ball->velocity * velMagnitude;
            break;

        case 4:
            ball->x -= ball->velocity * velMagnitude;
            ball->y += ball->velocity * velMagnitude;
            break;
    }
}

void ballScreenCollision(Ball *ball, Player *playerOne, Player *playerTwo)
{
    if (ball->x - ball->radius <= 0) {
        playerOne->lifes--;
        initBall(ball);
        WaitTime(1.0);

    } else if (ball->x + ball->radius >= GetScreenWidth()) {
        playerTwo->lifes--;
        initBall(ball);
        WaitTime(1.0);
        
    } else if (ball->y - ball->radius <= 0) {
        if (ball->direction == 1) ball->direction = 4;
        else ball->direction = 3;
    } else if (ball->y + ball->radius >= GetScreenHeight()) {
        if (ball->direction == 3) ball->direction = 2;
        else ball->direction = 1;
    }
}

void playerBallCollision(Player *player, Ball *ball) 
{
    if ((ball->y - ball->radius >= player->y && ball->y + ball->radius <= player->y + player->height)
        && ball->x - ball->radius == player->x + player->width) {
        if (ball->direction == 4) ball->direction = 3;
        else ball->direction = 2;
    } else if ((ball->y - ball->radius >= player->y && ball->y + ball->radius <= player->y + player->height)
        && ball->x - ball->radius == player->x - player->width) {
        if (ball->direction == 3) ball->direction = 4;
        else ball->direction = 1;
    } else if ((ball->x - ball->radius >= player->x && ball->x - ball->radius <= player->width)
        && ball->y + ball->radius == player->y) {
        if (ball->direction == 4) ball->direction = 1;
        else ball->direction = 2;
    }
    else if ((ball->x - ball->radius >= player->x && ball->x - ball->radius <= player->width)
        && ball->y - ball->radius == player->height) {
        if (ball->direction == 1) ball->direction = 4;
        else ball->direction = 3;
    }
}

void showWinner(Player *p1, Player *p2)
{
    if (!p1->lifes) {
        BeginDrawing();
            ClearBackground(BLACK);

            DrawText("PLAYER TWO WINS", 75, GetScreenHeight() / 2 - 50, 64, WHITE);
        EndDrawing();

        WaitTime(3.0);
    } else if (!p2->lifes) {
        BeginDrawing();
            ClearBackground(BLACK);

            DrawText("PLAYER ONE WINS", 75, GetScreenHeight() / 2 - 50, 64, WHITE);
        EndDrawing();

        WaitTime(3.0);
    }
}