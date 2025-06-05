#include <userlib.h>
#include <stdint.h>
#include <pongis.h>
#include <rand.h>

#define SPEED 2
#define FIELD_COLOR LIGHT_GREEN
#define PADDLE_COLOR RED
#define BALL_COLOR WHITE
#define HOLE_COLOR BLACK
#define BALL_SPEED 5
#define WALL_BOUNCE -1

static int screenW, screenH;
static int game_running = 1;
static int num_players;
static int last_player_hit = 0; // 1 para jugador 1, 2 para jugador 2
static double ball_dx = 0;
static double ball_dy = 0;
static int score_p1 = 0;
static int score_p2 = 0;

static Paddle player1;
static Paddle player2;
static Ball ball;
static Hole hole;

void drawGame();
void updateGame1(char input);
void updateGame2(char input1, char input2);
int ballInHole();

void start_game_pongis(char players) {
    game_running = 1;
    num_players = players - '0';
    screenW = set_width();
    screenH = set_height();

    srand(screenW * screenH * get_seconds() * get_minutes() * get_hours());

    clear();

    // Random positions for all objects, keeping margin from edges
    int margin = 50;  // Margin from screen edges

    // Initialize players with random positions
    player1.radius = 20;
    player1.color = PADDLE_COLOR;
    player1.x = rand_between(screenW - 2*margin) + margin;
    player1.y = rand_between(screenH - 2*margin) + margin;

    if (num_players == 2) {
        player2.radius = 20;
        player2.color = BLUE;
        // Keep trying until we get a position that doesn't overlap with player1
        do {
            player2.x = rand_between(screenW - 2*margin) + margin;
            player2.y = rand_between(screenH - 2*margin) + margin;
        } while (checkCollision(player1.x, player1.y, player1.radius, 
                              player2.x, player2.y, player2.radius));
    }

    // Initialize ball with random position
    ball.radius = 10;
    ball.color = BALL_COLOR;
    do {
        ball.x = rand_between(screenW - 2*margin) + margin;
        ball.y = rand_between(screenH - 2*margin) + margin;
    } while (checkCollision(ball.x, ball.y, ball.radius, player1.x, player1.y, player1.radius) ||
             (num_players == 2 && checkCollision(ball.x, ball.y, ball.radius, 
                                               player2.x, player2.y, player2.radius)));

    // Initialize hole with random position
    hole.radius = 15;
    hole.color = HOLE_COLOR;
    hole.outRadius = 18;
    hole.outColor = WHITE;

    do {
        hole.x = rand_between(screenW - 2*margin) + margin;
        hole.y = rand_between(screenH - 2*margin) + margin;
    } while (checkCollision(hole.x, hole.y, hole.radius, player1.x, player1.y, player1.radius) ||
             checkCollision(hole.x, hole.y, hole.radius, ball.x, ball.y, ball.radius) ||
             (num_players == 2 && checkCollision(hole.x, hole.y, hole.radius, 
                                               player2.x, player2.y, player2.radius)));


    drawGame();

    while (game_running) {
        char input1 = getChar();
        char input2 = 0;

        if (input1 == 27) { // ESC para salir
            game_running = 0;
            clear();
            break;
        }

        if (num_players == 2) {
            input2 = getChar();
        }

        if (num_players == 1) {
            updateGame1(input1);
        } else {
            updateGame2(input1, input2);
        }

        // drawGame();

        if (ballInHole()) {
            height_print_centered = 0;
            increase();
            printColorCentered("The ball entered the hole!\n", YELLOW, FIELD_COLOR, get_char_width(), get_char_height(), 1);
            reduce();
            beep(1000, 30);
            sleep(1);
            break;
        }
    }
    clear();
    game_running = 0;
}

void drawGame() {

    drawRectangle(0, 0, screenW, screenH, FIELD_COLOR);

    drawCircle(player1.x, player1.y, player1.radius, player1.color);

    if (num_players == 2) {
        drawCircle(player2.x, player2.y, player2.radius, player2.color);
    }

    drawCircle(ball.x, ball.y, ball.radius, ball.color);
    
    drawCircle(hole.x, hole.y, hole.outRadius, hole.outColor);

    drawCircle(hole.x, hole.y, hole.radius, hole.color);
}

void drawPlayer(Paddle p){
    drawCircle(p.x, p.y, p.radius, p.color);
}

