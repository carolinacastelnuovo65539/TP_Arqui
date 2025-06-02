#include <pognis.h>
#include <userlib.h>

Ball ball = {160, 100, BALL_SPEED, BALL_SPEED, BALL_RADIUS};
int game_running = 1;
int num_players = 1;

TPlayer player1, player2;

void init_game(){
    if(num_players == 1){
        init_player(player1);
    }else if(num_players == 2){
        init_players(player1, player2);
    }

}

void init_player(TPlayer p){
    if(p != 0){
        p->x = 10;
        p->y = (SCREEN_HEIGHT - PLAYER_RADIUS) / 2;
        p->radius = PLAYER_RADIUS;
        p->score = 0;
        p->playing = 1;
        p->color = ORANGE;
    }
}

void init_players(TPlayer player1, TPlayer player2){
    init_player(player1);
    player2->x = SCREEN_WIDTH - PLAYER_RADIUS - 10;
    player2->y = (SCREEN_HEIGHT - PLAYER_RADIUS) / 2;
    player2->radius = PLAYER_RADIUS;
    player2->score = 0;
    player2->playing = 1;
    player2->color = BLUE;
}

void update_game1() {
    char c;
    while(player1->playing && game_running){
        print_score(player1);
        if((c = getChar()) != 0){
            c = to_lower(c);

            if (c == 27) { // ESC
                game_running = 0;
                break;
            }

            move_player(player1, c);
        }
        // Aquí podrías actualizar la pelota y dibujar el juego si corresponde
    }
    clear();
    // end_game1();
}

void move_player(TPlayer p, char key) {
    // Jugador 1: WASD
    if (p == player1) {
        switch(key) {
            case 'w':
                if (p->y > 0)
                    p->y -= PLAYER_SPEED;
                break;
            case 's':
                if (p->y < SCREEN_HEIGHT - PLAYER_RADIUS)
                    p->y += PLAYER_SPEED;
                break;
            case 'a':
                if (p->x > 0)
                    p->x -= PLAYER_SPEED;
                break;
            case 'd':
                if (p->x < SCREEN_WIDTH - PLAYER_RADIUS)
                    p->x += PLAYER_SPEED;
                break;
        }
    }
    
    // Jugador 2: IJKL
    else if (p == player2) {
        switch(key) {
            case 'i':
                if (p->y > 0)
                    p->y -= PLAYER_SPEED;
                break;
            case 'k':
                if (p->y < SCREEN_HEIGHT - PLAYER_RADIUS)
                    p->y += PLAYER_SPEED;
                break;
            case 'j':
                if (p->x > 0)
                    p->x -= PLAYER_SPEED;
                break;
            case 'l':
                if (p->x < SCREEN_WIDTH - PLAYER_RADIUS)
                    p->x += PLAYER_SPEED;
                break;
        }
    }
}

void update_game2() {
    char c;
    while(player1->playing && player2->playing && game_running){
        print_score(player1);
        print_score(player2);

        if((c = getChar()) != 0){
            c = to_lower(c);

            if (c == 27) { // ESC
                game_running = 0;
                break;
            }

            move_player(player1, c);
            move_player(player2, c);
        }
        // Aquí podrías actualizar la pelota y dibujar el juego si corresponde
    }
    clear();
    // end_game1();
}


void reset_ball(void) {
    ball.x = SCREEN_WIDTH / 2;
    ball.y = SCREEN_HEIGHT / 2;
    
    // Dirección aleatoria
    ball.dx = (ball.dx > 0) ? -BALL_SPEED : BALL_SPEED;
    ball.dy = (ball.dy > 0) ? -BALL_SPEED : BALL_SPEED;
}

// void update_game() {
//     char key = getChar();
    
//     // Controles jugador 1 (WASD - movimiento completo)
//      if (key == 'w' || key == 'W') {  // W - Arriba
//         if (player1->y > 0)
//             player1->y -= PLAYER_SPEED;
//     }
//     if (key == 's' || key == 'S') {  // S - Abajo
//         if (player1->y < SCREEN_HEIGHT - PLAYER_RADIUS)
//             player1->y += PLAYER_SPEED;
//     }
//     if (key == 'a' || key == 'A') {  // A - Izquierda
//         if (player1->x > 0)
//             player1->x -= PLAYER_SPEED;
//     }
//     if (key == 'd' || key == 'D') {  // D - Derecha
//         if (player1->x < SCREEN_WIDTH - PLAYER_RADIUS)
//             player1->x += PLAYER_SPEED;
//     }
    
//     // Controles jugador 2 (Flechas direccionales - movimiento completo)

//     // los hexa ni idea, desp q nos anda player 1 vemos :/

