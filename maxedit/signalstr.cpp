#include "main_window.h"

extern "C" G_MODULE_EXPORT void on_action_step_type_activate (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnStepType();
}

extern "C" G_MODULE_EXPORT void on_action_step_selectionsetsype_activate (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
    g_mvView.at(gMain->m_uCurView)->OnStepSelectionSetType();
}

extern "C" G_MODULE_EXPORT void on_action_step_longstep_toggled (GtkToggleAction *action,
                                                        gpointer       user_data)
{
    g_mvView[gMain->m_uCurView]->OnStepLongStep(action);
}

extern "C" G_MODULE_EXPORT void on_action_step_singlestep_toggled (GtkToggleAction *action,
                                                        gpointer       user_data)
{
    g_mvView[gMain->m_uCurView]->OnStepSingleStep(action);
}

extern "C" G_MODULE_EXPORT void on_action_step_selectstep_toggled (GtkToggleAction *action,
                                                        gpointer       user_data)
{
    g_mvView[gMain->m_uCurView]->OnStepSelectStep(action);
}

extern "C" G_MODULE_EXPORT void on_action_step_erasestep_toggled (GtkToggleAction *action,
                                                        gpointer       user_data)
{
    g_mvView[gMain->m_uCurView]->OnStepEraseStep(action);
}

extern "C" G_MODULE_EXPORT void on_action_step_preferences_activate (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
    g_mvView[gMain->m_uCurView]->OnActionStepPreferences();
}

extern "C" G_MODULE_EXPORT void on_action_step_size_activate (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
    g_mvView[gMain->m_uCurView]->OnActionStepSize();
	
}

extern "C" G_MODULE_EXPORT void on_action_step_preview_activate (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
    g_mvView.at(gMain->m_uCurView)->OnStepPreview();
}

extern "C" G_MODULE_EXPORT void on_action_step_simulate_activate (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
    g_mvView.at(gMain->m_uCurView)->OnStepSimulate();
}

extern "C" G_MODULE_EXPORT void on_action_song_splash_activate (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
    g_mvView.at(gMain->m_uCurView)->OnSongSplash();
}

extern "C" G_MODULE_EXPORT void on_action_song_music_activate (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
    g_mvView.at(gMain->m_uCurView)->OnSongMusic();
}

extern "C" G_MODULE_EXPORT void on_action_song_bga_activate (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
    g_mvView.at(gMain->m_uCurView)->OnSongBGA();
}

extern "C" G_MODULE_EXPORT void on_action_song_options_activate (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
    g_mvView.at(gMain->m_uCurView)->OnSongOptions();
}

extern "C" G_MODULE_EXPORT void on_action_song_fullscreen_toggled (GtkToggleAction *toolbutton,
                                                        gpointer       user_data)
{
    g_mvView.at(gMain->m_uCurView)->OnSongFullscreen(toolbutton);
}

extern "C" G_MODULE_EXPORT void on_action_step_info_activate (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnStepInfo();
}

extern "C" G_MODULE_EXPORT void on_tool2_1tap_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(1);
}

extern "C" G_MODULE_EXPORT void on_tool2_2hold_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(2);
}

extern "C" G_MODULE_EXPORT void on_tool2_3mine_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(5);
}

extern "C" G_MODULE_EXPORT void on_tool2_4potion_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(6);
}

extern "C" G_MODULE_EXPORT void on_tool2_5heart_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(7);
}

extern "C" G_MODULE_EXPORT void on_tool2_6flash_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(8);
}

extern "C" G_MODULE_EXPORT void on_tool2_7normalize_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(9);
}

extern "C" G_MODULE_EXPORT void on_tool2_8vel1_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(10);
}

extern "C" G_MODULE_EXPORT void on_tool2_9vel2_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(11);
}

extern "C" G_MODULE_EXPORT void on_tool2_10vel3_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(12);
}

extern "C" G_MODULE_EXPORT void on_tool2_11vel4_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(13);
}

extern "C" G_MODULE_EXPORT void on_tool2_12vel5_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(14);
}

extern "C" G_MODULE_EXPORT void on_tool2_13vel8_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(15);
}

extern "C" G_MODULE_EXPORT void on_tool2_14drop_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(16);
}

extern "C" G_MODULE_EXPORT void on_tool2_15ua_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(17);
}

extern "C" G_MODULE_EXPORT void on_tool2_16la_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(18);
}

extern "C" G_MODULE_EXPORT void on_tool2_17ra_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(19);
}

extern "C" G_MODULE_EXPORT void on_tool2_18snake_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(20);
}

extern "C" G_MODULE_EXPORT void on_tool2_19bsq_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(21);
	g_mvView.at(gMain->m_uCurView)->OnStepType();
}

extern "C" G_MODULE_EXPORT void on_tool2_20bsa_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(22);
}

extern "C" G_MODULE_EXPORT void on_tool2_21bsp_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(23);
}

extern "C" G_MODULE_EXPORT void on_tool2_22dyne_vanish_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(24);
}

extern "C" G_MODULE_EXPORT void on_tool2_23dyne_attack_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(25);
}

extern "C" G_MODULE_EXPORT void on_tool2_24dyne_zz_clicked (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnSetCurStepType(26);
}