#include "igs_resource_cpu_endian.h"

const bool igs::resource::cpu_is_little_endian(void)
{
	union { unsigned short us; unsigned char uc[2]; } buf;
	buf.us = 1;
	/*
		CPU Byte Orderの説明
		Big Endian CPUではbuf.usに1いれるとbuf.uc[1]が1となる
				+---------------+
		unsigned short	|      us       |
				|           1   |
				+-------+-------+
		unsigned char	| uc[0] | uc[1] |
				|   0   |   1   |
				+-------+-------+
		Little Endian CPUではbuf.usに1いれるとbuf.uc[0]が1となる
				+---------------+
		unsigned short	|      us       |
				|   1           |
				+-------+-------+
		unsigned char	| uc[0] | uc[1] |
				|   1   |   0   |
				+-------+-------+
	*/
	return 1 == buf.uc[0]; /* true by Little Endian CPU */
}
