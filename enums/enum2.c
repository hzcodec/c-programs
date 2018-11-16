/*
    Auther       : Heinz Samuelsson
    Date         : 2018-10-22
    File         : enum2.c
    Reference    : -
    Description  : -
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
 
 
enum io_dout {
	DOUT_HE_1_ERR = 27,
	DOUT_HE_2_ERR,
	DOUT_HE_3_ERR,
	DOUT_HE_4_ERR,
	DOUT_HE_5_ERR,
	DOUT_HE_6_ERR,
	DOUT_HE_1_OK,
	DOUT_HE_2_OK,
	DOUT_HE_3_OK,
	DOUT_HE_4_OK,
	DOUT_HE_5_OK,
	DOUT_HE_6_OK,
	IO_DOUT_NUM ///< Number of digital outputs, not a valid value
};


int main(int argc, char *argv[])
{

        uint8_t he_err = 0x3f; 

	printf("he_err=%02x\n\n", he_err);

       for (int pos=0 ; pos<6 ; pos++) {
            if (he_err & (1 << pos)) {
	    	printf("[%d], 1, dout=%d\n", pos, DOUT_HE_1_ERR+pos);
            } else {
	    	printf("[%d], 0, dout=%d\n", pos, DOUT_HE_1_ERR+pos);
            }
	}

        return 0;
}

