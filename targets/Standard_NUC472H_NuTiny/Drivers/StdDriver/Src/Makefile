#
# Makefile for the SuperH specific drivers.
HW_DIR:= ../
INCLUDE:= -I$(HW_DIR)/Inc/
bin:adc.o
	gcc -o bin adc.o
adc.o:adc.c	$(INCLUDE)
	gcc -c adc.c
