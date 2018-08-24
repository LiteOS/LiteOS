#include "define_all.h"  

//cpu滴答定时器配置(软件延时用)
void Init_SysTick(void)
{
    SysTick_Config(0x1000000UL);
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |//关闭中断
                    //SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
}
//调用软件延时前必须先配置SysTick
void TicksDelay(uint32_t ClkNum)
{
    uint32_t last = SysTick->VAL;
    
    ClkNum = ClkNum*clkmode;//适应不同主频，8M时最大定时2ms，32M时最大定时0.5ms
    if(ClkNum>0xF00000)
    {
        ClkNum = 0xF00000;
    }
    while(((last - SysTick->VAL)&0xFFFFFFUL ) < ClkNum);
}
//ms软件延时
void TicksDelayMs(uint32_t ms , ConditionHook Hook)
{
    uint32_t ClkNum;
    
    ClkNum = (__SYSTEM_CLOCK/1000) ;
    for(;ms>0;ms--)
    {
        if(Hook!=NULL)
        {
            if(Hook()) return ;
        }
        TicksDelay(ClkNum);
    }
}
//us软件延时
void TicksDelayUs(uint32_t us)
{
    uint32_t ClkNum;
    
    if(us>100000)//不大于100ms
    {
        us = 100000;
    }
    ClkNum = us*(__SYSTEM_CLOCK/1000000) ;
    TicksDelay(ClkNum);
}

//校验寄存器
unsigned char CheckSysReg( __IO uint32_t *RegAddr, uint32_t Value )
{
	if( *RegAddr != Value ) 
	{
		*RegAddr = Value;
		return 1;
	}
	else
	{
		return 0;
	}
}

//查询NVIC寄存器对应向量号中断是否打开
//1 打开
//0 关闭
unsigned char CheckNvicIrqEn( IRQn_Type IRQn )
{
	if( 0 == ( NVIC->ISER[0U] & ((uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL)))) )
		return 0;
	else
		return 1;
}

//IO模拟功能配置:LCD/ADC
void AnalogIO( GPIOx_Type* GPIOx, uint32_t PinNum )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructureRun;
	
	GPIO_Get_InitPara(GPIOx, PinNum, &GPIO_InitStructureRun);
	
	if( (GPIO_InitStructureRun.Pin		!= PinNum) ||
		(GPIO_InitStructureRun.PxINEN	!= GPIO_IN_Dis) ||
		(GPIO_InitStructureRun.PxODEN	!= GPIO_OD_En) ||
		(GPIO_InitStructureRun.PxPUEN	!= GPIO_PU_Dis) ||
		(GPIO_InitStructureRun.PxFCR	!= GPIO_FCR_ANA) )
	{
		GPIO_InitStructure.Pin = PinNum;
		GPIO_InitStructure.PxINEN = GPIO_IN_Dis;
		GPIO_InitStructure.PxODEN = GPIO_OD_En;
		GPIO_InitStructure.PxPUEN = GPIO_PU_Dis;
		GPIO_InitStructure.PxFCR = GPIO_FCR_ANA;
		
		GPIO_Init(GPIOx, &GPIO_InitStructure);		
	}	
}

//IO输入口配置 
//type 0 = 普通 
//type 1 = 上拉
//#define IN_NORMAL	0
//#define IN_PULLUP	1
void InputtIO( GPIOx_Type* GPIOx, uint32_t PinNum, uint8_t Type )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructureRun;
	
	GPIO_Get_InitPara(GPIOx, PinNum, &GPIO_InitStructureRun);
		
	if( (GPIO_InitStructureRun.Pin		!= PinNum) ||
		(GPIO_InitStructureRun.PxINEN	!= GPIO_IN_En) ||
		(GPIO_InitStructureRun.PxODEN	!= GPIO_OD_En) ||
		((Type == IN_NORMAL)&&(GPIO_InitStructureRun.PxPUEN != GPIO_PU_Dis)) ||
		((Type == IN_PULLUP)&&(GPIO_InitStructureRun.PxPUEN != GPIO_PU_En)) ||
		(GPIO_InitStructureRun.PxFCR	!= GPIO_FCR_IN) )
	{
		GPIO_InitStructure.Pin = PinNum;	
		GPIO_InitStructure.PxINEN = GPIO_IN_En;
		GPIO_InitStructure.PxODEN = GPIO_OD_En;
		if(Type == IN_NORMAL)		GPIO_InitStructure.PxPUEN = GPIO_PU_Dis;
		else						GPIO_InitStructure.PxPUEN = GPIO_PU_En;	
		GPIO_InitStructure.PxFCR = GPIO_FCR_IN;
		
		GPIO_Init(GPIOx, &GPIO_InitStructure);	
	}
}

