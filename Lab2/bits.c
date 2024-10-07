/**************************************************************/
/* bits.c                                                     */
/* Author : Alain Couvreur                                    */
/* alain.couvreur@lix.polytechnique.fr                        */
/* Modifications: Maxime Bombar (maxime.bombar@inria.fr)      */
/* Last modification September 29, 2022 (Maxime Bombar)       */
/**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"
#include "bits.h"

void printDec(uchar* u, int length){
    int i;
    
    printf("[");
    if(length == 0){
	printf("]");
	return;
    }
    for(i  = 0; i < length - 1; i++){
	printf("%d, ", *(u++));
    }
    printf("%d]", *u);
}


void printHexa(uchar* u, int length){
    int i;
    
    printf("[");
    if(length == 0){
	printf("]");
	return;
    }
    for(i  = 0; i < length - 1; i++){
	if(*u < 0x10)
	    printf("0x0%x, ", *(u++));
	else
	    printf("0x%x, ", *(u++));
    }
    if(*u < 0x10)
	printf("0x0%x]", *(u++));
    else
	printf("0x%x]", *(u++));
}


void printBin(uchar* u, int length){
    int i, j;

    printf("[ ");
    if(length == 0){
	printf("]");
	return;
    }
    for(i  = 0; i < length - 1; i++){
	for(j = BYTE_SIZE - 1; j >= 0; j--){
	    printf("%d ", (*u >> j) & 1);
	}
	printf("| ");
	u++;
    }
    for(j = BYTE_SIZE - 1; j >= 0 ; j--){
	printf("%d ", (*u >> j) & 1);
    }
    printf("]");
}



uchar getBit(uchar t, int position){
    return (t >> position) & 1;
}



uchar setBit(uchar t, int position, uchar value){
    return value ? t | (1 << position) : t & (~(1 << position)); 
}


int HammingWeightByte(uchar c){
    int cnt = 0;
    while (c) {
        cnt += c & 1;
        c >>= 1;
    }
    return cnt;
}


int HammingWeight(buffer_t *buf){
    int cnt = 0;
    for (size_t i = 0; i < buf->length; ++i) {
        cnt += HammingWeightByte(buf->tab[i]);
    }
    return cnt;
}



void oneTimePad(buffer_t *encrypted, buffer_t *msg, buffer_t *key) {
    if (msg->length != key->length) {
        perror("Message and key should have the same length.\n");
        return;
    }
    buffer_reset(encrypted);
    for (size_t i = 0; i < msg->length; ++i) {
        buffer_append_uchar(encrypted, msg->tab[i] ^ key->tab[i]);
    }
}
