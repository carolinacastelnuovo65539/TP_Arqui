#ifndef SHELL_H
#define SHELL_H

void start();

void terminal();

void changeUsername();

void read();

void checkCommand(char * cmd);

void printHelp();

void command_help();

void command_time();

void command_registers();

void getUserName();

// void command_exit();

void invalid_command();

void command_reduce();

void command_clear();

void command_increase();

#endif