//IO输出口配置 
//type 0 = 普通 
//type 1 = OD
//#define OUT_PUSHPULL	0
//#define OUT_OPENDRAIN	1
void OutputIO( GPIOx_Type* GPIOx, uint32_t PinNum, uint8_t Type )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructureRun;
	
	GPIO_Get_InitPara(GPIOx, PinNum, &GPIO_InitStructureRun);
	
	if( (GPIO_InitStructureRun.Pin		!= PinNum) ||
		(GPIO_InitStructureRun.PxINEN	!= GPIO_IN_Dis) ||
		((Type == OUT_PUSHPULL)&&(GPIO_InitStructureRun.PxODEN	!= GPIO_OD_Dis)) ||
		((Type == OUT_OPENDRAIN)&&(GPIO_InitStructureRun.PxODEN	!= GPIO_OD_En)) ||
		(GPIO_InitStructureRun.PxPUEN	!= GPIO_PU_Dis) ||
		(GPIO_InitStructureRun.PxFCR	!= GPIO_FCR_OUT) )
	{
		GPIO_InitStructure.Pin = PinNum;
		GPIO_InitStructure.PxINEN = GPIO_IN_Dis;
		if(Type == OUT_PUSHPULL)	GPIO_InitStructure.PxODEN = GPIO_OD_Dis;
		else						GPIO_InitStructure.PxODEN = GPIO_OD_En;
		GPIO_InitStructure.PxPUEN = GPIO_PU_Dis;
		GPIO_InitStructure.PxFCR = GPIO_FCR_OUT;
		 
		GPIO_Init(GPIOx, &GPIO_InitStructure);		
	}
}
//IO数字特殊功能口 
//type 0 = 普通 
//type 1 = OD (OD功能仅部分特殊功能支持)
//type 2 = 普通+上拉 
//type 3 = OD+上拉
//#define ALTFUN_NORMAL				0
//#define ALTFUN_OPENDRAIN			1
//#define ALTFUN_PULLUP				2
//#define ALTFUN_OPENDRAIN_PULLUP	3
void AltFunIO( GPIOx_Type* GPIOx, uint32_t PinNum, uint8_t Type  )
{																
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructureRun;
	
	GPIO_Get_InitPara(GPIOx, PinNum, &GPIO_InitStructureRun);
	
	if( (GPIO_InitStructureRun.Pin		!= PinNum) ||
		(GPIO_InitStructureRun.PxINEN	!= GPIO_IN_Dis) ||
		(((Type & 0x01) == 0)&&(GPIO_InitStructureRun.PxODEN	!= GPIO_OD_Dis)) ||
		(((Type & 0x01) != 0)&&(GPIO_InitStructureRun.PxODEN	!= GPIO_OD_En)) ||
		(((Type & 0x02) == 0)&&(GPIO_InitStructureRun.PxPUEN	!= GPIO_PU_Dis)) ||
		(((Type & 0x02) != 0)&&(GPIO_InitStructureRun.PxPUEN	!= GPIO_PU_En)) ||
		(GPIO_InitStructureRun.PxFCR	!= GPIO_FCR_DIG) )
	{
		GPIO_InitStructure.Pin = PinNum;
		GPIO_InitStructure.PxINEN = GPIO_IN_Dis;
		if( (Type & 0x01) == 0 )	GPIO_InitStructure.PxODEN = GPIO_OD_Dis;
		else						GPIO_InitStructure.PxODEN = GPIO_OD_En;
		if( (Type & 0x02) == 0 )	GPIO_InitStructure.PxPUEN = GPIO_PU_Dis;
		else						GPIO_InitStructure.PxPUEN = GPIO_PU_En;	
		GPIO_InitStructure.PxFCR = GPIO_FCR_DIG;
		 
		GPIO_Init(GPIOx, &GPIO_InitStructure);		
	}
}

