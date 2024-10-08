/**************************************************************/
/* LFSR.c                                                     */
/* Author : Alain Couvreur                                    */
/* alain.couvreur@lix.polytechnique.fr                        */
/* Last modification September 30, 2020 (FJM)                 */
/**************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "buffer.h"
#include "bits.h"
#include "LFSR.h"
#define DEBUG 1


uchar weightMod2(uchar c){
    uchar result = 0;
    int j;
    
    for(j = 0; j < BYTE_SIZE; j++)
	result ^= 1 & (c >> j);
    return result;
}

static uchar getbit(uchar *tab, int i)
{
    uchar byte = tab[i >> 3];
    i = 7 - (i & 7);
    byte >>= i;
    return byte & 1;
}

static void setbit(uchar *tab, int i, uchar x)
{
    int j = i >> 3;
    i = 7 - (i & 7);
    tab[j] |= x << i;
}

/* INPUT: trans[0..L/8[, init IV[0..L/8[
   SIDE-EFFECT: stream[0..stream_length[ <- LFSR with coeffs trans and init IV.
   REQUIRES: L <= stream_length.
   We pack 8 bits in a byte and consider the coeff to be in GF(2).
   A byte is considered as b0b1...b7 (??)
*/
void LFSR(buffer_t *stream, buffer_t *trans, buffer_t *IV, int stream_length){
    int i, j, k;
    if(IV->length != trans->length) {
		perror("ERROR : IV and transition vector "
			"should have the same length\n");
		return;
    }

	buffer_t stream_bis;
    buffer_init(&stream_bis, stream->size);

    buffer_reset(&stream_bis);
	
    /* Initialization */
    buffer_append(&stream_bis, IV);
	stream_length += IV->length;
#if 0		
    /* Computation of the stream: stream[0..L[ is already filled */
    uchar current_bit;
    uchar *state = stream->tab + trans->length;
    uchar *cursor_trans;
    uchar *cursor_state;
    for(i = trans->length; i < stream_length; i++, state++){
	/* initialize the next byte to fill in stream[i] */
	buffer_append_uchar(stream, 0);
	/* construct bit-wise the next byte */
	for(j = 0; j < BYTE_SIZE; j++){
	    cursor_trans = trans->tab; /* start from c_0 */
	    cursor_state = state; /* this is s_t */
	    /* this is the "bit" we will compute in this iteration */
	    current_bit = 0;

	    /* we compute [s_{t+L}]_j=XOR(k=0, L-1) [c_i]_j AND [s_{t+k}]_j */
	    for(k = 0; k < trans->length; k++, cursor_trans++, cursor_state++){
		current_bit ^= weightMod2( (*cursor_state << j)  & *cursor_trans );
		current_bit ^= weightMod2( *(cursor_state + 1) &
					   ( *cursor_trans << (BYTE_SIZE - j) ));
	    }
	    /* order is b7b6...b0 for j=0..7 */
	    *state ^= current_bit << (BYTE_SIZE - 1 - j);
	}
    }
#else
    int t = -1, L = trans->length << 3;
    // uchar byte = 0, tmp;
    for (i = trans->length; i < stream_length; i++) {
	//for(i = trans->length; i < trans->length+1; i++){
	uchar byte = 0, tmp;
	buffer_append_uchar(&stream_bis, byte);
	for (j = 0; j < BYTE_SIZE; j++) {
	    t++;
	    tmp = 0;
#if 0
	    for(k = 0; k < L; k++)
		printf("%u", getbit(trans->tab, k));
	    printf("\n");
	    for(k = 0; k < L; k++)
		printf("%u", getbit(stream->tab, t+k));
	    printf("\n");
#endif
	    for(k = 0; k < L; k++) tmp ^= getbit(trans->tab, k) & getbit(stream_bis.tab, t+k);
	    setbit(stream_bis.tab, t+L, tmp);
	    byte = tmp + (byte << 1);
#if 0
	    printf("i=%d j=%d tmp=%u byte=%u\n", i, j, tmp, byte);
#endif
		}
    }
	// remove the first part of stream which is IV
    buffer_reset(stream);
	for (int i = 0; i < stream_length - IV->length; ++i) 
		buffer_append_uchar(stream, stream_bis.tab[i + IV->length]);
	stream->length = stream_length - IV->length;
	buffer_clear(&stream_bis);
#endif
}



void increment_buffer(buffer_t *buf){
    size_t i = buf->length - 1;
    while (1) {
        if (buf->tab[i] != UCHAR_MAX) {
            buf->tab[i] += 1;
            break;
        }
        if (i == 0) break; 
        buf->tab[i] = 0;
        i -= 1;
    }
}




void bourrinate_IV(buffer_t *searched_IV, buffer_t *trans, buffer_t *stream){
    buffer_t stream_candidate;
    int i;
    
    buffer_init(&stream_candidate, stream->length);
    buffer_reset(searched_IV);
    for(i = 0; i < trans->length; i++) buffer_append_uchar(searched_IV, 0);

    while (1) {
        LFSR(&stream_candidate, trans, searched_IV, stream->length);
        if (buffer_equality(stream, &stream_candidate)) break;
        increment_buffer(searched_IV);
    }
	
    buffer_clear(&stream_candidate);
}
