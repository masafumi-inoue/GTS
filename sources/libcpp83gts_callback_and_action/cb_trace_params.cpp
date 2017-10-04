#include <cassert>
#include <iostream>
#include "cb_trace_params.h"
#include "ids_path_level_from_files.h"
#include "gts_gui.h"
#include "gts_master.h"

namespace {
void set_target_rgb_( const int number , uchar r , uchar g , uchar b )
{
	Fl::set_color( static_cast<Fl_Color>( number + 16 ) ,r ,g ,b );
}
void get_target_rgb_( const int number , uchar& r , uchar& g , uchar& b )
{
	Fl::get_color( static_cast<Fl_Color>( number + 16 ) ,r ,g ,b );
}
void set_target_color_( const int number , Fl_Button* button )
{
	button->color( static_cast<Fl_Color>( number + 16 ) );
}
} // namespace

//---------------

void cb_trace_params::init_color(void)
{
	std::vector<cb_trace_params::widget_set>& vwset(this->widget_sets);
	for (int ii = 0 ; ii < static_cast<int>( vwset.size() ) ; ++ii) {
		cb_trace_params::widget_set& wset( vwset.at( ii ) );

		/* カラーテーブル番号を目的色ボタンの色に設定 */
		set_target_color_( ii ,wset.button_target_rgb );
	}
}

void cb_trace_params::cb_open_color_editor(
	Fl_Double_Window* flwin
	,Fl_Button* flbut
	,const int number
)
{
	assert( number < static_cast<int>(this->widget_sets.size()) );

	/* rgb値を記憶 */
	get_target_rgb_( number ,this->r_ ,this->g_ ,this->b_ );

	/* 色テーブル番号を記憶 */
	this->number_ = number;

	/* Color Editorにrgb初期値を設定 */
	cl_gts_gui.valinp_set_color_red->value(this->r_);
	cl_gts_gui.scrbar_set_color_red->value(this->r_);
	cl_gts_gui.valinp_set_color_gre->value(this->g_);
	cl_gts_gui.scrbar_set_color_gre->value(this->g_);
	cl_gts_gui.valinp_set_color_blu->value(this->b_);
	cl_gts_gui.scrbar_set_color_blu->value(this->b_);

	/* window開く */
	cl_gts_gui.window_set_color->position(
		 flwin->x() + flbut->x() - 10
		,flwin->y() + flbut->y() - 130
	);
	cl_gts_gui.window_set_color->show();
}

void cb_trace_params::cb_change_color(void)
{
	/* Color Editorのrgb値をGUIカラーテーブルにセット */
	set_target_rgb_( this->number_
	,static_cast<uchar>(cl_gts_gui.valinp_set_color_red->value())
	,static_cast<uchar>(cl_gts_gui.valinp_set_color_gre->value())
	,static_cast<uchar>(cl_gts_gui.valinp_set_color_blu->value())
	);

	/* ボタンのターゲット色を再表示 */
	std::vector<cb_trace_params::widget_set>& vwset(this->widget_sets);
	cb_trace_params::widget_set& wset( vwset.at(this->number_) );
	wset.button_target_rgb->redraw();

	/* Image(& hsv viewもredrawしてる)の再表示 */
	cl_gts_gui.image_view->redraw();
}

void cb_trace_params::cb_cancel(void)
{
	/* rgb値を戻す */
	set_target_rgb_( this->number_ ,this->r_ ,this->g_ ,this->b_ );

	/* ボタンのターゲット色を再表示 */
	std::vector<cb_trace_params::widget_set>& vwset(this->widget_sets);
	cb_trace_params::widget_set& wset( vwset.at(this->number_) );
	wset.button_target_rgb->redraw();

	/* OpenGLの表示をredraw */
	cl_gts_gui.image_view->redraw();
}

//---------------

void cb_trace_params::get_target_rgb(
	const int number
	, unsigned char& r , unsigned char& g , unsigned char& b
)
{
	get_target_rgb_( number ,r ,g ,b );
}

void cb_trace_params::set_target_rgb(
	const int number
	, unsigned char r , unsigned char g , unsigned char b
)
{
	set_target_rgb_( number ,r ,g ,b );
}

//---------------

