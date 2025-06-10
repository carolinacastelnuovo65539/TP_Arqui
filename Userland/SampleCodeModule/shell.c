#include <shell.h>
#include <userlib.h>
#include <pongis.h>

#define USER_MAX 32
#define COMMAND_MAX 100
#define COMMANDS 14
#define MAX_BUFF 15

static void leftLine();

int USER_SIZE = 5;

static char username[USER_MAX] = "user";
char command[COMMAND_MAX] = {0};

char * commands[] = {"-help", "-time", "-changeUsername", "-registers", "-username", "-exit", "-increaseSize", "-decreaseSize", "-clear", "-beep", "-zerodiv", "-invopcode", "-pongis", "-date"};

int newTerminal = 1;

int using = 1;

void changeUsername(){
	char c = 0;
	int len = 0;
	putChar('\n');
	leftLine();
	print("Enter your username:");
	while((c = getChar()) != '\n' && len < USER_MAX){
		if(c != 0){
			if(c == '\b' && len > 0){
				len--;
				putChar('\b');
			}else if(c != '\b'){
				username[len++] = c;
				putChar(c);
			}
		}
	}
	if(len >= USER_MAX){
		print("The username cannot exceed 32 characters");
		changeUsername();
		return;
	}else{
		username[len] = '\0';
		USER_SIZE = len;
		putChar('\n');
		print("Welcome ");
		print(username);
		putChar('\n');
	}
}

void read(){
	char c;
    int i = 0;
    while((c = getChar()) != '\n' && i < COMMAND_MAX){
        if(c != 0){
            if(c == '\b' && i > 0){
                i--;
                putChar('\b');
            }else if(c != '\b'){
                command[i++] = c;
                putChar(c);
            }
        }
    }
    command[i] = '\0';
	checkCommand(command);
}

void checkCommand(char * cmd){
	for(int i = 0; i < COMMANDS; i++){
		if(strcmp(cmd, commands[i]) == 0){
			switch (i)
			{
			case 0:
				command_help();
				break;
			case 1:
				command_time();
				break;
			case 2:
				changeUsername();
				break;
			case 3:
				command_registers();
				break;
			case 4:
				getUserName();
				break;
			case 5:
				command_exit();
				break;
			case 6:
				command_increase();
				break;
			case 7:
				command_reduce();
				break;
			case 8:
				command_clear();
				break;
			case 9:
				command_beep();
				break;
			case 10:
				command_zerodiv();
				break;
			case 11:
				command_invopcode();
				break;
			case 12:
				command_pognis();
				break;
			case 13:
				command_date();
				break;
			default:
				invalid_command();
			}
			return;
		}
	}
	invalid_command();
}

void start(){
    if(newTerminal){
		newTerminal = 0;
		print("Welcome to our OS!\n");	
		changeUsername();
		getTime();
		print("       [hour:min:sec]\n");
	}
}


void command_pognis(){
	using = 0;
    command_clear();
    increase();
    printColorCentered("Welcome to Pongis game!\n",LIGHT_BLUE, BLACK, get_char_width(), get_char_height(), 1);
    reduce();

	printColorCentered("Player 1 uses W/A/S/D keys to move, player 2 uses I/J/K/L to move.\n",LIGHT_BLUE, BLACK, get_char_width(), get_char_height(), 0);
    printColorCentered("To exit the game, press 'ESC' key.\n", LIGHT_BLUE, BLACK, get_char_width(),get_char_height(), 0);
    printColorCentered("Enter the amount of players (1 or 2): ",LIGHT_BLUE, BLACK, get_char_width(), get_char_height(), 0);
	char players;

	while(((players = getChar()) != '1' && players != '2')) {
		
	}
	start_game_pongis(players);

	clear();
	using = 1;
	
    // char c;
    // while((c = getChar()) != '1' && c != '2'){
    // }
    // if(c == '1'){
    //     num_players = 1;
    //     print("Starting Pognis with 1 player...\n", 30);
    //     init_game();
    //     while(game_running){
    //         update_game1();
    //         draw_game();
    //     }
    // }else if(c == '2'){
    //     num_players = 2;
    //     print("Starting Pognis with 2 players...\n", 30);
    //     init_game();
    //     while(game_running){
    //         update_game2();
    //         draw_game();
    //     }
    // }
	// command_clear();
	// using = 0;
}

void command_reduce(){
	clear();
	reduce();
}

void command_increase(){
	clear();
	increase();
}

void command_clear(){
	clear();
}

static void leftLine(){
	printColor(username, TEAL, BLACK);
	printColor("~$ > ", VIOLET, BLACK);
}

void terminal(){
	leftLine();
	print("Write '-help' to see the list of commands.\n");
	leftLine();
	print("Enter a command: \n");
	while(using){
		leftLine();
		read();
	}
}

void command_zerodiv(){
	zero();
}

void command_invopcode(){
	opcode();
}	

void command_beep(){
	beep(100, 15);
	putChar('\n');
}

void command_help() {
	printColor("\n\n=== Available commands ===\n", YELLOW, BLACK);
	printHelp();
}

void printHelp() {
	printColor("\n    -help                  ", ORANGE, BLACK);
	print("- displays this shell information");
	printColor("\n    -changeUsername        ", ORANGE, BLACK);
	print("- change username");
	printColor("\n    -username              ", ORANGE, BLACK);
	print("- display current username");
	printColor("\n    -time                  ", ORANGE, BLACK);
	print("- display current time");
	printColor("\n    -date                  ", ORANGE, BLACK);
	print("- display UTC date");
	printColor("\n    -clear                 ", ORANGE, BLACK);
	print("- clear the display");
	printColor("\n    -increaseSize          ", ORANGE, BLACK);
	print("- increase font size (scaled)");
	printColor("\n    -decreaseSize          ", ORANGE, BLACK);
	print("- decrease font size (scaled)");
	printColor("\n    -registers             ", ORANGE, BLACK);
	print("- print current register values");
	printColor("\n    -beep                  ", ORANGE, BLACK);
	print("- produces a beep");
	printColor("\n    -zerodiv               ", ORANGE, BLACK);
	print("- test zero division exception");
	printColor("\n    -invopcode             ", ORANGE, BLACK);
	print("- test invalid opcode exception");
	printColor("\n    -exit                  ", ORANGE, BLACK);
	print("- exit the OS");
	printColor("\n    -pongis                ", ORANGE, BLACK);
	print("- start the game\n");
	putChar('\n');
}

void command_time(){
	// cuando ss < 10 los imprime sin un cero adelante
	print("\nThe local time [hour:min:sec] is: ");
	getTime();
	putChar('\n');
}

void command_date() {
	print("\nThe UTC date [month/day/year] is: ");
	getDate();
	putChar('\n');
}

void getUserName(){
	putChar('\n');;
	leftLine();
	print(username);
	putChar('\n');
}

void command_registers(){
	print("\nRegisters:\n");
	printRegisters();
	// print(" \n", 30); //si no lo hacemos así, se imprime cualquier cosa al final de los registros
	putChar('\n');
}

void invalid_command(){
	putChar('\n');
	leftLine();
	print("Invalid command. Write -help to see the list of commands\n");
}
	
void command_exit() {
	print("\nSee you next time!\n");
	using = 0;
}