//IO关闭（od输出高）
//当输入使能有效时，如果外部信号浮空，也可能导致FM385管脚漏电；
//可以将FCR配置为01（GPIO输出），ODEN配置为1，即伪开漏输出，关闭上拉使能，并将输出数据设为1
//注意SWD接口的PG8,9如果程序改变它们的配置将无法仿真
void CloseeIO( GPIOx_Type* GPIOx, uint32_t PinNum )
{
	GPIO_InitTypeDef  GPIO_InitStructureRun;
	
	GPIO_Get_InitPara(GPIOx, PinNum, &GPIO_InitStructureRun);
		
	if((GPIO_InitStructureRun.PxFCR	!= GPIO_FCR_OUT))
	{
		GPIO_SetBits(GPIOx, PinNum);
		OutputIO( GPIOx, PinNum, OUT_OPENDRAIN );
	}
	else
	{
		OutputIO( GPIOx, PinNum, OUT_OPENDRAIN );
		GPIO_SetBits(GPIOx, PinNum);		
	}
}

//IO口初始输出状态配置
void Init_Pad_Io(void)
{	
	GPIOx_DO_Write(GPIOA, 0x0000);	//
	GPIOx_DO_Write(GPIOB, 0x0000);	//
	GPIOx_DO_Write(GPIOC, 0x0000);	//
	GPIOx_DO_Write(GPIOD, 0x0000);	//
	GPIOx_DO_Write(GPIOE, 0x0000);	//
	GPIOx_DO_Write(GPIOF, 0x0000);	//
	GPIOx_DO_Write(GPIOG, 0x0000);	//
}

//关闭80脚芯片不可使用的IO
void Close_None_GPIO_80pin(void)
{
	CloseeIO( GPIOC, GPIO_Pin_0 );
	CloseeIO( GPIOC, GPIO_Pin_1 );

	CloseeIO( GPIOD, GPIO_Pin_8);
	CloseeIO( GPIOD, GPIO_Pin_9 );
	CloseeIO( GPIOD, GPIO_Pin_10 );	
	CloseeIO( GPIOD, GPIO_Pin_11 );	
	CloseeIO( GPIOD, GPIO_Pin_12 );	
	CloseeIO( GPIOD, GPIO_Pin_13 );	
	CloseeIO( GPIOD, GPIO_Pin_14 );	
	CloseeIO( GPIOD, GPIO_Pin_15 );	

	CloseeIO( GPIOE, GPIO_Pin_0 );
	CloseeIO( GPIOE, GPIO_Pin_1 );
	CloseeIO( GPIOE, GPIO_Pin_5 );
	CloseeIO( GPIOE, GPIO_Pin_6 );
	CloseeIO( GPIOE, GPIO_Pin_7 );
	CloseeIO( GPIOE, GPIO_Pin_8 );
	CloseeIO( GPIOE, GPIO_Pin_9 );
	CloseeIO( GPIOE, GPIO_Pin_10 );
	CloseeIO( GPIOE, GPIO_Pin_11 );
	CloseeIO( GPIOE, GPIO_Pin_12 );
	CloseeIO( GPIOE, GPIO_Pin_13 );
	CloseeIO( GPIOE, GPIO_Pin_14 );
	CloseeIO( GPIOE, GPIO_Pin_15 );

	CloseeIO( GPIOF, GPIO_Pin_0 );
	CloseeIO( GPIOF, GPIO_Pin_1 );
	CloseeIO( GPIOF, GPIO_Pin_2 );
	CloseeIO( GPIOF, GPIO_Pin_7 );
	CloseeIO( GPIOF, GPIO_Pin_8 );
	CloseeIO( GPIOF, GPIO_Pin_9 );
	CloseeIO( GPIOF, GPIO_Pin_10 );

	CloseeIO( GPIOG, GPIO_Pin_0 );
	CloseeIO( GPIOG, GPIO_Pin_1 );
	CloseeIO( GPIOG, GPIO_Pin_4 );
	CloseeIO( GPIOG, GPIO_Pin_5 );
	CloseeIO( GPIOG, GPIO_Pin_10 );
	CloseeIO( GPIOG, GPIO_Pin_11 );
	CloseeIO( GPIOG, GPIO_Pin_12 );
	CloseeIO( GPIOG, GPIO_Pin_13 );
	CloseeIO( GPIOG, GPIO_Pin_14 );
	CloseeIO( GPIOG, GPIO_Pin_15 );
}

void Close_AllIOEXTI(void)
{
	CheckSysReg( &GPIO->EXTI0_SEL	, 0xFFFF0000 );
	CheckSysReg( &GPIO->EXTI1_SEL	, 0xFFFF0000 );
	CheckSysReg( &GPIO->EXTI2_SEL	, 0xFFFF0000 );
}