void cb_trace_params::init_widget_set_(void) {
	this->widget_sets = {
	{cl_gts_gui.chebut_trace_0_enable_sw
	,cl_gts_gui.group_trace_0
	,cl_gts_gui.button_trace_0_target_rgb
	,cl_gts_gui.valinp_trace_0_thickness
	,cl_gts_gui.roller_trace_0_thickness
	,cl_gts_gui.valinp_trace_0_hue_min
	,cl_gts_gui.valinp_trace_0_hue_max
	,cl_gts_gui.valinp_trace_0_slope_deg
	,cl_gts_gui.valinp_trace_0_intercept
	,cl_gts_gui.chebut_trace_0_display_sw
	}
	,{cl_gts_gui.chebut_trace_1_enable_sw
	,cl_gts_gui.group_trace_1
	,cl_gts_gui.button_trace_1_target_rgb
	,cl_gts_gui.valinp_trace_1_thickness
	,cl_gts_gui.roller_trace_1_thickness
	,cl_gts_gui.valinp_trace_1_hue_min
	,cl_gts_gui.valinp_trace_1_hue_max
	,cl_gts_gui.valinp_trace_1_slope_deg
	,cl_gts_gui.valinp_trace_1_intercept
	,cl_gts_gui.chebut_trace_1_display_sw
	}
	,{cl_gts_gui.chebut_trace_2_enable_sw
	,cl_gts_gui.group_trace_2
	,cl_gts_gui.button_trace_2_target_rgb
	,cl_gts_gui.valinp_trace_2_thickness
	,cl_gts_gui.roller_trace_2_thickness
	,cl_gts_gui.valinp_trace_2_hue_min
	,cl_gts_gui.valinp_trace_2_hue_max
	,cl_gts_gui.valinp_trace_2_slope_deg
	,cl_gts_gui.valinp_trace_2_intercept
	,cl_gts_gui.chebut_trace_2_display_sw
	}
	,{cl_gts_gui.chebut_trace_3_enable_sw
	,cl_gts_gui.group_trace_3
	,cl_gts_gui.button_trace_3_target_rgb
	,cl_gts_gui.valinp_trace_3_thickness
	,cl_gts_gui.roller_trace_3_thickness
	,cl_gts_gui.valinp_trace_3_hue_min
	,cl_gts_gui.valinp_trace_3_hue_max
	,cl_gts_gui.valinp_trace_3_slope_deg
	,cl_gts_gui.valinp_trace_3_intercept
	,cl_gts_gui.chebut_trace_3_display_sw
	}
	,{cl_gts_gui.chebut_trace_4_enable_sw
	,cl_gts_gui.group_trace_4
	,cl_gts_gui.button_trace_4_target_rgb
	,cl_gts_gui.valinp_trace_4_thickness
	,cl_gts_gui.roller_trace_4_thickness
	,cl_gts_gui.valinp_trace_4_hue_min
	,cl_gts_gui.valinp_trace_4_hue_max
	,cl_gts_gui.valinp_trace_4_slope_deg
	,cl_gts_gui.valinp_trace_4_intercept
	,cl_gts_gui.chebut_trace_4_display_sw
	}
	,{cl_gts_gui.chebut_trace_5_enable_sw
	,cl_gts_gui.group_trace_5
	,cl_gts_gui.button_trace_5_target_rgb
	,cl_gts_gui.valinp_trace_5_thickness
	,cl_gts_gui.roller_trace_5_thickness
	,cl_gts_gui.valinp_trace_5_hue_min
	,cl_gts_gui.valinp_trace_5_hue_max
	,cl_gts_gui.valinp_trace_5_slope_deg
	,cl_gts_gui.valinp_trace_5_intercept
	,cl_gts_gui.chebut_trace_5_display_sw
	}
	,{cl_gts_gui.chebut_trace_6_enable_sw
	,cl_gts_gui.group_trace_6
	,cl_gts_gui.button_trace_6_target_rgb
	,cl_gts_gui.valinp_trace_6_thickness
	,cl_gts_gui.roller_trace_6_thickness
	,cl_gts_gui.valinp_trace_6_hue_min
	,cl_gts_gui.valinp_trace_6_hue_max
	,cl_gts_gui.valinp_trace_6_slope_deg
	,cl_gts_gui.valinp_trace_6_intercept
	,cl_gts_gui.chebut_trace_6_display_sw
	}
	};
}

//---------------

void cb_trace_params::set_params_for_speedup(
	std::vector<calcu_sep_hsv>& param_sets
)
{
	assert( this->widget_sets.size() == param_sets.size() );

	for (unsigned ii = 0 ; ii < this->widget_sets.size() ; ++ii) {
		auto& wset = this->widget_sets.at(ii);
		auto& vset = param_sets.at(ii);

		vset.enable_sw = (wset.chebut_enable_sw->value()==1
				?true:false);

		uchar r=0,g=0,b=0;
		this->get_target_rgb( ii,r,g,b );
		const double mx = (std::numeric_limits<uchar>::max)();

		vset.target_r = static_cast<double>(r) / mx;
		vset.target_g = static_cast<double>(g) / mx;
		vset.target_b = static_cast<double>(b) / mx;
		vset.thickness = wset.valinp_thickness->value()
				/wset.valinp_thickness->maximum();
		vset.hue_min = wset.valinp_hue_min->value();
		vset.hue_max = wset.valinp_hue_max->value();
		vset.slope_deg = wset.valinp_slope_deg->value();
		vset.intercept = wset.valinp_intercept->value()
				/wset.valinp_intercept->maximum();
	}
}

