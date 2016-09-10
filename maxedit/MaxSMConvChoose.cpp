#include "MaxSMConvChoose.h"
#include "main_window.h"
#include "main_document.h"

GtkWidget *CMaxSMConvChoose::m_main_dlg;

CMaxDataExchange CMaxSMConvChoose::m_exc;

// ** Custom controls here
GtkTreeView* CMaxSMConvChoose::m_treeview_smconv_modes;

// ** Data here
CStringArray CMaxSMConvChoose::m_vnGroups;
int CMaxSMConvChoose::m_nIndexGrab;
int CMaxSMConvChoose::m_nSel;

CMaxSMConvChoose::CMaxSMConvChoose(void)
{
	m_nIndexGrab = -1;
	m_nSel = 0;
	m_vnGroups.clear();
}


CMaxSMConvChoose::~CMaxSMConvChoose(void)
{
}

void CMaxSMConvChoose::Set(GtkBuilder* builder)
{
	m_main_dlg = g_slice_new( GtkWidget );
	m_main_dlg = GTK_WIDGET( gtk_builder_get_object( builder, "dialog_smconv_which" ) );

	// Declare controls here
	m_treeview_smconv_modes = GTK_TREE_VIEW (
				gtk_builder_get_object( builder, "treeview_smconv_modes" ) );

	// Declare data exchanges here
	m_exc.Add(GTK_WIDGET(m_treeview_smconv_modes), MAXCONTROL_TREEVIEW, &m_nSel, MAXDATA_INT);
	m_nIndexGrab = (int)(m_exc.m_data.size())-1;

	// Connect signals here
	g_signal_connect(m_main_dlg, 
					 "response", 
					 G_CALLBACK( on_dialog_set_size_response), 
					 NULL);
	g_signal_connect(m_main_dlg, 
					 "show", 
					 G_CALLBACK( on_dialog_set_size_show), 
					 NULL);

}

int CMaxSMConvChoose::DoModal(void)
{
	gtk_window_set_modal( GTK_WINDOW(m_main_dlg), true);
	gint r = gtk_dialog_run ( GTK_DIALOG( m_main_dlg ) );
	return r;
}

int CMaxSMConvChoose::DoNoModal(void)
{
	gtk_window_set_modal( GTK_WINDOW(m_main_dlg), false);
	gtk_widget_show_all ( GTK_WIDGET( m_main_dlg ) );
	g_signal_connect_swapped (m_main_dlg,
                             "delete_event",
                             G_CALLBACK (gtk_widget_hide_on_delete),
                             m_main_dlg);
	return 1;
}


int CMaxSMConvChoose::UpdateData(bool bGet)
{
	int w = m_exc.DoDataExchange(bGet);
	if(w != -1)
	{
		gtk_widget_grab_focus(m_exc.m_data.at(w).control);
		return 0;
	}
	return 1;
}

// ** CUSTOM SIGNALS DECLARATIONS

void CMaxSMConvChoose::on_dialog_set_size_response(GtkDialog* dialog, 
		gint response_id, gpointer userdata)
{
	if(response_id == GTK_RESPONSE_ACCEPT)
	{
		UpdateData(true);
	}
	gtk_widget_hide(GTK_WIDGET(m_main_dlg));
}

void CMaxSMConvChoose::on_dialog_set_size_show(GtkDialog* dialog,
										  gpointer userdata)
{
	// INITIAL UPDATE
	if(m_nIndexGrab >= 0) 
		gtk_widget_grab_focus(m_exc.m_data.at(m_nIndexGrab).control);

	int m_nSizeGroups = m_vnGroups.size();

	GtkListStore *store = gtk_list_store_new (2, G_TYPE_INT, G_TYPE_STRING);
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;
	GtkTreeIter iter;

	for(size_t i = 0; i < m_vnGroups.size(); i++)
	{
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, 
						   0, i, 
						   1, (LPCSTR)m_vnGroups[i], 
						   -1);
	}

	gtk_tree_view_set_model(m_treeview_smconv_modes, GTK_TREE_MODEL( store ));

	GtkTreeSelection *selection = 
			gtk_tree_view_get_selection(m_treeview_smconv_modes);
	if(gtk_tree_model_get_iter_first(GTK_TREE_MODEL( store ), &iter) != FALSE)
		gtk_tree_selection_select_iter(selection, &iter);
	g_object_unref (G_OBJECT (store));

	GtkTreeViewColumn* col =
		gtk_tree_view_get_column(m_treeview_smconv_modes, 0);
	if(col == NULL)
	{
		renderer = gtk_cell_renderer_text_new ();
		column = gtk_tree_view_column_new_with_attributes ("NOTES", renderer,
                                                      "text", 1,
                                                      NULL);
		gtk_tree_view_insert_column (m_treeview_smconv_modes, column, 0);
	}

	

	UpdateData(false);
}

