#include "pl181.h"
#include "interrupt.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FBLK_SIZE 512
volatile pl181_t* const MCI = (pl181_t*)PL181_IOBASE;
uint32_t* sd_read_buf;
volatile void *rxbuf,*txbuf;
static volatile int rxcount,txcount,rxdone,txdone;

void sd_send_cmd(int cmd, int arg, int resp){
	//printf("SD CMD [cmd]:%d,[arg]:%d,[resp]:%d\n",cmd,arg,resp);
	MCI->MCIArgument = arg;
	MCI->MMCCommand = (1<<10) | (resp<<6) | cmd;

}

void sd_handler(){
	int i;
	uint32_t *ptr;
	uint32_t status,err;
	status = MCI->MCIStatus;
	
	if(status & RXFULL){
		//printf("I'm in RX rxcount: %d..\n",rxcount);
		ptr = (uint32_t*)rxbuf;
		err = status & (DCRCFAIL | DTIMEOUT | RXOVERR);
		if(!err && rxcount){
			for(i=0;i<16;i++){
				*(ptr+i) = MCI->MCIFIFO[0];
			}
			rxcount-=64;
			rxbuf+=64; 
			status = MCI->MCIStatus; // this is needed to re-read the status to check status to see if there are more things to read
		}
		if(rxcount==0){
			sd_send_cmd(12,0,MMC_RSP_R1); //CMD12 will stop the transmission
			rxdone=1;
			//printf("done reading..\n");
		}
	}else if(status & TXEMPTY){
		//printf("I'm in TX txcount: %d..\n",txcount);
		ptr = (uint32_t*)txbuf;
		err = status & (DCRCFAIL | DTIMEOUT);
		if(!err && txcount){
			for(i=0;i<16;i++){
				MCI->MCIFIFO[0] = ptr[i];	
			}
			txcount-=64;
			txbuf+=64; 
			status = MCI->MCIStatus;
			//printf("remaining bytes to tx sd:%d\n",txcount);
		}
		if(txcount==0){
			sd_send_cmd(12,0,MMC_RSP_R1); //CMD12 will stop the transmission
			txdone=1;	
		}
	}
	MCI->MCIClear = 0xffffffff;
}

void sd_init(){
	uint32_t RCA = 0x45670000;
	printf("--starting sd init\n");
	// power on, setup default clk
	MCI->MCIPower = 0xbf;
	MCI->MCIClock = 0xc6;
	sd_send_cmd(0, 0, MMC_RSP_NONE);// idle state
	sd_send_cmd(55, 0, MMC_RSP_R1); // ready state
	sd_send_cmd(41, 0x00ffffff, MMC_RSP_R3); // argument must not be zero
	sd_send_cmd(2, 0, MMC_RSP_R2); // ask card CID
	sd_send_cmd(3, RCA, MMC_RSP_R1); // assign RCA
	sd_send_cmd(7, RCA, MMC_RSP_R1); // transfer state: must use RCA
	sd_send_cmd(16, FBLK_SIZE, MMC_RSP_R1); // set data block length
	MCI->MCIMask0 = RXFULL | TXEMPTY; // mask used to set what events will trigger intrrupt
	install_isr(MCI_INTR0_IRQn, sd_handler);
	enable_irq(MCI_INTR0_IRQn);
	printf("--finished sd init\n");
}

void sd_read(void* buff,uint32_t sector,uint32_t count){
	rxbuf = buff;
	rxcount = FBLK_SIZE * count;
	rxdone = 0;
	MCI->MCIDataTimer = 0xffff0000;
	MCI->MCIDataLength = FBLK_SIZE*count;
	sd_send_cmd(18,sector*FBLK_SIZE,MMC_RSP_R1); // read multi blks
	MCI->MCIDataCtrl = 0x93;
	while(rxdone==0);
	//printf("Done reading %ld from sector: %ld ..\n",FBLK_SIZE * count, sector);
}


void sd_write(void* buff,uint32_t sector,uint32_t count){
	txbuf = buff;
	txcount = FBLK_SIZE * count;
	txdone = 0;
	MCI->MCIDataTimer = 0xffff0000;
	MCI->MCIDataLength = FBLK_SIZE*count;
	sd_send_cmd(25,sector*FBLK_SIZE,MMC_RSP_R1); // write multi blks
	MCI->MCIDataCtrl = 0x91;
	while(txdone==0);
	//printf("Done writing %ld to sector: %ld ..\n",FBLK_SIZE * count, sector);
}


/*
void sd_write(void* buff,uint32_t sector,uint32_t count){
	txbuf = buff;
	uint32_t *ptr = buff;
	txcount = FBLK_SIZE * count;
	txdone = 0;
	int i;
	MCI->MCIDataTimer = 0xffff0000;
	MCI->MCIDataLength = FBLK_SIZE*count;
	MCI->MCIDataCtrl = 0x91;
	//sd_send_cmd(23,count,MMC_RSP_R1);
	//sd_send_cmd(23,count,MMC_RSP_R1); // write multi blks
	sd_send_cmd(25,sector*FBLK_SIZE,MMC_RSP_R1);
	uint32_t status = MCI->MCIStatus;
	while(status & TXEMPTY && txcount){
		for(i=0;i<16;i++){
			MCI->MCIFIFO[0] = *(ptr+i);	
			printf("%x",*(ptr+i));
		}
		printf("\n");
		txcount-=64;
		txbuf+=64;
		ptr+=16;//forgot to advance the ptr to write the next 64 block
		status = MCI->MCIStatus;
	}
	//while(txdone==0);
	sd_send_cmd(12,0,MMC_RSP_R1);
	printf("Done writing %ld bytes to sector: %ld ..\n",FBLK_SIZE * count, sector);
}
*/
void sd_test_read(){
	int i = 0;
	sd_read_buf = malloc(1024);
	sd_read(sd_read_buf,0,2);
	char* ch = (char*)sd_read_buf;
	for(i=0;i<23;i++){
		printf("%c",*(ch+i));
	}

	for(i=0;i<512;i++){
		((char *)sd_read_buf)[i] = 'O';
	}
	sd_write(sd_read_buf,1,1);
}
