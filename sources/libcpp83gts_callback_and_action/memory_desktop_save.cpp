#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>
#include "ptbl_returncode.h"
#ifdef _WIN32
#include "osapi_mbs_wcs.h"	// osapi::cp932_from_utf8(-)
#endif
#include "osapi_getusername.h"
#include "osapi_gethostname.h"
#include "pri.h"
#include "memory_desktop.h"
#include "gts_gui.h"
#include "gts_master.h"

int memory_desktop::_save_by_fp( FILE *fp )
{
	int i_ret;
	const char	*ccp_show = this->str_show_,
			*ccp_hide = this->str_hide_;

	i_ret = fprintf(fp, "# %s : %s %s\n"
	,cl_gts_master.cp_release_name()
	,cl_gts_master.cp_release_number()
	,cl_gts_master.cp_release_date()
	);
	if (i_ret < 0) { return NG; }

	time_t tt = time(nullptr);
	i_ret = fprintf(fp, "# date time : %s"
	,asctime(localtime(&tt))
	);
	if (i_ret < 0) { return NG; }

#ifdef UNICODE
	std::string usern( osapi::mbs_from_wcs( osapi::getusername() ) );
	std::string hostn( osapi::mbs_from_wcs( osapi::gethostname() ) );
#else
	std::string usern( osapi::getusername() );
	std::string hostn( osapi::gethostname() );
#endif
	if (usern.empty()) { usern = "unknown"; }
	i_ret = fprintf( fp, "# username  : %s\n" ,usern.c_str() );
	if (i_ret < 0) { return NG; }
	if (hostn.empty()) { hostn = "unknown"; }
	i_ret = fprintf( fp, "# hostname  : %s\n\n" ,hostn.c_str() );
	if (i_ret < 0) { return NG; }

	//---------------------------------------

	i_ret = fprintf(fp, "%-32s %s %d %d %d %d\n"
	, this->str_window_main_
	, cl_gts_gui.window_main_view->visible()?ccp_show:ccp_hide
	, cl_gts_gui.window_main_view->x()
	, cl_gts_gui.window_main_view->y()
	, cl_gts_gui.window_main_view->w()
	, cl_gts_gui.window_main_view->h()
	);
	if (i_ret < 0) { return NG; }

	i_ret = fprintf(fp, "%-32s %s %d %d\n"
	, this->str_window_next_scan_
	, cl_gts_gui.window_next_scan->visible()?ccp_show:ccp_hide
	, cl_gts_gui.window_next_scan->x()
	, cl_gts_gui.window_next_scan->y()
	);
	if (i_ret < 0) { return NG; }

	i_ret = fprintf(fp, "%-32s %s %d %d\n"
	, this->str_window_area_and_rot90
	, cl_gts_gui.menite_area_and_rot90->value()?ccp_show:ccp_hide
	, cl_gts_gui.window_area_and_rot90->x()
	, cl_gts_gui.window_area_and_rot90->y()
	);
	if (i_ret < 0) { return NG; }

	i_ret = fprintf(fp, "%-32s %s %d %d %d %d\n"
	, this->str_window_pixel_type_and_bright_
	, cl_gts_gui.menite_pixel_type_and_bright->value()?ccp_show:ccp_hide
	, cl_gts_gui.window_pixel_type_and_bright->x()
	, cl_gts_gui.window_pixel_type_and_bright->y()
	, cl_gts_gui.window_pixel_type_and_bright->w()
	, cl_gts_gui.window_pixel_type_and_bright->h()
	);
	if (i_ret < 0) { return NG; }

	i_ret = fprintf(fp, "%-32s %s %d %d %d %d\n"
	, this->str_window_scan_and_save_
	, cl_gts_gui.menite_scan_and_save->value()?ccp_show:ccp_hide
	, cl_gts_gui.window_scan_and_save->x()
	, cl_gts_gui.window_scan_and_save->y()
	, cl_gts_gui.window_scan_and_save->w()
	, cl_gts_gui.window_scan_and_save->h()
	);
	if (i_ret < 0) { return NG; }

	i_ret = fprintf(fp, "%-32s %s %d %d %d %d\n"
	, this->str_window_trace_files_
	, cl_gts_gui.menite_trace_files->value()?ccp_show:ccp_hide
	, cl_gts_gui.window_trace_files->x()
	, cl_gts_gui.window_trace_files->y()
	, cl_gts_gui.window_trace_files->w()
	, cl_gts_gui.window_trace_files->h()
	);
	if (i_ret < 0) { return NG; }

	i_ret = fprintf(fp, "%-32s %s %d %d %d %d\n"
	, this->str_window_number_
	, cl_gts_gui.menite_number->value()?ccp_show:ccp_hide
	, cl_gts_gui.window_number->x()
	, cl_gts_gui.window_number->y()
	, cl_gts_gui.window_number->w()
	, cl_gts_gui.window_number->h()
	);
	if (i_ret < 0) { return NG; }

	i_ret = fprintf(fp, "%-32s %s %d %d %d %d\n"
	, this->str_window_trace_params_
	, cl_gts_gui.menite_trace_params->value()?ccp_show:ccp_hide
	, cl_gts_gui.window_trace_params->x()
	, cl_gts_gui.window_trace_params->y()
	, cl_gts_gui.window_trace_params->w()
	, cl_gts_gui.window_trace_params->h()
	);
	if (i_ret < 0) { return NG; }

	i_ret = fprintf(fp, "%-32s %s %d %d %d %d\n"
	, this->str_window_trace_hsv_view_
	, cl_gts_gui.menite_trace_hsv_view->value()?ccp_show:ccp_hide
	, cl_gts_gui.window_trace_hsv_view->x()
	, cl_gts_gui.window_trace_hsv_view->y()
	, cl_gts_gui.window_trace_hsv_view->w()
	, cl_gts_gui.window_trace_hsv_view->h()
	);
	if (i_ret < 0) { return NG; }

	i_ret = fprintf(fp, "%-32s %s %d %d %d %d\n"
	, this->str_window_trace_batch_
	, cl_gts_gui.menite_trace_batch->value()?ccp_show:ccp_hide
	, cl_gts_gui.window_trace_batch->x()
	, cl_gts_gui.window_trace_batch->y()
	, cl_gts_gui.window_trace_batch->w()
	, cl_gts_gui.window_trace_batch->h()
	);
	if (i_ret < 0) { return NG; }

	i_ret = fprintf(fp, "%-32s %s %d %d %d %d\n"
	, this->str_window_trace_hue_minmax_
	, cl_gts_gui.menite_trace_hue_minmax->value()?ccp_show:ccp_hide
	, cl_gts_gui.window_trace_hue_minmax->x()
	, cl_gts_gui.window_trace_hue_minmax->y()
	, cl_gts_gui.window_trace_hue_minmax->w()
	, cl_gts_gui.window_trace_hue_minmax->h()
	);
	if (i_ret < 0) { return NG; }

# ifndef _WIN32
    if(cl_gts_master.cl_iip_scan.device_name()) {
        i_ret = fprintf(fp, "%-32s \"%s\"\n"
	, this->str_sane_device_name_
	, cl_gts_master.cl_iip_scan.device_name()
	);
        if(i_ret < 0) {
            return NG;
        }
    }
# endif

	return OK;
}
int memory_desktop::_save_startup_path_by_fp( FILE *fp )
{
	int i_ret;

	i_ret = fprintf(fp, "%-32s \"%s\"\n"
		,this->str_config_dir_path_
		,cl_gts_master.cl_config.get_dir_path().c_str()
	);
	i_ret = fprintf(fp, "%-32s \"%s\"\n"
		,this->str_image_dir_path_
		,cl_gts_master.cl_image.get_dir_path().c_str()
	);
	if (i_ret < 0) { return NG; }

	return OK;
}

