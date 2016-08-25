#ifndef cb_file_number_list_h
#define cb_file_number_list_h

class cb_file_number_list {
public:
	cb_file_number_list()
		:crnt_list_num_(-1)
		,crnt_file_num_(-1)
		,next_list_num_(-1)
		,next_file_num_(-1)
		,crnt_list_num_for_reset_(-1)
		,crnt_file_num_for_reset_(-1)
		,endress_sw_(false)
	{
	}

	//--------------------------------------------------

	/* Insert項目に入力したフレーム番号をリストに追加(sort)セットする */
	void set_list_from_string( void );

	//--------------------------------------------------

	/* ファイル存在マークを付加したファイル番号をlistの最後に追加 */
	/* level okの時使用 */
	void append_fnum_list_with_chk_mark( const int file_num );

	/* 指定範囲の番号でlistを追加生成する(ファイル存在マーク付き) */
	/* configの設定時使用 */
	void make_fnum_list_with_chk_mark( const int start_num, const int end_num );

	//--------------------------------------------------

	void select_all( void );	/* 全て選択状態にする */
	void remove_all( void );	/* 選択に関わらずすべて削除 */
	void remove_selected( void );	/* 選択状態の項目をすべて削除 */
	void unselect( int list_num );	/* 選択状態を外す */
	int marking_scan_file( int list_num );/* Scan  fileの存在Mark付加 */
	int marking_trace_file(int list_num );/* Trace fileの存在Mark付加 */

	//--------------------------------------------------

	int get_crnt_list_num(void) const { return this->crnt_list_num_; }
	int get_crnt_file_num(void) const { return this->crnt_file_num_; }
	int get_next_file_num(void) const { return this->next_file_num_; }

	int set_first_number( void );
	void reset_next_number( void );
	int set_next_number( void );
	void reset_next_to_crnt_number( void );
	void set_next_to_crnt_number( void );

	//--------------------------------------------------

	void set_endress_sw( bool sw ) { this->endress_sw_ = sw; }

private:
	int	crnt_list_num_
		, crnt_file_num_
		, next_list_num_
		, next_file_num_
		, crnt_list_num_for_reset_
		, crnt_file_num_for_reset_
		;
	bool endress_sw_;

	int _next_selected( int i_list_num );
};

#endif /* !cb_file_number_list_h */
