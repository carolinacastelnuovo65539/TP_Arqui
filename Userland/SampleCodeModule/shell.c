#include <shell.h>
#include <userlib.h>

#define USER_MAX 32
#define COMMAND_MAX 100

static void leftLine();

int USER_SIZE = 5;

static char username[USER_MAX] = "user";
char command[COMMAND_MAX] = {0};

char * commands[] = {"-help", "-time", "-changeUsername", "-registers", "-username", "-exit"};

int newTerminal = 1;

int using = 1;

void changeUsername(){
	char c = 0;
	int len = 0;
	print("Ingrese su nobre de usuario:", 40);
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
		print("El nombre de usuario no debe superar los 32 caracteres", 50);
		changeUsername();
		return;
	}else{
		username[len] = '\0';
		USER_SIZE = len;
		putChar('\n');
		print("Bienvenido ", 30);
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
}

void start(){
    if(newTerminal){
		newTerminal = 0;
		print("Bienvenido a nuestra terminal!\n", 40);
		print("Escriba '-help' para ver la lista de comandos disponibles.\n", 70);
		
		changeUsername();
		// print("Nombre de usuario: ", 11);
		leftLine();
	}
}

static void leftLine(){
	print(username, USER_SIZE);
	print("~$ ", 5);
}

void terminal(){
	getTime();
	print("\nIngrese un comando: \n", 30);
	while(using){
		leftLine();
		read();
		putChar('\n');
	}
}