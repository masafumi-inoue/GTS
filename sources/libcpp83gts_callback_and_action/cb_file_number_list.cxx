#include <cstdlib>	// atoi()
#include <cctype>	// isdigit()
#include <cassert>	// assert()
#include <cstring>
//#include "ptbl_returncode.h"
//#include "pri.h"
#include "cb_file_number_list.h"
#include "gts_gui.h"
#include "gts_master.h"

namespace {
 const char *next_num_( const char* str ,int* rsz ,int* frame_num )
 {
	int	ii = 0 ,jj = 0;
	char	buffer[1024];

	assert( nullptr != str );
	assert( nullptr != rsz );	/* remainssize */
	assert( nullptr != frame_num );

	if ( (*rsz) <= 0 ) {
		return nullptr;
	}

	/* 10進数字('0'〜'9')の文字位置(ii)を探す */
	for ( ;'\0' != str[ii] && 0 < (*rsz) ;++ii ,--(*rsz)) {
		if (isdigit(str[ii])) { break; }
	}

	/* 続いて、10進数字('0'〜'9')以外の文字位置(ii)を探す
	   かつ、10進数字('0'〜'9')の文字を抜出す */
	for ( ;'\0' != str[ii] && 0 < (*rsz) ;++ii ,--(*rsz)) {
		if (!isdigit(str[ii])) { break; }
		buffer[jj++] = str[ii];
	}

	/* 数値を得る */
	if ( 0 < jj ) {
		buffer[jj] = '\0';
		*frame_num = atoi( buffer );
	}

	/* 続けて、次の10進数字('0'〜'9')の文字位置(ii)を探す */
	for ( ;'\0' != str[ii] && 0 < (*rsz) ;++ii ,--(*rsz)) {
		if (isdigit(str[ii])) {
			return &str[ii]; /* 数値位置発見 */
		}
	}

	/* 現位置で数値を得たが、次の数値文字がない場合 */
	if ( 0 < jj ) {
		return &str[ii-1];
		/* 最後の数値位置(nullptr以外を返すための無意味な値) */
	}

	/* 次の10進数字('0'〜'9')の文字位置(ii)はない */
	return nullptr;
 }
}

void cb_file_number_list::set_list_from_string( void )
{
	const char* str;/* 複数数値含文字列の数値始まり位置へのpointer */
	int	rsz;	/* 残り文字数(Remain SiZe) */
	int	frame_num;
	int	ii ,jj;
	char	ca8_tmp[8];

	/* "1" ,"0001" ,"1 2 3" ,"0001 0002 0003" ,etc. */
	str = cl_gts_gui.norinp_fnum_insert->value();
	rsz = cl_gts_gui.norinp_fnum_insert->size();

	while (nullptr != (str = next_num_( str ,&rsz ,&frame_num ))) {
		/* frame number listから、insertする
		(frame_numと同じかより大きいframe(jj)の)位置(ii)を得る */
		for (ii=1; ii<=cl_gts_gui.selbro_fnum_list->size(); ++ii) {
			jj = atoi( cl_gts_gui.selbro_fnum_list->text(ii) );
			if (frame_num <= jj) { break; }
		}
		/* すでにあるフレームは飛ばして次へ */
		if (jj == frame_num) { continue; }
		/* insert&Scroll for viewing */
		sprintf( ca8_tmp ,"%04d" ,frame_num );
		cl_gts_gui.selbro_fnum_list->insert( ii ,ca8_tmp );
		cl_gts_gui.selbro_fnum_list->middleline( ii );
	}
}

//------------------------------------------------------------

namespace {
 /* ファイルのヘッダー情報を得る */
 int set_file_header_info_( const char *cp_file, iip_read *clp_read )
 {
	assert(nullptr != cp_file);
	assert(nullptr != clp_read);

	/* ファイルが存在しなければ、なにもせず、OKで終る */
	if (!ptbl_dir_or_file_is_exist(cp_file)) {
		return OK;
	}

	/* ファイル名をセットして... */
	if (OK != clp_read->cl_name.set_name(cp_file)) {
		pri_funct_err_bttvr(
 	 "Error : clp_read->cl_name.set_name(%s) returns NG",
			cp_file);
		return NG;
	}
	/* ファイルヘッダー情報を得る */
	if (OK != clp_read->header()) {
		pri_funct_err_bttvr(
 	 "Error : clp_read->header() returns NG");
		return NG;
	}
	return OK;
 }

