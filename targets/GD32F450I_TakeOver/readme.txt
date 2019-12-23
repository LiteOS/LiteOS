/*!
    \file  readme.txt
    \brief description of USART printf example
*/

/*
    Copyright (C) 2016 GigaDevice

    2016-10-19, V1.0.0, demo for GD32F450I
*/

  This demo is based on the GD32450I-EVAL board, it shows how to use the key control
the LED and how to retarget the C library printf function to the USART. If the tamper
key is detected pressed, LED1 will be turned on, and the EVAL_COM1 will print "USART 
printf example", otherwise LED1 will be turned off.

  JP5 must be fitted.
