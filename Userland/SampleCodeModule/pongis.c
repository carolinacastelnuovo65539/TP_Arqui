#include <userlib.h>
#include <stdint.h>
#include <pongis.h>
#include <rand.h>

#define SPEED 0.001
#define FIELD_COLOR LIGHT_GREEN
#define PADDLE_COLOR RED
#define BALL_COLOR WHITE
#define HOLE_COLOR BLACK
#define BALL_SPEED 5
#define WALL_BOUNCE -1

#define SC_W 17
#define SC_S 31
#define SC_A 30
#define SC_D 32
#define SC_I 23
#define SC_J 36
#define SC_K 37
#define SC_L 38
#define SC_ESC 1
#define SC_SPACE 57

static int screenW, screenH;
static int game_running = 1;
static int num_players;
static int last_player_hit = 0; // 1 para jugador 1, 2 para jugador 2
static double ball_dx = 0;
static double ball_dy = 0;
static int current_level = 1;
static int max_levels = 5;
static int score_width;
static int score_height;
static double oldBallX;
static double oldBallY;

static Paddle player1;
static Paddle player2;
static Ball ball;
static Hole hole;


void start_game_pongis(char players) {
    game_running = 1;
    current_level = 1;
    num_players = players - '0';
    screenW = set_width();
    screenH = set_height();

    srand(screenW * screenH * get_seconds() * get_minutes() * get_hours());

    clear();

    // Random positions for all objects, keeping margin from edges
    int margin = 50;  // Margin from screen edges

    score_width = get_char_width() * 10; // Width for score display
    score_height = get_char_height();

    // Initialize players with random positions
    player1.radius = 20;
    player1.color = PADDLE_COLOR;
    player1.x = rand_between(screenW - 2*margin) + margin;
    player1.y = rand_between(screenH - 2*margin) + margin;
    player1.score.scoreNum = 0;
    player1.score.x = 0;
    player1.score.y = 0;

    if (num_players == 2) {
        player2.radius = 20;
        player2.color = BLUE;
        player2.score.scoreNum = 0;
        player2.score.x = get_char_width() * 10; // Position for player 2 score
        player2.score.y = 0;
        // Keep trying until we get a position that doesn't overlap with player1
        do {
            player2.x = rand_between(screenW - 2*margin) + margin;
            player2.y = rand_between(screenH - 2*margin) + margin;
        } while (checkCollision(player1.x, player1.y, player1.radius, player2.x, player2.y, player2.radius));
    }

    // Initialize ball with random position
    ball.radius = 10;
    ball.color = BALL_COLOR;
    do {
        ball.x = rand_between(screenW - 2*margin) + margin;
        ball.y = rand_between(screenH - 2*margin) + margin;
    } while (checkCollision(ball.x, ball.y, ball.radius, player1.x, player1.y, player1.radius) ||
            (num_players == 2 && checkCollision(ball.x, ball.y, ball.radius, player2.x, player2.y, player2.radius)));

    // Initialize hole with random position
    hole.radius = 35;
    hole.color = HOLE_COLOR;
    hole.outRadius = 42;
    hole.outColor = WHITE;

    do {
        hole.x = rand_between(screenW - 2*margin) + margin;
        hole.y = rand_between(screenH - 2*margin) + margin;
    } while (checkCollision(hole.x, hole.y, hole.radius, player1.x, player1.y, player1.radius) ||
             checkCollision(hole.x, hole.y, hole.radius, ball.x, ball.y, ball.radius) ||
             (num_players == 2 && checkCollision(hole.x, hole.y, hole.radius, 
                                               player2.x, player2.y, player2.radius)));


    drawGame();
    drawScore(&player1);
    if (num_players == 2) {
        drawScore(&player2);
    }
    

    while (game_running) {
        getChar();  // consumo la tecla al leer
        uint8_t pressed_keys[128];
        get_pressed_keys(pressed_keys);

        if (pressed_keys[SC_ESC]) { // ESC para salir
            game_running = 0;
            clear();
            break;
        }

        if (num_players == 1) {
            updateGame1(pressed_keys);
        } else {
            updateGame2(pressed_keys);
        }

        if (ballInHole()) {
            if( last_player_hit == 1) {
                player1.score.scoreNum++;
                drawScore(&player1);
            } else if (last_player_hit == 2) {
                player2.score.scoreNum++;
                drawScore(&player2);
            }
            current_level++;
            if( current_level > max_levels){
                drawCircle(oldBallX, oldBallY, ball.radius, FIELD_COLOR);
                drawCircle(hole.x, hole.y, hole.outRadius, hole.outColor);
                drawCircle(hole.x, hole.y, hole.radius, hole.color);
                drawCircle(hole.x, hole.y, ball.radius, ball.color);
                increase();
                printColorCentered("Congratulations! You completed all levels!\n", BRIGHT_YELLOW, FIELD_COLOR, get_char_width(), get_char_height(), 1);
                if(player1.score.scoreNum > player2.score.scoreNum){
                    printColorCentered("Player 1 won the game!\n", BRIGHT_YELLOW, FIELD_COLOR, get_char_width(), get_char_height(), 0);
                }else{
                    printColorCentered("Player 2 won the game!\n", BRIGHT_YELLOW, FIELD_COLOR, get_char_width(), get_char_height(), 0);
                }
                reduce();
                beep(1000, 10);
                sleep(3);
                break;
            }else {
                // Siguiente nivel
                drawCircle(oldBallX, oldBallY, ball.radius, FIELD_COLOR);
                drawCircle(hole.x, hole.y, hole.outRadius, hole.outColor);
                drawCircle(hole.x, hole.y, hole.radius, hole.color);
                drawCircle(hole.x, hole.y, ball.radius, ball.color);
                increase();
                printColorCentered("Level completed! Get ready for the next one!\n", BRIGHT_YELLOW, FIELD_COLOR, get_char_width(), get_char_height(), 1);
                
                reduce();
                beep(1000, 10);
                sleep(1);
                
                // Resetear posiciones para el siguiente nivel
                resetLevel();
                drawGame();

                drawScore(&player1);
                if (num_players == 2) {
                    drawScore(&player2);
                }
            }
        }
    }
    clean_buff();
    // clear();
    game_running = 0;
}