 /* ファイルの存在マークを文字列で返す
	マークは、"0001 ST"の、ST部分
	画像の種類毎に扱いが違う
	rgbのとき
		S -> Scanファイル("A_full.0001.tif"等)
		T -> Traceファイル("A.0001.tif"等)
	grayscaleのとき
		S -> scanファイル("A.0001.tif")
		T -> 存在しない
	BWのとき
		S -> scanファイル("A.0001.tif")
		T -> 存在しない
 */
 const char* get_mark_str_by_checking_file_( const int file_num )
 {
	iip_read	cl_read_target,
			cl_read_rgbscan;
	const char* cp_path;
	const char	*cp_s = "S",
			*cp_t = "T",
			*cp_st = "ST";

	/* 画像のファイルパスを得る */
	cp_path = cl_gts_master.cl_bro_level.cp_filepath(file_num);
	if (nullptr == cp_path) {
		pri_funct_err_bttvr(
	 "Error : cl_gts_master.cl_bro_level.cp_filepath(%d) returns nullptr."
			, file_num );
		return NG;
	}

	/* 画像ファイルの情報を取る */
	if (OK != set_file_header_info_(cp_path, &cl_read_target )) {
		pri_funct_err_bttvr(
 	 "Error : set_file_header_info_(%s,) returns NG",
			cp_path);
		return NG;
	}

	/* ファイルヘッドに_fullを付加した画像ファイルパスを得る */
	cp_path = cl_gts_master.cl_bro_level.cp_filepath_full(file_num);
	if (nullptr == cp_path) {
		pri_funct_err_bttvr(
	 "Error : cl_gts_master.cl_bro_level.cp_filepath_full(%d) returns nullptr."
	 		, file_num );
		return NG;
	}
	/* ファイルヘッドに_fullを付加した画像ファイルの情報を取る */
	if (OK != set_file_header_info_(cp_path, &cl_read_rgbscan )) {
		pri_funct_err_bttvr(
 	 "Error : set_file_header_info_(%s,) returns NG",
			cp_path);
		return NG;
	}

	/* モノクロ２値画像 */
	if (E_CH_NUM_BITBW ==
	cl_read_target.cl_ch_info.get_e_ch_num_type()) { return cp_s; }

	/* GrayScale画像 */
	else if (1L == cl_read_target.get_l_channels()) { return cp_s; }

	/* RGB Scan and Trace画像 */
	else if ((3L <= cl_read_target.get_l_channels()) &&
		 (3L <= cl_read_rgbscan.get_l_channels())) {return cp_st;}
	/* RGB Scan画像 */
	else if (3L <= cl_read_rgbscan.get_l_channels()) { return cp_s; }
	/* RGB Trace画像 */
	else if (3L <= cl_read_target.get_l_channels()) { return cp_t; }

	/* ファイルがない */
	else if ((0L == cl_read_target.get_l_channels()) &&
		 (0L == cl_read_rgbscan.get_l_channels())) {return nullptr;}

	pri_funct_err_bttvr( "Error : bad file type" );
	return nullptr;
 }
}

/* ファイル存在マークを付加したファイル番号をlistの最後に追加 */
void cb_file_number_list::append_fnum_list_with_chk_mark( const int file_num )
{
	char buffer[8];
	const char *mark_str;

	(void)sprintf(buffer, "%04d", file_num );

	mark_str = get_mark_str_by_checking_file_(file_num);
	/* ファイルチェックでエラーが起きたとしても実行を続ける */
	if ( mark_str != nullptr ) {
		(void)strcat( buffer ," " );
		(void)strcat( buffer ,mark_str );
	}

	cl_gts_gui.selbro_fnum_list->insert(
		cl_gts_gui.selbro_fnum_list->size()+1,
		buffer
	);
}

/* 指定範囲の番号でlistを追加生成する(ファイル存在マーク付き) */
void cb_file_number_list::make_fnum_list_with_chk_mark( const int start_num, const int end_num )
{
	int	ii;
	/* ファイルの存在をチェックしながらリストを設定 */
	for (ii = start_num; ii <= end_num; ++ii) { 
		this->append_fnum_list_with_chk_mark( ii );
	}
}

//------------------------------------------------------------

