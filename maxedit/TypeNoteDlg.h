#ifndef TYPENOTEDLG
#define TYPENOTEDLG

#include "MaxDialog.h"
class CTypeNoteDlg :
	public CMaxDialog
{
public:
	CTypeNoteDlg(void);
	~CTypeNoteDlg(void);

	static void Set(GtkBuilder *builder);

	int DoModal(void);
	int DoNoModal(void);

	static int UpdateData(bool bGet);

	static CMaxDataExchange m_exc;

	// ** Custom controls here
	static GtkTreeView* m_treeview_type_note_type;
	static GtkComboBox* m_combo_type_note;
	static GtkLabel* m_label_descr_type_note;
	static int m_nIndexGrab;

	// ** Data here
	static int m_nSel;		// This is the type
	static int m_nValue;	// This is the value
	static int m_nArg1;		// Arg1
	static int m_nArg2;		// Arg2
	static int m_nType;		// This is first instance

	// ** Signals here
	static void on_dialog_type_note_response(GtkDialog* dialog,
		gint response_id, gpointer userdata);
	static void on_dialog_type_note_show(GtkDialog* dialog,
		gpointer userdata);

	static void on_combo_type_note_changed (GtkComboBox *widget, gpointer user_data);
	static void on_tree_view_type_note_type_changed (GtkTreeView *tree_view, gpointer user_data);

private:
	static GtkWidget *m_main_dlg;
};

#endif // TYPENOTEDLG