void drawGame() {

    drawRectangle(0, 0, screenW, screenH, FIELD_COLOR);

    drawPlayer(player1);


    if (num_players == 2) {
        drawPlayer(player2);
    }

    drawCircle(ball.x, ball.y, ball.radius, ball.color);
    
    drawCircle(hole.x, hole.y, hole.outRadius, hole.outColor);

    drawCircle(hole.x, hole.y, hole.radius, hole.color);
}

void drawPlayer(Paddle p){
    drawCircle(p.x, p.y, p.radius, p.color);
}

void updateGame1(uint8_t * pressed_keys) {
    // Guardar posiciones anteriores
    int oldX = (int)player1.x;
    int oldY = (int) player1.y;

    if (pressed_keys[SC_W] && (player1.y - SPEED) > player1.radius) 
         player1.y -= SPEED;
    if (pressed_keys[SC_S] && (player1.y + SPEED) < (screenH - player1.radius)) 
        player1.y += SPEED;
    if (pressed_keys[SC_A] && (player1.x - SPEED) > player1.radius) 
        player1.x -= SPEED;
    if (pressed_keys[SC_D] && (player1.x + SPEED) < (screenW - player1.radius)) 
        player1.x += SPEED;

    // Actualizar posiciones enteras solo si hubo un cambio significativo
    int newX = (int) player1.x;
    int newY = (int) player1.y;

    if (newX != oldX || newY != oldY) {
        // Borrar la posición anterior
        drawCircle(oldX, oldY, player1.radius, FIELD_COLOR);
        
        // Verificar límites
        checkIfBorderPlayer(&player1);
        
        // Redibujar el hoyo si fue tapado
        if (checkCollision(player1.x, player1.y, player1.radius, hole.x, hole.y, hole.outRadius)) {
            drawCircle(hole.x, hole.y, hole.outRadius, hole.outColor);
            drawCircle(hole.x, hole.y, hole.radius, hole.color);
        }
        
        if(checkScoreCollision(player1.x, player1.y, player1.radius, player1.score.x , player1.score.y)){
            drawScore(&player1);
        }

        if(num_players == 2 && 
           checkScoreCollision(player1.x, player1.y, player1.radius, player2.score.x, player2.score.y)){
            // Si hay dos jugadores, verificar colisión con el score del jugador 2
            drawScore(&player2);
        }

        // Dibujar en la nueva posición
        drawCircle(player1.x, player1.y, player1.radius, player1.color);

        if(checkCollision(player1.x, player1.y, player1.radius, player2.x, player2.y, player2.radius) && num_players == 2) {
            sleep(4);
            drawCircle(player1.x, player1.y, player1.radius, FIELD_COLOR);
            drawCircle(player2.x, player2.y, player2.radius, FIELD_COLOR);
            if(player1.x < player2.x) {
                player1.x = oldX - 3; 
            } else {
                player1.x = oldX + 3;
            }
            if(player1.y < player2.y) {
                player1.y = oldY - 3; 
            } else {
                player1.y = oldY + 3;
            }
            drawCircle(player1.x, player1.y, player1.radius, player1.color);
            drawCircle(player2.x, player2.y, player2.radius, player2.color);
        }

        // Verificar colisión con la pelotaMore actions
        if (checkCollision(player1.x, player1.y, player1.radius, ball.x, ball.y, ball.radius)) {
            double dx = ball.x - player1.x;
            double dy = ball.y - player1.y;

            // Movemos la pelota gradualmente
            for(int i = 0; i < BALL_SPEED; i++) {
                sleep(1);
                // Guardamos la última posicion
                oldBallX = ball.x;
                oldBallY = ball.y;

                // Borramos la posicion anterior
                drawCircle(oldBallX, oldBallY, ball.radius, FIELD_COLOR);

                // Calculamos la nueva posicion
                ball.x += (dx * BALL_SPEED) / 5;
                ball.y += (dy * BALL_SPEED) / 5;

                // Verificamos limites
                checkIfBorderBall(&ball, &ball_dx, &ball_dy);

                if(ballInHole()){
                    last_player_hit = 1;
                    return ;
                }

                if (checkCollision(oldBallX, oldBallY, ball.radius, hole.x, hole.y, hole.outRadius)) {
                    drawCircle(hole.x, hole.y, hole.outRadius, hole.outColor);
                    drawCircle(hole.x, hole.y, hole.radius, hole.color);
                }

                if(checkScoreCollision(oldBallX, oldBallY, ball.radius, player1.score.x, player1.score.y)){
                    drawScore(&player1);
                }

                if(num_players == 2 && checkScoreCollision(oldBallX, oldBallY, ball.radius, player2.score.x, player2.score.y)){
                    drawScore(&player2);
                }

                checkCollisionBallPlayer(oldBallX, oldBallY, ball.radius, &player2, &ball_dx, &ball_dy);

                // Dibujamos la nueva posición
                drawCircle(ball.x, ball.y, ball.radius, ball.color);
                drawPlayer(player1);

                // Actualizamos la posicion anterior
                oldBallX = ball.x;
                oldBallY = ball.y;
            }
            last_player_hit = 1;
            // beep(800, 15);
        }
    }
}


