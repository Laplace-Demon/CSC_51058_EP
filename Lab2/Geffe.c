/**************************************************************/
/* Geffe.c                                                    */
/* Author : Alain Couvreur                                    */
/* alain.couvreur@lix.polytechnique.fr                        */
/* Last modification September 24, 2018                       */
/**************************************************************/

#include <stdio.h>
#include "buffer.h"
#include "bits.h"
#include "LFSR.h"
#include "Geffe.h"
#define DEBUG 1


void Geffe(buffer_t *output, buffer_t *s1, buffer_t *s2, buffer_t *s3){
	if (s1->length != s2->length || s2->length != s3->length) {
		perror("Input streams should have the same length.\n");
		return;
	}
	size_t length = s1->length;
	buffer_reset(output);
	for (size_t i = 0; i < length; ++i) {
		uchar temp = (s1->tab[i] & s2->tab[i]) ^ (s2->tab[i] & s3->tab[i]) ^ s3->tab[i];
		buffer_append_uchar(output, temp);
	}
}


double correlation(buffer_t *s1, buffer_t *s2){
	if (s1->length != s2->length) {
		perror("Buffers should have the same length.\n");
		return 0;
	}
	int total = 8 * (int)s1->length;
	int same = 0;
	for (size_t i = 0; i < s1->length; ++i) same += HammingWeightByte(~(s1->tab[i] ^ s2->tab[i]));
	return (double)same / (double)total;
}


void searchIV(buffer_t *IV_candidate, buffer_t *stream, buffer_t *trans, double threshold){
	buffer_t stream_candidate;
	int i;
	
	buffer_init(&stream_candidate, trans->length);
	buffer_reset(IV_candidate);
	for(i = 0; i < trans->length; i++) buffer_append_uchar(IV_candidate, 0);

	while (1) {
		LFSR(&stream_candidate, trans, IV_candidate, stream->length);
		if (correlation(&stream_candidate, stream) > threshold) break;
		increment_buffer(IV_candidate);
	}
	
	buffer_clear(&stream_candidate);
}


void positions(buffer_t *output, buffer_t *s1, buffer_t *s3){
	if(s1->length != s3->length){
		perror("Input streams should have the same length.\n");
		return;
	}
	buffer_reset(output);
	for (size_t i = 0; i < s1->length; ++i) buffer_append_uchar(output, s1->tab[i] & (~s3->tab[i]));
}

int match_at(buffer_t *s, buffer_t *s1, buffer_t *pos){
	if(s->length != s1->length || s->length != pos->length){
		perror("Input buffers should have the same lengths\n");
		return -1;
	}
	for (size_t i = 0; i < s->length; ++i)
		if ((pos->tab[i] & s->tab[i]) != (pos->tab[i] & s1->tab[i])) return 0; 
	return 1;
}


void search_with_match(buffer_t *IV_candidate, buffer_t *stream,
					   buffer_t *trans, buffer_t *pos){
	buffer_t stream_candidate;
	int i;
	
	buffer_init(&stream_candidate, stream->length);
	buffer_reset(IV_candidate);
	for(i = 0; i < trans->length; i++) buffer_append_uchar(IV_candidate, 0);
	
	while (1) {
		LFSR(&stream_candidate, trans, IV_candidate, stream->length);
		if (match_at(stream, &stream_candidate, pos)) break;
		increment_buffer(IV_candidate);
	}	
	
	buffer_clear(&stream_candidate);
}


void attack(buffer_t *IV_candidate1, buffer_t *IV_candidate2,
			   buffer_t *IV_candidate3, buffer_t *stream,
			   buffer_t *trans1, buffer_t *trans2, buffer_t *trans3,
			   double threshold){
	searchIV(IV_candidate1, stream, trans1, 0.75 - 0.05);
	searchIV(IV_candidate3, stream, trans3, 0.75 - 0.05);
	buffer_t pos, stream1, stream3;
	buffer_init(&pos, stream->length);
	buffer_init(&stream1, stream->length);
	buffer_init(&stream3, stream->length);
	LFSR(&stream1, trans1, IV_candidate1, stream->length);
	LFSR(&stream3, trans3, IV_candidate3, stream->length);
	positions(&pos, &stream1, &stream3);
	search_with_match(IV_candidate2, stream, trans2, &pos);
	buffer_clear(&pos);
}