void IO_AnalogFunSet(void)
{
	/* PE4模拟功能选择 */
	GPIO_ANASEL_PE4ANS_Set(GPIO_ANASEL_PE4ANS_ACMP1_INP1);

	/* PE3模拟功能选择 */
	GPIO_ANASEL_PE3ANS_Set(GPIO_ANASEL_PE3ANS_ACMP1_INN1);

	/* PC15模拟功能选择 */
	GPIO_ANASEL_PC15ANS_Set(GPIO_ANASEL_PC15ANS_ACMP1_INP0);

	/* PC14模拟功能选择 */
	GPIO_ANASEL_PC14ANS_Set(GPIO_ANASEL_PC14ANS_ACMP1_INN0);

	/* PC13模拟功能选择 */
	GPIO_ANASEL_PC13ANS_Set(GPIO_ANASEL_PC13ANS_ADC_IN2);

	/* PC12模拟功能选择 */
	GPIO_ANASEL_PC12ANS_Set(GPIO_ANASEL_PC12ANS_ADC_IN1);
}

void IO_DFFunSet(void)
{
	GPIO_IODF_SetableEx(IODF_PF3	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PF2	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PF1	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PE9	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PE8	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PE7	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PE6	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PE2	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PD3	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PD2	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PD1	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PD0	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PC15	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PC14	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PC13	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PC12	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PB7	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PB6	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PB5	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PB4	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PA11	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PA10	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PA9	, DISABLE);
	GPIO_IODF_SetableEx(IODF_PA8	, DISABLE);
}

void IO_WKENFunSet(void)
{
	GPIO_PINWKEN_SetableEx(PINWKEN_PD6	, DISABLE);
	GPIO_PINWKEN_SetableEx(PINWKEN_PE9	, DISABLE);
	GPIO_PINWKEN_SetableEx(PINWKEN_PE2	, DISABLE);
	GPIO_PINWKEN_SetableEx(PINWKEN_PA13	, DISABLE);
	GPIO_PINWKEN_SetableEx(PINWKEN_PG7	, DISABLE);
	GPIO_PINWKEN_SetableEx(PINWKEN_PC13	, DISABLE);
	GPIO_PINWKEN_SetableEx(PINWKEN_PB0	, DISABLE);
	GPIO_PINWKEN_SetableEx(PINWKEN_PF5	, DISABLE);
}

