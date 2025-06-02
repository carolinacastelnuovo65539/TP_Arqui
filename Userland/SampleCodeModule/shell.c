#include <shell.h>
#include <userlib.h>

#define USER_MAX 32
#define COMMAND_MAX 100
#define COMMANDS 14
#define MAX_BUFF 15

static void leftLine();

int USER_SIZE = 5;

static char username[USER_MAX] = "user";
char command[COMMAND_MAX] = {0};

char * commands[] = {"-help", "-time", "-changeUsername", "-registers", "-username", "-exit", "-increaseSize", "-decreaseSize", "-clear", "-beep", "-zerodiv", "-invopcode", "-pognis", "-date"};

int newTerminal = 1;

int using = 1;

void changeUsername(){
	char c = 0;
	int len = 0;
	putChar('\n');
	leftLine();
	print("Enter your username:", 40);
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
		print("The username cannot exceed 32 characters", 50);
		changeUsername();
		return;
	}else{
		username[len] = '\0';
		USER_SIZE = len;
		putChar('\n');
		print("Welcome ", 30);
		print(username, USER_SIZE);
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
		print("Welcome to our OS!\n", 40);	
		changeUsername();
		getTime();
		print("       [hour:min:sec]\n", 30);
	}
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
	printColor(username, USER_SIZE, LIGHT_BLUE, BLACK);
	printColor("~$ > ", 5, LIGHT_GREEN, BLACK);
}

void terminal(){
	leftLine();
	print("Write '-help' to see the list of commands.\n", 70);
	leftLine();
	print("Enter a command: \n", 30);
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

void command_pognis(){

}

void command_help() {
	printColor("\n\n=== Available commands ===\n", 25, YELLOW, BLACK);
	printHelp();
}

void printHelp() {
	printColor("\n    -help                  ", MAX_BUFF, ORANGE, BLACK);
	print("- displays this shell information", MAX_BUFF);
	printColor("\n    -changeUsername        ", MAX_BUFF, ORANGE, BLACK);
	print("- change username", MAX_BUFF);
	printColor("\n    -username              ", MAX_BUFF, ORANGE, BLACK);
	print("- display current username", MAX_BUFF);
	printColor("\n    -time                  ", MAX_BUFF, ORANGE, BLACK);
	print("- display current time", MAX_BUFF);
	printColor("\n    -date                ", MAX_BUFF, ORANGE, BLACK);
	print("- display UTC date\n", MAX_BUFF);
	printColor("\n    -clear                 ", MAX_BUFF, ORANGE, BLACK);
	print("- clear the display", MAX_BUFF);
	printColor("\n    -increaseSize          ", MAX_BUFF, ORANGE, BLACK);
	print("- increase font size (scaled)", MAX_BUFF);
	printColor("\n    -decreaseSize          ", MAX_BUFF, ORANGE, BLACK);
	print("- decrease font size (scaled)", MAX_BUFF);
	printColor("\n    -registers             ", MAX_BUFF, ORANGE, BLACK);
	print("- print current register values", MAX_BUFF);
	printColor("\n    -beep                  ", MAX_BUFF, ORANGE, BLACK);
	print("- produces a beep", MAX_BUFF);
	printColor("\n    -zerodiv               ", MAX_BUFF, ORANGE, BLACK);
	print("- test zero division exception", MAX_BUFF);
	printColor("\n    -invopcode             ", MAX_BUFF, ORANGE, BLACK);
	print("- test invalid opcode exception", MAX_BUFF);
	printColor("\n    -exit                  ", MAX_BUFF, ORANGE, BLACK);
	print("- exit the OS", MAX_BUFF);
	printColor("\n    -pognis                ", MAX_BUFF, ORANGE, BLACK);
	print("- start the game\n", MAX_BUFF);
	putChar('\n');
}

void command_time(){
	// cuando ss < 10 los imprime sin un cero adelante
	print("\nThe local time [hour:min:sec] is: ", 10);
	getTime();
	putChar('\n');
}

void command_date() {
	print("\nThe UTC date [month/day/year] is: ", 10);
	getDate();
	putChar('\n');
}

void getUserName(){
	putChar('\n');;
	leftLine();
	print(username, 12);
	putChar('\n');
}

void command_registers(){
	print("\nRegisters:\n", 20);
	printRegisters();
	// print(" \n", 30); //si no lo hacemos así, se imprime cualquier cosa al final de los registros
	putChar('\n');
}

void invalid_command(){
	putChar('\n');
	leftLine();
	print("Invalid command. Write -help to see the list of commands\n", 20);
}
	
void command_exit() {
	print("\nSee you next time!\n", 20);
	using = 0;
}