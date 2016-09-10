#include "main_window.h"

extern "C" G_MODULE_EXPORT void                on_action_cut_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnCut();
}

extern "C" G_MODULE_EXPORT void                on_action_copy_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnCopy();
}

extern "C" G_MODULE_EXPORT void                on_action_paste_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnPaste();
}

extern "C" G_MODULE_EXPORT void                on_action_undo_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnUndo();
}

extern "C" G_MODULE_EXPORT void                on_action_redo_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnRedo();
}

extern "C" G_MODULE_EXPORT void                on_action_delete_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnDelete();
}

extern "C" G_MODULE_EXPORT void                on_action_mirror_h_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnMirrorH();
}

extern "C" G_MODULE_EXPORT void                on_action_mirror_v_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnMirrorV();
}

extern "C" G_MODULE_EXPORT void                on_action_incrstep_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnIncrStep();
}

extern "C" G_MODULE_EXPORT void                on_action_addgroup_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnAddGroup();
}

extern "C" G_MODULE_EXPORT void                on_action_deletegroup_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnDeleteGroup();
}

extern "C" G_MODULE_EXPORT void                on_action_deletendisplace_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnDeletendisplace();
}

extern "C" G_MODULE_EXPORT void                on_action_mod_primary_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnModPrimary();
}

extern "C" G_MODULE_EXPORT void                on_action_cut_r_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnCutR();
}

extern "C" G_MODULE_EXPORT void                on_action_copy_r_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnCopyR();
}

extern "C" G_MODULE_EXPORT void                on_action_paste_r_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnPasteR();
}

extern "C" G_MODULE_EXPORT void                on_action_delete_r_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnDeleteR();
}

extern "C" G_MODULE_EXPORT void                on_action_deletendisplace_r_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{
	g_mvView.at(gMain->m_uCurView)->OnDeletendisplaceR();
}

extern "C" G_MODULE_EXPORT void                on_action_edit_mod_primary_activate                      (GtkToolButton *toolbutton,
                                                        gpointer       user_data)
{

	g_mvView.at(gMain->m_uCurView)->OnEditModPrimary();
}
