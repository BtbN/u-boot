// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2018 MediaTek Inc.
 */

#include <common.h>
#include <init.h>
#include <asm/global_data.h>
#include <linux/io.h>
#include <linux/sizes.h>
#include <asm/arch/misc.h>

#include "preloader.h"

DECLARE_GLOBAL_DATA_PTR;

struct boot_argument *preloader_param;

int mtk_soc_early_init(void)
{
	return 0;
}

int dram_init(void)
{
	u32 i;

	if (((size_t)preloader_param >= CFG_SYS_SDRAM_BASE) &&
	    ((size_t)preloader_param % sizeof(size_t) == 0) &&
	    preloader_param->magic == BOOT_ARGUMENT_MAGIC &&
	    preloader_param->dram_rank_num <=
	    ARRAY_SIZE(preloader_param->dram_rank_size)) {
		gd->ram_size = 0;

		for (i = 0; i < preloader_param->dram_rank_num; i++)
			gd->ram_size += preloader_param->dram_rank_size[i];
	} else {
		gd->ram_size = get_ram_size((long *)CFG_SYS_SDRAM_BASE,
					    SZ_2G);
	}

	return 0;
}

#include <asm/io.h>

#define UART_BASE				0x11004000 //uart2 maybe use CONFIG_DEBUG_UART_BASE
#define UART_BAUD				115200

// Real XTAL
#define UART_CLK				25000000 //mt7623 use 25MHz

#define UART_RBR				(0x0)
#define UART_THR				(0x0)
#define UART_DLL				(0x0)
#define UART_DLH				(0x4)
#define UART_IER				(0x4)
#define UART_LCR				(0xc)
#define UART_LSR				(0x14)
#define UART_HIGHSPEED				(0x24)
#define UART_SAMPLE_COUNT			(0x28)
#define UART_SAMPLE_POINT			(0x2c)

#define UART_LSR_DR				(0x01)	/* Data ready */
#define UART_LSR_THRE				(0x20)	/* Xmit holding register empty */

#define UART_WRITE_REG(offset, val)		setbits_32(UART_BASE + (offset), val)
#define UART_READ_REG(offset)			readl(UART_BASE + (offset))

#define QUOT_VAL				((UART_CLK / (0xff * UART_BAUD)) + 1)
#define SAMPLE_COUNT_VAL			((UART_CLK / (UART_BAUD * QUOT_VAL)) - 1)
#define SAMPLE_POINT_VAL			((SAMPLE_COUNT_VAL - 1) / 2)


void board_debug_uart_init(void)
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

int print_cpuinfo(void)
{
	void __iomem *chipid;
	u32 swver;

	chipid = ioremap(VER_BASE, VER_SIZE);
	swver = readl(chipid + APSW_VER);

	printf("CPU:   MediaTek MT7623 E%d\n", (swver & 0xf) + 1);

	return 0;
}