// void updateGame1(char input) {
//     switch (to_lower(input)) {
//         case 'w': player1.y -= SPEED; break;
//         case 's': player1.y += SPEED; break;
//         case 'a': player1.x -= SPEED; break;
//         case 'd': player1.x += SPEED; break;
//         case ' ': // disparo
//             ball.x += (hole.x - ball.x) / 10;
//             ball.y += (hole.y - ball.y) / 10;
//             break;
//     }
//     // sleep(3);
//     drawGame();
//     // drawPlayer(player1);
// }

void updateGame1(char input) {
    // Guardar posiciones anteriores
    int oldX = player1.x;
    int oldY = player1.y;
    int oldBallX = ball.x;
    int oldBallY = ball.y;

    switch (to_lower(input)) {
        case 'w': player1.y -= SPEED; break;
        case 's': player1.y += SPEED; break;
        case 'a': player1.x -= SPEED; break;
        case 'd': player1.x += SPEED; break;
        case ' ': // disparo
            ball.x += (hole.x - ball.x) / 10;
            ball.y += (hole.y - ball.y) / 10;
            break;
    }

    // Solo redibujar si hubo cambios
    if (oldX != player1.x || oldY != player1.y) {
        checkIfBorderPlayer(&player1);
        drawCircle(oldX, oldY, player1.radius, FIELD_COLOR);
        drawCircle(player1.x, player1.y, player1.radius, player1.color);
        
        if (checkCollision(player1.x, player1.y, player1.radius, hole.x, hole.y, hole.radius)) {
            drawCircle(hole.x, hole.y, hole.outRadius, hole.outColor);
            drawCircle(hole.x, hole.y, hole.radius, hole.color);
        }
        
        if (checkCollision(player1.x, player1.y, player1.radius, ball.x, ball.y, ball.radius)) {
            int dx = ball.x - player1.x;
            int dy = ball.y - player1.y;
            
            for(int i = 0; i < BALL_SPEED; i++){
                // Guardamos la última posición antes de mover
                oldBallX = ball.x;
                oldBallY = ball.y;

                // Movemos y dibujamos la pelota
                ball.x += (dx * BALL_SPEED) / 5;
                ball.y += (dy * BALL_SPEED) / 5;
                
                checkIfBorderBall(&ball, &ball_dx, &ball_dy);
                // Borramos la posición anterior
                drawCircle(oldBallX, oldBallY, ball.radius, FIELD_COLOR);
                // Dibujamos la nueva posición
                drawCircle(ball.x, ball.y, ball.radius, ball.color);
                sleep(1); // Pausa para simular el movimiento de la pelota
            }
        }
    }
}

// void updateGame2(char input1, char input2) {
//     updateGame1(input1);

//     switch (to_lower(input2)) {
//         case 'i': player2.y -= SPEED; break;
//         case 'k': player2.y += SPEED; break;
//         case 'j': player2.x -= SPEED; break;
//         case 'l': player2.x += SPEED; break;
//         case ' ': // disparo
//             ball.x += (hole.x - ball.x) / 10;
//             ball.y += (hole.y - ball.y) / 10;
//             break;
//     }
//     sleep(3);
//     drawGame();
//     // drawPlayer(player2);
// }

void updateGame2(char input1, char input2) {
    updateGame1(input1);

    int oldX = player2.x;
    int oldY = player2.y;
    int oldBallX = ball.x;
    int oldBallY = ball.y;

    switch (to_lower(input2)) {
        case 'i': player2.y -= SPEED; break;
        case 'k': player2.y += SPEED; break;
        case 'j': player2.x -= SPEED; break;
        case 'l': player2.x += SPEED; break;
        case ' ': // disparo
            ball.x += (hole.x - ball.x) / 10;
            ball.y += (hole.y - ball.y) / 10;
            break;
    }

    if (oldX != player2.x || oldY != player2.y) {
        checkIfBorderPlayer(&player2);
        drawCircle(oldX, oldY, player2.radius, FIELD_COLOR);
        drawCircle(player2.x, player2.y, player2.radius, player2.color);
        
        if (checkCollision(player2.x, player2.y, player2.radius, hole.x, hole.y, hole.radius)) {
            drawCircle(hole.x, hole.y, hole.radius, hole.color);
        }
        
        if (checkCollision(player2.x, player2.y, player2.radius, ball.x, ball.y, ball.radius)) {
            int dx = ball.x - player2.x;
            int dy = ball.y - player2.y;
            
            // Guardamos la última posición antes de mover
            oldBallX = ball.x;
            oldBallY = ball.y;

            // Movemos y dibujamos la pelota
            ball.x += (dx * BALL_SPEED) / 5;
            ball.y += (dy * BALL_SPEED) / 5;

            checkIfBorderBall(&ball, &ball_dx, &ball_dy);
            
            // Borramos la posición anterior
            drawCircle(oldBallX, oldBallY, ball.radius, FIELD_COLOR);
            // Dibujamos la nueva posición
            drawCircle(ball.x, ball.y, ball.radius, ball.color);
        }
    }
}


