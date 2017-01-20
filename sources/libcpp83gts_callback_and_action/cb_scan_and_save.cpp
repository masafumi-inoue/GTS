#include <cstdio> // std::rename(-)
#include <utility> // std::swap(-)
#include <sstream> // std::ostringstream
#include <FL/fl_ask.H>  // fl_alert(-) fl_input(-)
#include "ptbl_funct.h" // ptbl_dir_or_file_is_exist(-)
#include "ptbl_returncode.h"
#include "pri.h"
#include "ids_path_fltk_native_browse.h"
#include "ids_path_level_from_files.h"
#include "cb_scan_and_save.h"
#include "gts_gui.h"
#include "gts_master.h"

//------------------------------------------------------------
/* スキャン＆保存実行 */
void cb_scan_and_save::cb_start( void )
{
	/* 先頭を得る */
	cl_gts_master.cl_number.counter_start(
		cl_gts_gui.choice_scan_num_continue_type->value()
	);

	/* チェック：保存ファイルのLevel名がない */
	{
	std::string name(cl_gts_gui.strinp_scan_save_file_head->value());
	if ( name.empty() ) {
		fl_alert("Need Scan Save Name!");
		return;
	}
	}

	/* 番号選択がない/設定できない */
	if (cl_gts_master.cl_number.get_crnt_file_num() < 1) {
		if (cl_gts_gui.choice_scan_num_continue_type->value()
		== cl_gts_master.cl_number.get_end_type_value()
		) {	/* End type */
			fl_alert("Select Number!");
		}
		else {	/* Endless type */
			fl_alert("Bad Number in Start!");
		}
		return;
	}

	/* カレントのスキャンと保存をして、次があるなら準備もする */
	if (this->next_scan_and_save_() != OK) {
		return;
	}

	/* 次のスキャンがあるなら */
	if (1 <= cl_gts_master.cl_number.get_next_file_num()) {
		/* Spaceに関しては常にここでfocus設定が必要2014-02-03 */
	this->cl_memo_short_cut_key.set_space_key_focus_in_next_scan();

		/* 次をどうするかwindowを表示して指示を仰ぐ */
		cl_gts_gui.window_next_scan->show();
	}
}
void cb_scan_and_save::cb_next( void )
{
	/* windowを消す */
	cl_gts_gui.window_next_scan->hide();

	/* 次の番号を得る */
	cl_gts_master.cl_number.counter_next(
		cl_gts_gui.choice_scan_num_continue_type->value()
	);

	/* カレントのスキャンと保存をして、次があるなら準備もする */
	if (this->next_scan_and_save_() != OK) {
		return;
	}

	/* 次のスキャンがあるなら */
	if (1 <= cl_gts_master.cl_number.get_next_file_num()) {
		/* Spaceに関しては常にここでfocus設定が必要2014-02-03 */
	this->cl_memo_short_cut_key.set_space_key_focus_in_next_scan();

		/* 次をどうするかwindowを表示して指示を仰ぐ */
		cl_gts_gui.window_next_scan->show();
	}
}
void cb_scan_and_save::cb_rescan( void )
{
	/* windowを消す */
	cl_gts_gui.window_next_scan->hide();

	/* カレントのスキャンと保存をして、次があるなら準備もする */
	if ( this->next_scan_and_save_() != OK ) {
		return;
	}

	/* 次のスキャンがあるなら */
	if (1 <= cl_gts_master.cl_number.get_next_file_num()) {
		/* Spaceに関しては常にここでfocus設定が必要2014-02-03 */
	this->cl_memo_short_cut_key.set_space_key_focus_in_next_scan();

		/* 次をどうするかwindowを表示して指示を仰ぐ */
		cl_gts_gui.window_next_scan->show();
	}
}
int cb_scan_and_save::next_scan_and_save_( void )
{
	/* 01 次(始め)の位置を得る */
	int crnt_list_num = cl_gts_master.cl_number.get_crnt_list_num();
	int crnt_file_num = cl_gts_master.cl_number.get_crnt_file_num();
	int next_file_num = cl_gts_master.cl_number.get_next_file_num();
	if (crnt_list_num< 1 || crnt_file_num< 1) {
		pri_funct_err_bttvr(
   "Error : crnt_list_num=%d or crnt_file_num=%d"
			,crnt_list_num ,crnt_file_num );
		return NG;
	}

	/* 02 保存するファイルパスを得る
		DirPath/Level.Number.Ext-->例:"C:/users/public/A.0001.tif"
	*/
	std::string fpath_save( this->get_save_path_(crnt_file_num) );
	if (fpath_save.empty()) {
		pri_funct_err_bttvr(
			"Error : this->get_save_path_(%d) returns empty"
			, crnt_file_num );
		return NG;
	}

	/* 03 フレーム番号(crnt_list_num)を表示するようリストをスクロール */
	cl_gts_gui.selbro_fnum_list->middleline(crnt_list_num);

	/* 04 スキャンを実行 */
	iip_canvas* clp_scan = this->_iipg_scan();
	if (nullptr == clp_scan) {
		pri_funct_err_bttvr(
		      "Error : this->_iipg_scan() returns nullptr" );
// std::string str("Error in scaning at next!";
// fl_alert(str.c_str());
		return NG;
	}

	/* 05 回転、2値化、ドットノイズ消去　処理 */
	if (this->rot_and_trace_and_enoise(
		clp_scan ,cl_gts_gui.choice_rot90->value()
	) != OK) {
		return NG;
	}

	/* 06 画像を保存する
		BW/Grayscale/RGB/RGBTraceのどれか
	*/
	if (OK != this->iipg_save(
		&(cl_gts_master.cl_iip_edot)/* Effectの最後Node画像を保存 */
		, const_cast<char *>(fpath_save.c_str())
		, cl_gts_gui.valinp_area_reso->value()
		/* 回転値後を正対として角度ゼロ(default)として保存する */
	) ) {
		pri_funct_err_bttvr(
	 "Error : this->iipg_save(-) returns NG" );
		return NG;
	}

	/* 07 リストに保存済マーク付け "0000" --> "0000 S" */
	cl_gts_master.cl_number.add_S(crnt_list_num);

	/* 08 リストの選択解除 */
	cl_gts_master.cl_number.unselect(crnt_list_num);

	/* 09 画像表示 */
	if (this->redraw_image( clp_scan ,false ,false ) != OK) {
		return NG;
	}

	/* 10 保存するタイプで画像を表示する */
	if ( cl_gts_gui.chkbtn_scan_trace_sw->value() ) {
		/* TracenImage画像のみ表示 */
		cl_gts_master.cb_change_wview_sub();

		/* 画像表示状態をメニューに設定 */
		cl_gts_gui.menite_wview_sub->setonly();
	}
	else {
		/* ScanImage(メイン)画像のみ表示 */
		cl_gts_master.cb_change_wview_main();

		/* 画像表示状態をメニューに設定 */
		cl_gts_gui.menite_wview_main->setonly();
	}

	/* 11 切抜きはしないのでOFFにしておく */
	this->cl_ogl_view.set_crop_disp_sw(OFF);

	/* 12 次のフレーム番号があるなら、
	次の処理を促すwindowの設定をしておく */
	if (0 < cl_gts_master.cl_number.get_next_file_num()) {
		char	ca8_but[8];
		(void)sprintf( ca8_but ,"%d" ,crnt_file_num );
		cl_gts_gui.norout_crnt_scan_number->value(ca8_but);

		(void)sprintf( ca8_but ,"%d" ,next_file_num );
		cl_gts_gui.norout_next_scan_number->value(ca8_but);

		cl_gts_gui.norout_crnt_scan_level->value(
		 cl_gts_gui.strinp_scan_save_file_head->value()
		);
	}

	return OK;
}

