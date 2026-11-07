#include "raylib.h"
#include <stdlib.h>
#include <math.h>
#define WIDTH  1280
#define HEIGHT 960
#define RADIUS 50.0
#define LENGTH 100.0

typedef enum GAAMESTATE{Login, InGame, GameOver};

struct Ball{
    double x;
    double y;
    double vel ;
};

Rectangle paddleLeft = {50, 400, 30, 120}; //20:WIDTH 120:HEIGHT
Rectangle paddleRight = {1210, 400, 30, 120};

double Random(void) {
    double r;
    if (rand() % 2 == 0)
        r = 0.4 + (double)rand() / RAND_MAX * 0.4;
    else
        r = -0.8 + (double)rand() / RAND_MAX * 0.4;
    return r;
}


int main() {
    InitWindow(WIDTH, HEIGHT, "Ping Pong");
    SetTargetFPS(120);
    int score = 0, bestScore = 0;
    int Unit = 20;

    struct Ball b = {600, 480, 900};
    double k = Random();
    double VelBX = b.vel * k / sqrt(1 + k*k);
    double VelBY = b.vel / sqrt(1 + k*k);
    bool Gamestate = true;

    while (!WindowShouldClose()) {
        if (Gamestate) {
            float dt = GetFrameTime();
            b.x += VelBX * dt;
            b.y += VelBY * dt;

            if (IsKeyDown(KEY_W)    && paddleLeft.y  >= Unit) paddleLeft.y  -= Unit;
            if (IsKeyDown(KEY_S)    && paddleLeft.y  <= HEIGHT - paddleLeft.height - Unit) paddleLeft.y  += Unit;
            if (IsKeyDown(KEY_UP)   && paddleRight.y >= Unit) paddleRight.y -= Unit;
            if (IsKeyDown(KEY_DOWN) && paddleRight.y <= HEIGHT - paddleRight.height - Unit) paddleRight.y += Unit;

            // Boundary Check
            if (b.y <= RADIUS || b.y >= HEIGHT - RADIUS)
                VelBY = -VelBY;

            // Collision Check
            if (VelBX < 0 &&
                b.x - RADIUS <= paddleLeft.x + paddleLeft.width &&
                b.x - RADIUS >= paddleLeft.x &&
                b.y >= paddleLeft.y - paddleLeft.height/3.0 &&
                b.y <= paddleLeft.y + paddleLeft.height*4/3.0) {
                VelBX = -VelBX * 1.05;
                score += 10;
            }
            if (VelBX > 0 &&
                b.x + RADIUS >= paddleRight.x &&
                b.x + RADIUS <= paddleRight.x + paddleRight.width &&
                b.y >= paddleRight.y - paddleRight.height/3.0 &&
                b.y <= paddleRight.y + paddleRight.height*4/3.0) {
                VelBX = -VelBX * 1.05;
                score += 10;
            }

            // Over
            if (b.x < 0 || b.x > WIDTH) {
                Gamestate = false;
                if (score > bestScore) bestScore = score;
            }
        } else if (IsKeyPressed(KEY_ENTER)) {
            break;
        }

        BeginDrawing();
            ClearBackground(DARKBLUE);
            DrawCircle(b.x, b.y, RADIUS, YELLOW);
            DrawRectangleRec(paddleLeft, SKYBLUE);
            DrawRectangleRec(paddleRight, SKYBLUE);
            DrawText(TextFormat("%d", score),
                     WIDTH/2 - MeasureText(TextFormat("%d", score), 80)/2, 30, 80, WHITE);

            if (!Gamestate) {
                DrawText("Game Over", WIDTH/2 - MeasureText("Game Over", 120)/2, HEIGHT/2 - 60, 120, RED);
                    DrawText("Press R to restart", WIDTH/2 - MeasureText("Press R to restart", 40)/2, HEIGHT/2 + 80, 40, DARKGRAY);
                    DrawText("Press ENTER to exit", WIDTH/2 - MeasureText("Press ENTER to exit", 40)/2, HEIGHT/2 + 130, 40, DARKGRAY);
                    DrawText(TextFormat("Best: %d", bestScore),
                             WIDTH/2 - MeasureText(TextFormat("Best: %d", bestScore), 40)/2, 115, 40, LIGHTGRAY);

            }

            if (IsKeyPressed(KEY_R)){
                b.x = 600;
                b.y = 480;
                Gamestate = true;
                score = 0;
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
