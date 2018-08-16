
#ifndef _APP_PCT2075_H_
#define _APP_PCT2075_H_

#include "stdint.h"


extern uint8_t pct2075_i2c_init(void);
extern uint8_t pct2075_i2c_read(float *data);

#endif
