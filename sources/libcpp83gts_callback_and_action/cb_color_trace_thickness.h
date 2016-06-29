#ifndef cb_color_trace_thickness_h
#define cb_color_trace_thickness_h

#include "calcu_color_trace_hab.h"

class cb_color_trace_thickness {
public:
	cb_color_trace_thickness()
	{
	}
	void set_bla_hue_min(const double val) {this->src_bla_hue_min_=val;}
	void set_bla_hue_max(const double val) {this->src_bla_hue_max_=val;}
	void set_bla_sat_min(const double val) {this->src_bla_sat_min_=val;}
	void set_bla_sat_max(const double val) {this->src_bla_sat_max_=val;}
	void set_bla_val_min(const double val) {this->src_bla_val_min_=val;}
	void set_bla_val_max(const double val) {this->src_bla_val_max_=val;}

	void set_red_hue_min(const double val) {this->src_red_hue_min_=val;}
	void set_red_hue_max(const double val) {this->src_red_hue_max_=val;}
	void set_red_sat_min(const double val) {this->src_red_sat_min_=val;}
	void set_red_sat_max(const double val) {this->src_red_sat_max_=val;}
	void set_red_val_min(const double val) {this->src_red_val_min_=val;}
	void set_red_val_max(const double val) {this->src_red_val_max_=val;}

	void set_gre_hue_min(const double val) {this->src_gre_hue_min_=val;}
	void set_gre_hue_max(const double val) {this->src_gre_hue_max_=val;}
	void set_gre_sat_min(const double val) {this->src_gre_sat_min_=val;}
	void set_gre_sat_max(const double val) {this->src_gre_sat_max_=val;}
	void set_gre_val_min(const double val) {this->src_gre_val_min_=val;}
	void set_gre_val_max(const double val) {this->src_gre_val_max_=val;}

	void set_blu_hue_min(const double val) {this->src_blu_hue_min_=val;}
	void set_blu_hue_max(const double val) {this->src_blu_hue_max_=val;}
	void set_blu_sat_min(const double val) {this->src_blu_sat_min_=val;}
	void set_blu_sat_max(const double val) {this->src_blu_sat_max_=val;}
	void set_blu_val_min(const double val) {this->src_blu_val_min_=val;}
	void set_blu_val_max(const double val) {this->src_blu_val_max_=val;}

	void init( void );
	void cb_preset_01( const int choice );
	void cb_preset_02( const int choice );
	void cb_preset_03( const int choice );
	void cb_preset_04( const int choice );

	void cb_src_01( void );
	void cb_src_02( void );
	void cb_src_03( void );
	void cb_src_04( void );

	void cb_scrbar_01( const double value );
	void cb_scrbar_02( const double value );
	void cb_scrbar_03( const double value );
	void cb_scrbar_04( const double value );

	void cb_valinp_01( const double value );
	void cb_valinp_02( const double value );
	void cb_valinp_03( const double value );
	void cb_valinp_04( const double value );

	//-------------------------------------

	void cb_enh_01( void );
	void cb_enh_02( void );
	void cb_enh_03( void );
	void cb_enh_04( void );

	void set_scrbar_inpval( E_COLOR_TRACE_HAB_COLORS col_tra_num );

private:
	// BL
	double src_bla_hue_min_=  0.;
	double src_bla_hue_max_=  0.;
	double src_bla_sat_min_=  0.;
	double src_bla_sat_max_=  0.;
	double src_bla_val_min_=  0.;
	double src_bla_val_max_=  0.;
	const int tgt_bla_red_ =  0;
	const int tgt_bla_gre_ =  0;
	const int tgt_bla_blu_ =  0;

	// R
	double src_red_hue_min_=  0.;
	double src_red_hue_max_=  0.;
	double src_red_sat_min_=  0.;
	double src_red_sat_max_=  0.;
	double src_red_val_min_=  0.;
	double src_red_val_max_=  0.;
	const int tgt_red_red_ =0xff;
	const int tgt_red_gre_ =  0;
	const int tgt_red_blu_ =  0;

	// G
	double src_gre_hue_min_=  0.;
	double src_gre_hue_max_=  0.;
	double src_gre_sat_min_=  0.;
	double src_gre_sat_max_=  0.;
	double src_gre_val_min_=  0.;
	double src_gre_val_max_=  0.;
	const int tgt_gre_red_ =  0;
	const int tgt_gre_gre_ =0xff;
	const int tgt_gre_blu_ =  0;

	// B
	double src_blu_hue_min_=  0.;
	double src_blu_hue_max_=  0.;
	double src_blu_sat_min_=  0.;
	double src_blu_sat_max_=  0.;
	double src_blu_val_min_=  0.;
	double src_blu_val_max_=  0.;
	const int tgt_blu_red_ =  0;
	const int tgt_blu_gre_ =  0;
	const int tgt_blu_blu_ =0xff;

	/* "Color Trace Enhancement"ウインドウの各tgtボタン色設定し再表示 */
	void set_tgt_color_(
		const E_COLOR_TRACE_HAB_COLORS trace_list_pos
		, const int color_choice_from_gui
	);

	/* "Color Trace Enhancement"ウインドウの各scr,val値を設定再表示 */
	void set_src_color_(
		const E_COLOR_TRACE_HAB_COLORS trace_list_pos
		, const int color_choice_from_gui
	);

	/* "Color Trace Enhancement"ウインドウの各val値を、
	"Thickness"ウインドウの各値に移し再表示 */
	void set_thickness_(
		const E_COLOR_TRACE_HAB_COLORS trace_list_pos
		, const int color_choice_from_gui
	);
};

#endif /* !cb_color_trace_thickness_h */