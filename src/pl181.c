#include "pl181.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

volatile pl181_t* const MCI = (pl181_t*)PL181_IOBASE;
uint32_t* sd_read_buf;
void sd_send_cmd(int cmd, int arg, int resp){
	MCI->MCIArgument = arg;
	MCI->MMCCommand = (1<<10) | (resp<<6) | cmd;
	int i; for (i=0; i<100; i++);
}
void sd_init(){
	uint32_t RCA = 0x45670000;
	printf("starting sd init\n");
	// power on, setup default clk
	MCI->MCIPower = 0xbf;
	MCI->MCIClock = 0xc6;
	
	sd_send_cmd(0, 0, MMC_RSP_NONE);// idle state
	sd_send_cmd(55, 0, MMC_RSP_R1); // ready state
	sd_send_cmd(41, 0x00ffffff, MMC_RSP_R3); // argument must not be zero
	//sd_send_cmd(41, 0x00ffffff, MMC_RSP_R3); // argument must not be zero
	sd_send_cmd(2, 0, MMC_RSP_R2); // ask card CID
	sd_send_cmd(3, RCA, MMC_RSP_R1); // assign RCA
	sd_send_cmd(7, RCA, MMC_RSP_R1); // transfer state: must use RCA
	sd_send_cmd(16, 512, MMC_RSP_R1); // set data block length

	MCI->MCIMask0 = (1<<21) | (1<<18);
	printf("finished sd init\n");
}
void sd_test_read(){
	int rxcount = 512;
	int error = 0;
	int i = 0;
	sd_read_buf = malloc(512);
	MCI->MCIDataTimer = 0xffff0000;
	MCI->MCIDataLength = 512;
	MCI->MCIDataCtrl = 0x93;

	sd_send_cmd(17,0*512,MMC_RSP_R1);
	if(MCI->MCIStatus & (1<<21)){
		printf("there is data\n");
		error = MCI->MCIStatus & (DCRCFAIL | DTIMEOUT | RXOVERR);
		while(!error && rxcount){
			//let's read data
			sd_read_buf[i++] = MCI->MCIFIFO[0];
			rxcount -= sizeof(uint32_t);
			error = MCI->MCIStatus & (DCRCFAIL | DTIMEOUT | RXOVERR);
		}
		char* ch = (char*)sd_read_buf;
		for(i=0;i<23;i++){
			printf("%c",*(ch+i));
		}
		printf("\n");
	}
}
//void sd_handler();

