#include "StepOptionsDlg.h"
#include "main_window.h"

GtkWidget *CStepOptionsDlg::m_main_dlg;

CMaxDataExchange CStepOptionsDlg::m_exc;

// ** Custom controls here
GtkTreeView* CStepOptionsDlg::m_treeview_step_opt_attrib;
GtkTreeView* CStepOptionsDlg::m_treeview_step_opt_group;

// ** Data here
int CStepOptionsDlg::m_nDelay;
int CStepOptionsDlg::m_nRuntime;
float CStepOptionsDlg::m_fAttrib;
int CStepOptionsDlg::m_nLevel;
int CStepOptionsDlg::m_nNoMiss;
int CStepOptionsDlg::m_nNoCount;
int CStepOptionsDlg::m_nIsVanishSup;
int CStepOptionsDlg::m_nIsVanishInf;
int CStepOptionsDlg::m_nIsSkin;
float CStepOptionsDlg::m_fVanishSup;
float CStepOptionsDlg::m_fVanishInf;
int CStepOptionsDlg::m_nSkin;
int CStepOptionsDlg::m_nArrowsPerLine;

int CStepOptionsDlg::m_nIndexGrab;
vector<STEPOPT_GROUP> CStepOptionsDlg::m_vGroup;
vector<float> CStepOptionsDlg::m_vfAttrib;
gint CStepOptionsDlg::m_nResponse;

CStepOptionsDlg::CStepOptionsDlg(void)
{
	m_nDelay = 0;
	m_fAttrib = 0.0f;
	m_nLevel = 0;
	m_nNoMiss = 0;
	m_nNoCount = 0;
	m_nIsVanishSup = 0;
	m_nIsVanishInf = 0;
	m_nIsSkin = 0;
	m_fVanishSup = 0.0f;
	m_fVanishInf = 0.0f;
	m_nSkin = 0;

	m_nIndexGrab = 0;
	m_nResponse = 0;
}


CStepOptionsDlg::~CStepOptionsDlg(void)
{
}

void CStepOptionsDlg::Set(GtkBuilder* builder)
{
	m_main_dlg = g_slice_new( GtkWidget );
	m_main_dlg = GTK_WIDGET( gtk_builder_get_object( builder, "dialog_step_options" ) );

	// Declare controls here
	m_treeview_step_opt_attrib = GTK_TREE_VIEW (
				gtk_builder_get_object( builder, "treeview_step_opt_attrib" ) );
	m_treeview_step_opt_group = GTK_TREE_VIEW (
				gtk_builder_get_object( builder, "treeview_step_opt_group" ) );

	// Temporal group
	GtkWidget* button_step_opt_newattrib, *button_step_opt_eraseattrib,
		*button_step_opt_newgroup, *button_step_opt_erasegroup;
	button_step_opt_newattrib = GTK_WIDGET (
				gtk_builder_get_object( builder, "button_step_opt_newattrib" ) );
	button_step_opt_eraseattrib = GTK_WIDGET (
				gtk_builder_get_object( builder, "button_step_opt_eraseattrib" ) );
	button_step_opt_newgroup = GTK_WIDGET (
				gtk_builder_get_object( builder, "button_step_opt_newgroup" ) );
	button_step_opt_erasegroup = GTK_WIDGET (
				gtk_builder_get_object( builder, "button_step_opt_erasegroup" ) );

	// Declare data exchanges here
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "spin_step_opt_delay" ) ),
		MAXCONTROL_SPIN, &m_nDelay, MAXDATA_INT);
	m_nIndexGrab = m_exc.m_data.size()-1;
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "spin_step_opt_runtime" ) ),
		MAXCONTROL_SPIN, &m_nRuntime, MAXDATA_INT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "hscale_step_opt_attrib" ) ),
		MAXCONTROL_SCALE, &m_fAttrib, MAXDATA_FLOAT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "spinbutton_step_opt_level" ) ),
		MAXCONTROL_SPIN, &m_nLevel, MAXDATA_INT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_step_opt_nomiss" ) ),
		MAXCONTROL_CHECK, &m_nNoMiss, MAXDATA_INT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_step_opt_nocount" ) ),
		MAXCONTROL_CHECK, &m_nNoCount, MAXDATA_INT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_step_opt_vanishsup" ) ),
		MAXCONTROL_CHECK, &m_nIsVanishSup, MAXDATA_INT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_step_opt_vanishinf" ) ),
		MAXCONTROL_CHECK, &m_nIsVanishInf, MAXDATA_INT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_step_opt_oblskin" ) ),
		MAXCONTROL_CHECK, &m_nIsSkin, MAXDATA_INT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "spin_song_opt_vanish_sup" ) ),
		MAXCONTROL_SPIN, &m_fVanishSup, MAXDATA_FLOAT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "spin_song_opt_vanish_inf" ) ),
		MAXCONTROL_SPIN, &m_fVanishInf, MAXDATA_FLOAT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "spin_song_opt_skin" ) ),
		MAXCONTROL_SPIN, &m_nSkin, MAXDATA_INT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "spin_arrows_per_line" ) ),
		MAXCONTROL_SPIN, &m_nArrowsPerLine, MAXDATA_INT);

	// Connect signals here
	g_signal_connect(m_main_dlg, 
					 "response", 
					 G_CALLBACK( on_dialog_set_size_response), 
					 NULL);
	g_signal_connect(m_main_dlg, 
					 "show", 
					 G_CALLBACK( on_dialog_set_size_show), 
					 NULL);
	g_signal_connect(button_step_opt_newattrib, 
					 "clicked", 
					 G_CALLBACK( on_button_step_opt_newattrib_clicked), 
					 NULL);
	g_signal_connect(button_step_opt_eraseattrib, 
					 "clicked", 
					 G_CALLBACK( on_button_step_opt_eraseattrib_clicked), 
					 NULL);
	g_signal_connect(button_step_opt_newgroup, 
					 "clicked", 
					 G_CALLBACK( on_button_step_opt_newgroup_clicked), 
					 NULL);
	g_signal_connect(button_step_opt_erasegroup, 
					 "clicked", 
					 G_CALLBACK( on_button_step_opt_erasegroup_clicked), 
					 NULL);
	GtkTreeSelection *selection = 
		gtk_tree_view_get_selection(m_treeview_step_opt_group);
	g_signal_connect(selection, 
					 "changed", 
					 G_CALLBACK( on_treeview_step_opt_group_change), 
					 NULL);
	GtkTreeSelection *selection2 = 
		gtk_tree_view_get_selection(m_treeview_step_opt_attrib);
	g_signal_connect(selection2, 
					 "changed", 
					 G_CALLBACK( on_treeview_step_opt_attrib_change), 
					 NULL);
	
}

