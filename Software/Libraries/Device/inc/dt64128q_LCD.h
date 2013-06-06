#ifndef DT64128Q_LCD_H_
#define DT64128Q_LCD_H_

/*Control registers - These commands shall be used to configure the LCD display*/
/*
 * (Read/!Write)   = 0
 * (Data/!Command) = 0
 */

#define DT64128Q_LCD_CMD_ON_OFF                  0x3E
#define DT64128Q_LCD_CMD_ON                      0x01
#define DT64128Q_LCD_CMD_OFF                     0x00

#define DT64128Q_LCD_CMD_SET_START_LINE              0xC0
#define DT64128Q_LCD_CMD_SET_START_LINE_MASK         0x3F

#define DT64128Q_LCD_CMD_SET_DATAPAGE_ADDRESS        0xB8
#define DT64128Q_LCD_CMD_SET_DATAPAGE_ADDRESS_MASK   0x07

#define DT64128Q_LCD_CMD_SET_COLUMN_ADDRESS          0x40
#define DT64128Q_LCD_CMD_SET_COLUMN_ADDRESS_MASK     0x3F

/*Status register*/
/*
 * (Read/!Write)   = 1
 * (Data/!Command) = 0
 *
 * These settings guarantees that the LCD display should interpret the
 * access as a status register query
 */

#define DT64128Q_LCD_STATUS_REGISTER_BUSY_BIT         (1 << 7)
#define DT64128Q_LCD_STATUS_REGISTER_BUSY             (1 << 7)
#define DT64128Q_LCD_STATUS_REGISTER_NOT_BUSY               0

#define DT64128Q_LCD_STATUS_REGISTER_ON_OFF_BIT       (1 << 5)
#define DT64128Q_LCD_STATUS_REGISTER_OFF              (1 << 5)
#define DT64128Q_LCD_STATUS_REGISTER_ON                     0

#define DT64128Q_LCD_STATUS_REGISTER_RESET_BIT          (1 << 4)
#define DT64128Q_LCD_STATUS_REGISTER_RESET_ACTIVE       (1 << 4)
#define DT64128Q_LCD_STATUS_REGISTER_RESET_NOT_ACTIVE         0

/*Data memory specific definitions*/

#define DT64128Q_LCD_DATA_PAGE_COUNT       8
#define DT64128Q_LCD_DATA_COLUMN_COUNT    64

#endif /* DT64128Q_LCD_H_ */
