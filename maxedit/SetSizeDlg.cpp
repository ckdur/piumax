#include "SetSizeDlg.h"
#include "main_window.h"
#include "main_document.h"

GtkWidget *CSetSizeDlg::m_main_dlg;

CMaxDataExchange CSetSizeDlg::m_exc;

// ** Custom controls here
GtkTreeView* CSetSizeDlg::m_treeview_step_size_group;
GtkTreeView* CSetSizeDlg::m_treeview_step_size_mod;

// ** Data here
vector<int> CSetSizeDlg::m_vnGroups;
vector<int> CSetSizeDlg::m_vnActive;
vector<int> CSetSizeDlg::m_vnMods;
int CSetSizeDlg::m_nIndexGrab;

CSetSizeDlg::CSetSizeDlg(void)
{
	m_nIndexGrab = -1;
	m_vnGroups.clear();
	m_vnActive.clear();
	m_vnMods.clear();
}


CSetSizeDlg::~CSetSizeDlg(void)
{
}

void CSetSizeDlg::Set(GtkBuilder* builder)
{
	m_main_dlg = g_slice_new( GtkWidget );
	m_main_dlg = GTK_WIDGET( gtk_builder_get_object( builder, "dialog_set_size" ) );

	// Declare controls here
	m_treeview_step_size_group = GTK_TREE_VIEW (
				gtk_builder_get_object( builder, "treeview_step_size_group" ) );
	m_treeview_step_size_mod = GTK_TREE_VIEW (
				gtk_builder_get_object( builder, "treeview_step_size_mod" ) );

	// Declare data exchanges here
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

int CSetSizeDlg::DoModal(void)
{
	gtk_window_set_modal( GTK_WINDOW(m_main_dlg), true);
	gint r = gtk_dialog_run ( GTK_DIALOG( m_main_dlg ) );
	return r;
}

int CSetSizeDlg::DoNoModal(void)
{
	gtk_window_set_modal( GTK_WINDOW(m_main_dlg), false);
	gtk_widget_show_all ( GTK_WIDGET( m_main_dlg ) );
	g_signal_connect_swapped (m_main_dlg,
                             "delete_event",
                             G_CALLBACK (gtk_widget_hide_on_delete),
                             m_main_dlg);
	return 1;
}


int CSetSizeDlg::UpdateData(bool bGet)
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

void CSetSizeDlg::on_dialog_set_size_response(GtkDialog* dialog, 
		gint response_id, gpointer userdata)
{
	if(response_id == GTK_RESPONSE_ACCEPT)
	{
		UpdateData(true);
		GtkTreeIter iter;
		GtkTreeModel *model = gtk_tree_view_get_model(m_treeview_step_size_group);
		int value;
	
		for(gboolean b = gtk_tree_model_get_iter_first(model, &iter); 
				b!=FALSE; b=gtk_tree_model_iter_next(model, &iter))
		{
			gboolean check;
			gint nsize;
			gtk_tree_model_get(model, &iter, 1, &value, 2, &check, 3, &nsize, -1);
			m_vnGroups[value] = nsize;
			m_vnActive[value] = check;
		}

		model = gtk_tree_view_get_model(m_treeview_step_size_mod);
		for(gboolean b = gtk_tree_model_get_iter_first(model, &iter); 
				b!=FALSE; b=gtk_tree_model_iter_next(model, &iter))
		{
			gboolean check;
			gtk_tree_model_get(model, &iter, 1, &value, 2, &check, -1);
			m_vnMods[value] = check;
		}
	}
	gtk_widget_hide(GTK_WIDGET(m_main_dlg));
}

void CSetSizeDlg::on_dialog_set_size_show(GtkDialog* dialog,
										  gpointer userdata)
{
	// INITIAL UPDATE
	if(m_nIndexGrab >= 0) 
		gtk_widget_grab_focus(m_exc.m_data.at(m_nIndexGrab).control);

	int m_nSizeGroups = m_vnGroups.size();

	GtkListStore *store = gtk_list_store_new (4, G_TYPE_STRING, G_TYPE_INT, G_TYPE_BOOLEAN, G_TYPE_INT);
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;
	GtkTreeIter iter;
	m_vnActive.resize(m_nSizeGroups, 0);

	for(int i = 0; i < m_nSizeGroups; i++)
	{
		CStdStringA txt;
		txt.Format("Grupo %d", i+1);
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, 
						   0, (LPCSTR)txt, 
						   1, i, 
						   2, (gboolean)m_vnActive[i],
						   3, (gint)m_vnGroups[i],
						   -1);
	}

	gtk_tree_view_set_model(m_treeview_step_size_group, GTK_TREE_MODEL( store ));

	GtkTreeSelection *selection = 
			gtk_tree_view_get_selection(m_treeview_step_size_group);
	if(gtk_tree_model_get_iter_first(GTK_TREE_MODEL( store ), &iter) != FALSE)
		gtk_tree_selection_select_iter(selection, &iter);
	g_object_unref (G_OBJECT (store));

	GtkTreeViewColumn* col =
		gtk_tree_view_get_column(m_treeview_step_size_group, 0);
	if(col == NULL)
	{
		renderer = gtk_cell_renderer_text_new ();
		column = gtk_tree_view_column_new_with_attributes ("Grupos", renderer,
                                                      "text", 0,
                                                      NULL);
		gtk_tree_view_insert_column (m_treeview_step_size_group, column, 0);
	}

	col =
		gtk_tree_view_get_column(m_treeview_step_size_group, 1);
	if(col == NULL)
	{
		GtkCellRenderer* ren;
		ren = gtk_cell_renderer_toggle_new ();
		g_object_set(ren, "activatable", TRUE, NULL);
		column = gtk_tree_view_column_new_with_attributes ("Ver", ren,
														"active", 2,
														NULL);
		gtk_tree_view_insert_column (m_treeview_step_size_group, column, 1);
		g_signal_connect(ren, "toggled", 
			(GCallback) on_treeview_step_size_group_toggled_callback, NULL);
	}

	col =
		gtk_tree_view_get_column(m_treeview_step_size_group, 2);
	if(col == NULL)
	{
		GtkCellRenderer* ren;
		GtkAdjustment* adj;
		adj = GTK_ADJUSTMENT(gtk_adjustment_new(0, 0, G_MAXDOUBLE, 1, 10, 0));
		ren = gtk_cell_renderer_spin_new ();
		g_object_set(ren, "editable", TRUE, "adjustment", adj, NULL);
		column = gtk_tree_view_column_new_with_attributes ("Tamano", ren,
														"text", 3,
														NULL);
		gtk_tree_view_insert_column (m_treeview_step_size_group, column, 2);
		g_signal_connect(ren, "edited", 
			(GCallback) on_treeview_step_size_group_edited, NULL);
	}

	// Mods
	int m_nSizeMods = m_vnMods.size();
	store = gtk_list_store_new (3, G_TYPE_STRING, G_TYPE_INT, G_TYPE_BOOLEAN);

	for(int i = 0; i < m_nSizeMods; i++)
	{
		CStdStringA txt;
		switch(i)
		{
		case MAXSTEP_BEATSPLIT:
			txt = "BeatSplit";
			break;
		case MAXSTEP_CHANGEBPM:
			txt = "BPM";
			break;
		case MAXSTEP_CHANGEVEL:
			txt = "Vel";
			break;
		case MAXSTEP_JUMP:
			txt = "Jump";
			break;
		case MAXSTEP_TICKCOUNT:
			txt = "TickCount";
			break;
		case MAXSTEP_STOP:
			txt = "Stop";
			break;
		default:
			txt.Format("Mod Cod: %d", i);
			break;
		}
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, 
						   0, (LPCSTR)txt, 
						   1, i, 
						   2, (gboolean)m_vnMods[i],
						   -1);

	}

	gtk_tree_view_set_model(m_treeview_step_size_mod, GTK_TREE_MODEL( store ));

	selection = 
			gtk_tree_view_get_selection(m_treeview_step_size_mod);
	if(gtk_tree_model_get_iter_first(GTK_TREE_MODEL( store ), &iter) != FALSE)
		gtk_tree_selection_select_iter(selection, &iter);
	g_object_unref (G_OBJECT (store));

	col =
		gtk_tree_view_get_column(m_treeview_step_size_mod, 0);
	if(col == NULL)
	{
		renderer = gtk_cell_renderer_text_new ();
		column = gtk_tree_view_column_new_with_attributes ("Mods", renderer,
                                                      "text", 0,
                                                      NULL);
		gtk_tree_view_insert_column (m_treeview_step_size_mod, column, 0);
	}

	col =
		gtk_tree_view_get_column(m_treeview_step_size_mod, 1);
	if(col == NULL)
	{
		GtkCellRenderer* ren;
		ren = gtk_cell_renderer_toggle_new ();
		g_object_set(ren, "activatable", TRUE, NULL);
		column = gtk_tree_view_column_new_with_attributes ("Ver", ren,
														"active", 2,
														NULL);
		gtk_tree_view_insert_column (m_treeview_step_size_mod, column, 1);
		g_signal_connect(ren, "toggled", 
			(GCallback) on_treeview_step_size_mod_toggled_callback, NULL);
	}

	UpdateData(false);
}

