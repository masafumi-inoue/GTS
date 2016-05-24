#if defined _WIN32 // win32 or win64
#include "igs_resource_msg_from_err_win.h"
#else
#include "igs_resource_msg_from_err_unix.h"
#endif
/*
"/DUNICODE"定義してコンパイル --> WideCharactor(2byte文字)
定義なしでコンパイル          --> MultiBytes(1〜2byte文字)
*/
