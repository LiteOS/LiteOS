
#if LOSCFG_PLATFORM_HWI	//如果采用接管中断，才会编辑该C文件代码

#include <stdbool.h>
#include <string.h>
#include "dw_usart.h"

#include "pin.h"
#include "pinmux.h"

#include <los_typedef.h>
#include <los_hwi.h>
#include <los_task.h>
#include <los_sem.h>
#include "soc.h"		//消除警告（宏展开问题）：  note: in expansion of macro 'CSKY_UART0_BASE'
#include "osport.h"//#include "osport.h"		// warning: implicit declaration of function 'task_sleepms'


//#define ASYNC_TX

#define RING_SIZE       256

static dw_usart_reg_t * usart_base [3] = 
{
    (dw_usart_reg_t *)CSKY_UART0_BASE,
    (dw_usart_reg_t *)CSKY_UART1_BASE,
    (dw_usart_reg_t *)CSKY_UART2_BASE
};

static int dw_irqs [3] = 
{
    UART0_IRQn,
    UART1_IRQn,
    UART2_IRQn
};

struct ring
{
    unsigned char buff[RING_SIZE];                   // ring buff
    unsigned char mask;
    unsigned char head;
    unsigned char tail;
};

struct uart_dev
{
    struct ring rxring;
    struct ring txring;
    UINT32 rxsem;
    UINT32 txsem;
};

static struct uart_dev * uart_devs [3] = {NULL, NULL, NULL};

#define CN_PPP_UART     0
#define UART_CHAN       2


/*
 * setting config may be accessed when the USART is not
 * busy(USR[0]=0) and the DLAB bit(LCR[7]) is set.
 */
//消除警告：warning: return type defaults to 'int'
static inline void wait_usart_idle (int channel)
{
    int32_t timecount = 0;

    while ((usart_base[channel]->USR & USR_UART_BUSY) && (timecount < UART_BUSY_TIMEOUT))
    {
        timecount++;
    }

    if (timecount >= UART_BUSY_TIMEOUT) {
        return;// -1;
    }
	//return 0;//（新增）消除警告：返回类型默认值为“int”[ Wimplicit int ]
}

static bool ring_empty (struct ring * ring)
{
    return ring->tail == ring->head;
}

static bool ring_full (struct ring * ring)
{
    return ring->tail == ((ring->head + 1) & ring->mask);
}

static int ring_len(struct ring * ring)
{   
    int ret = 0;
    if(ring->head >= ring->tail)
    {
        ret = ring->head -ring->tail;
    }
    else
    {
        ret = ring->head + RING_SIZE -ring->tail;
    }
    return ret;
}

static unsigned char ring_get (struct ring * ring)
{
    unsigned char ch = ring->buff [ring->tail];

    ring->tail = (ring->tail + 1) & ring->mask;

    return ch;
}

static void ring_put (struct ring * ring, unsigned char ch)
{
    if (ring_full (ring))
    {
        ring->tail = (ring->tail + 1) & ring->mask;
    }

    ring->buff [ring->head] = ch;
    ring->head = (ring->head + 1) & ring->mask;
}

int uart_read(unsigned char *buf,int len,int timeout)
{   
    int ret = 0;
    int readleft = 0;
    unsigned char     ch;
    uintptr_t         key;
    struct uart_dev * dev = uart_devs[CN_PPP_UART];

    do{
        ret = ring_len(&dev->rxring);
        task_sleepms(1);	
    }while((ret < len)&&(timeout-- > 0));
    readleft = ret;
    key = LOS_IntLock ();       /* for SMP, spinlock is needed */
    while(readleft > 0)
    {
        ch  = ring_get (&dev->rxring);
        *buf = ch;
        buf++;
        readleft--;
    }
    LOS_IntRestore (key);
    return ret;
}
extern void usart_putc (int channel, unsigned char ch);//消除警告：

int uart_write(unsigned char *buf,int len,int timeout)
{
	int ret=0;
	ret = len;
    while(len > 0)
    {
        usart_putc(CN_PPP_UART,*buf);
        len--;
        buf++;
    }
    return ret;
}
 
