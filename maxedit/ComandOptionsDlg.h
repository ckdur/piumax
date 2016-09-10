#ifndef COMMANDOPTIONSDLG
#define COMMANDOPTIONSDLG

#include "MaxDialog.h"
#include "argstep.h"
class CComandOptionsDlg :
	public CMaxDialog
{
public:
	CComandOptionsDlg(void);
	~CComandOptionsDlg(void);

	static void Set(GtkBuilder *builder);

	int DoModal(void);
	int DoNoModal(void);

	static int UpdateData(bool bGet);

	static CMaxDataExchange m_exc;

	// ** Custom controls here
	static int m_nIndexGrab;

	// ** Data here
	static int m_nSize;
	static ARGSTEP m_arg;
	static int m_nVel;
	static bool m_b05;
	static int m_nRush;

	// ** Signals here
	static void on_dialog_comand_options_response(GtkDialog* dialog,
		gint response_id, gpointer userdata);
	static void on_dialog_comand_options_show(GtkDialog* dialog,
		gpointer userdata);

private:
	static GtkWidget *m_main_dlg;
};

#endif // COMMANDOPTIONSDLG