void updateGame2(uint8_t * pressed_keys) {
    updateGame1(pressed_keys);

    int oldX = (int) player2.x;
    int oldY = (int) player2.y;

    // Mover jugador usando variables con decimales
    if (pressed_keys[SC_I] && player2.y > player2.radius) player2.y -= SPEED;
    if (pressed_keys[SC_K] && player2.y < screenH - player2.radius) player2.y += SPEED;
    if (pressed_keys[SC_J] && player2.x > player2.radius) player2.x -= SPEED;
    if (pressed_keys[SC_L] && player2.x < screenW - player2.radius) player2.x += SPEED;

    // Actualizar posición del jugador solo si el cambio es significativo
    int newX = (int) player2.x;
    int newY = (int) player2.y;

    if (newX != oldX || newY != oldY) {
        // Borrar posición anterior
        drawCircle(oldX, oldY, player2.radius, FIELD_COLOR);
        
        // Verificar límites
        checkIfBorderPlayer(&player2);

        // Redibujar el hoyo si fue tapado
        if (checkCollision(player2.x, player2.y, player2.radius, hole.x, hole.y, hole.outRadius)) {
            drawCircle(hole.x, hole.y, hole.outRadius, hole.outColor);
            drawCircle(hole.x, hole.y, hole.radius, hole.color);
        }

        if(checkScoreCollision(player2.x, player2.y, player2.radius, player1.score.x, player1.score.y)){
            drawScore(&player1);
        }

        if(checkScoreCollision(player2.x, player2.y, player2.radius, player2.score.x, player2.score.y)){
            drawScore(&player2);
        }
        
        // Dibujar nueva posición
        drawPlayer(player2);
        
        if(checkCollision(player1.x, player1.y, player1.radius, player2.x, player2.y, player2.radius)){
            drawCircle(player1.x, player1.y, player1.radius, FIELD_COLOR);
            drawCircle(player2.x, player2.y, player2.radius, FIELD_COLOR);
            if(player2.x < player1.x) {
                player2.x = oldX - 3; 
            } else {
                player2.x = oldX + 3;
            }
            if(player2.y < player1.y) {
                player2.y = oldY - 3; 
            } else {
                player2.y = oldY + 3;
            }
            drawPlayer(player1);
            drawPlayer(player2);
        }


        // Verificar colisión con la pelotaAdd commentMore actions
        if (checkCollision(player2.x, player2.y, player2.radius, ball.x, ball.y, ball.radius)) {
            double dx = ball.x - player2.x;
            double dy = ball.y - player2.y;



            for(int i = 0; i < BALL_SPEED; i++) {
                sleep(1);
                // Guardamos la última posición
                oldBallX = ball.x;
                oldBallY = ball.y;

                // Borrar la posición anterior
                drawCircle(oldBallX, oldBallY, ball.radius, FIELD_COLOR);

                // Mover la pelota gradualmente
                ball.x += (dx * BALL_SPEED) / 5;
                ball.y += (dy * BALL_SPEED) / 5;

                // Verificar límites
                checkIfBorderBall(&ball, &ball_dx, &ball_dy);

                if(ballInHole()){
                    last_player_hit = 2;
                    return ;
                }

                if (checkCollision(oldBallX, oldBallY, ball.radius, hole.x, hole.y, hole.outRadius)) {
                    drawCircle(hole.x, hole.y, hole.outRadius, hole.outColor);
                    drawCircle(hole.x, hole.y, hole.radius, hole.color);
                }

                if(checkScoreCollision(oldBallX, oldBallY, ball.radius, player1.score.x, player1.score.y)){
                    drawScore(&player1);
                }

                if(checkScoreCollision(oldBallX, oldBallY, ball.radius, player2.score.x, player2.score.y)){
                    drawScore(&player2);
                }

                checkCollisionBallPlayer(oldBallX, oldBallY, ball.radius, &player1, &ball_dx, &ball_dy);

                // Dibujar nueva posición
                drawCircle(ball.x, ball.y, ball.radius, ball.color);
                drawPlayer(player2);

                // Actualizamos la posición anterior
                oldBallX = ball.x;
                oldBallY = ball.y;
            }

            last_player_hit = 2;
            // beep(800, 15);
        }
    }
}


