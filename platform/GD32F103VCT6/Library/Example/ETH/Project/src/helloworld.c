/**
  ******************************************************************************
  * @file    ETH/Project/src/helloworld.c
  * @author  MCU SD
  * @version V1.0
  * @date    26-Dec-2014
  * @brief   Telnet demo program  
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "helloworld.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>
#include "gd32f10x.h"

/* Private define ------------------------------------------------------------*/
#define GREETING "\n\r======= Gigadevice =======\
                  \n\r== GD32 ==\
                  \n\r== Telnet SUCCESS ==\
                  \n\rHello. What is your name?\r\n"
#define HELLO "\n\rGigaDevice¡¾23¡¿PORT Hello "
#define MAX_NAME_SIZE 32

/* Private variables ---------------------------------------------------------*/
extern const uint8_t GD32F10x_STR[];
struct name 
{
    int length;
    char bytes[MAX_NAME_SIZE];
};

/* Private function prototypes -----------------------------------------------*/
static err_t HelloWorld_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
static err_t HelloWorld_accept(void *arg, struct tcp_pcb *pcb, err_t err);
static void HelloWorld_conn_err(void *arg, err_t err);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Called when a data is received on the telnet connection
  * @param  arg :the user argument
  * @param  pcb :the tcp_pcb that has received the data
  * @param  p   :the packet buffer
  * @param  err :the error value linked with the received data
  * @retval error value
  */
static err_t HelloWorld_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    struct pbuf *q;
    struct name *name = (struct name *)arg;
    int done;
    char *c;
    int i;
 
    /* We perform here any necessary processing on the pbuf */
    if (p != NULL) 
    {        
        /* We call this function to tell the LwIp that we have processed the data */
        /* This lets the stack advertise a larger window, so more data can be received*/
        tcp_recved(pcb, p->tot_len);

        /* Check the name if NULL, no data passed, return withh illegal argument error */
        if(!name) 
        {
            pbuf_free(p);
            return ERR_ARG;
        }

        done = 0;
        for(q=p; q != NULL; q = q->next) 
        {
            c = q->payload;
            for(i=0; i<q->len && !done; i++) 
            {
                done = ((c[i] == '\r') || (c[i] == '\n'));
                if(name->length < MAX_NAME_SIZE) 
                {
                    name->bytes[name->length++] = c[i];
                }
            }
        }
        if(done) 
        {
            if(name->bytes[name->length-2] != '\r' || name->bytes[name->length-1] != '\n') 
            {
                if((name->bytes[name->length-1] == '\r' || name->bytes[name->length-1] == '\n') 
                    && (name->length+1 <= MAX_NAME_SIZE)) 
                {
                    name->length += 1;
                } 
                else if(name->length+2 <= MAX_NAME_SIZE) 
                {
                    name->length += 2;
                } 
                else 
                {
                    name->length = MAX_NAME_SIZE;
                }

                name->bytes[name->length-2] = '\r';
                name->bytes[name->length-1] = '\n';
            }
            tcp_write(pcb, HELLO, strlen(HELLO), 1);

            tcp_write(pcb, name->bytes, name->length, TCP_WRITE_FLAG_COPY);
            printf("\n\rGigadevice\n\rTelnet %s %s", HELLO, name->bytes);
            name->length = 0;
        }
    
        /* End of processing, we free the pbuf */
        pbuf_free(p);
    }  
    else if (err == ERR_OK) 
    {
        /* When the pbuf is NULL and the err is ERR_OK, the remote end 
           is closing the connection. */
        /* We free the allocated memory and we close the connection */
        mem_free(name);
        return tcp_close(pcb);
    }
    return ERR_OK;
}

/**
  * @brief  This function when the Telnet connection is established
  * @param  arg  :user supplied argument 
  * @param  pcb  :the tcp_pcb which accepted the connection
  * @param  err  :error value
  * @retval ERR_OK
  */
static err_t HelloWorld_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{     
    u32_t IPaddress;
    u8_t iptxt[50];
    volatile u8_t iptab[4];
  
    IPaddress = pcb->remote_ip.addr;
    printf("\n\rTelnet HelloWorld_accept:%d.%d.%d.%d  %s", 
                (u8_t)(IPaddress),(u8_t)(IPaddress >> 8),(u8_t)(IPaddress >> 16),
                (u8_t)(IPaddress >> 24),GREETING);
     
    /* Read its IP address */
    iptab[0] = (u8_t)(IPaddress >> 24);
    iptab[1] = (u8_t)(IPaddress >> 16);
    iptab[2] = (u8_t)(IPaddress >> 8);
    iptab[3] = (u8_t)(IPaddress);

    sprintf((char*)iptxt, "Telnet:%d.%d.%d.%d   ", 
                iptab[3], iptab[2], iptab[1], iptab[0]);  
  
    printf("%s\r\n",iptxt);
  
    /* Tell LwIP to associate this structure with this connection. */
    tcp_arg(pcb, mem_calloc(sizeof(struct name), 1)); 
  
    /* Configure LwIP to use our call back functions. */
    tcp_err(pcb, HelloWorld_conn_err);
    tcp_recv(pcb, HelloWorld_recv);
  
    /* Send out the first message */  
    tcp_write(pcb, GD32F10x_STR, strlen(GD32F10x_STR), 1); 
    sprintf((char*)iptxt, "You telnet computer's IP is: %d.%d.%d.%d\n", 
                            iptab[3], iptab[2], iptab[1], iptab[0]);
    tcp_write(pcb, iptxt, strlen(iptxt), 1); 
    tcp_write(pcb, GREETING, strlen(GREETING), 1); 

    return ERR_OK;
}

/**
  * @brief  Initialize the hello application  
  * @param  None 
  * @retval None 
  */
void HelloWorld_init(void)
{
    struct tcp_pcb *pcb;
  
    /* Create a new TCP control block  */
    pcb = tcp_new();

    /* Assign to the new pcb a local IP address and a port number */
    /* Using IP_ADDR_ANY allow the pcb to be used by any local interface */
    tcp_bind(pcb, IP_ADDR_ANY, 23);       

    /* Set the connection to the LISTEN state */
    pcb = tcp_listen(pcb);

    /* Specify the function to be called when a connection is established */	
    tcp_accept(pcb, HelloWorld_accept);   
}

/**
  * @brief  This function is called when an error occurs on the connection 
  * @param  arg
  * @parm   err
  * @retval None 
  */
static void HelloWorld_conn_err(void *arg, err_t err)
{
    struct name *name;
    name = (struct name *)arg;

    mem_free(name);
}

/******************* (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
