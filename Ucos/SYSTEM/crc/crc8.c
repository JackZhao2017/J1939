/*
 * Copyright (c) 2011 Broadcom Corporation
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "crc8.h"
#include <string.h>
#include <stdio.h>



#define u8 unsigned char 
/*
 * crc8_populate_msb - fill crc table for given polynomial in reverse bit order.
 *
 * table:	table to be filled.
 * polynomial:	polynomial for which table is to be filled.
 */
static unsigned char  crc_table[CRC8_TABLE_SIZE];

void crc8_populate_msb(u8 table[CRC8_TABLE_SIZE], u8 polynomial)
{
	int i, j;
	const u8 msbit = 0x80;
	u8 t = msbit;

	table[0] = 0;

	for (i = 1; i < CRC8_TABLE_SIZE; i *= 2) {
		t = (t << 1) ^ (t & msbit ? polynomial : 0);
		for (j = 0; j < i; j++)
			table[i+j] = table[j] ^ t;
	}
}


/*
 * crc8_populate_lsb - fill crc table for given polynomial in regular bit order.
 *
 * table:	table to be filled.
 * polynomial:	polynomial for which table is to be filled.
 */
void crc8_populate_lsb(u8 table[CRC8_TABLE_SIZE], u8 polynomial)
{
	int i, j;
	u8 t = 1;
	table[0] = 0;

	for (i = (CRC8_TABLE_SIZE >> 1); i; i >>= 1) {
		t = (t >> 1) ^ (t & 1 ? polynomial : 0);
		for (j = 0; j < CRC8_TABLE_SIZE; j += 2*i)
			table[i+j] = table[j] ^ t;
	}

//	for(i=0;i<CRC8_TABLE_SIZE;i++){	
//		if(i%16==0)
//			printf("\r\n");
//		printf("0x%02x ",table[i]);
//	}
//	printf("crc lsb poly =0x%02x\r\n",polynomial);
}
/*
 * crc8 - calculate a crc8 over the given input data.
 *
 * table: crc table used for calculation.
 * pdata: pointer to data buffer.
 * nbytes: number of bytes in data buffer.
 * crc:	previous returned crc8 value.
 */
u8 crc8_c1( u8 *pdata, u8 nbytes, u8 crc)
{
	/* loop over the buffer data */
	while (nbytes-- > 0){
		//printf("[0x%02x  0x%02x  0x%02x 0x%02x] \r\n",crc,*pdata,(crc ^ *pdata)&0xff,crc_table[(crc ^ *pdata)&0xff]);
		crc = crc_table[(crc ^ *pdata++) & 0xff];
	}
	//printf("\r\n");
	return crc;
}
u8 crc8_c2( u8 *pdata, u8 nbytes, u8 crc)
{
	/* loop over the buffer data */
	while (nbytes-- > 0){
		//printf("[0x%02x  0x%02x  0x%02x 0x%02x] \r\n",crc,*pdata,(crc ^ *pdata)&0xff,crc_table[(crc ^ *pdata)&0xff]);
		crc = crc_table[(crc ^ *pdata++) & 0xff];
	}
	return crc;
}
u8 crc8_creator(u8 *m,u8 start,u8 len)
{
	u8 crc=0;
	crc=crc8_c1(&m[start],len,0);
	return crc;
}
u8 crc8_detector(u8 *m,u8 start,u8 len)
{
	u8 crc=0;
	crc=crc8_c2(&m[start],len,0);
	return crc;
}
void crcInit(u8 type, u8 poly)
{
	memset(crc_table,0,sizeof(crc_table));
	if(type==MSB){
		 crc8_populate_msb(crc_table, poly);
	}
	else if(type==LSB){
		crc8_populate_lsb(crc_table, poly);
	}
}