int CStepOptionsDlg::DoModal(void)
{
	gtk_window_set_modal( GTK_WINDOW(m_main_dlg), true);
	gint r = gtk_dialog_run ( GTK_DIALOG( m_main_dlg ) );
	return r;
}

int CStepOptionsDlg::DoNoModal(void)
{
	gtk_window_set_modal( GTK_WINDOW(m_main_dlg), false);
	gtk_widget_show_all ( GTK_WIDGET( m_main_dlg ) );
	g_signal_connect_swapped (m_main_dlg,
                             "delete_event",
                             G_CALLBACK (gtk_widget_hide_on_delete),
                             m_main_dlg);
	return 1;
}


int CStepOptionsDlg::UpdateData(bool bGet)
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

void CStepOptionsDlg::on_dialog_set_size_response(GtkDialog* dialog, 
		gint response_id, gpointer userdata)
{
	m_nResponse = response_id;
	if(response_id == GTK_RESPONSE_ACCEPT)
	{
		UpdateData(true);
		GtkTreeIter iter;
		GtkTreeModel *model;
		int value;
	
		GtkTreeSelection *selection = 
				gtk_tree_view_get_selection(m_treeview_step_opt_group);
		if (gtk_tree_selection_get_selected(selection, &model, &iter)) 
		{
			gtk_tree_model_get(model, &iter, 1, &value,  -1);
			m_vGroup[value].nNoMiss = m_nNoMiss;
			m_vGroup[value].nNoCount = m_nNoCount;
			m_vGroup[value].nIsVanishSup = m_nIsVanishSup;
			m_vGroup[value].nIsVanishInf = m_nIsVanishInf;
			m_vGroup[value].nIsSkin = m_nIsSkin;
			m_vGroup[value].fVanishSup = m_fVanishSup;
			m_vGroup[value].fVanishInf = m_fVanishInf;
			m_vGroup[value].nSkin = m_nSkin;
		}
		GtkTreeSelection *selection2 = 
			gtk_tree_view_get_selection(m_treeview_step_opt_attrib);
		if(gtk_tree_selection_get_selected(selection2, &model, &iter)) 
		{
			int value2;
			gtk_tree_model_get(model, &iter, 1, &value2,  -1);
			m_vfAttrib[value2] = m_fAttrib;
		}
	}
	gtk_widget_hide(GTK_WIDGET(m_main_dlg));
}

