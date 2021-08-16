/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PDEF_TBB_EXT_H
#define PDEF_TBB_EXT_H

#include <tbbr/tbb_ext.h>

/* Plat Defined TBBR extensions */
enum {
	DDR_FW_CONTENT_CERT_PK_EXT = FWU_HASH_EXT + 1,
	DDR_IMEM_UDIMM_1D_HASH_EXT,
	DDR_IMEM_UDIMM_2D_HASH_EXT,
	DDR_DMEM_UDIMM_1D_HASH_EXT,
	DDR_DMEM_UDIMM_2D_HASH_EXT,
	DDR_IMEM_RDIMM_1D_HASH_EXT,
	DDR_IMEM_RDIMM_2D_HASH_EXT,
	DDR_DMEM_RDIMM_1D_HASH_EXT,
	DDR_DMEM_RDIMM_2D_HASH_EXT
};

#endif /* PDEF_TBB_EXT_H */