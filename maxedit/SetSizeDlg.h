#ifndef SETSIZEDLG
#define SETSIZEDLG

#include "MaxDialog.h"
class CSetSizeDlg :
	public CMaxDialog
{
public:
	CSetSizeDlg(void);
	~CSetSizeDlg(void);

	static void Set(GtkBuilder *builder);

	int DoModal(void);
	int DoNoModal(void);

	static int UpdateData(bool bGet);

	static CMaxDataExchange m_exc;

	// ** Custom controls here
	static GtkTreeView* m_treeview_step_size_group;
	static GtkTreeView* m_treeview_step_size_mod;
	static int m_nIndexGrab;

	// ** Data here

	// ** External
	static vector<int> m_vnGroups;
	static vector<int> m_vnActive;
	static vector<int> m_vnMods;

	// ** Signals here
	static void on_dialog_set_size_response(GtkDialog* dialog,
		gint response_id, gpointer userdata);
	static void on_dialog_set_size_show(GtkDialog* dialog,
		gpointer userdata);	static void on_treeview_step_size_group_toggled_callback (GtkCellRendererToggle *cell,
									   gchar                 *path_string,
									   gpointer               user_data);
	static void on_treeview_step_size_mod_toggled_callback (GtkCellRendererToggle *cell,
									   gchar                 *path_string,
									   gpointer               user_data);
	static void	on_treeview_step_size_group_edited (GtkCellRendererSpin *renderer,
					gchar *path,
					gchar *text,
					gpointer user_data);

private:
	static GtkWidget *m_main_dlg;
};

#endif // SETSIZEDLG
