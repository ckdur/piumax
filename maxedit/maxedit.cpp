#include "main_window.h"
#include "libc_max_common.h"
#include <locale.h>
#include <clocale>

/*#ifdef __cplusplus
extern "C"
#endif*/
int main(int argc, _TCHAR* argv[])
{
	if(!Max_init_iconv()) return 1;
	av_register_all();
    /* Init GTK+ */
    gtk_init( &argc, &argv );

	setlocale(LC_NUMERIC, "C");
	std::setlocale(LC_NUMERIC, "C");
    //setlocale (LC_ALL, "");

    main_window_create();

    main_window_destroy();

	Max_delete_iconv();
    return( 0 );
}

