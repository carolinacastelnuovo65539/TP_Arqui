#ifndef SHELL_H
#define SHELL_H

void start();

void terminal();

void read();

void checkCommand(char * cmd);

void command_help();

void printHelp();

void command_time();

void changeUsername();

void command_registers();

void command_get_username();

void command_exit();

void command_increase();

void command_reduce();

void command_clear();

void command_beep();

void command_zerodiv();

void command_invopcode();

void command_pongis();

void command_date();

void invalid_command();

#endif