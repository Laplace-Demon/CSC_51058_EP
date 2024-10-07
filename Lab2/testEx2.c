/**************************************************************/
/* testEx2.c                                                  */
/* Author: Alain Couvreur                                     */
/* alain.couvreur@lix.polytechnique.fr                        */
/* Estelle Blin, 2024/10/01                                   */
/* Last modification September 30, 2020 (FJM)                 */
/**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "bits.h"
#include "LFSR.h"



void test1() {
    printf("***************** Testing LFSR ********************\n");
    int streamLength = 50;
    int registerLength = 4;
    uchar IV[4] = {178, 20, 146, 183};
    uchar trans[4] = {201, 89, 14, 246};
    uchar verif[50] = {55, 141, 140, 21, 199, 205,
		       210, 240, 178, 108, 2, 49, 37, 17, 107, 206, 246,
		       215, 40, 69, 114, 190, 234, 152, 150, 169, 179,
		       32, 151, 150, 14, 49, 41, 224, 213, 201, 98, 200,
		       231, 207, 184, 228, 224, 97, 141, 184, 193, 185, 13, 60};
    buffer_t buf_IV, buf_trans, buf_verif, stream;
    buffer_init(&buf_IV, registerLength);
    buffer_init(&buf_trans, registerLength);
    buffer_init(&stream, streamLength);
    buffer_init(&buf_verif, streamLength);
    buffer_from_string(&buf_IV, IV, registerLength);
    buffer_from_string(&buf_trans, trans, registerLength);
    buffer_from_string(&buf_verif, verif, streamLength);

    printf("\n\nStream (Stream): \n");
    printBin(stream.tab, stream.length);
    
    LFSR(&stream, &buf_trans, &buf_IV, streamLength);
	
    printf("\nInitial Value : ");
    printBin(IV, registerLength);
    printf("\nTransitions :   ");
    printBin(trans, registerLength);
    printf("\n\nStream (Stream): \n");
    printBin(stream.tab, stream.length);
    // printf("\n\nStream (INT): \n");
    // buffer_print_int(stdout, &stream);
    printf("\n\n");
	
    printf("Longueur : %d\n\n", (int)stream.length);
    if(memcmp(stream.tab, verif, streamLength) == 0)
	printf("[OK]\n\n");
    else
	printf("[Failed]\n\n");
    buffer_clear(&buf_IV);
    buffer_clear(&buf_trans);
    buffer_clear(&buf_verif);
    buffer_clear(&stream);
}


void test2() {
    printf("***************** Testing repartition ********************\n");
    /* You can modify these fields */
    int streamLength = 10000;
    int registerLength = 5;
    uchar IV[5] = {57, 43, 25, 200, 151};
    uchar trans[5] = {138, 86, 137, 117, 77};
    /* End */
    buffer_t buf_IV, buf_trans, buf_stream;
    buffer_init(&buf_IV, registerLength);
    buffer_init(&buf_trans, registerLength);
    buffer_init(&buf_stream, streamLength);
    buffer_from_string(&buf_IV, IV, registerLength);
    buffer_from_string(&buf_trans, trans, registerLength);
	
    printf("\nInitial Value : ");
    printBin(IV, registerLength);
    printf("\nTransitions :   ");
    printBin(trans, registerLength);
    LFSR(&buf_stream, &buf_trans, &buf_IV, streamLength);
    printf("\n\nTotal number of bits = %d.\n", BYTE_SIZE * streamLength);
    printf("Total number of zeroes = %d.\n\n", HammingWeight(&buf_stream));

    buffer_clear(&buf_IV);
    buffer_clear(&buf_trans);
    buffer_clear(&buf_stream);
}

void test3() {
    printf("***************** A small example ********************\n");
    /* You can modify these fields */
    int streamLength = 4;
    int registerLength = 2;
    uchar IV[2] = {0, 1};
    uchar trans[2] = {0, 2};
    /* End */
    buffer_t buf_IV, buf_trans, buf_stream;
    buffer_init(&buf_IV, registerLength);
    buffer_init(&buf_trans, registerLength);
    buffer_init(&buf_stream, streamLength);
    buffer_from_string(&buf_IV, IV, registerLength);
    buffer_from_string(&buf_trans, trans, registerLength);
	
    printf("\nInitial Value : ");
    printBin(IV, registerLength);
    printf("\nTransitions :   ");
    printBin(trans, registerLength);
    LFSR(&buf_stream, &buf_trans, &buf_IV, streamLength);
    printf("\nStream :        ");
    printBin(buf_stream.tab, streamLength);
    printf("\n");

    buffer_clear(&buf_IV);
    buffer_clear(&buf_trans);
    buffer_clear(&buf_stream);
}


void usage(char *s){
    fprintf(stderr, "Usage: %s <test_number in 1..3>\n", s);
}



int main(int argc, char *argv[]){
    if (argc == 1){
        usage(argv[0]);
	    return 0;
    }
    int n = atoi(argv[1]);

    switch (n) {
        case 1:
            test1();
            break;
        case 2:
            test2();
            break;
        case 3:
            test3();
            break;
        default:
            usage(argv[0]);
    }
    return 0;
}
