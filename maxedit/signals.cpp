#include "main_window.h"

extern "C" G_MODULE_EXPORT void on_action_new_activate (GtkMenuItem *menuitem,
                                            gpointer     user_data)
{
    // TODO: Add response here
    // Call the function
    if(!main_window_new_document())
    {
        message_box( GTK_DIALOG_DESTROY_WITH_PARENT,
                                 GTK_MESSAGE_ERROR,
                                 GTK_BUTTONS_CLOSE,
                                 "Error creating document");
    }
}

extern "C" G_MODULE_EXPORT void                on_action_open_activate                      (GtkMenuItem *menuitem,
                                                        gpointer     user_data)
{
    // TODO: Add response here
    if(!main_window_open_document())
    {
        message_box( GTK_DIALOG_DESTROY_WITH_PARENT,
                                 GTK_MESSAGE_ERROR,
                                 GTK_BUTTONS_CLOSE,
                                 "Error opening document");
    }
}

extern "C" G_MODULE_EXPORT void                on_action_close_activate                      (GtkMenuItem *menuitem,
                                                        gpointer     user_data)
{
    // TODO: Add response here
    main_window_close_cur_document();
}

extern "C" G_MODULE_EXPORT void                on_action_save_activate                      (GtkMenuItem *menuitem,
                                                        gpointer     user_data)
{
    // TODO: Add response here
    if(!main_window_save_document())
    {
        message_box( GTK_DIALOG_DESTROY_WITH_PARENT,
                                 GTK_MESSAGE_ERROR,
                                 GTK_BUTTONS_CLOSE,
                                 "Error saving document");
    }
}

extern "C" G_MODULE_EXPORT void                on_action_saveas_activate                      (GtkMenuItem *menuitem,
                                                        gpointer     user_data)
{
    // TODO: Add response here
    if(!main_window_saveas_document())
    {
        message_box( GTK_DIALOG_DESTROY_WITH_PARENT,
                                 GTK_MESSAGE_ERROR,
                                 GTK_BUTTONS_CLOSE,
                                 "Error saving document");
    }
}

extern "C" G_MODULE_EXPORT void                on_action_about_activate                      (GtkMenuItem *menuitem,
                                                        gpointer     user_data)
{
    // TODO: Add response here
    gtk_dialog_run( GTK_DIALOG( gMain->main_about ) );
    gtk_widget_hide( gMain->main_about );
}

extern "C" G_MODULE_EXPORT void                on_action_exit_activate                      (GtkMenuItem *menuitem,
                                                        gpointer     user_data)
{
    while(!main_window_is_saved_documents())
	{
	}
	gtk_main_quit();
}

extern "C" G_MODULE_EXPORT void                on_main_window_delete_event                      (GtkToolButton *toolbutton,
                                                        gpointer     user_data)
{
	if(main_window_is_saved_documents())
	{
		gtk_main_quit();
	}
	else
	{
		gtk_widget_show_all(gMain->main_window);
	}
}