void CSetSizeDlg::on_treeview_step_size_group_toggled_callback (GtkCellRendererToggle *cell,
									   gchar                 *path_string,
									   gpointer               user_data)
{	GtkTreeModel* model = gtk_tree_view_get_model(m_treeview_step_size_group);
	GtkTreeIter iter;
	gtk_tree_model_get_iter_from_string(model,&iter,path_string);
	gboolean active;
	gtk_tree_model_get(model, &iter, 2, &active, -1);
	active = active?0:1;
	gtk_list_store_set(GTK_LIST_STORE(model), &iter, 2, active, -1);
	
}

void CSetSizeDlg::on_treeview_step_size_group_edited (GtkCellRendererSpin *renderer,
					gchar *path,
					gchar *text,
					gpointer user_data)
{	GtkTreeModel* model = gtk_tree_view_get_model(m_treeview_step_size_group);
	GtkTreeIter iter;

	if (gtk_tree_model_get_iter_from_string (model, &iter, path))
	{
		gint value, rep;
		gtk_tree_model_get (model, &iter, 3, &value, -1);
		if(sscanf(text, "%d", &rep)==1)
			value = rep;
	
		gtk_list_store_set (GTK_LIST_STORE (model), &iter, 3, value, -1);
	}
	
}

void CSetSizeDlg::on_treeview_step_size_mod_toggled_callback (GtkCellRendererToggle *cell,
									   gchar                 *path_string,
									   gpointer               user_data)
{	GtkTreeModel* model = gtk_tree_view_get_model(m_treeview_step_size_mod);
	GtkTreeIter iter;
	gtk_tree_model_get_iter_from_string(model,&iter,path_string);
	gboolean active;
	gtk_tree_model_get(model, &iter, 2, &active, -1);
	active = active?0:1;
	gtk_list_store_set(GTK_LIST_STORE(model), &iter, 2, active, -1);
	
}