void cb_trace_params::cb_swap_widget_set( const unsigned num1 , const unsigned num2 )
{
	assert( num1 < this->widget_sets.size() );
	assert( num2 < this->widget_sets.size() );

	auto& wset1 = this->widget_sets.at(num1);
	auto& wset2 = this->widget_sets.at(num2);

	int tmpint = 0;
	tmpint = wset1.chebut_enable_sw->value();
	wset1.chebut_enable_sw->value( wset2.chebut_enable_sw->value() );
	wset2.chebut_enable_sw->value(tmpint);
	if (	wset1.chebut_enable_sw->value()==1) {
		wset1.group_trace->activate();
	} else {wset1.group_trace->deactivate(); }
	if (	wset2.chebut_enable_sw->value()==1) {
		wset2.group_trace->activate();
	} else {wset2.group_trace->deactivate(); }

	uchar r1=0 ,g1=0 ,b1=0, r2=0 ,g2=0 ,b2=0;
	this->get_target_rgb( num1 ,r1,g1,b1 );
	this->get_target_rgb( num2 ,r2,g2,b2 );
	this->set_target_rgb( num1 ,r2,g2,b2 );
	this->set_target_rgb( num2 ,r1,g1,b1 );
	wset1.button_target_rgb->redraw();
	wset2.button_target_rgb->redraw();

	double tmpdbl = 0.;
	tmpdbl = wset1.valinp_thickness->value();
	wset1.valinp_thickness->value( wset2.valinp_thickness->value() );
	wset1.roller_thickness->value( wset2.valinp_thickness->value() );
	wset2.valinp_thickness->value(tmpdbl);
	wset2.roller_thickness->value(tmpdbl);

	tmpdbl = wset1.valinp_hue_min->value();
	wset1.valinp_hue_min->value( wset2.valinp_hue_min->value() );
	wset2.valinp_hue_min->value(tmpdbl);

	tmpdbl = wset1.valinp_hue_max->value();
	wset1.valinp_hue_max->value( wset2.valinp_hue_max->value() );
	wset2.valinp_hue_max->value(tmpdbl);

	tmpdbl = wset1.valinp_slope_deg->value();
	wset1.valinp_slope_deg->value( wset2.valinp_slope_deg->value() );
	wset2.valinp_slope_deg->value(tmpdbl);

	tmpdbl = wset1.valinp_intercept->value();
	wset1.valinp_intercept->value( wset2.valinp_intercept->value() );
	wset2.valinp_intercept->value(tmpdbl);

	tmpint = wset1.chebut_display_sw->value();
	wset1.chebut_display_sw->value( wset2.chebut_display_sw->value() );
	wset2.chebut_display_sw->value(tmpint);
}

//--------------------

void fltk_button_trace_params::draw()
{
	Fl_Button::draw();
}

namespace {

/* エラーがあればその情報を文字で返す、成功ならempty文字列を返す */
const std::string dnd_paste_( const std::string &dnd_str )
{
	/* 複数のファイルパスはエラー */
	if (std::string::npos != dnd_str.find("\n")) {
		return "Error : Need Only 1 Filepath";
	}

	/* 必要な情報に変える */
	std::string dpath , head , num_form , ext;
	int number=-1;
	std::vector<int> nums;
	ids::path::level_from_files(
		dnd_str ,dpath ,head ,num_form ,number ,ext ,nums
	);

	/* Config file */
	if (ext == ".txt") {
		if (cl_gts_master.cl_memo_config.load_only_trace_params(
		dnd_str ) == NG) {
		 return "Error : loading trace_params in config";
		}
	}
	/* 拡張子が対応外エラー */
	else {
		return "Error : Need extension .txt";
	}
	return std::string();
}

} // namespace

#include <FL/fl_ask.H>	// fl_alert(-) 
int fltk_button_trace_params::handle( int event )
{
	switch (event) {
	case FL_DND_ENTER:
	case FL_DND_DRAG:
	case FL_DND_RELEASE:
		return 1;

	case FL_PASTE: // DND Paste
	{
		std::string err(dnd_paste_( Fl::event_text() ));
		if (!err.empty()) {
			fl_alert( err.c_str() );
		}
	}
		return 1;

	default:
		/* pass other events to the base class */
		return Fl_Button::handle(event);
	}
	return 0;
}

//---------------
