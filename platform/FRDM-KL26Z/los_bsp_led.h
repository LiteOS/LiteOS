#ifndef _LOS_BSP_LED_H
#define _LOS_BSP_LED_H


#ifdef FRDM_KL25Z
    #define SIM_SCGC5_LED1_MASK  SIM_SCGC5_PORTD_MASK
    #define SIM_SCGC5_LED2_MASK  SIM_SCGC5_PORTB_MASK
    #define SIM_SCGC5_LED3_MASK  SIM_SCGC5_PORTB_MASK

    #define LED1_PORT_PCR      PORTD_PCR1
    #define LED2_PORT_PCR      PORTB_PCR18
    #define LED3_PORT_PCR      PORTB_PCR19

    #define LED1_GPIO_PDOR     GPIOD_PDOR
    #define LED2_GPIO_PDOR     GPIOB_PDOR
    #define LED3_GPIO_PDOR     GPIOB_PDOR

    #define LED1_GPIO_PDDR     GPIOD_PDDR
    #define LED2_GPIO_PDDR     GPIOB_PDDR
    #define LED3_GPIO_PDDR     GPIOB_PDDR


    #define LED1_PIN   1
    #define LED2_PIN   18
    #define LED3_PIN   19
#endif

#ifdef FRDM_KL26Z
    #define SIM_SCGC5_LED1_MASK  SIM_SCGC5_PORTD_MASK
    #define SIM_SCGC5_LED2_MASK  SIM_SCGC5_PORTE_MASK
    #define SIM_SCGC5_LED3_MASK  SIM_SCGC5_PORTE_MASK

    #define LED1_PORT_PCR      PORTD_PCR5
    #define LED2_PORT_PCR      PORTE_PCR29
    #define LED3_PORT_PCR      PORTE_PCR31

    #define LED1_GPIO_PDOR     GPIOD_PDOR
    #define LED2_GPIO_PDOR     GPIOE_PDOR
    #define LED3_GPIO_PDOR     GPIOE_PDOR

    #define LED1_GPIO_PDDR     GPIOD_PDDR
    #define LED2_GPIO_PDDR     GPIOE_PDDR
    #define LED3_GPIO_PDDR     GPIOE_PDDR


    #define LED1_PIN   5
    #define LED2_PIN   29
    #define LED3_PIN   31
#endif

#define LED1           0
#define LED2           1
#define LED3           2

#define LED_ON      (1)
#define LED_OFF     (0)

#define LOS_LED1    (1)
#define LOS_LED2    (2)
#define LOS_LED3    (3)
#define LOS_LED4    (4)
#define LOS_LEDX    (5)


void LOS_EvbLedInit(void);
void LOS_EvbLedControl(int index, int cmd);
#endif
