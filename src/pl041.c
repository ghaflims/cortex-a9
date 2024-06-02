#include "pl041.h"
#include "audiosample.h"
#include "ff.h"
#include <stdlib.h>
#include <stdio.h>

volatile pl041_t* const AACI = (pl041_t*)AACI_BASE;
// pl0141 init
/*	
	(*AACIRXCR) = 0;
	(*AACITXCR) = 0;
	(*AACIIE) = 0;
	(*AACIMAINCR) = (1<<6)|(1<<5)|(1<<4)|(1<<3)|(1<<0);
	(*AACIRESET) = 0;
	pl041_delay(2);
	(*AACIRESET) = 1;

	// ac97 reset
		// select ac97 codec [check here if it doesn't work]
	(*AACIMAINCR) = (1<<6)|(1<<5)|(1<<4)|(1<<3)|(1<<0);
		// ac97 write
	(*AACISL2TX) = 0xffff << 4;
	(*AACISL1TX) = AC97_RESET << 12;
	pl041_delay(50); // frame period in us
	pl041_delay(50*8); // wait for timeout [check here if it doesn't work]

	// pl041 channel config [don't enable just yet]
	(*AACITXCR) = (1<<16) | (1<<3);
		// ac97 set volume to max 0x3f [check here if it doesn't work set others]
	(*AACIMAINCR) = (1<<6)|(1<<5)|(1<<4)|(1<<3)|(1<<0);
	(*AACISL2TX) = (0x3f|(0x3f<<8)) << 4;
	(*AACISL1TX) = AC97_MASTER << 12;
	pl041_delay(50); // frame period in us
	pl041_delay(50*8); // wait for timeout [check here if it doesn't work]

	(*AACISL2TX) = (0x3f|(0x3f<<8)) << 4;
	(*AACISL1TX) = AC97_HEADPHONE << 12;
	pl041_delay(50); // frame period in us
	pl041_delay(50*8); // wait for timeout [check here if it doesn't work]

	(*AACISL2TX) = (0x3f|(0x3f<<8)) << 4;
	(*AACISL1TX) = AC97_MASTER_MONO << 12;
	pl041_delay(50); // frame period in us
	pl041_delay(50*8); // wait for timeout [check here if it doesn't work]

	(*AACISL2TX) = (0x3f|(0x3f<<8)) << 4;
	(*AACISL1TX) = AC97_PCM << 12;
	pl041_delay(50); // frame period in us
	pl041_delay(50*8); // wait for timeout [check here if it doesn't work]


		// ac97 set rate
	(*AACIMAINCR) = (1<<6)|(1<<5)|(1<<4)|(1<<3)|(1<<0);
	(*AACISL2TX) = 44100 << 4;
	(*AACISL1TX) = AC97_PCM_FRONT_DAC_RATE << 12;
	pl041_delay(50); // frame period in us
	pl041_delay(50*8); // wait for timeout [check here if it doesn't work]

	// pl041 channel enable
	(*AACITXCR) = (1<<16) | (1<<3) | (1<<0);

	// send data
	for(int j=0;j<60;j++){
		for(int i=0;i<44100;i++){
			int16_t s = audioSamples[i];
			(*AACIDR1) = (s<<16)|s;
		}
	}
*/
void aaci_delay(uint32_t us){
    for(int i=0;i<us*2;i++){
        asm("nop");
    }
}

void aaci_init(){
	printf("--starting aaci init\n");
    AACI->AACITXCR1 = 0;
    AACI->AACIRXCR1 = 0;
    AACI->AACIIE1 = 0;
    ac97_reset();
	printf("--finished aaci init\n");
}
void aaci_config_channel(){
    //TX3 enable fifo stores slot3 data, FIFO enable
    AACI->AACITXCR1 = (1<<16) | (1<<3);
    ac97_set_volume(0x3f);
    ac97_set_rate(44100);
}
void aaci_enable_channel(){
    //TxEn transmit enable for FIFO, TX3 enable fifo stores slot3 data, FIFO enable
    AACI->AACITXCR1 = (1<<16) | (1<<3) | (1<<0);
}
void aaci_write_channel(const uint16_t *buff, int count){
	int i = 0;
	do{
		if(!(AACI->AACISR1 & (1<<5))){
			int16_t s = buff[i];
			AACI->AACIDR1[0] = (s<<16)|s;
			i++;
		}
	}while(i<count);
}
void aaci_disable_channel(){
    //TxEn transmit disable for FIFO, TX3 enable fifo stores slot3 data, FIFO enable
    AACI->AACITXCR1 = (1<<16) | (1<<3) | (0<<0);
}

void ac97_reset(){
    ac97_select();
    AACI->AACIRESET = 0;
    aaci_delay(2);
    AACI->AACIRESET = 1;
    // write anything to AC97 register
    ac97_write(AC97_RESET,0xffff);
}

void ac97_write(uint16_t reg, uint16_t val){
    ac97_select();
    AACI->AACISL2TX = val << 4;
    AACI->AACISL1TX = reg << 12;
    aaci_delay(50);
    aaci_delay(50*8);
}

void ac97_select(){
    // AACI Enable, Slot 1 recieve/transmit enable, Slot 2 recieve/transmit enable
    AACI->AACIMAINCR = (1<<6)|(1<<5)|(1<<4)|(1<<3)|(1<<0); 
    aaci_delay(1);
}
void ac97_set_volume(int vol){
    uint16_t v = (vol&0x3f) | (vol&0x3f)<<8;
    ac97_write(AC97_MASTER,v);
    ac97_write(AC97_PCM,v);
    ac97_write(AC97_MASTER_MONO,v);
    ac97_write(AC97_HEADPHONE,v);
}

void ac97_set_rate(uint16_t rate){
    ac97_write(AC97_PCM_FRONT_DAC_RATE, rate);
}

void aaci_test_sound(){
    aaci_config_channel();
    aaci_enable_channel();
    aaci_write_channel(audioSamples,44100);
    aaci_disable_channel();
}

void aaci_test_wavfile(){
	//FATFS fs0;
	FIL f;
	FRESULT fr;
	const size_t wav_fsize = 589818;
	uint16_t* wavdata = malloc(wav_fsize);
	unsigned int wc;
	//f_mount(&fs0,"",0);
	fr = f_open(&f,"pcm.bin",FA_READ);
	if(!fr){
		f_read(&f,wavdata,wav_fsize,&wc);
		printf("total bytes read from sd card for pcm.bin is: %d\n",wc);
		aaci_config_channel();
    	aaci_enable_channel();
    	aaci_write_channel(wavdata,wav_fsize/2);
    	aaci_disable_channel();
	}else{
		printf("error in reading pcm.bin file\n");
	}
	f_close(&f);
	//f_unmount("");
}