void CStepOptionsDlg::on_dialog_set_size_show(GtkDialog* dialog,
										  gpointer userdata)
{
	// INITIAL UPDATE
	if(m_nIndexGrab >= 0) 
		gtk_widget_grab_focus(m_exc.m_data.at(m_nIndexGrab).control);

	int m_nSizeGroups = m_vGroup.size();

	GtkListStore *store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_INT);
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;
	GtkTreeIter iter;

	GtkTreeViewColumn* col =
		gtk_tree_view_get_column(m_treeview_step_opt_group, 0);
	if(col == NULL)
	{
		renderer = gtk_cell_renderer_text_new ();
		column = gtk_tree_view_column_new_with_attributes ("Grupos", renderer,
                                                      "text", 0,
                                                      NULL);
		gtk_tree_view_insert_column (m_treeview_step_opt_group, column, 0);
	}

	for(int i = 0; i < m_nSizeGroups; i++)
	{
		CStdStringA txt;
		txt.Format("Grupo %d", i+1);
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, 
						   0, (LPCSTR)txt, 
						   1, i, 
						   -1);
	}

	gtk_tree_view_set_model(m_treeview_step_opt_group, GTK_TREE_MODEL( store ));

	GtkTreeSelection *selection = 
			gtk_tree_view_get_selection(m_treeview_step_opt_group);
	if(gtk_tree_model_get_iter_first(GTK_TREE_MODEL( store ), &iter) != FALSE)
		gtk_tree_selection_select_iter(selection, &iter);
	g_object_unref (G_OBJECT (store));

	// Attrib

	store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_INT);
	col =
		gtk_tree_view_get_column(m_treeview_step_opt_attrib, 0);
	if(col == NULL)
	{
		renderer = gtk_cell_renderer_text_new ();
		column = gtk_tree_view_column_new_with_attributes ("Atributos", renderer,
														"text", 0,
														NULL);
		gtk_tree_view_insert_column (m_treeview_step_opt_attrib, column, 0);
	}


	int m_nSizeAttribs = m_vfAttrib.size();
	for(int i = 0; i < m_nSizeAttribs; i++)
	{
		CStdStringA txt;
		txt.Format("Atributo %d", i+1);
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, 
							0, (LPCSTR)txt, 
							1, i, 
							-1);
	}

	gtk_tree_view_set_model(m_treeview_step_opt_attrib, GTK_TREE_MODEL( store ));
	UpdateData(false);
	
	GtkTreeSelection *selection2 = 
			gtk_tree_view_get_selection(m_treeview_step_opt_attrib);
	if(gtk_tree_model_get_iter_first(GTK_TREE_MODEL( store ), &iter) != FALSE)
		gtk_tree_selection_select_iter(selection2, &iter);
	g_object_unref (G_OBJECT (store));

	UpdateData(false);
}

void CStepOptionsDlg::on_button_step_opt_newattrib_clicked(GtkDialog* dialog, 
		gpointer userdata)
{
	m_vfAttrib.push_back(0.0f);

	GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(m_treeview_step_opt_attrib));

	CStdStringA txt;
	txt.Format("Atributo %d", (int)(m_vfAttrib.size()));
	GtkTreeIter iter2;
	gtk_list_store_append(store, &iter2);
	gtk_list_store_set(store, &iter2, 
						0, (LPCSTR)txt, 
						1, (int)(m_vfAttrib.size()-1), 
						-1);
}

void CStepOptionsDlg::on_button_step_opt_eraseattrib_clicked(GtkDialog* dialog, 
		gpointer userdata)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	int value2;
	vector<float>::iterator it;
	
	GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(m_treeview_step_opt_attrib));
	GtkTreeSelection *selection2 = 
				gtk_tree_view_get_selection(m_treeview_step_opt_attrib);
	if (gtk_tree_selection_get_selected(selection2, &model, &iter)) 
	{
		gtk_tree_model_get(model, &iter, 1, &value2,  -1);
		gtk_list_store_remove(store, &iter);
		it=m_vfAttrib.begin()+value2;
		m_vfAttrib.erase(it);
		
		for(gboolean b = gtk_tree_model_get_iter_first(model, &iter); 
				b!=FALSE; b=gtk_tree_model_iter_next(model, &iter))
		{
			int value1;
			gtk_tree_model_get(model, &iter, 1, &value1,  -1);
			if(value2 < value1) value1--;
			gtk_list_store_set(store, &iter, 1, value1,  -1);
		}
	}
}

