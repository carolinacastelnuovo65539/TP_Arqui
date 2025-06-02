#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

uint8_t rtc_bin(uint8_t value);


uint64_t get_seconds();
uint64_t get_minutes();
uint64_t get_hours();

uint64_t get_day();
uint64_t get_month();
uint64_t get_year();

void sound(uint64_t frecuencia);
void stop_sound();

#endif