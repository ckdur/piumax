#include "IncrStepDlg.h"
#include "main_window.h"

GtkWidget *CIncrStepDlg::m_main_dlg;

CMaxDataExchange CIncrStepDlg::m_exc;

// ** Data here
int CIncrStepDlg::m_nSize;
int CIncrStepDlg::m_nIndexGrab;

CIncrStepDlg::CIncrStepDlg(void)
{
	m_nSize = 0;
	m_nIndexGrab = 0;
}


CIncrStepDlg::~CIncrStepDlg(void)
{
}

void CIncrStepDlg::Set(GtkBuilder* builder)
{
	m_main_dlg = g_slice_new( GtkWidget );
	m_main_dlg = GTK_WIDGET( gtk_builder_get_object( builder, "dialog_incr_size" ) );

	// Declare controls here
	// ** NONE

	// Declare data exchanges here
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "spinbutton_incr_step" ) ),
		MAXCONTROL_SPIN, &m_nSize, MAXDATA_INT);
	m_nIndexGrab = m_exc.m_data.size()-1;

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

int CIncrStepDlg::DoModal(void)
{
	gtk_window_set_modal( GTK_WINDOW(m_main_dlg), true);
	gint r = gtk_dialog_run ( GTK_DIALOG( m_main_dlg ) );
	return r;
}

int CIncrStepDlg::DoNoModal(void)
{
	gtk_window_set_modal( GTK_WINDOW(m_main_dlg), false);
	gtk_widget_show_all ( GTK_WIDGET( m_main_dlg ) );
	g_signal_connect_swapped (m_main_dlg,
                             "delete_event",
                             G_CALLBACK (gtk_widget_hide_on_delete),
                             m_main_dlg);
	return 1;
}


int CIncrStepDlg::UpdateData(bool bGet)
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

void CIncrStepDlg::on_dialog_set_size_response(GtkDialog* dialog, 
		gint response_id, gpointer userdata)
{
	if(response_id == GTK_RESPONSE_ACCEPT)
	{
		UpdateData(true);
	}
	gtk_widget_hide(GTK_WIDGET(m_main_dlg));
}

void CIncrStepDlg::on_dialog_set_size_show(GtkDialog* dialog,
										  gpointer userdata)
{
	// INITIAL UPDATE
	UpdateData(false);

	if(m_nIndexGrab >= 0) 
		gtk_widget_grab_focus(m_exc.m_data.at(m_nIndexGrab).control);
}