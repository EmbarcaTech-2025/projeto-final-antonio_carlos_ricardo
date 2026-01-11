#ifndef LOOP_PRINTF_H
#define LOOP_PRINTF_H

#include "code_config.h"


#ifdef ENABLE_LOOP_PRINTF
  #define loop_printf(...) printf(__VA_ARGS__)
#else
  #define loop_printf(...) ((void)0)
#endif


#endif // LOOP_PRINTF_H


