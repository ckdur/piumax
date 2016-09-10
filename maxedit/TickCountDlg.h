#ifndef TICKCOUNTDLG
#define TICKCOUNTDLG

#include "MaxDialog.h"
class CTickCountDlg :
	public CMaxDialog
{
public:
	CTickCountDlg(void);
	~CTickCountDlg(void);

	static void Set(GtkBuilder *builder);

	int DoModal(void);
	int DoNoModal(void);

	static int UpdateData(bool bGet);

	static CMaxDataExchange m_exc;

	// ** Custom controls here
	static int m_nIndexGrab;

	// ** Data here
	static int m_nTickCount;

	// ** Signals here
	static void on_dialog_set_size_response(GtkDialog* dialog,
		gint response_id, gpointer userdata);
	static void on_dialog_set_size_show(GtkDialog* dialog,
		gpointer userdata);

private:
	static GtkWidget *m_main_dlg;
};

#endif // TICKCOUNTDLG



