#ifndef cb_file_number_list_h
#define cb_file_number_list_h

class cb_file_number_list {
public:
	cb_file_number_list()
		:crnt_list_num_(-1)
		,crnt_file_num_(-1)
		,next_list_num_(-1)
		,next_file_num_(-1)
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

	//--------------------------------------------------

	void unselect( int list_num );	/* 選択状態を外す */
	int marking_scan_file( int list_num );/* Scan  fileの存在Mark付加 */
	int marking_trace_file(int list_num );/* Trace fileの存在Mark付加 */

	//--------------------------------------------------

	/* 画像の番号をたどる場合と動作
		-スキャンする
			--> Endlessの場合list番号とfile番号は一致する
			--> Endlessの-1の場合1で止まる。+1ではEndless
		-トレスする
			--> Endlessであってもlist選択部分のみの処理
		-プレビューする
			--> 選択した最初の一枚を得る。
	*/
	int get_crnt_list_num(void) const { return this->crnt_list_num_; }
	int get_crnt_file_num(void) const { return this->crnt_file_num_; }
	int get_next_file_num(void) const { return this->next_file_num_; }

	/* 画像の番号をたどる方法を指定するための番号(scan時はGUI値から) */
	int get_end_type_value(void) const { return this->end_type_value_; }
	int get_endless_type_value(void) const {
		return this->endless_type_value_;
	}

	/* 画像の番号をたどる (file番号とlist番号) */
	/* 1 初期化し、開始位置 */
	void counter_start( const int continue_type_value );
	/* 2 次へ辿り、現位置を得る */
	void counter_next( const int continue_type_value );

private:
	int	crnt_list_num_
		, crnt_file_num_
		, next_list_num_
		, next_file_num_
		;

	enum choice_level_continue_type_value_ {
		end_type_value_		// 0
		,endless_type_value_	// 1
	};

	void set_next_num_from_crnt_( const int continue_type_value );
};

#endif /* !cb_file_number_list_h */