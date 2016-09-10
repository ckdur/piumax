#ifndef MAXSMCONVCHOOSEDLG
#define MAXSMCONVCHOOSEDLG

#include "MaxDialog.h"
#include "stdafx.h"
class CMaxSMConvChoose :
	public CMaxDialog
{
public:
	CMaxSMConvChoose(void);
	~CMaxSMConvChoose(void);

	static void Set(GtkBuilder *builder);

	int DoModal(void);
	int DoNoModal(void);

	static int UpdateData(bool bGet);

	static CMaxDataExchange m_exc;

	// ** Custom controls here
	static GtkTreeView* m_treeview_smconv_modes;
	static int m_nIndexGrab;

	// ** Data here

	// ** External
	static CStringArray m_vnGroups;
	static int m_nSel;

	// ** Signals here
	static void on_dialog_set_size_response(GtkDialog* dialog,
		gint response_id, gpointer userdata);
	static void on_dialog_set_size_show(GtkDialog* dialog,
		gpointer userdata);

private:
	static GtkWidget *m_main_dlg;
};

#endif // MAXSMCONVCHOOSEDLG
