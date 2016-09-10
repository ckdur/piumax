#ifndef INCRSTEPDLG
#define INCRSTEPDLG

#include "MaxDialog.h"
class CIncrStepDlg :
	public CMaxDialog
{
public:
	CIncrStepDlg(void);
	~CIncrStepDlg(void);

	static void Set(GtkBuilder *builder);

	int DoModal(void);
	int DoNoModal(void);

	static int UpdateData(bool bGet);

	static CMaxDataExchange m_exc;

	// ** Custom controls here
	static int m_nIndexGrab;

	// ** Data here
	static int m_nSize;

	// ** Signals here
	static void on_dialog_set_size_response(GtkDialog* dialog,
		gint response_id, gpointer userdata);
	static void on_dialog_set_size_show(GtkDialog* dialog,
		gpointer userdata);

private:
	static GtkWidget *m_main_dlg;
};

#endif // INCRSTEPDLG



