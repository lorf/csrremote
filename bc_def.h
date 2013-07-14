#ifndef BC_DEF_H
#define BC_DEF_H

/*
 * These constants are taken from pttransport/io_map_public_bc.h
 * and pttransport/io_defs_bc.h, all found in bluesuite source code.
 */

#define RST_WATCHDOG_EN_MASK            0x0001
#define RST_UART_EN_MASK                0x0002
#define RST_FULL_CHIP_RESET             0x0004

#define SPI_EMU_CMD_XAP_STEP_MASK       0x0001
#define SPI_EMU_CMD_XAP_RUN_B_MASK      0x0002
#define SPI_EMU_CMD_XAP_BRK_MASK        0x0004
#define SPI_EMU_CMD_XAP_WAKEUP_MASK     0x0008

#define SPI_EMU_CMD                     0x006A  /* RW   8 bits */
#define SPI_USER_CMD                    0x006B  /* RW   8 bits */
#define SPI_BRK_DATA_ADDR               0x006C  /* RW  16 bits */
#define SPI_BRK_DATA_RW_EN              0x006D  /* RW   2 bits */
#define MMU_FREE_LIST_ADDR              0x0070  /* RW  16 bits */
#define MMU_FLASH_BANK_SELECT           0x0073  /* RW  12 bits */
#define MMU_CONST_BANK_SELECT           0x0074  /* RW   9 bits */
#define MMU_FREE_LIST_NEXT_ADDR         0xFFCF  /* R   16 bits */
#define RSTGEN_WATCHDOG_DELAY           0x0076  /* RW  16 bits */
#define RSTGEN_WATCHDOG_KICK            0x0077  /* RW   1 bit  */
#define ANA_VERSION_ID                  0xFF7D  /* R   16 bits */
#define ANA_CONFIG2                     0xFF7E  /* RW  16 bits */
#define ANA_LO_FREQ                     0xFF82  /* RW  16 bits */
#define ANA_LO_FTRIM                    0xFF83  /* RW  16 bits */
#define GBL_RST_ENABLES                 0xFF91  /* RW   2 bits */
#define GBL_TIMER_ENABLES               0xFF94  /* RW   4 bits */
#define GBL_MISC_ENABLES                0xFF97  /* RW   8 bits */
#define GBL_MISC2_ENABLES               0xFF52  /* RW   4 bits */
#define GBL_CHIP_VERSION                0xFF9A  /* R   16 bits */
#define GBL_CLK_RATE                    0xFFDE  /* RW   5 bits */
#define TIMER_SLOW_TIMER_PERIOD         0xFFB9  /* R   13 bits */
#define XAP_AH                          0xFFE0  /* RW  16 bits */
#define XAP_AL                          0xFFE1  /* RW  16 bits */
#define XAP_UXH                         0xFFE2  /* RW   8 bits */
#define XAP_UXL                         0xFFE3  /* RW  16 bits */
#define XAP_UY                          0xFFE4  /* RW  16 bits */
#define XAP_IXH                         0xFFE5  /* RW   8 bits */
#define XAP_IXL                         0xFFE6  /* RW  16 bits */
#define XAP_IY                          0xFFE7  /* RW  16 bits */
#define XAP_FLAGS                       0xFFE8  /* RW   8 bits */
#define XAP_PCH                         0xFFE9  /* RW   8 bits */
#define XAP_PCL                         0xFFEA  /* RW  16 bits */
#define XAP_BRK_REGH                    0xFFEB  /* RW   8 bits */
#define XAP_BRK_REGL                    0xFFEC  /* RW  16 bits */
#define XAP_RSVD_13                     0xFFED  /* RW  16 bits */
#define XAP_RSVD_14                     0xFFEE  /* RW  16 bits */
#define XAP_RSVD_15                     0xFFEF  /* RW  16 bits */

#endif // BC_DEF_H
