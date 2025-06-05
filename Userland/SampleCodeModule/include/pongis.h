#ifndef PONGIS_H
#define PONGIS_H

void start_game_pongis(char players);

typedef struct {
    int x, y;
    int scoreNum;
} Score;

typedef struct {
    int x, y;
    int radius;
    Score score;
    Color color;
} Paddle;

typedef struct {
    int x, y;
    int radius;
    Color color;
} Ball;

typedef struct {
    int x, y;
    int radius;
    int outRadius;
    Color outColor;
    Color color;
} Hole;

void start_game_pongis(char players);

void handleBallCollision(Paddle *player, int player_num);

int checkCollision(int x1, int y1, int r1, int x2, int y2, int r2);

void checkIfBorderPlayer(Paddle * p);

void checkIfBorderBall(Ball * b, double *dx, double *dy);

int checkScoreCollision(int x1, int y1, int r1, int x2, int y2);

void resetLevel();

void drawScore(Paddle * p);

void moveCursor(uint64_t x, uint64_t y);

void updateGame1(uint8_t * pressed_keys);

void updateGame2(uint8_t * pressed_keys);

void drawGame();

void drawPlayer(Paddle p);

int ballInHole();

#endif