unsigned char usart_getc (int channel)
{
    uintptr_t         key;
    unsigned char     ch;
    struct uart_dev * dev = uart_devs[channel];

    LOS_SemPend (dev->rxsem, LOS_WAIT_FOREVER);
    key = LOS_IntLock ();       /* for SMP, spinlock is needed */
    ch  = ring_get (&dev->rxring);
    LOS_IntRestore (key);

    return ch;
}

#ifndef ASYNC_TX
void usart_putc (int channel, unsigned char ch)
{
    //uintptr_t key;
    //key = LOS_IntLock ();       
	//LOS_IntRestore (key);

    
    while ((!(usart_base[channel]->LSR & DW_LSR_TRANS_EMPTY)));

    usart_base[channel]->THR = ch;
    

   // return 0;
   return;//消除警告

}
#else
void usart_putc (int channel, unsigned char ch)
{
    bool              empty;
    uintptr_t         key;
    struct uart_dev * dev = uart_devs[channel];
    
    if (channel > 2)
    {
        return;
    }
    
    if (dev == 0)
    {
        return;
    }

    LOS_SemPend (dev->txsem, LOS_WAIT_FOREVER);

    key = LOS_IntLock ();       /* for SMP, spinlock is needed */

    empty = ring_empty (&dev->txring);

    ring_put (&dev->txring, ch);

    if (empty)
    {
        usart_base[channel]->IER |= IER_THRE_INT_ENABLE;
    }

    LOS_IntRestore (key);
}
#endif

void usart_puts (int channel, unsigned char * str)
{
    unsigned char ch;

    while ((ch = *str++) != '\0')
    {
        usart_putc (channel, ch);

        if (ch == '\n')
        {
            usart_putc (channel, '\r');
        }
    }
}

int32_t usart_set_baud(int channel, uint32_t baud)
{
    uintptr_t flags;

    /*
     * baudrate = (seriak clock freq) / (16 * divisor);
     * divisor  = (seriak clock freq) / (16 * baudrate);
     *          = ((seriak clock freq) / baudrate) >> 4
     */

    uint32_t divisor = ((LSP_DEFAULT_FREQ  * 10) / baud) >> 4;

    divisor = (divisor + 5) / 10;

    wait_usart_idle(channel);
    
    flags = LOS_IntLock ();

    usart_base[channel]->LCR |= LCR_SET_DLAB;

    /* DLL and DLH is lower 8-bits and higher 8-bits of divisor.*/

    usart_base[channel]->DLL = divisor & 0xff;
    usart_base[channel]->DLH = (divisor >> 8) & 0xff;

    /*
     * The DLAB must be cleared after the baudrate is setted
     * to access other registers.
     */

    usart_base[channel]->LCR &= ~LCR_SET_DLAB;
    
    LOS_IntRestore (flags);

    wait_usart_idle(channel);
    usart_base[channel]->LCR &= ~LCR_PARITY_ENABLE;
    wait_usart_idle(channel);
    usart_base[channel]->LCR &= LCR_STOP_BIT1;
    wait_usart_idle(channel);
    usart_base[channel]->LCR |= LCR_WORD_SIZE_8;

    return 0;
}

void usart_tx(int channel)
{
    int i;
    struct uart_dev * dev = uart_devs[channel];
    
    /* fifo enabled and interrupt on fifo empty */

    for (i = 0; i < UART_MAX_FIFO; i++)
    {
        if (ring_empty (&dev->txring))
        {
            usart_base[channel]->IER &= ~IER_THRE_INT_ENABLE;
            return;
        }
        
        usart_base[channel]->THR = ring_get (&dev->txring);
        LOS_SemPost (dev->txsem);
    }
}

static void usart_rx(int channel)
{
    struct uart_dev * dev = uart_devs[channel];
    bool full = ring_full (&dev->rxring);

    ring_put (&dev->rxring, usart_base[channel]->RBR);

    if (!full)
    {
        LOS_SemPost (dev->rxsem);
    }
}