/* 全て選択状態にする */
void cb_file_number_list::select_all( void )
{
	for (int ii = 1; ii <= cl_gts_gui.selbro_fnum_list->size(); ++ii) {
		cl_gts_gui.selbro_fnum_list->select(ii);
	}
}

/* 選択に関わらずすべて削除 */
void cb_file_number_list::remove_all( void )
{
	while (0 < cl_gts_gui.selbro_fnum_list->size()) { 
	   	cl_gts_gui.selbro_fnum_list->remove(1);
	}
}

/* 選択状態の項目をすべて削除 */
void cb_file_number_list::remove_selected( void )
{
	for (int ii = 1; ii <= cl_gts_gui.selbro_fnum_list->size();) {
		if ( cl_gts_gui.selbro_fnum_list->selected(ii) ) {
			cl_gts_gui.selbro_fnum_list->remove(ii);
		} else { ++ii; }
	}
}

/* 選択状態を外す */
void cb_file_number_list::unselect( int list_num )
{
	cl_gts_gui.selbro_fnum_list->select(list_num, 0);
}

/* Scan image fileの存在Mark付加
	"0000"--> "0000 S" , "0000 T" --> "0000 ST" */
int cb_file_number_list::marking_scan_file( int list_num )
{
	const char *list_text:
	char buffer[8];

	list_text = cl_gts_gui.selbro_fnum_list->text(list_num);
	if (nullptr == list_text) {
		pri_funct_err_bttvr(
	"Error : cl_gts_gui.selbro_fnum_list->text(%d) returns nullptr"
			, list_num
		);
		return NG;
	}

	if (4 == strlen(list_text)) {
		/* "0001"のとき"0001 S"に変更する */
		(void)strcpy( buffer, list_text );
		(void)strcat( buffer, " S" );
		cl_gts_gui.selbro_fnum_list->text(list_num, buffer);
	} else
	/* "0001 S" or "0001 T" */
	if (6 == strlen(list_text)) {
		if ('T' == list_text[5]) {
			/* "0001 T"のとき"0001 ST"に変更する */
			(void)strcpy( buffer, list_text );
			buffer[5] = 'S';
			buffer[6] = 'T';
			buffer[7] = '\0';
			cl_gts_gui.selbro_fnum_list->text(list_num, buffer);
		}
		/* "0001 S"ではなにもしない */
	}
	/* "0001 ST"ではなにもしない */

	return OK;
}

/* Trace image fileの存在Mark付加
	"0000"--> "0000 T" , "0000 S" --> "0000 ST" */
int cb_file_number_list::marking_trace_file( int list_num )
{
	const char *list_text;
	char buffer[8];

	list_text = cl_gts_gui.selbro_fnum_list->text(list_num);
	if (nullptr == list_text) {
		pri_funct_err_bttvr(
	 "Error : cl_gts_gui.selbro_fnum_list->text(%d) returns nullptr"
			, list_num
		);
		return NG;
	}

	if (4 == strlen(list_text)) {
		/* "0001"のとき"0001 T"に変更する */
		(void)strcpy( buffer, list_text );
		(void)strcat( buffer, " T" );
		cl_gts_gui.selbro_fnum_list->text(list_num, buffer);
	} else
	/* "0001 S" or "0001 T" */
	if (6 == strlen(list_text)) {
		if ('S' == list_text[5]) {
			/* "0001 S"のとき"0001 ST"に変更する */
			(void)strcpy( buffer, list_text );
			buffer[5] = 'S';
			buffer[6] = 'T';
			buffer[7] = '\0';
			cl_gts_gui.selbro_fnum_list->text(list_num, buffer);
		}
		/* "0001 T"ではなにもしない */
	}
	/* "0001 ST"ではなにもしない */

	return OK;
}

//----------

namespace {
 bool check_next_selected_( int list_num )
 {
	for (;list_num<=cl_gts_gui.selbro_fnum_list->size() ;++list_num) {
		if (cl_gts_gui.selbro_fnum_list->selected(list_num)) {
			return true;
		}
	}
	return false;
 }
}