int ballInHole() {
    // Si la pelota está lo suficientemente cerca del centro del hoyo
    double dx = ball.x - hole.x;
    double dy = ball.y - hole.y;
    
    // Aumentamos el área de detección multiplicando por 2
    double detection_radius = (hole.outRadius + ball.radius);
    return dx * dx + dy * dy <= detection_radius * detection_radius;
}

int checkCollision(double x1, double y1, double r1, double x2, double y2, double r2) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    return dx * dx + dy * dy <= (r1 + r2) * (r1 + r2);
}

int checkScoreCollision(double x1, double y1, double r1, double x2, double y2){
    // Calcular los límites del rectángulo del score
    double rect_left = x2;
    double rect_right = x2 + score_width;
    double rect_top = y2;
    double rect_bottom = y2 + score_height;
    
    // Encontrar el punto más cercano del rectángulo al centro del círculo
    double closest_x = x1;
    double closest_y = y1;
    
    // Limitar las coordenadas del círculo al rectángulo
    if (x1 < rect_left) closest_x = rect_left;
    else if (x1 > rect_right) closest_x = rect_right;
    
    if (y1 < rect_top) closest_y = rect_top;
    else if (y1 > rect_bottom) closest_y = rect_bottom;
    
    // Calcular la distancia entre el centro del círculo y el punto más cercano
    double dx = x1 - closest_x;
    double dy = y1 - closest_y;
    
    // Verificar si la distancia es menor que el radio
    return (dx * dx + dy * dy) <= (r1 * r1);
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

void checkIfBorderBall(Ball *b, double *dx, double *dy){
    const double OFFSET = 25; // Reducir el offset para un rebote más suave
    
    // Rebote horizontal
    if(b->x - b->radius <= 0){
        // Posicionar correctamente la pelota
        b->x = b->radius + OFFSET;
        if(*dx < 0) {
            *dx = -*dx;
        }
    }else if(b->x + b->radius >= set_width()){
        // Posicionar correctamente la pelota
        b->x = set_width() - b->radius - OFFSET;
        if(*dx > 0) {
            *dx = -*dx;
        }
    }

    // Rebote vertical
    if(b->y - b->radius <= 0){
        // Posicionar correctamente la pelota
        b->y = b->radius + OFFSET;
        if(*dy < 0) {
            *dy = -*dy;
        }
    }else if(b->y + b->radius >= set_height()){
        // Posicionar correctamente la pelota
        b->y = set_height() - b->radius - OFFSET;
        if(*dy > 0) {
            *dy = -*dy;
        }
    }
}

void checkCollisionBallPlayer(double oldBallX, double oldBallY, double ball_radius, Paddle * p ,double *dx, double *dy) {
    if (checkCollision(oldBallX, oldBallY, ball_radius, p->x, p->y, p->radius)) {
        drawPlayer(*p);
        // Rebote horizontal
        if (oldBallX < p->x) { // Pelota a la izquierda del jugador
            ball.x = p->x - p->radius - ball_radius - 25; // Offset pequeño
            if (*dx > 0) 
                *dx = -*dx; // Invertir dirección horizontal
        } else if (oldBallX > p->x) { // Pelota a la derecha del jugador
            ball.x = p->x + p->radius + ball_radius + 25; // Offset pequeño
            if (*dx < 0) 
                *dx = -*dx; // Invertir dirección horizontal
        }

        // Rebote vertical
        if (oldBallY < p->y) { // Pelota arriba del jugador
            ball.y = p->y - p->radius - ball_radius - 25; // Offset pequeño
            if (*dy > 0) 
                *dy = -*dy; // Invertir dirección vertical
        } else if (oldBallY > p->y) { // Pelota abajo del jugador
            ball.y = p->y + p->radius + ball_radius + 25; // Offset pequeño
            if (*dy < 0)
                *dy = -*dy; // Invertir dirección vertical
        }
    }
}

void handleBallCollision(Paddle *player, int player_num) {
    if (checkCollision(player->x, player->y, player->radius, ball.x, ball.y, ball.radius)) {
        // Calcula el vector de dirección desde el jugador hacia la pelota
        double dx = ball.x - player->x;
        double dy = ball.y - player->y;
        
        // Mueve la pelota en esa dirección
        ball.x += dx / 5;
        ball.y += dy / 5;
        
        // Registra qué jugador golpeó la pelota
        last_player_hit = player_num;
        
        // Efecto de sonido al golpear
        beep(500, 10);
    }
}

void resetLevel() {
    int margin = 50;

    // Aumentar la dificultad con cada nivel
    switch(current_level) {
        case 2:
            //Nivel 2: Hoyo mas pequeño
            hole.radius = 30;
            hole.outRadius = 36;
        case 3:
            // Nivel 3: Hoyo más pequeño
            hole.radius = 25;
            hole.outRadius = 30;
            break;
        case 4:
            // Nivel 4: Hoyo más pequeño
            hole.radius = 20;
            hole.outRadius = 24;
            break;
        case 5:
            // Nivel 5: Hoyo aún más pequeño y pelota más pequeña
            hole.radius = 15;
            hole.outRadius = 18;
            ball.radius = 8;
            break;

    }

    // Nuevas posiciones aleatorias
    do {
        ball.x = rand_between(screenW - 2*margin) + margin;
        ball.y = rand_between(screenH - 2*margin) + margin;
    } while (checkCollision(ball.x, ball.y, ball.radius, player1.x, player1.y, player1.radius) ||
             (num_players == 2 && checkCollision(ball.x, ball.y, ball.radius, player2.x, player2.y, player2.radius)));

    do {
        hole.x = rand_between(screenW - 2*margin) + margin;
        hole.y = rand_between(screenH - 2*margin) + margin;
    } while (checkCollision(hole.x, hole.y, hole.radius, player1.x, player1.y, player1.radius) ||
             checkCollision(hole.x, hole.y, hole.radius, ball.x, ball.y, ball.radius) ||
             (num_players == 2 && checkCollision(hole.x, hole.y, hole.radius, player2.x, player2.y, player2.radius)));
    
    // Resetear velocidades de la pelota
    ball_dx = 0;
    ball_dy = 0;

}

void drawScore(Paddle * p){
    if(p == &player1){
        moveCursor(p->score.x, p->score.y);
        printColor("SCORE 1:", BLACK, LIGHT_RED);
        printDec(p->score.scoreNum);
    }else{
        moveCursor(p->score.x, p->score.y);
        printColor("SCORE 2:", BLACK, LIGHT_BLUE);
        printDec(p->score.scoreNum);
    }

}

void moveCursor(uint64_t x, uint64_t y){
    set_cursorX(x);
    set_cursorY(y);
}