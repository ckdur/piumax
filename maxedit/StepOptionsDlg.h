#ifndef STEPOPTIONSDLG
#define STEPOPTIONSDLG

#include "MaxDialog.h"

struct STEPOPT_GROUP
{
	int nNoMiss;
	int nNoCount;
	int nIsVanishSup;
	int nIsVanishInf;
	int nIsSkin;
	float fVanishSup;
	float fVanishInf;
	int nSkin;
	int nGroup;	// This is not data from dialog
};

class CStepOptionsDlg :
	public CMaxDialog
{
public:
	CStepOptionsDlg(void);
	~CStepOptionsDlg(void);

	static void Set(GtkBuilder *builder);

	int DoModal(void);
	int DoNoModal(void);

	static int UpdateData(bool bGet);

	static CMaxDataExchange m_exc;

	// ** Custom controls here
	static GtkTreeView* m_treeview_step_opt_group;
	static GtkTreeView* m_treeview_step_opt_attrib;
	static int m_nIndexGrab;

	// ** Data here
	// Internal
	static int m_nDelay;
	static int m_nRuntime;
	static float m_fAttrib;
	static int m_nLevel;
	static int m_nNoMiss;
	static int m_nNoCount;
	static int m_nIsVanishSup;
	static int m_nIsVanishInf;
	static int m_nIsSkin;
	static float m_fVanishSup;
	static float m_fVanishInf;
	static int m_nSkin;
	static int m_nArrowsPerLine;

	// External
	static vector<STEPOPT_GROUP> m_vGroup;
	static vector<float> m_vfAttrib;
	static gint m_nResponse;

	// ** Signals here
	static void on_dialog_set_size_response(GtkDialog* dialog,
		gint response_id, gpointer userdata);
	static void on_dialog_set_size_show(GtkDialog* dialog,
		gpointer userdata);
	static void on_button_step_opt_newattrib_clicked(GtkDialog* dialog,
		gpointer userdata);
	static void on_button_step_opt_eraseattrib_clicked(GtkDialog* dialog,
		gpointer userdata);
	static void on_button_step_opt_newgroup_clicked(GtkDialog* dialog,
		gpointer userdata);
	static void on_button_step_opt_erasegroup_clicked(GtkDialog* dialog,
		gpointer userdata);
	static void on_treeview_step_opt_group_change(GtkWidget* widget,
		gpointer userdata);
	static void on_treeview_step_opt_attrib_change(GtkWidget* widget,
		gpointer userdata);

private:
	static GtkWidget *m_main_dlg;
};

#endif // STEPOPTIONSDLG
