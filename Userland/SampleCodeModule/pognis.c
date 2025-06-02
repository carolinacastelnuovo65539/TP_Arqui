#include <pognis.h>
#include <userlib.h>

Ball ball = {160, 100, BALL_SPEED, BALL_SPEED, BALL_RADIUS};
int game_running = 1;
int num_players = 1;

TPlayer player1, player2;

static const uint16_t cursorScoreX1=0;
static const uint16_t cursorScoreY1=0;
static const uint16_t cursorScoreX2=28*SQUARESIZE - 10;
static const uint16_t cursorScoreY2=0;

void init_game(){
    draw_game();
    if(num_players == 1){
        init_player(player1);
    }else if(num_players == 2){
        init_players(player1, player2);
    }
}

void init_player(TPlayer p){
    if(p != 0){
        p->x = 10;
        p->y = (set_height() - PLAYER_RADIUS) / 2;
        p->radius = PLAYER_RADIUS;
        p->score = 0;
        p->playing = 1;
        p->color = ORANGE;
    }
}

void init_players(TPlayer player1, TPlayer player2){
    init_player(player1);
    player2->x = set_width() - PLAYER_RADIUS - 10;
    player2->y = (set_height() - PLAYER_RADIUS) / 2;
    player2->radius = PLAYER_RADIUS;
    player2->score = 0;
    player2->playing = 1;
    player2->color = BLUE;
}


void start_game(char players){
    draw_game();
    num_players = players;
    if(num_players == '1') {
        update1();
    }else if(num_players == '2'){
        update2();
    }
    clear();
}

void update2(){
    char c;
    init_players(player1, player2);
    while(player1->playing && player2->playing && game_running){
        move_cursor(cursorScoreX1, cursorScoreY1);
        print_score(player1);
        move_cursor(cursorScoreX2, cursorScoreY2);
        print_score(player2);
        if((c = getChar()) != 0){
            c = to_lower(c);

            if (c == 27) { // ESC
                game_running = 0;
                clear();
                break;
            }

            move_player(player1, c);
            move_player(player2, c);
        }
    }
    clear();

        
    }

void update1(){
    char c;
    init_player(player1);
    while(player1->playing && game_running){
        move_cursor(cursorScoreX1, cursorScoreY1);
        print_score(player1);
        if((c = getChar()) != 0){
            c = to_lower(c);

            if (c == 27) { // ESC
                game_running = 0;
                clear();
                break;
            }

            move_player(player1, c);
        }
    }
    clear();
    //end1();
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
        

        if((c = getChar()) != 0){
            c = to_lower(c);

            if (c == 27) { // ESC
                game_running = 0;
                break;
            }
            move_cursor(cursorScoreX1, cursorScoreY1);
            print_score(player1);
            move_player(player1, c);
            move_cursor(cursorScoreX2, cursorScoreY2);
            print_score(player2);
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

    // 1. Dibujar el fondo/cancha como un rectángulo verde
    drawRectangle(0, 0, set_width(), set_height(), LIGHT_GREEN);

    // 2. Dibujar los jugadores como "bolbs" (círculos de colores)
    drawCircle(player1->x, player1->y, player1->radius, BLUE);
    drawCircle(player2->x, player2->y, player2->radius, ORANGE);

    // 3. Dibujar la pelota
    drawCircle(ball.x, ball.y, ball.size, WHITE);

}


void print_score(TPlayer p) {
    printColor("SCORE: ", 7, BLUE, WHITE);
    printDec(p->score);
}

void move_cursor(uint64_t x, uint64_t y){
    set_cursorX(x);
    set_cursorY(y);
}