#include <iostream>
#ifndef igs_tga_header_h
#define igs_tga_header_h

namespace igs {
 namespace tga {
  namespace header {

   namespace compression {
    enum e_type {
        uncompressed = 0,	/* 圧縮をしない         */
        rle = 1,		/* ランレングス圧縮法   */
    };
   }

   /******namespace image_type {
    namespace pixel_type {
	const unsigned char color_map  = 1;
	const unsigned char full_color = 2;
	const unsigned char grayscale  = 3;
    }
    namespace compress_bit {
	const unsigned char rle = 0x08;
    }
   }******/

   /* R/W専用ファイルヘッダ構造

説明：なぜR/W専用にファイルヘッダ構造が必要か
ヘッダー全体igs::tga::read::header::parameter param_p;
をまとめて読み込むと(rhel4及びvc2005mdにて)、
-------------------------------+-------+----------
member				bytes	address
-------------------------------+-------+----------
id_length			1	0xbfe9aaa0
cmap_type			1	0xbfe9aaa1
image_type			1	0xbfe9aaa2
cmap_index_of_entry		2	0xbfe9aaa4
cmap_count_of_entries		2	0xbfe9aaa6
cmap_bits_depth			1	0xbfe9aaa8
left_term_pos			2	0xbfe9aaaa
lower_term_pos			2	0xbfe9aaac
width				2	0xbfe9aaae
height				2	0xbfe9aab0
bits_par_pixel			1	0xbfe9aab2
descriptor			1	0xbfe9aab3
-------------------------------+-------+----------
igs::tga::header::parameters	20	0xbfe9aaa0
-------------------------------+-------+----------
となり、「2bytes境界にアライン」されてしまう。
そこで、cmap...のshortメンバーを二つのunsigned charとすると、
-------------------------------+-------+----------
member				bytes	address
-------------------------------+-------+----------
id_length			1	0xbfe9aa80
cmap_type			1	0xbfe9aa81
image_type			1	0xbfe9aa82
cmap_index_of_entry0		1	0xbfe9aa83
cmap_index_of_entry1		1	0xbfe9aa84
cmap_count_of_entries0		1	0xbfe9aa85
cmap_count_of_entries1		1	0xbfe9aa86
cmap_bits_depth			1	0xbfe9aa87
left_term_pos			2	0xbfe9aa88
lower_term_pos			2	0xbfe9aa8a
width				2	0xbfe9aa8c
height				2	0xbfe9aa8e
bits_par_pixel			1	0xbfe9aa90
descriptor			1	0xbfe9aa91
-------------------------------+-------+----------
igs::tga::header::parameters	18	0xbfe9aa80
-------------------------------+-------+----------
となり、OK
shortに戻す必要あり。
   */
   struct parameters_rw {
	parameters_rw():
		id_length(0)
		, cmap_type(0)
		, image_type(0)
		, cmap_index_of_entry0(0)
		, cmap_index_of_entry1(0)
		, cmap_count_of_entries0(0)
		, cmap_count_of_entries1(0)
		, cmap_bits_depth(0)
		, left_term_pos(0)
		, lower_term_pos(0)
		, width(0)
		, height(0)
		, bits_par_pixel(0)
		, descriptor(0)
	{}
	unsigned char id_length;
	unsigned char cmap_type;
	unsigned char image_type;
	unsigned char cmap_index_of_entry0;
	unsigned char cmap_index_of_entry1;
	unsigned char cmap_count_of_entries0;
	unsigned char cmap_count_of_entries1;
	unsigned char cmap_bits_depth;
	short left_term_pos;
	short lower_term_pos;
	short width;
	short height;
	unsigned char bits_par_pixel;
	unsigned char descriptor;
   };
   /* ファイルヘッダ構造 */
   struct alignas(1) parameters {
	parameters():
		id_length(0)
		, cmap_type(0)
		, image_type(0)
		, cmap_index_of_entry(0)
		, cmap_count_of_entries(0)
		, cmap_bits_depth(0)
		, left_term_pos(0)
		, lower_term_pos(0)
		, width(0)
		, height(0)
		, bits_par_pixel(0)
		, descriptor(0)
	{}
	unsigned char id_length;	/* ID field
					0=なし
					1<=文字数
					*/
	unsigned char cmap_type;	/* ColorMap field
					0=ColorMapなし
					1=ColorMapあり
					*/
	unsigned char image_type;	/* 画像 field
					0=画像なし
					1=ColorMap(256色)(無圧縮)
					2=FullColor(BGRA/BGR)(無圧縮)
					3=Grayscale(白黒)(無圧縮)
					9=ColorMap(256色)(RLE圧縮)
					10=FullColor(BGRA/BGR)(RLE圧縮)
					11=Grayscale(白黒)(RLE圧縮)
					*/

	short cmap_index_of_entry;	/* 0値の時のcmap位置 */
	short cmap_count_of_entries;
	unsigned char cmap_bits_depth;	/* 15,16,24,32bits
					32bits=BGRA
					24bits=BGR
					16bits=ARRRRRGG GGGBBBBB(bit)
					15bits=xRRRRRGG GGGBBBBB(bit)
					*/

	short left_term_pos;		/* 画像左端位置 0固定 */
	short lower_term_pos;		/* 画像下端位置 0固定 */
	short width;			/* 画像幅 */
	short height;			/* 画像高さ */
	unsigned char bits_par_pixel;	/* 画像深度(8,16,24,32Bits)*/
	unsigned char descriptor;	/* 画像属性
				bit0...3=各Pixelに関係する属性のBits数
					TGA16=0 or 1
					TGA24=0
					TGA32=8
				bit4=左右格納方向
					0=左から右
					1=右から左
				bit5=上下格納方向
					0=下から上
					1=上から下
				bit6,7=データ保存時のInterleave状態
					00=non-interleaved
					01=two-way(even/odd)interleaving
					10=four way interleaving
					11=reserved
					*/
   };
  }
 }
}

#endif	/* !igs_tga_header_h */

int main(int argc ,char *argv[])
{
	std::cout
	<< "parameters_rw size=" << sizeof(igs::tga::header::parameters_rw)
	<< std::endl
	<< "parameters    size=" << sizeof(igs::tga::header::parameters)
	<< std::endl
	;
	return 0;
}
/*
01 char or short
parameters_rw size=18
parameters    size=20

02 short height --> long height
parameters_rw size=24
parameters    size=24
 */
