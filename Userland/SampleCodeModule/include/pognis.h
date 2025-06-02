#ifndef POGNIS_H
#define POGNIS_H

#include <userlib.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define PLAYER_RADIUS 8
#define BALL_RADIUS 4
#define PLAYER_SPEED 3
#define BALL_SPEED 2

#define SQUARESIZE 32

// Estructuras
typedef struct Player{
    int x, y;
    int radius;
    int score;
    int playing;
    Color color
} Player;

typedef struct Player * TPlayer;

typedef struct {
    int x, y;
    int dx, dy;  // Velocidad en X e Y
    int size;
} Ball;

// Variables globales
extern TPlayer player1, player2;
extern Ball ball;
extern int game_running;
extern int num_players;


typedef struct Ball * TBall;

// Funciones
void init_game(void);
// void update_game(void);
void draw_game(void);
void check_collisions(void);
void reset_ball(void);
void init_player(TPlayer p);
void init_players(TPlayer player1, TPlayer player2);
void move_player(TPlayer p, char key);
void update1(void);
void update2(void);
// void end_game1(void);
void print_score(TPlayer p);
void move_cursor(uint64_t x, uint64_t y);
void start_game(char players);

#endif