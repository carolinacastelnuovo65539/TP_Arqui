#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdint.h>
#include "userlib.h"

// Add function declarations
extern uint64_t exc_zero(void);
extern uint64_t exc_opcode(void);
extern uint64_t _hlt(void);

#endif