int memory_desktop::_save_by_fname( const char *cp_fname )
{
	FILE *fp;

#ifdef _WIN32
	fp = fopen( osapi::cp932_from_utf8( cp_fname ).c_str() , "w" );
#else
	fp = fopen( cp_fname , "w" );
#endif
	if (NULL == fp) {
		pri_funct_err_bttvr(
		"fopen(%s,w) returns NULL.",cp_fname);
		return NG;
	} 

	if (OK != this->_save_by_fp( fp )) {
		pri_funct_err_bttvr(
		 "this->_save_by_fp(-) returns NG.");
		return NG;
	}
	if (OK != this->_save_startup_path_by_fp( fp )) {
		pri_funct_err_bttvr(
		 "this->_save_startup_path_by_fp(-) returns NG.");
		return NG;
	}
	if (fclose(fp)) {
		pri_funct_err_bttvr("fclose(-) returns not zero.");
		return NG;
	}
	return OK;
}

int memory_desktop::save( void )
{
	if (this->set_desktop_file_path_() != OK) {
		pri_funct_err_bttvr(
		 "set_desktop_file_path_(-) can not set dir");
		return NG;
	}

	if (OK != this->_save_by_fname(this->desktop_file_path_.c_str())) {
		pri_funct_err_bttvr(
		"memory_desktop::_save_by_fname(-) returns NG.");
		return NG;
	}

	return OK;
}
