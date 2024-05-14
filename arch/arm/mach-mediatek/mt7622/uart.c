#include "mmio.h"

// MT7988
// #define UART_BASE						0x11000000
// MT7986/MT7622
#define UART_BASE						0x11002000

#define UART_BAUD						115200

// Real XTAL (MT7622/MT7986/MT7988)
#define UART_CLK						40000000
// Real XTAL (MT7629)
// #define UART_CLK						26000000
// FPGA UART
//#define UART_CLK						12000000

#define UART_RBR						(0x0)
#define UART_THR						(0x0)
#define UART_DLL						(0x0)
#define UART_DLH						(0x4)
#define UART_IER						(0x4)
#define UART_LCR						(0xc)
#define UART_LSR						(0x14)
#define UART_HIGHSPEED					(0x24)
#define UART_SAMPLE_COUNT				(0x28)
#define UART_SAMPLE_POINT				(0x2c)

#define UART_LSR_DR						(0x01)	/* Data ready */
#define UART_LSR_THRE					(0x20)	/* Xmit holding register empty */

#define UART_WRITE_REG(offset, val)		mmio_write_32(UART_BASE + (offset), val)
#define UART_READ_REG(offset)			mmio_read_32(UART_BASE + (offset))

#define QUOT_VAL						((UART_CLK / (0xff * UART_BAUD)) + 1)
#define SAMPLE_COUNT_VAL				((UART_CLK / (UART_BAUD * QUOT_VAL)) - 1)
#define SAMPLE_POINT_VAL				((SAMPLE_COUNT_VAL - 1) / 2)


const char * print_data=
" ----------------------------------------\r\n"      
" Hello World !                           \r\n"
" ----------------------------------------\r\n"
"        o   ^__^                         \r\n"
"         o  (oo)\\_______                 \r\n"
"            (__)\\       )\\/\\             \r\n"
"                ||----w |                \r\n"
"                ||     ||                \r\n"
"---------------------------------------- \r\n\0";

void mtk_putc(char c)
{
	while ((UART_READ_REG(UART_LSR)&UART_LSR_THRE) == 0) {;}
	UART_WRITE_REG(UART_THR, c);
}

int mtk_strlen(const char *s)
{
	int len = 0;
	while (*s++) 
		++len;
	return len;
}

void mtk_puts(const char *s)
{
	int i;
	int len;
	len = mtk_strlen(s);
	for(i=0 ; i<len ; i++)
		mtk_putc(*(s+i));
}

void mtk_hello_world()
{
	mtk_puts(print_data);
}

void mtk_uart_init(void)
{
	int tmp;

	UART_WRITE_REG(UART_HIGHSPEED, 0x3);

	tmp = UART_READ_REG(UART_LCR);
	tmp |= (1<<7);
	UART_WRITE_REG(UART_LCR, tmp);

	UART_WRITE_REG(UART_THR, QUOT_VAL);
	UART_WRITE_REG(UART_IER, 0x0);
	UART_WRITE_REG(UART_SAMPLE_COUNT, SAMPLE_COUNT_VAL);
	UART_WRITE_REG(UART_SAMPLE_POINT, SAMPLE_POINT_VAL);

	tmp = UART_READ_REG(UART_LCR);
	tmp &= ~(1<<7);
	UART_WRITE_REG(UART_LCR, tmp);
}