//------------------------------------------------------------
/* ファイルブラウズ/フォルダーブラウズ */
void cb_scan_and_save::cb_browse_save_folder( void )
{
	/* NativeブラウザーOpenで開く */
	const std::string filepath = ids::path::fltk_native_browse_save(
		"Set Saving Folder(&File) for Scan"
		,cl_gts_gui.filinp_scan_save_dir_path->value()
		,this->get_save_name_(
		static_cast<int>(cl_gts_gui.valinp_scan_num_start->value())
		)
		,this->ext_save.get_native_filters()
		,cl_gts_gui.choice_scan_save_image_format->value()
	).at(0);

	/* Cancel */
	if (filepath.empty()) {
		return;
	}

	/* 必要な情報に変える */
	std::string dpath , head , num , ext;
	int number=-1;
	std::vector<int> nums;
	ids::path::level_from_files(
		filepath ,dpath ,head ,num ,number ,ext ,nums
	);

	/* ファイルヘッド(Level名)が空だとなにもしない */
	if (head.empty()) {
		fl_alert("No Head of SaveFileName");
		return;
	}

	/* 01 拡張子が対応した種類かどうか確認 */
	if (!ext.empty()) {
	 const int ext_num = this->ext_save.num_from_str( ext );
	 if ( ext_num < 0 ) {
		fl_alert("Bad Extension\"%s\" of SaveFileName",ext.c_str());
		return;
	 }
	}

	/* Save設定 */
	cl_gts_gui.filinp_scan_save_dir_path->value(dpath.c_str());
/*
	cl_gts_gui.strinp_scan_save_file_head->value(head.c_str());
	cl_gts_gui.strinp_scan_save_number_format->value(num.c_str());
	if (!ext.empty()) {
	 const int ext_num = this->ext_save.num_from_str( ext );
	 cl_gts_gui.choice_scan_save_image_format->value(ext_num);
	}
*/
}

