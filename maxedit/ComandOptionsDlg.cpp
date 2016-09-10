#include "ComandOptionsDlg.h"
#include "main_window.h"

GtkWidget *CComandOptionsDlg::m_main_dlg;

CMaxDataExchange CComandOptionsDlg::m_exc;

// ** Data here
int CComandOptionsDlg::m_nSize;
int CComandOptionsDlg::m_nIndexGrab;
ARGSTEP CComandOptionsDlg::m_arg;
int CComandOptionsDlg::m_nVel;
bool CComandOptionsDlg::m_b05;
int CComandOptionsDlg::m_nRush;

CComandOptionsDlg::CComandOptionsDlg(void)
{
	m_nSize = 0;
	m_nIndexGrab = 0;
}


CComandOptionsDlg::~CComandOptionsDlg(void)
{
}

void CComandOptionsDlg::Set(GtkBuilder* builder)
{
	m_main_dlg = g_slice_new( GtkWidget );
	m_main_dlg = GTK_WIDGET( gtk_builder_get_object( builder, "dialog_command_options" ) );

	// Declare controls here
	// ** NONE

	// Declare data exchanges here
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "combo_opt_speed" ) ),
		MAXCONTROL_COMBO, &m_nVel, MAXDATA_INT);
	m_nIndexGrab = m_exc.m_data.size()-1;
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_AC" ) ),
		MAXCONTROL_CHECK, &m_arg.bAC, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_DC" ) ),
		MAXCONTROL_CHECK, &m_arg.bDC, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_EW" ) ),
		MAXCONTROL_CHECK, &m_arg.bEW, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_RV" ) ),
		MAXCONTROL_CHECK, &m_arg.bRV, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_05" ) ),
		MAXCONTROL_CHECK, &m_b05, MAXDATA_INT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "hscale_opt_vanish_e" ) ),
		MAXCONTROL_SCALE, &m_arg.fEVanish, MAXDATA_FLOAT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "hscale_opt_vanish_i" ) ),
		MAXCONTROL_SCALE, &m_arg.fIVanish, MAXDATA_FLOAT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_FD" ) ),
		MAXCONTROL_CHECK, &m_arg.bFreedom, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_FL" ) ),
		MAXCONTROL_CHECK, &m_arg.bFlash, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "combo_opt_skin" ) ),
		MAXCONTROL_COMBO, &m_arg.nSkin, MAXDATA_INT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_rsk" ) ),
		MAXCONTROL_CHECK, &m_arg.bRandomSkin, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_UA" ) ),
		MAXCONTROL_CHECK, &m_arg.bUnderAttack, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_DR" ) ),
		MAXCONTROL_CHECK, &m_arg.bDrop, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_X" ) ),
		MAXCONTROL_CHECK, &m_arg.bXMode, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_NX" ) ),
		MAXCONTROL_CHECK, &m_arg.bNXMode, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_SN" ) ),
		MAXCONTROL_CHECK, &m_arg.bSnake, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_SI" ) ),
		MAXCONTROL_CHECK, &m_arg.bSink, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_RI" ) ),
		MAXCONTROL_CHECK, &m_arg.bRise, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_RS" ) ),
		MAXCONTROL_CHECK, &m_arg.bRS, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_M" ) ),
		MAXCONTROL_CHECK, &m_arg.bMirror, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_JR" ) ),
		MAXCONTROL_CHECK, &m_arg.bJR, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "radio_opt_EJ" ) ),
		MAXCONTROL_RADIO, &m_arg.bJR, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "radio_opt_NJ" ) ),
		MAXCONTROL_RADIO, &m_arg.bNJ, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "radio_opt_HJ" ) ),
		MAXCONTROL_RADIO, &m_arg.bHJ, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "combo_opt_rush" ) ),
		MAXCONTROL_COMBO, &m_nRush, MAXDATA_INT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_character" ) ),
		MAXCONTROL_CHECK, &m_arg.bCharacter, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "combo_opt_character" ) ),
		MAXCONTROL_COMBO, &m_arg.nCharacter, MAXDATA_INT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "spin_opt_level_character" ) ),
		MAXCONTROL_SPIN, &m_arg.nLevelCharacer, MAXDATA_INT);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_single" ) ),
		MAXCONTROL_CHECK, &m_arg.bIsSingle, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_autoplay" ) ),
		MAXCONTROL_CHECK, &m_arg.bAutoPlay, MAXDATA_BOOL);
	m_exc.Add(GTK_WIDGET( gtk_builder_get_object( builder, "check_opt_AT" ) ),
		MAXCONTROL_CHECK, &m_arg.bAssistTick, MAXDATA_BOOL);

	// Connect signals here
	g_signal_connect(m_main_dlg, 
					 "response", 
					 G_CALLBACK( on_dialog_comand_options_response), 
					 NULL);
	g_signal_connect(m_main_dlg, 
					 "show", 
					 G_CALLBACK( on_dialog_comand_options_show), 
					 NULL);

	
}