int ballInHole() {
    // Si la pelota está lo suficientemente cerca del centro del hoyo
    int dx = ball.x - hole.x;
    int dy = ball.y - hole.y;
    
    // Aumentamos el área de detección multiplicando por 2
    int detection_radius = (hole.radius + ball.radius);
    return dx * dx + dy * dy <= detection_radius * detection_radius;
}

int checkCollision(int x1, int y1, int r1, int x2, int y2, int r2) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    return dx * dx + dy * dy <= (r1 + r2) * (r1 + r2);
}

void checkIfBorderPlayer(Paddle * p){
    if(p->x - p->radius < 0){
        p->x = p->radius;
    }else if( p->x + p->radius > set_width()){
        p->x = set_width() - p->radius;
    }

    if(p->y - p->radius < 0){
        p->y = p->radius;
    }else if(p->y + p->radius > set_height()){
        p->y = set_height() - p->radius;
    }
}

void checkIfBorderBall(Ball *b, int *dx, int *dy){
    // Rebote horizontal
    if(b->x - b->radius <= 0){
        b->x = b->radius + 25;  // Offset pequeño
        if(*dx < 0) 
            *dx = -*dx;  // Solo invertir si va hacia la izquierda
    }else if(b->x + b->radius >= set_width()){
        b->x = set_width() - b->radius - 25;  // Offset pequeño
        if(*dx > 0) 
            *dx = -*dx;  // Solo invertir si va hacia la derecha
    }

    // Rebote vertical
    if(b->y - b->radius <= 0){
        b->y = b->radius + 25;  // Offset pequeño
        if(*dy < 0) 
            *dy = -*dy;  // Solo invertir si va hacia arriba
    }else if(b->y + b->radius >= set_height()){
        b->y = set_height() - b->radius - 25;  // Offset pequeño
        if(*dy > 0) 
            *dy = -*dy;  // Solo invertir si va hacia abajo
    }
}

void handleBallCollision(Paddle *player, int player_num) {
    if (checkCollision(player->x, player->y, player->radius, ball.x, ball.y, ball.radius)) {
        // Calcula el vector de dirección desde el jugador hacia la pelota
        int dx = ball.x - player->x;
        int dy = ball.y - player->y;
        
        // Mueve la pelota en esa dirección
        ball.x += dx / 5;
        ball.y += dy / 5;
        
        // Registra qué jugador golpeó la pelota
        last_player_hit = player_num;
        
        // Efecto de sonido al golpear
        beep(500, 10);
    }
}

// // Agregar esta función nueva
// void keepInBounds(int *x, int *y, int radius) {
//     // Límites horizontales
//     if (*x - radius < 0) *x = radius;
//     if (*x + radius > screenW) *x = screenW - radius;
    
//     // Límites verticales
//     if (*y - radius < 0) *y = radius;
//     if (*y + radius > screenH) *y = screenH - radius;
// }

// // Agregar esta función para el rebote de la pelota
// void handleBallBounce() {
//     int bounced = 0;
    
//     // Rebote horizontal
//     if (ball.x - ball.radius < 0 || ball.x + ball.radius > screenW) {
//         ball_dx *= WALL_BOUNCE;
//         bounced = 1;
//     }
    
//     // Rebote vertical
//     if (ball.y - ball.radius < 0 || ball.y + ball.radius > screenH) {
//         ball_dy *= WALL_BOUNCE;
//         bounced = 1;
//     }
    
//     if (bounced) {
//         beep(400, 10);  // Sonido de rebote
//     }
// }