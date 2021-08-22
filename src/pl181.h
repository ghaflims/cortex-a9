#ifndef __PL181_H__
#define __PL181_H__
#include <stdint.h>
#define PL181_IOBASE 0x10005000

#define MMC_RSP_PRESENT	(1 << 0)
#define MMC_RSP_136	(1 << 1)		/* 136 bit response */
#define MMC_RSP_CRC	(1 << 2)		/* expect valid crc */
#define MMC_RSP_BUSY	(1 << 3)		/* card may send busy */
#define MMC_RSP_OPCODE	(1 << 4)		/* response contains opcode */

#define MMC_CMD_MASK	(3 << 5)		/* non-SPI command type */
#define MMC_CMD_AC	(0 << 5)
#define MMC_CMD_ADTC	(1 << 5)
#define MMC_CMD_BC	(2 << 5)
#define MMC_CMD_BCR	(3 << 5)

#define MMC_RSP_SPI_S1	(1 << 7)		/* one status byte */
#define MMC_RSP_SPI_S2	(1 << 8)		/* second byte */
#define MMC_RSP_SPI_B4	(1 << 9)		/* four data bytes */
#define MMC_RSP_SPI_BUSY (1 << 10)		/* card may send busy */

#define MMC_RSP_NONE	(0)
#define MMC_RSP_R1	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1B	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE|MMC_RSP_BUSY)
#define MMC_RSP_R2	(MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)
#define MMC_RSP_R3	(MMC_RSP_PRESENT)
#define MMC_RSP_R4	(MMC_RSP_PRESENT)
#define MMC_RSP_R5	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R6	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R7	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define DCRCFAIL (1<<1)
#define DTIMEOUT (1<<3)
#define RXOVERR (1<<5)
#define RXFULL (1<<17)
#define TXEMPTY (1<<18)

typedef volatile struct {
	uint32_t MCIPower;
	uint32_t MCIClock;
	uint32_t MCIArgument;
	uint32_t MMCCommand;
	const uint32_t MCIRespCmd;
	const uint32_t MCIResponse0;
	const uint32_t MCIResponse1;
	const uint32_t MCIResponse2;
	const uint32_t MCIResponse3;
	uint32_t MCIDataTimer;
	uint32_t MCIDataLength;
	uint32_t MCIDataCtrl;
	const uint32_t MCIDataCnt;
	const uint32_t MCIStatus;
	uint32_t MCIClear;
	uint32_t MCIMask0;
	uint32_t MCIMask1;
	uint32_t MCISelect;
	const uint32_t MCIFifoCnt;
	uint32_t Reserved[13];
	uint32_t MCIFIFO[64];
} pl181_t;

void sd_init();
void sd_send_cmd(int cmd, int arg, int resp);
void sd_test_read();
void sd_read(void* buff,uint32_t sector,uint32_t count);
void sd_write(void* buff,uint32_t sector,uint32_t count);
#endif

