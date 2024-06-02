#ifndef __PL041_H__
#define __PL041_H__

#include <stdint.h>
#define AACI_BASE 0x10004000

// AC97 registers
#define AC97_RESET				0x00
#define AC97_MASTER				0x02
#define AC97_PCM_FRONT_DAC_RATE 0x2c
#define AC97_HEADPHONE			0x04
#define AC97_MASTER_MONO		0x06
#define AC97_MASTER_TONE		0x08
#define AC97_PC_BEEP			0x0A    //mixer volume
#define AC97_PHONE				0x0C
#define AC97_MIC				0x0E    //qwert db
#define AC97_LINE				0x10		
#define AC97_CD					0x12
#define AC97_VIDEO				0x14
#define AC97_AUX				0x16
#define AC97_PCM				0x18
#define AC97_REC_SEL			0x1A	//0 represent mic
#define AC97_REC_GAIN			0x1C
#define AC97_REC_GAIN_MIC		0x1E
#define AC97_GENERAL_PURPOSE	0x20
#define AC97_3D_CONTROL			0x22
#define AC97_INT_PAGING			0x24    //qwert
#define AC97_POWERDOWN			0x26
#define AC97_PCM_FRONT_DAC_RATE 0x2c	/* PCM Front DAC Rate */
#define AC97_PCM_SURR_DAC_RATE	0x2e	/* PCM Surround DAC Rate */
#define AC97_PCM_LFE_DAC_RATE	0x30	/* PCM LFE DAC Rate */
#define AC97_PCM_LR_ADC_RATE	0x32	/* PCM LR ADC Rate */
#define AC97_PCM_MIC_ADC_RATE	0x34	/* PCM MIC ADC Rate */
#define AC97_DAC_SLOT_MAP		0x6C
#define AC97_ADC_SLOT_MAP		0x6E

typedef volatile struct {
	uint32_t AACIRXCR1;
	uint32_t AACITXCR1;
	const uint32_t AACISR1;
	const uint32_t AACIISR1;
	uint32_t AACIIE1;
	uint32_t AACIRXCR2;
	uint32_t AACITXCR2;
	const uint32_t AACISR2;
    const uint32_t AACIISR2;
    uint32_t AACIIE2;
    uint32_t AACIRXCR3;
    uint32_t AACITXCR3;
    const uint32_t AACISR3;
    const uint32_t AACIISR3;
    uint32_t AACIIE3;
    uint32_t AACIRXCR4;
    uint32_t AACITXCR4;
    const uint32_t AACISR4;
    const uint32_t AACIISR4;
    uint32_t AACIIE4;
    const uint32_t AACISL1RX;
    uint32_t AACISL1TX;
    const uint32_t AACISL2RX;
    uint32_t AACISL2TX;
    const uint32_t AACISL12RX;
    uint32_t AACISL12TX;
    uint32_t AACISLFR;
    const uint32_t AACISLISTAT;
    uint32_t AACISLIEN;
    uint32_t AACIINTCLR;
    uint32_t AACIMAINCR;
    uint32_t AACIRESET;
    uint32_t AACISYNC;
    const uint32_t AACIALLINTS;
    const uint32_t AACIMAINFR;
    const uint32_t DUMMY;
    uint32_t AACIDR1[8];
    uint32_t AACIDR2[8];
    uint32_t AACIDR3[8];
    uint32_t AACIDR4[8];
    const uint32_t RESERVED[948];
    const uint32_t AACIPERIPHID0;
    const uint32_t AACIPERIPHID1;
    const uint32_t AACIPERIPHID2;
    const uint32_t AACIPERIPHID3;
    const uint32_t AACIPCELLID0;
    const uint32_t AACIPCELLID1;
    const uint32_t AACIPCELLID2;
    const uint32_t AACIPCELLID3;
} pl041_t;

void aaci_init();
void aaci_config_channel();
void aaci_enable_channel();
void aaci_write_channel(const uint16_t *buff, int count);
void aaci_disable_channel();
void aaci_delay(uint32_t us);

void ac97_reset();
void ac97_write(uint16_t reg, uint16_t val);
void ac97_select();
void ac97_set_volume();
void ac97_set_rate(uint16_t rate);

void aaci_test_sound();
void aaci_test_wavfile();

#endif