//关闭80脚芯片可使用的全部IO(SWD口除外，关闭后不能仿真)
void Close_AllIO_GPIO_80pin( void )
{	
	//关闭所有IO的中断功能
	Close_AllIOEXTI();
	
	//强驱动关闭
	GPIO_HDSEL_PE2HDEN_Setable(DISABLE);
	GPIO_HDSEL_PG6HDEN_Setable(DISABLE);
	
	//FOUT输出信号选择
	GPIO_FOUTSEL_FOUTSEL_Set(GPIO_FOUTSEL_FOUTSEL_LSCLK);
	
	//模拟口液晶、AD功能选择	
	IO_AnalogFunSet();
	
	//部分IO输入口数字滤波功能
	IO_DFFunSet();
	
	//关闭NWKUP
	IO_WKENFunSet();
	
	CloseeIO( GPIOA, GPIO_Pin_0 );	//PA0; //
	CloseeIO( GPIOA, GPIO_Pin_1 );	//PA1; //
	CloseeIO( GPIOA, GPIO_Pin_2 );	//PA2; //
	CloseeIO( GPIOA, GPIO_Pin_3 );	//PA3; //
	CloseeIO( GPIOA, GPIO_Pin_4 );	//PA4; //
	CloseeIO( GPIOA, GPIO_Pin_5 );	//PA5; //
	CloseeIO( GPIOA, GPIO_Pin_6 );	//PA6; //
	CloseeIO( GPIOA, GPIO_Pin_7 );	//PA7; //
	CloseeIO( GPIOA, GPIO_Pin_8 );	//PA8; //
	CloseeIO( GPIOA, GPIO_Pin_9 );	//PA9; //
	CloseeIO( GPIOA, GPIO_Pin_10 );	//PA10;//
	CloseeIO( GPIOA, GPIO_Pin_11 );	//PA11;//
	CloseeIO( GPIOA, GPIO_Pin_12 );	//PA12;//
	CloseeIO( GPIOA, GPIO_Pin_13 );	//PA13;//
	CloseeIO( GPIOA, GPIO_Pin_14 );	//PA14;//
	CloseeIO( GPIOA, GPIO_Pin_15 );	//PA15;//

	CloseeIO( GPIOB, GPIO_Pin_0 );	//PB0; //
	CloseeIO( GPIOB, GPIO_Pin_1 );	//PB1; //
	CloseeIO( GPIOB, GPIO_Pin_2 );	//PB2; //
	CloseeIO( GPIOB, GPIO_Pin_3 );	//PB3; //
	CloseeIO( GPIOB, GPIO_Pin_4 );	//PB4; //
	CloseeIO( GPIOB, GPIO_Pin_5 );	//PB5; //
	CloseeIO( GPIOB, GPIO_Pin_6 );	//PB6; //
	CloseeIO( GPIOB, GPIO_Pin_7 );	//PB7; //
	CloseeIO( GPIOB, GPIO_Pin_8 );	//PB8; //
	CloseeIO( GPIOB, GPIO_Pin_9 );	//PB9; //
	CloseeIO( GPIOB, GPIO_Pin_10 );	//PB10;//
	CloseeIO( GPIOB, GPIO_Pin_11 );	//PB11;//
	CloseeIO( GPIOB, GPIO_Pin_12 );	//PB12;//
	CloseeIO( GPIOB, GPIO_Pin_13 );	//PB13;//
	CloseeIO( GPIOB, GPIO_Pin_14 );	//PB14;//
	CloseeIO( GPIOB, GPIO_Pin_15 );	//PB15;//
	
	CloseeIO( GPIOC, GPIO_Pin_2 );	//PC2; //
	CloseeIO( GPIOC, GPIO_Pin_3 );	//PC3; //
	CloseeIO( GPIOC, GPIO_Pin_4 );	//PC4; //
	CloseeIO( GPIOC, GPIO_Pin_5 );	//PC5; //
	CloseeIO( GPIOC, GPIO_Pin_6 );	//PC6; //
	CloseeIO( GPIOC, GPIO_Pin_7 );	//PC7; //
	CloseeIO( GPIOC, GPIO_Pin_8 );	//PC8; //
	CloseeIO( GPIOC, GPIO_Pin_9 );	//PC9; //
	CloseeIO( GPIOC, GPIO_Pin_10 );	//PC10;//
	CloseeIO( GPIOC, GPIO_Pin_11 );	//PC11;//
	CloseeIO( GPIOC, GPIO_Pin_12 );	//PC12;//
	CloseeIO( GPIOC, GPIO_Pin_13 );	//PC13;//
	CloseeIO( GPIOC, GPIO_Pin_14 );	//PC14;//
	CloseeIO( GPIOC, GPIO_Pin_15 );	//PC15;//
	
	CloseeIO( GPIOD, GPIO_Pin_0 );	//PD0;//
	CloseeIO( GPIOD, GPIO_Pin_1 );	//PD1;//
	CloseeIO( GPIOD, GPIO_Pin_2 );	//PD2;//
	CloseeIO( GPIOD, GPIO_Pin_3 );	//PD3;//
	CloseeIO( GPIOD, GPIO_Pin_4 );	//PD4;//
	CloseeIO( GPIOD, GPIO_Pin_5 );	//PD5;//
	CloseeIO( GPIOD, GPIO_Pin_6 );	//PD6;//
	CloseeIO( GPIOD, GPIO_Pin_7 );	//PD7;//
	
	CloseeIO( GPIOE, GPIO_Pin_2 );	//PE2;//
	CloseeIO( GPIOE, GPIO_Pin_3 );	//PE3;//
	CloseeIO( GPIOE, GPIO_Pin_4 );	//PE4;//

	CloseeIO( GPIOF, GPIO_Pin_3 );	//PF3; //
	CloseeIO( GPIOF, GPIO_Pin_4 );	//PF4; //
	CloseeIO( GPIOF, GPIO_Pin_5 );	//PF5; //
	CloseeIO( GPIOF, GPIO_Pin_6 );	//PF6; //
	CloseeIO( GPIOF, GPIO_Pin_11 );	//PF11;//
	CloseeIO( GPIOF, GPIO_Pin_12 );	//PF12;//
	CloseeIO( GPIOF, GPIO_Pin_13 );	//PF13;//
	CloseeIO( GPIOF, GPIO_Pin_14 );	//PF14;//
	CloseeIO( GPIOF, GPIO_Pin_15 );	//PF15;//
	
	CloseeIO( GPIOG, GPIO_Pin_2 );	//PG2;//
	CloseeIO( GPIOG, GPIO_Pin_3 );	//PG3;//
	CloseeIO( GPIOG, GPIO_Pin_6 );	//PG6;//
	CloseeIO( GPIOG, GPIO_Pin_7 );	//PG7;//
	//注意SWD接口的PG8,9如果程序改变它们的配置将无法仿真
	AltFunIO( GPIOG, GPIO_Pin_8, ALTFUN_NORMAL );	//PG8;//SWDTCK
	AltFunIO( GPIOG, GPIO_Pin_9, ALTFUN_NORMAL );	//PG9;//SWDTDO	
}

