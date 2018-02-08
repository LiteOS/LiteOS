/*!
    \file  gd32f1x0_it.h
    \brief the header file of the ISR
*/

/*
    Copyright (C) 2016 GigaDevice

    2016-01-15, V1.0.0, demo for GD32F1x0(x=7,9)
    2016-05-13, V2.0.0, demo for GD32F1x0(x=7,9)
*/

#ifndef GD32F1X0_IT_H
#define GD32F1X0_IT_H

#include "gd32f1x0.h"

/* function declarations */
/* NMI handle function */
void NMI_Handler(void);
/* HardFault handle function */
void HardFault_Handler(void);
/* MemManage handle function */
void MemManage_Handler(void);
/* BusFault handle function */
void BusFault_Handler(void);
/* UsageFault handle function */
void UsageFault_Handler(void);
/* SVC handle function */
void SVC_Handler(void);
/* DebugMon handle function */
void DebugMon_Handler(void);
/* PendSV handle function */
void PendSV_Handler(void);
/* SysTick handle function */
void SysTick_Handler(void);

#endif /* GD32F1X0_IT_H */
