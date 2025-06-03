#include <userlib.h>
#include <stdint.h>
#include <pongis.h>

#define SPEED 2
#define FIELD_COLOR LIGHT_GREEN
#define PADDLE_COLOR RED
#define BALL_COLOR WHITE
#define HOLE_COLOR BLACK

static int screenW, screenH;
static int game_running = 1;
static int num_players;
static int last_player_hit = 0; // 1 para jugador 1, 2 para jugador 2

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

    clear();

    // Inicializar jugadores
    player1 = (Paddle){20, screenH / 2, 20, PADDLE_COLOR};
    player2 = (Paddle){screenW - 20, screenH / 2, 20, BLUE};

    // Inicializar pelota
    ball = (Ball){screenW / 2, screenH / 2, 10, BALL_COLOR};

    // Inicializar hoyo
    hole = (Hole){screenW / 2, screenH / 4, 15, HOLE_COLOR};

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
            printColorCentered("¡La bola entró en el hoyo!\n", YELLOW, FIELD_COLOR, get_char_width(), get_char_height());
            beep(1000, 30);
            game_running = 0;
            //sleep(2);  // Pequeña pausa para mostrar el mensaje
            clear();
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
        // Borrar posición anterior
        drawCircle(oldX, oldY, player1.radius, FIELD_COLOR);
        // Dibujar nueva posición
        drawCircle(player1.x, player1.y, player1.radius, player1.color);
        
        // Redibujar el hoyo si el jugador está cerca
        if (checkCollision(player1.x, player1.y, player1.radius, hole.x, hole.y, hole.radius)) {
            drawCircle(hole.x, hole.y, hole.radius, hole.color);
        }
        
        // Verificar colisión con la pelota y moverla
        if (checkCollision(player1.x, player1.y, player1.radius, ball.x, ball.y, ball.radius)) {
            // Calcula dirección del empuje
            int dx = ball.x - player1.x;
            int dy = ball.y - player1.y;
            
            // Mueve la pelota
            ball.x += dx / 5;
            ball.y += dy / 5;
            
            // Redibujar la pelota en su nueva posición
            drawCircle(oldBallX, oldBallY, ball.radius, FIELD_COLOR);
            drawCircle(ball.x, ball.y, ball.radius, ball.color);
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
        drawCircle(oldX, oldY, player2.radius, FIELD_COLOR);
        drawCircle(player2.x, player2.y, player2.radius, player2.color);
        
        // Redibujar el hoyo si el jugador está cerca
        if (checkCollision(player2.x, player2.y, player2.radius, hole.x, hole.y, hole.radius)) {
            drawCircle(hole.x, hole.y, hole.radius, hole.color);
        }
        
        // Verificar colisión con la pelota y moverla
        if (checkCollision(player2.x, player2.y, player2.radius, ball.x, ball.y, ball.radius)) {
            int dx = ball.x - player2.x;
            int dy = ball.y - player2.y;
            
            ball.x += dx / 5;
            ball.y += dy / 5;
            
            drawCircle(oldBallX, oldBallY, ball.radius, FIELD_COLOR);
            drawCircle(ball.x, ball.y, ball.radius, ball.color);
        }
    }
}


int ballInHole() {
    int dx = ball.x - hole.x;
    int dy = ball.y - hole.y;
    return dx * dx + dy * dy <= hole.radius * hole.radius;
}


int checkCollision(int x1, int y1, int r1, int x2, int y2, int r2) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    return dx * dx + dy * dy <= (r1 + r2) * (r1 + r2);
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