//默认开启大部分外设时钟，用户可按需求关闭不需要的时钟
//时钟开启关闭对功耗影响不大
void Init_RCC_PERIPH_clk(void)
{
	//PERCLKCON1
	RCC_PERCLK_SetableEx( DCUCLK, 		ENABLE );		//debug contro时钟使能，建议打开
	RCC_PERCLK_SetableEx( EXTI2CLK, 	ENABLE );		//EXTI外部引脚中断采样时钟，IO数字滤波时钟使能
	RCC_PERCLK_SetableEx( EXTI1CLK, 	ENABLE );		//EXTI外部引脚中断采样时钟，IO数字滤波时钟使能
	RCC_PERCLK_SetableEx( EXTI0CLK, 	ENABLE );		//EXTI外部引脚中断采样时钟，IO数字滤波时钟使能
	RCC_PERCLK_SetableEx( PDCCLK, 		ENABLE );		//IO控制时钟寄存器使能
	RCC_PERCLK_SetableEx( ANACCLK, 		ENABLE );		//模拟电路总线时钟使能
	RCC_PERCLK_SetableEx( IWDTCLK, 		ENABLE );		//IWDT总线时钟使能
	RCC_PERCLK_SetableEx( SCUCLK, 		ENABLE );		//system control时钟使能，建议打开
	RCC_PERCLK_SetableEx( PMUCLK, 		ENABLE );		//电源管理模块时钟使能
	RCC_PERCLK_SetableEx( RTCCLK, 		ENABLE );		//RTC总线时钟使能
	RCC_PERCLK_SetableEx( LPTFCLK, 		ENABLE );		//LPTIM功能时钟使能
	RCC_PERCLK_SetableEx( LPTRCLK, 		ENABLE );		//LPTIM总线时钟使能
				
    //PERCLKCON2 SETTING
	RCC_PERCLK_SetableEx( ADCCLK, 		ENABLE );		//ADC时钟使能
	RCC_PERCLK_SetableEx( WWDTCLK, 		ENABLE );		//WWDT时钟使能
	RCC_PERCLK_SetableEx( RAMBISTCLK, 	DISABLE );		//RAMBIST时钟使能，建议关闭
	RCC_PERCLK_SetableEx( FLSEPCLK, 	DISABLE );		//Flash擦写控制器时钟使能，用完就关
	RCC_PERCLK_SetableEx( DMACLK, 		ENABLE );		//DMA时钟使能
	RCC_PERCLK_SetableEx( LCDCLK, 		ENABLE );		//LCD时钟使能
	RCC_PERCLK_SetableEx( AESCLK, 		ENABLE );		//AES时钟使能
	RCC_PERCLK_SetableEx( TRNGCLK, 		ENABLE );		//TRNG时钟使能
	RCC_PERCLK_SetableEx( CRCCLK, 		ENABLE );		//CRC时钟使能

    //PERCLKCON3 SETTING
	RCC_PERCLK_SetableEx( I2CCLK, 		ENABLE );		//I2C时钟使能
	RCC_PERCLK_SetableEx( U7816CLK1, 	ENABLE );		//78161时钟使能
	RCC_PERCLK_SetableEx( U7816CLK0, 	ENABLE );		//78160时钟使能
	RCC_PERCLK_SetableEx( UARTCOMCLK, 	ENABLE );		//UART0~5共享寄存器时钟使能
	RCC_PERCLK_SetableEx( UART5CLK, 	ENABLE );		//UART5时钟使能
	RCC_PERCLK_SetableEx( UART4CLK, 	ENABLE );		//UART4时钟使能
	RCC_PERCLK_SetableEx( UART3CLK, 	ENABLE );		//UART3时钟使能
	RCC_PERCLK_SetableEx( UART2CLK, 	ENABLE );		//UART2时钟使能
	RCC_PERCLK_SetableEx( UART1CLK, 	ENABLE );		//UART1时钟使能
	RCC_PERCLK_SetableEx( UART0CLK, 	ENABLE );		//UART0时钟使能
	RCC_PERCLK_SetableEx( HSPICLK, 		ENABLE );		//HSPI时钟使能
	RCC_PERCLK_SetableEx( SPI2CLK, 		ENABLE );		//SPI2时钟使能
	RCC_PERCLK_SetableEx( SPI1CLK, 		ENABLE );		//SPI1时钟使能

    //PERCLKCON4 SETTING
	RCC_PERCLK_SetableEx( ET4CLK, 		ENABLE );		//ET4时钟使能
	RCC_PERCLK_SetableEx( ET3CLK, 		ENABLE );		//ET3时钟使能
	RCC_PERCLK_SetableEx( ET2CLK, 		ENABLE );		//ET2时钟使能
	RCC_PERCLK_SetableEx( ET1CLK, 		ENABLE );		//ET1时钟使能
	RCC_PERCLK_SetableEx( BT2CLK, 		ENABLE );		//BT2时钟使能
	RCC_PERCLK_SetableEx( BT1CLK, 		ENABLE );		//BT1时钟使能
}

