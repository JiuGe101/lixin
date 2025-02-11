/**
 * @file fpb.h
 * @author liufeng (liufeng@injoinic.com)
 * @brief FPB Unit Register define.
 * @version 0.1
 * @date 2024-12-19
 *
 * @copyright Copyright (c) 2024  injoinic
 *
 */

#ifndef __FPB_H__
#define __FPB_H__

#include "stdint.h"

typedef struct
{
    uint32_t CTRL;
    uint32_t REMAP;
    uint32_t COMP[8];
} FPB_Type;

#define FPB_BASE               (0xE0002000UL)
#define FPB                    ((FPB_Type*)FPB_BASE)

/* --- FPB_CTRL values ----------------------------------------------------- */

/* Bits [31:15]: Reserved, read as zero, writes ignored */
#define FPB_CTRL_NUM_CODE2_Pos (12)
#define FPB_CTRL_NUM_CODE2_Msk (0x7 << FPB_CTRL_NUM_CODE2_Pos)
#define FPB_CTRL_NUM_LIT_Pos   (8)
#define FPB_CTRL_NUM_LIT_Msk   (0xf << FPB_CTRL_NUM_LIT_Pos)
#define FPB_CTRL_NUM_CODE1_Pos (4)
#define FPB_CTRL_NUM_CODE1_Msk (0xf << FPB_CTRL_NUM_CODE1_Pos)
/* Bits [3:2]: Reserved */
#define FPB_CTRL_KEY_Pos       (1)
#define FPB_CTRL_KEY_Msk       (0x1 << FPB_CTRL_KEY_Pos)
#define FPB_CTRL_ENABLE_Pos    (0)
#define FPB_CTRL_ENABLE_Msk    (0x1 << FPB_CTRL_ENABLE_Pos)

/* --- FPB_REMAP values ---------------------------------------------------- */

/* Bits [31:30]: Reserved */
#define FPB_REMAP_RMPSPT_Pos   (29)
#define FPB_REMAP_RMPSPT_Msk   (0x1 << FPB_REMAP_RMPSPT_Pos)
#define FPB_REMAP_ENABLE_Pos   (5)
#define FPB_REMAP_ENABLE_Msk   (0xFFFFFF << FPB_CTRL_ENABLE_Pos)
/* Bits [4:0]: Reserved */

/* --- FPB_COMPx values ---------------------------------------------------- */

#define FPB_COMP_REPLACE_Pos   (30)
#define FPB_COMP_REPLACE_Msk   (0x3 << FPB_COMP_REPLACE_Pos)
/* Bits [29]: Reserved */
#define FPB_COMP_COMP_Pos      (2)
#define FPB_COMP_COMP_Msk      (0x7FFFFFF << FPB_COMP_COMP_Pos)
/* Bits [1]: Reserved */
#define FPB_COMP_ENABLE_Pos    (0)
#define FPB_COMP_ENABLE_Msk    (0x1 << FPB_COMP_ENABLE_Pos)

#endif /*__FPB_H__ */