//------------------------------------------------------------
/* numberセット表示/file存在確認表示 */
void cb_scan_and_save::cb_set_number( void )
{
	/* 番号範囲(start ,end)を得る */
	int num_s = -1;
	int num_e = -1;
	if (cl_gts_gui.choice_scan_num_continue_type->value()
	== cl_gts_master.cl_number.get_endless_type_value()) {
 num_s = static_cast<int>(cl_gts_gui.valinp_scan_num_start->value());
 num_e = num_s;
	}
	else {
 num_s = static_cast<int>(cl_gts_gui.valinp_scan_num_start->value());
 num_e = static_cast<int>(cl_gts_gui.valinp_scan_num_end->value());
		if (num_e < num_s) {
			std::swap( num_s ,num_e );
		}
	}

	/* 全削除 */
	cl_gts_master.cl_number.remove_all();
	/* Numberウインドウ List再構築 ファイル存在マーク付ける */
	for (int num =num_s ;num <= num_e ; ++num) {
		if (ptbl_dir_or_file_is_exist(const_cast<char*>(
			this->get_save_path_(num).c_str()
		))) {
		 cl_gts_master.cl_number.append_with_S(num);
		}
		else {
		 cl_gts_master.cl_number.append_without_S(num);
		}
	}
	/* 全選択 */
	cl_gts_master.cl_number.select_all();

	/* Numberウインドウ 保存level名表示 */
	cl_gts_gui.norout_crnt_scan_level_of_fnum->value(
	 cl_gts_gui.strinp_scan_save_file_head->value()
	);

	/* 即表示 */
	cl_gts_gui.window_number->flush();
}