void Init_PLL(void)
{
	RCC_PLL_InitTypeDef PLL_InitStruct;
    
	PLL_InitStruct.PLLDB = 499;	//pll倍频数 = PLLDB + 1
	PLL_InitStruct.PLLINSEL = RCC_PLLCON_PLLINSEL_XTLF;	//PLL时钟源选择XTLF
	PLL_InitStruct.PLLOSEL = RCC_PLLCON_PLLOSEL_MUL1;	//默认选择1倍输出，当超出PLLDB的1023时，可使用2倍输出实现更高的倍频
	PLL_InitStruct.PLLEN = DISABLE;	//默认关闭PLL
	
	RCC_PLL_Init(&PLL_InitStruct);
	RCC_PLLCON_PLLEN_Setable(DISABLE);//关闭PLL
}

//系统时钟配置
//使用RCHF做主时钟,define_all.h 中SYSCLKdef宏控制系统时钟频率
void Init_SysClk(void)
{
	RCC_RCHF_InitTypeDef RCHF_InitStruct;
	RCC_SYSCLK_InitTypeDef SYSCLK_InitStruct;
	
	RCHF_InitStruct.FSEL = SYSCLKdef;//define_all.h 中SYSCLKdef宏控制系统时钟频率
	RCHF_InitStruct.RCHFEN = ENABLE;//打开RCHF
	
	RCC_RCHF_Init(&RCHF_InitStruct);
	
	SYSCLK_InitStruct.SYSCLKSEL = RCC_SYSCLKSEL_SYSCLKSEL_RCHF;	//选择RCHF做主时钟
	SYSCLK_InitStruct.AHBPRES = RCC_SYSCLKSEL_AHBPRES_DIV1;		//AHB不分频
	SYSCLK_InitStruct.APBPRES = RCC_SYSCLKSEL_APBPRES_DIV1;		//APB不分频
	SYSCLK_InitStruct.EXTICKSEL = RCC_SYSCLKSEL_EXTICKSEL_AHBCLK;	//EXTI,数字滤波时钟使用AHB时钟
	SYSCLK_InitStruct.SLP_ENEXTI = ENABLE;//休眠模式使能外部中断采样
	SYSCLK_InitStruct.LPM_RCLP_OFF = DISABLE;//休眠模式下开启RCLP	
	
	RCC_SysClk_Init(&SYSCLK_InitStruct);
}

//Mode:0 仿真模式下运行看门狗，运行所有定时器
//Mode:1 仿真模式关闭看门狗，运行所有定时器
void SCU_Init(uint08 Mode)
{
	if(Mode == 1)//debug
	{
		SCU_MCUDBGCR_DBG_WWDT_STOP_Setable(ENABLE);//仿真模式下关闭WWDT
		SCU_MCUDBGCR_DBG_IWDT_STOP_Setable(ENABLE);//仿真模式下关闭IWDT
	}
	else//release
	{
		SCU_MCUDBGCR_DBG_WWDT_STOP_Setable(DISABLE);//仿真模式下运行WWDT
		SCU_MCUDBGCR_DBG_IWDT_STOP_Setable(DISABLE);//仿真模式下运行IWDT
	}
	
	SCU_MCUDBGCR_DBG_ET4_STOP_Setable(DISABLE);//仿真模式下运行ET4
	SCU_MCUDBGCR_DBG_ET3_STOP_Setable(DISABLE);//仿真模式下运行ET3
	SCU_MCUDBGCR_DBG_ET2_STOP_Setable(DISABLE);//仿真模式下运行ET2
	SCU_MCUDBGCR_DBG_ET1_STOP_Setable(DISABLE);//仿真模式下运行ET1
	SCU_MCUDBGCR_DBG_BT2_STOP_Setable(DISABLE);//仿真模式下运行BT2
	SCU_MCUDBGCR_DBG_BT1_STOP_Setable(DISABLE);//仿真模式下运行BT1
}

