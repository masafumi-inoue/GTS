#include <limits> /* std::numeric_limits */
#include "igs_image_irw.h"

const size_t igs::image::bytes_of_scanline(
	  const int width
	, const int channels
	, const int bits
) {
	const size_t sl = static_cast<size_t>(width * channels * bits);
	return sl / std::numeric_limits<unsigned char>::digits +
	 ((0==(sl % std::numeric_limits<unsigned char>::digits))? 0: 1);
}
