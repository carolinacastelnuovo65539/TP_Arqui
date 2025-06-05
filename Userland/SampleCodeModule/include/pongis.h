#ifndef PONGIS_H
#define PONGIS_H

void start_game_pongis(char players);

typedef struct {
    double x, y;
    int scoreNum;
} Score;

typedef struct {
    double x, y;
    double radius;
    Score score;
    Color color;
} Paddle;

typedef struct {
    double x, y;
    double radius;
    Color color;
} Ball;

typedef struct {
    double x, y;
    double radius;
    double outRadius;
    Color outColor;
    Color color;
} Hole;

void start_game_pongis(char players);

void handleBallCollision(Paddle *player, int player_num);

int checkCollision(double x1, double y1, double r1, double x2, double y2, double r2);

void checkIfBorderPlayer(Paddle * p);

void checkIfBorderBall(Ball * b, double *dx, double *dy);

int checkScoreCollision(double x1, double y1, double r1, double x2, double y2);

void checkCollisionBallPlayer(double oldBallX, double oldBallY, double ball_radius, Paddle * p ,double *dx, double *dy);

void resetLevel();

void drawScore(Paddle * p);

void moveCursor(uint64_t x, uint64_t y);

void updateGame1(uint8_t * pressed_keys);

void updateGame2(uint8_t * pressed_keys);

void drawGame();

void drawPlayer(Paddle p);

int ballInHole();

#endif