void Init_SysClk_Gen( void )				//时钟选择相关
{	
	
	/*系统时钟超过24M后需要打开wait1*/
	if( RCHFCLKCFG >= 24 ) FLASH_FLSRDCON_WAIT_Set(FLASH_FLSRDCON_WAIT_1CYCLE);
	else FLASH_FLSRDCON_WAIT_Set(FLASH_FLSRDCON_WAIT_0CYCLE);	
	
	/*PLL配置*/
	Init_PLL();	//默认关闭PLL
	
	/*系统时钟配置*/
	Init_SysClk();	//默认使用RCHF做主时钟
	
	/*外设时钟使能配置*/	
	Init_RCC_PERIPH_clk();	//默认开启大部分外设时钟
	
	/*DMA访问RAM优先级配置*/
	RCC_MPRIL_MPRIL_Set(RCC_MPRIL_MPRIL_DMA);	//默认AHB Master优先级配置DMA优先
	
	
	/*下电复位配置*/
	//pdr和bor两个下电复位至少要打开一个
	//当电源电压低于下电复位电压时，芯片会被复位住		
	//pdr电压档位不准但是功耗极低（几乎无法测量）
	//bor电压档位准确但是需要增加2uA功耗
	ANAC_PDRCON_PDRCFG_Set(ENABLE);		//打开PDR
	ANAC_PDRCON_PDREN_Setable(ENABLE);	//打开BOR
	
	/*仿真控制寄存器配置*/
	#ifdef __DEBUG
	SCU_Init(1);//仿真时运行定时器,关闭看门狗
	#else
	SCU_Init(0);
	#endif
}

void IWDT_Init(void)
{
	RCC_PERCLK_SetableEx(IWDTCLK, ENABLE);		//IWDT总线时钟使能
	IWDT_Clr();									//清IWDT
	IWDT_IWDTCFG_IWDTOVP_Set(IWDT_IWDTCFG_IWDTOVP_2s);//配置IWDT溢出周期
	IWDT_IWDTCFG_IWDTSLP4096S_Setable(DISABLE);	//配置休眠时是否启用4096s长周期
}

void Init_IO(void)
{
	LED0_OFF;

	OutputIO( LED0_GPIO, LED0_PIN, 0 );	 //led0
	
	//fout 输出系统时钟64分频
//	GPIO_FOUTSEL_FOUTSEL_Set(GPIO_FOUTSEL_FOUTSEL_AHBCLKD64);
//	AltFunIO( GPIOG, GPIO_Pin_6, ALTFUN_NORMAL );
}

void LED0_Flash(uint08 Times)
{
	uint08 i;
	
	for( i=0; i<Times; i++ )
	{
        LED0_ON;
		TicksDelayMs( 100, NULL );		
		LED0_OFF;
		TicksDelayMs( 100, NULL );	
	}
			LED0_OFF;

}

void Init_System(void)
{		
	/*基础系统配置*/
    __disable_irq();			//关闭全局中断使能
    IWDT_Init();				//系统看门狗设置
    IWDT_Clr();  				//清系统看门狗	
	
	Init_SysTick();				//cpu滴答定时器配置(软件延时用)	
	TicksDelayMs( 10, NULL );	//软件延时,系统上电后不要立刻将时钟切换为非RCHF8M，也不要立刻进休眠否则可能导致无法下载程序
	
	Init_SysClk_Gen();			//系统时钟配置
	RCC_Init_RCHF_Trim(clkmode);//RCHF振荡器校准值载入(芯片复位后自动载入8M的校准值)
	
	/*外设初始化配置*/
	Init_Pad_Io();				//IO口输出寄存器初始状态配置
	Close_None_GPIO_80pin();    //关闭80脚芯片不支持的IO
	Close_AllIO_GPIO_80pin();   //关闭全部IO
	
	/*用户初始化代码*/
	Init_IO();
	
	/*准备进入主循环*/
	TicksDelayMs( 100, NULL );	//软件延时
	
	LED0_Flash(5);				//软件延时，闪灯五次
	
	__enable_irq();				//打开全局中断使能
}