void CStepOptionsDlg::on_button_step_opt_newgroup_clicked(GtkDialog* dialog, 
		gpointer userdata)
{
	// Find a group index missing, if any
	int k = 0;
	for(guint j = 0; j < m_vGroup.size(); j++)
	{
		if(m_vGroup[j].nGroup==k)
			k++;
	}

	// Create a blank group
	STEPOPT_GROUP group;
	group.nNoMiss = 0;
	group.nNoCount = 0;
	group.nIsVanishSup = 0;
	group.nIsVanishInf = 0;
	group.nIsSkin = 0;
	group.fVanishSup = 0.0f;
	group.fVanishInf = 0.0f;
	group.nSkin = 0;
	group.nGroup = k;
	m_vGroup.push_back(group);

	// Add this to list_store
	GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(m_treeview_step_opt_group));
	GtkTreeIter iter;

	CStdStringA txt;
	txt.Format("Grupo %d", k+1);
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 
						0, (LPCSTR)txt, 
						1, (int)(m_vGroup.size()-1), 
						-1);

	//gtk_tree_view_set_model(m_treeview_step_opt_group, GTK_TREE_MODEL( store ));
}

void CStepOptionsDlg::on_button_step_opt_erasegroup_clicked(GtkDialog* dialog, 
		gpointer userdata)
{
	vector<STEPOPT_GROUP>::iterator it;

	GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(m_treeview_step_opt_group));
	GtkTreeIter iter;
	GtkTreeModel *model;
	int value;
	
	GtkTreeSelection *selection = 
		gtk_tree_view_get_selection(m_treeview_step_opt_group);
	if (gtk_tree_selection_get_selected(selection, &model, &iter)) 
	{
		gtk_tree_model_get(model, &iter, 1, &value,  -1);
		gtk_list_store_remove(store, &iter);
		it=m_vGroup.begin()+value;
		m_vGroup.erase(it);
		
		for(gboolean b = gtk_tree_model_get_iter_first(model, &iter); 
				b!=FALSE; b=gtk_tree_model_iter_next(model, &iter))
		{
			int value1;
			gtk_tree_model_get(model, &iter, 1, &value1,  -1);
			if(value < value1) value1--;
			gtk_list_store_set(store, &iter, 1, value1,  -1);
		}
	}
}

void CStepOptionsDlg::on_treeview_step_opt_group_change(GtkWidget* widget,
										  gpointer userdata)
{
	static int previous = -1;
	GtkTreeIter iter;
	GtkTreeModel *model;
	int value;
	
	GtkTreeSelection *selection = 
			gtk_tree_view_get_selection(m_treeview_step_opt_group);
	if(previous != -1)
	{
		UpdateData(true);
		m_vGroup[previous].nNoMiss = m_nNoMiss;
		m_vGroup[previous].nNoCount = m_nNoCount;
		m_vGroup[previous].nIsVanishSup = m_nIsVanishSup;
		m_vGroup[previous].nIsVanishInf = m_nIsVanishInf;
		m_vGroup[previous].nIsSkin = m_nIsSkin;
		m_vGroup[previous].fVanishSup = m_fVanishSup;
		m_vGroup[previous].fVanishInf = m_fVanishInf;
		m_vGroup[previous].nSkin = m_nSkin;
	}

	if (gtk_tree_selection_get_selected(selection, &model, &iter)) 
	{
		gtk_tree_model_get(model, &iter, 1, &value,  -1);

		m_nNoMiss = m_vGroup[value].nNoMiss;
		m_nNoCount = m_vGroup[value].nNoCount;
		m_nIsVanishSup = m_vGroup[value].nIsVanishSup;
		m_nIsVanishInf = m_vGroup[value].nIsVanishInf;
		m_nIsSkin = m_vGroup[value].nIsSkin;
		m_fVanishSup = m_vGroup[value].fVanishSup;
		m_fVanishInf = m_vGroup[value].fVanishInf;
		m_nSkin = m_vGroup[value].nSkin;
		UpdateData(false);
		previous = value;
	}
	else previous = -1;
}

void CStepOptionsDlg::on_treeview_step_opt_attrib_change(GtkWidget* widget,
										  gpointer userdata)
{
	static int previous2 = -1;
	GtkTreeIter iter;
	GtkTreeModel *model;
	int value2;
	
	GtkTreeSelection *selection2 = 
				gtk_tree_view_get_selection(m_treeview_step_opt_attrib);
	
	if(previous2 != -1)
	{
		UpdateData(true);
		m_vfAttrib[previous2] = m_fAttrib;
	}
	if (gtk_tree_selection_get_selected(selection2, &model, &iter)) 
	{
		gtk_tree_model_get(model, &iter, 1, &value2,  -1);
		m_fAttrib = m_vfAttrib[value2];
		previous2 = value2;
		UpdateData(false);
	}
	else 
		previous2 = -1;
}