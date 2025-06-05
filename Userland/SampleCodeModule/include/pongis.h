#ifndef PONGIS_H
#define PONGIS_H

void start_game_pongis(char players);

typedef struct {
    int x, y;
    int radius;
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

void handleBallCollision(Paddle *player, int player_num);

int checkCollision(int x1, int y1, int r1, int x2, int y2, int r2);

void checkIfBorderPlayer(Paddle * p);

void checkIfBorderBall(Ball * b, int *dx, int *dy);

#endif