//------------------------------------------------------------
/* 保存する連番ファイルが存在するならファイル名の背景を黄色表示 */
void cb_scan_and_save::cb_check_existing_saved_file(void)
{
	bool overwrite_sw = this->is_exist_save_files_();

	if (overwrite_sw) {	/* 上書き */
		Fl_Color col = FL_YELLOW;
		cl_gts_gui.strinp_scan_save_file_head->color(col);
		cl_gts_gui.strinp_scan_save_file_head->redraw();
	} else {	/* 新規ファイル */
		Fl_Color col = FL_WHITE;
		cl_gts_gui.strinp_scan_save_file_head->color(col);
		cl_gts_gui.strinp_scan_save_file_head->redraw();
	}
}
bool cb_scan_and_save::is_exist_save_files_(void)
{
/* Numberの非選択含めた番号ファイルで一つでも存在するならtrueを返す */
	for (int ii = 1; ii <= cl_gts_gui.selbro_fnum_list->size(); ++ii) {
		std::string filepath( this->get_save_path_(
			std::stoi( /* リストの項目に表示した番号 */
				cl_gts_gui.selbro_fnum_list->text(ii)
			)
		) );
		if (ptbl_dir_or_file_is_exist(const_cast<char*>(
			filepath.c_str()
		))) {
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------------
/* save file/path */
const std::string cb_scan_and_save::get_save_path_( const int number )
{
	std::string filepath;
	filepath += cl_gts_gui.filinp_scan_save_dir_path->value();
	filepath += '/';
	filepath += this->get_save_name_( number );
	return filepath;
}
const std::string cb_scan_and_save::get_save_name_( const int number )
{
	std::string filename;
	filename += cl_gts_gui.strinp_scan_save_file_head->value();
	if (filename.empty()) {
	 filename += "untitled";
	}
	if (0 <= number) {
	 filename += ids::path::str_from_number(
	   number , cl_gts_gui.strinp_scan_save_number_format->value()
	 );
	}
	filename += cl_gts_gui.choice_scan_save_image_format->text();
	return filename;
}

//----------------------------------------------------------------------
void cb_scan_and_save::cb_choice_and_num_continue_type(const std::string& type)
{
	const Fl_Menu_Item* crnt =
	cl_gts_gui.choice_scan_num_continue_type->find_item( type.c_str() );
	if (crnt == nullptr) { return; }

	/* End/Endless表示のChoice切替 */
	cl_gts_gui.choice_scan_num_continue_type->value(crnt);

	/* End/Endless表示のChoice以外の切替 */
	this->cb_change_num_continue_type( type );
}
void cb_scan_and_save::cb_change_num_continue_type(const std::string& type)
{
	if (std::string("End") == type) {
		cl_gts_gui.valinp_scan_num_end->show();
		cl_gts_gui.choice_scan_num_endless_direction->hide();
		cl_gts_gui.selbro_fnum_list->activate();
	} else
	if (std::string("Endless") == type) {
		cl_gts_gui.valinp_scan_num_end->hide();
		cl_gts_gui.choice_scan_num_endless_direction->show();
		cl_gts_gui.selbro_fnum_list->deactivate();
	}
}

void cb_scan_and_save::cb_choice_num_endless_direction( const std::string& type )
{
	const Fl_Menu_Item *crnt =
	cl_gts_gui.choice_scan_num_endless_direction->find_item(
		type.c_str() );
	if (crnt == nullptr) { return; }

	cl_gts_gui.choice_scan_num_endless_direction->value(crnt);
}

void cb_scan_and_save::cb_choice_save_image_format( const std::string& type );
{
	const Fl_Menu_Item *crnt =
	cl_gts_gui.choice_scan_save_image_format->find_item(
		type.c_str() );
	if (crnt == nullptr) { return; }

	cl_gts_gui.choice_scan_save_image_format->value(crnt);
}

void cb_scan_and_save::cb_switch_scan_filter_trace( const bool sw )
{
	if (sw) {
		cl_gts_gui.chkbtn_scan_filter_trace_sw->box(FL_SHADOW_BOX);
		cl_gts_gui.chkbtn_scan_filter_trace_sw->value(1);//ON
	}
	else {
		cl_gts_gui.chkbtn_scan_filter_trace_sw->box(FL_FLAT_BOX);
		cl_gts_gui.chkbtn_scan_filter_trace_sw->value(0);//OFF
	}
}

void cb_scan_and_save::cb_switch_scan_filter_erase_dot_noise( const bool sw )
{
	if (sw) {
 cl_gts_gui.chkbtn_scan_filter_erase_dot_noise_sw->box(FL_SHADOW_BOX);
 cl_gts_gui.chkbtn_scan_filter_erase_dot_noise_sw->value(1);//ON
	}
	else {
 cl_gts_gui.chkbtn_scan_filter_erase_dot_noise_sw->box(FL_FLAT_BOX);
 cl_gts_gui.chkbtn_scan_filter_erase_dot_noise_sw->value(0);//OFF
	}
}

