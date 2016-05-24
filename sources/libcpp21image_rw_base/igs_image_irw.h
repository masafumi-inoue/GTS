#ifndef igs_image_irw_h
#define igs_image_irw_h

#include <cstddef> // size_t

#ifndef IGS_IMAGE_IRW_EXPORT
# define IGS_IMAGE_IRW_EXPORT
#endif

namespace igs {
 namespace image { /* 画像に関する宣言 ---------------------------- */
  IGS_IMAGE_IRW_EXPORT const size_t bytes_of_scanline(
		/* scanline毎のbyteサイズを得る ------------------- */
	const int width ,const int channels ,const int bits
  );

  namespace orientation { /* データの並びに対する画像の向き ------- */
   enum side {
	/* 例で説明 : topleftの場合 : Data並びの
		Raw(scanline)の開始は上(top)、
		Column(pixel)の開始は左(left)
	*/
	topleft = 0	/* Video(=default) */
	, toprigh
	, botrigh
	, botleft	/* OpenGL */
	, lefttop
	, rightop
	, righbot
	, leftbot
	, not_defined	/* 定義してない */ /* defaultはtopleft */
   };
  }
  namespace resolution { /* 解像度の単位 -------------------------- */
   enum unit {
	not_defined=0	/* pic,sgi=定義してない=pixelに1対1=default */
	, meter		/* png(Spec 1.2) */
	, centimeter	/* tif(Spec 6.0) */
	, millimeter
	, inch		/* tif(Spec 6.0)=default */
	, nothing	/* tif(Spec 6.0)単位の指定がないとき */
   };
  }
  struct geometry { /* 幾何図形的配置、大きさ情報 ----------------- */
	geometry(): height(0), width(0), channels(0), bits(0) {}
	int height, width, channels, bits;
  };
  struct attribute { /* 属性、表示形態情報 ------------------------ */
	attribute():
		orie_side(orientation::not_defined)
		, reso_unit(resolution::not_defined)
		, reso_x(0.)
		, reso_y(0.)
	{}
	orientation::side orie_side;
	resolution::unit  reso_unit;
	double	reso_x, reso_y;
		/* tif(Spec 6.0) is float x y resolution */
		/* png(Spec 1.2) is unsigned integer x y resolution */
  };
  struct properties : /* 固有性、特性情報 ------------------------- */
	public geometry
	, public attribute
  {};
 }
}

#include <string>  /* std::string */
#include <sstream> /* std::ostringstream */

namespace igs {
 namespace image {
  class IGS_IMAGE_IRW_EXPORT reader {
		/* 画像ファイル読み込み抽象クラス ----------------- */
  public:
	virtual const char *const name(void) const = 0;/* file書式名 */
	virtual const unsigned int ext_count(void) = 0;/* 拡張子数 */
	virtual const char *const *const ext_lists(void) = 0;/* 拡張子リスト */

	virtual const bool istag(
		const size_t bytes_count
		, const unsigned char *const tag_array
	) = 0;
	virtual void get_info(
		const std::string& file_path
		, std::string& info
	) = 0;
	virtual void get_line_head(
		std::string& line_head
	) = 0;
	virtual void get_line_info(
		const std::string& file_path
		, std::string& line_info
	) = 0;
	virtual void get_size( /* 用意すべきサイズを得る */
		const std::string& file_path
		, size_t& image_bytes	// size_t for x32 and x64
		, size_t& history_bytes	// size_t for x32 and x64
	) = 0;
	virtual void get_data(
		const std::string& file_path
		, properties& props
		, const size_t image_bytes // size_t for x32 and x64
		, unsigned char *image_array
			/* モノ画像はゼロが黒 */
		, std::string& history_str
			/* date,time,host,user,application,project */
		, bool& break_sw /* 中断Switch。外からtureで中断 */
		, const size_t segment_bytes = 1000000
			/* 分割読込の個々サイズ(tif以外で使用する) */
	) = 0;

	/* 派生Classのdestructorを呼び出すため仮想destructorの空定義 */
	virtual ~reader(){}
  };
 }
}

namespace igs {
 namespace image {
  class IGS_IMAGE_IRW_EXPORT writer {
		/* 画像ファイル書き出し抽象クラス ----------------- */
  public:
	virtual const char *const name(void) const = 0;/* file書式名 */
	virtual const unsigned int ext_count(void) = 0;
	virtual const char *const *const ext_lists(void) = 0;

	virtual const unsigned int compression_count(void) = 0;
	virtual const char *const *const compression_lists(void) = 0;

	virtual const bool error_from_properties(
		const properties& props
		, const int ext_number
		, std::ostringstream& error_or_warning_msg
	) = 0;
	virtual void put_data(
		const properties& props
		, const size_t image_bytes // size_t for x32 and x64
		, const unsigned char *image_array
			/* モノ画像はゼロが黒 */
		, const int compression_number
		, const std::string& history_str
			/* date,time,host,user,application,project */
		, const std::string& file_path
	) = 0;

	/* 派生Classのdestructorを呼び出すため仮想destructorの空定義 */
	virtual ~writer(){}
  };
 }
}

#endif /* !igs_image_irw_h */