//     if(num_players == 2){
//         if (key == 0x48) {  // Flecha arriba
//             if (player2->y > 0)
//                 player2->y -= PLAYER_SPEED;
//         }
//         if (key == 0x50) {  // Flecha abajo
//             if (player2->y < SCREEN_HEIGHT - PLAYER_RADIUS)
//                 player2->y += PLAYER_SPEED;  // Corregido: era PLAYER_RADIUS
//         }
//         if (key == 0x4B) {  // Flecha izquierda
//             if (player2->x > 0)
//                 player2->x -= PLAYER_SPEED;
//         }
//         if (key == 0x4D) {  // Flecha derecha
//             if (player2->x < SCREEN_WIDTH - PLAYER_RADIUS)
//                 player2->x += PLAYER_SPEED;
//         }
//     }
    
//     // ESC para salir
//     if (key == 0x01) {
//         game_running = 0;
//         return;
//     }
    
//     // Mover pelota
//     ball.x += ball.dx;
//     ball.y += ball.dy;
    
//     // Colisiones con bordes superior/inferior
//     if (ball.y <= 0 || ball.y >= SCREEN_HEIGHT - BALL_RADIUS) {
//         ball.dy = -ball.dy;
//     }
    
//     // Verificar goles
//     if (ball.x <= 0) {
//         player2->score++;
//         reset_ball();
//     }
//     if (ball.x >= SCREEN_WIDTH - BALL_RADIUS) {
//         player1->score++;
//         reset_ball();
//     }
    
//     check_collisions();
// }

void check_collisions(void) {
    // Colisión con paddle izquierdo (player1)
    if (ball.x <= player1->x + player1->radius &&
        ball.x + ball.size >= player1->x &&
        ball.y <= player1->y + player1->radius &&
        ball.y + ball.size >= player1->y) {
        
        if (ball.dx < 0) {  // Solo si viene desde la derecha
            ball.dx = -ball.dx;
            // Añadir efecto según donde golpee
            int paddle_center = player1->y + player1->radius / 2;
            int ball_center = ball.y + ball.size / 2;
            int diff = ball_center - paddle_center;
            ball.dy += diff / 8;  // Efecto de "spin"
        }
    }
    
    // Colisión con paddle derecho (player2)
    if (ball.x + ball.size >= player2->x &&
        ball.x <= player2->x + player2->radius &&
        ball.y <= player2->y + player2->radius &&
        ball.y + ball.size >= player2->y) {
        
        if (ball.dx > 0) {  // Solo si viene desde la izquierda
            ball.dx = -ball.dx;
            // Añadir efecto según donde golpee
            int paddle_center = player2->y + player2->radius / 2;
            int ball_center = ball.y + ball.size / 2;
            int diff = ball_center - paddle_center;
            ball.dy += diff / 8;  // Efecto de "spin"
        }
    }
    
    // Limitar velocidad Y
    if (ball.dy > 4) ball.dy = 4;
    if (ball.dy < -4) ball.dy = -4;
}

void draw_game(void) {
    // Limpiar pantalla
    clear();
    
    // Dibujar players
    drawCircle(player1->x, player1->y, player1->radius, player1->color);
    drawCircle(player2->x, player2->y, player2->radius, player2->color);
    
    // Dibujar pelota
    drawCircle(ball.x, ball.y, ball.size, WHITE);
    
    // Dibujar línea central (punteada)
    for (int i = 0; i < SCREEN_HEIGHT; i += 8) {
        drawRectangle(SCREEN_WIDTH/2 - 1, i, 2, 4, WHITE);
    }
    
    // Dibujar puntuación (simplificado - números como rectángulos)
    // draw_score(50, 20, player1.score);
    // draw_score(250, 20, player2.score);

    print_score(player1);
    print_score(player2);
}


void print_score(TPlayer p) {
    printColor("SCORE: ", 7, LIGHT_BLUE, DARK_PINK);
    printDec(p->score);
}

/*
void draw_score(int x, int y, int score) {
    // Representación simple de números usando rectángulos
    // Por simplicidad, solo mostramos hasta 9
    if (score > 9) score = 9;
    
    // Dibujar el número como un patrón de puntos
    switch(score) {
        case 0:
            drawRectangle(x, y, 20, 4, WHITE);
            drawRectangle(x, y, 4, 20, WHITE);
            draw_rectangle(x+16, y, 4, 20, WHITE);
            draw_rectangle(x, y+16, 20, 4, WHITE);
            break;
        case 1:
            draw_rectangle(x+8, y, 4, 20, WHITE);
            break;
        case 2:
            drawRectangle(x, y, 20, 4, WHITE);
            drawRectangle(x+16, y, 4, 8, WHITE);
            draw_rectangle(x, y+8, 20, 4, WHITE);
            draw_rectangle(x, y+12, 4, 8, WHITE);
            draw_rectangle(x, y+16, 20, 4, WHITE);
            break;
        // ... más números
    }
}
    */