/* リストを指定番号からサーチし、
次の選択されたリストの番号(1...)を得る
ない場合は-1を返す */
int cb_file_number_list::_next_selected( int list_num )
{
	for (;list_num<=cl_gts_gui.selbro_fnum_list->size() ;++list_num) {
		if (cl_gts_gui.selbro_fnum_list->selected(list_num)) {
			/* EndressスイッチがON、かつ最後の選択だった */
			if (this->endress_sw_ == true
			&& !check_next_selected_(list_num+1)) {
				/* 最後の番号に+1の番号を得る */
				char numarray[8];
				(void)sprintf( numarray, "%04d"
	,atoi( cl_gts_gui.selbro_fnum_list->text(
		cl_gts_gui.selbro_fnum_list->size()
	) ) +1
				);
				/* listに追加して */
				cl_gts_gui.selbro_fnum_list->add(numarray);
				/* 選択状態にする */
				cl_gts_gui.selbro_fnum_list->select(
				 cl_gts_gui.selbro_fnum_list->size()
				);
			}
			return list_num;
		}
	}
	return -1;
}
int cb_file_number_list::set_first_number( void )
{
	const char *cp_tmp;

	/* 01 選択されたフレームの先頭の順番を得る */
	this->crnt_list_num_ = this->_next_selected( 1 );

	/* 02 選択されていないならゼロを返す */
	if (this->crnt_list_num_ < 1) {
		pri_funct_err_bttvr(
			"Error : this->_next_selected(1) returns <%d>",
			this->crnt_list_num_ );
		return 0;
	}

	/* 03 先頭の順番から、フレーム番号名を得る */
	cp_tmp = cl_gts_gui.selbro_fnum_list->text(
			this->crnt_list_num_);

	/* 04 選択されているのにフレーム番号名がない --> エラー */
	if (nullptr == cp_tmp) {
		pri_funct_err_bttvr(
	"Error : cl_gts_gui.selbro_fnum_list->text(%d) returns nullptr",
			this->crnt_list_num_
		);
		return -1;/* エラーリターンはマイナスの値 */
	}

	/* 05 フレーム番号名からフレーム番号を得る */
	this->crnt_file_num_ = atoi(cp_tmp);

	/* 06 次の番号は未定 */
	this->next_list_num_ = -1;
	this->next_file_num_ = -1;

	/* 07 選択されたフレームの先頭の順番(1以上の値)を返す */
	return this->crnt_list_num_;
}
/*
エラー時のリカバリー用
int cb_file_number_list::set_next_number( void )
と対で使うこと
this->next_list_num_とthis->next_file_num_を初期化する
*/
void cb_file_number_list::reset_next_number( void )
{
	this->next_list_num_ = -1;
	this->next_file_num_ = -1;
}
/* this->next_list_num_とthis->next_file_num_を設定する */
int cb_file_number_list::set_next_number( void )
{
	char *cp_tmp;

	/* 01 選択されたフレームの次の順番を得る */
	this->next_list_num_ = this->_next_selected(
	 this->crnt_list_num_ + 1 );

	/* 02 次がなければ(無効)次のフレーム番号を無効にしてこのmethod終 */
	if (this->next_list_num_ < 1) {
	    this->next_file_num_ = -1;
		return OK;
	}

	/* 03 次の順番から、フレーム番号名を得る */
	cp_tmp = (char *)cl_gts_gui.selbro_fnum_list->text(
		this->next_list_num_);

	/* 04 選択されているのにフレーム番号名がない --> エラー */
	if (nullptr == cp_tmp) {
		pri_funct_err_bttvr(
	"Error : cl_gts_gui.selbro_fnum_list->text(%d) returns nullptr",
		this->next_list_num_
		);
		return NG;
	}

	/* 05 フレーム番号名からフレーム番号を得る */
	this->next_file_num_ = atoi( cp_tmp );

	return OK;
}

/*
次スキャンしようとしたらエラーの時のリカバリー用
void cb_file_number_list::set_next_to_crnt_number( void )
と対で使うこと
*/
void cb_file_number_list::reset_next_to_crnt_number( void )
{
	this->crnt_list_num_ = this->crnt_list_num_for_reset_;
	this->crnt_file_num_ = this->crnt_file_num_for_reset_;
}
/* 次のフレームを処理するとき呼ぶ */
void cb_file_number_list::set_next_to_crnt_number( void )
{
	this->crnt_list_num_for_reset_ = this->crnt_list_num_;
	this->crnt_file_num_for_reset_ = this->crnt_file_num_;
	this->crnt_list_num_ = this->next_list_num_;
	this->crnt_file_num_ = this->next_file_num_;
	this->next_list_num_ = -1;
	this->next_file_num_ = -1;
}