#define UNUSED(x) (void)x 

static void usart_recv_line(int channel)
{
    uint32_t lsr_stat = usart_base[channel]->LSR;
	UNUSED(lsr_stat);// 消除警告：warning: unused variable 'lsr_stat'
    while (usart_base[channel]->LSR & 0x1) {
        usart_rx (channel);
    }
}

/**
  \brief       the interrupt service function.
  \param[in]   index of usart instance.
*/
void ck_usart_irqhandler(int channel)
{
    uint8_t intr_state = usart_base[channel]->IIR & 0xf;

    switch (intr_state) {
        case DW_IIR_THR_EMPTY:       /* interrupt source:transmitter holding register empty */
            usart_tx(channel);
            break;

        case DW_IIR_RECV_DATA:       /* interrupt source:receiver data available or receiver fifo trigger level reached */
            usart_rx(channel);
            break;

        case DW_IIR_RECV_LINE:
            usart_recv_line(channel);
            break;

        case DW_IIR_CHAR_TIMEOUT:
            usart_rx(channel);
            break;

        default:
            break;
    }
}

int usart_init(int channel)
{
    UINT32 ret;
    struct uart_dev * dev;
    
//    static txpins[3] = {PA10, PA17, PA24};
//    static rxpins[3] = {PA11, PA16, PA25};
//    static txpmux[3] = {PA10_UART0_TX, PA17_UART1_TX, PA24_UART2_TX};
//    static rxpmux[3] = {PA11_UART0_RX, PA16_UART1_RX, PA25_UART2_RX};
	static UINT32 txpins[3] = {PA10, PA17, PA24};
    static UINT32 rxpins[3] = {PA11, PA16, PA25};
    static UINT32 txpmux[3] = {PA10_UART0_TX, PA17_UART1_TX, PA24_UART2_TX};
    static UINT32 rxpmux[3] = {PA11_UART0_RX, PA16_UART1_RX, PA25_UART2_RX};
 
    drv_pinmux_config (txpins[channel], txpmux[channel]);
    drv_pinmux_config (rxpins[channel], rxpmux[channel]);
    
    if (uart_devs[channel] != NULL)
    {
        return LOS_OK;
    }
    
    dev = (struct uart_dev *) malloc (sizeof (struct uart_dev));

    
    if (dev == NULL)
    {
        return LOS_NOK;
    }
	memset(dev,0,sizeof(struct uart_dev));
    dev->rxring.mask = RING_SIZE - 1;
    dev->txring.mask = RING_SIZE - 1;

    ret = LOS_SemCreate (0, &dev->rxsem);

    if (ret != LOS_OK)
    {
        return LOS_NOK;
    }

    ret = LOS_SemCreate (RING_SIZE, &dev->txsem);

    if (ret != LOS_OK)
    {
        LOS_SemDelete (dev->rxsem);
        return LOS_NOK;
    }

    /* FIFO enable: when tx fifo have two, rx one */
    usart_base[channel]->FCR = DW_FCR_FIFOE | DW_FCR_TET_FIFO_EMPTY | 
                               DW_FCR_RT_FIFO_SINGLE | DW_FCR_RFIFOR | DW_FCR_XFIFOR;
     
    usart_base[channel]->IER = IER_RDA_INT_ENABLE | IIR_RECV_LINE_ENABLE;
    
    #if 1
    while (usart_base[channel]->LSR & 1)
        (void) usart_base[channel]->RBR;
    #endif
	usart_set_baud(channel, 115200);

    
    LOS_HwiCreate (dw_irqs[channel], 0, 0, (void (*) (uintptr_t))ck_usart_irqhandler, channel);

    csi_vic_enable_irq(dw_irqs[channel]);

    
    uart_devs[channel] = dev;

    return LOS_OK;
}


int fputc (int ch, FILE *stream)
{
    usart_putc (UART_CHAN, ch);
    return 1;
}
int fgetc(FILE *stream)
{
	
	int ret;
	ret = usart_getc(UART_CHAN);
	return ret;
}

#endif