int CComandOptionsDlg::DoModal(void)
{
	gtk_window_set_modal( GTK_WINDOW(m_main_dlg), true);
	gint r = gtk_dialog_run ( GTK_DIALOG( m_main_dlg ) );
	return r;
}

int CComandOptionsDlg::DoNoModal(void)
{
	gtk_window_set_modal( GTK_WINDOW(m_main_dlg), false);
	gtk_widget_show_all ( GTK_WIDGET( m_main_dlg ) );
	g_signal_connect_swapped (m_main_dlg,
                             "delete_event",
                             G_CALLBACK (gtk_widget_hide_on_delete),
                             m_main_dlg);
	return 1;
}


int CComandOptionsDlg::UpdateData(bool bGet)
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

void CComandOptionsDlg::on_dialog_comand_options_response(GtkDialog* dialog, 
		gint response_id, gpointer userdata)
{
	if(response_id == GTK_RESPONSE_ACCEPT)
	{
		UpdateData(true);
		switch(m_nVel)
		{
			case 1: m_arg.fUserVel = 2.0f; break;
			case 2: m_arg.fUserVel = 3.0f; break;
			case 3: m_arg.fUserVel = 4.0f; break;
			case 4: m_arg.fUserVel = 5.0f; break;
			case 5: m_arg.fUserVel = 6.0f; break;
			case 6: m_arg.fUserVel = 8.0f; break;
			default: m_arg.fUserVel = 1.0f; break;
		}
		switch(m_nRush)
		{
			case 0: m_arg.fRush = 0.8f; break;
			case 1: m_arg.fRush = 0.9f; break;
			case 3: m_arg.fRush = 1.1f; break;
			case 4: m_arg.fRush = 1.2f; break;
			case 5: m_arg.fRush = 1.5f; break;
			default: m_arg.fRush = 1.0f; break;
		}
		if(m_b05) m_arg.fUserVel += 0.5;
		m_arg.fDecrBar = 0.07f;
		m_arg.fIncrBar = 0.005f;
	}
	gtk_widget_hide(GTK_WIDGET(m_main_dlg));
}

void CComandOptionsDlg::on_dialog_comand_options_show(GtkDialog* dialog,
										  gpointer userdata)
{
	// INITIAL UPDATE

	if(m_arg.fUserVel >= 8.0)
		m_nVel = 6;
	else if(m_arg.fUserVel >= 6.0)
		m_nVel = 5;
	else if(m_arg.fUserVel >= 5.0)
		m_nVel = 4;
	else if(m_arg.fUserVel >= 4.0)
		m_nVel = 3;
	else if(m_arg.fUserVel >= 3.0)
		m_nVel = 2;
	else if(m_arg.fUserVel >= 2.0)
		m_nVel = 1;
	else
		m_nVel = 0;

	if(m_arg.fRush >= 1.5)
		m_nRush = 5;
	else if(m_arg.fRush >= 1.2)
		m_nRush = 4;
	else if(m_arg.fRush >= 1.1)
		m_nRush = 3;
	else if(m_arg.fRush >= 1.0)
		m_nRush = 2;
	else if(m_arg.fRush >= 0.9)
		m_nRush = 1;
	else
		m_nRush = 0;

	if(float(int(m_arg.fUserVel)) == m_arg.fUserVel)
		m_b05 = false;
	else
		m_b05 = true;

	UpdateData(false);

	if(m_nIndexGrab >= 0) 
		gtk_widget_grab_focus(m_exc.m_data.at(m_nIndexGrab).control);

	
}