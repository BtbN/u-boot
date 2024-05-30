# SPDX-License-Identifier: GPL-2.0+

include $(srctree)/scripts/Makefile.dts

targets += $(dtb-y)

# Add any required device tree compiler flags here
DTC_FLAGS += -a 0x8 -@

PHONY += dtbs dtbo
dtbs: $(addprefix $(obj)/, $(dtb-y))
	@:

dtbo: $(addprefix $(obj)/, $(dtbo-y))
	@:

#mt7986a-bananapi-bpi-r3.dts
#mt7986a-bananapi-bpi-r3-sd.dtso
#mt7986a-bananapi-bpi-r3-emmc.dtso

#mt7986a-bananapi-bpi-r3-nand.dtso
#mt7986a-bananapi-bpi-r3-nor.dtso


clean-files := */*.dtb */*.dtbo
