#include "main_window.h"

#include "SetSizeDlg.h"
#include "BeatSplitDlg.h"
#include "ChangeBPMDlg.h"
#include "TickCountDlg.h"
#include "ChangeVelDlg.h"
#include "IncrStepDlg.h"
#include "StepOptionsDlg.h"
#include "TypeNoteDlg.h"
#include "ComandOptionsDlg.h"
#include "MaxSMConvChoose.h"

#include "libc_max_common.h"
//#include "cmnpiu.h"

// ** Resource string declaration
static const char *im[] = {
"T1.png",
"T2.png",
"T3.png",
"T4.png",
"T5.png",
"F1.png",
"F2.png",
"F3.png",
"F4.png",
"F5.png",
"LF1.png",
"LF2.png",
"LF3.png",
"LF4.png",
"LF5.png",
"tool2_3mine.png",
"tool2_4potion.png",
"tool2_5heart.png",
"tool2_6flash.png",
"tool2_7normalize.png",
"tool2_8vel1.png",
"tool2_9vel2.png",
"tool2_10vel3.png",
"tool2_11vel4.png",
"tool2_12vel5.png",
"tool2_13vel8.png",
"tool2_14drop.png",
"tool2_15ua.png",
"tool2_16la.png",
"tool2_17ra.png",
"tool2_18snake.png",
"tool2_19bsq.png",
"tool2_20bsa.png",
"tool2_21bsp.png",
"tool2_22dyne_vanish.png",
"tool2_23dyne_attack.png",
"tool2_24dyne_zz.png"};

SMain *gMain;
vector<CMaxDocument*> g_mdDocument;
vector<CMaxView*> g_mvView;
vector<cairo_surface_t*> sim;

int main_window_create(void)
{
    GtkBuilder *builder;
    GError     *error = NULL;

	gMain = g_slice_new( SMain );

	// Get the last working directory used
	TCHAR chCore[MAX_PATH], chCur[MAX_PATH];

	GetMaxCurrentDirectory(chCur);
	GetMaxProcessDirectory(chCore);
	SetMaxCurrentDirectory(chCore);

	fstream fs;
	fs.open("curdir.txt", ios::in);
	if(fs.is_open())
	{
		fs.getline(chCur, MAX_PATH);
		fs.close();
	}

	// ******************************************************************
	// PIUMaX 2 StepSmu Init Basics



	// ******************************************************************
	// PIUMaX 2 StepSmu Init Basics (End)

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if( ! gtk_builder_add_from_file( builder, "maxedit.glade", &error ) )
    {
        g_warning( "%s", error->message );
        g_free( error );
        return( 0 );
    }

    /* Get main window pointer from UI */
    gMain->main_window = GTK_WIDGET( gtk_builder_get_object( builder, "main_window" ) );
    gMain->main_alignment = GTK_WIDGET( gtk_builder_get_object( builder, "main_alignment" ) );
    gMain->main_about = GTK_WIDGET( gtk_builder_get_object( builder, "main_about" ) );

	/* Get menus from UI */
	gMain->menu_edit = GTK_WIDGET( gtk_builder_get_object( builder, "menu_edit" ) );
	gMain->menu_outside = GTK_WIDGET( gtk_builder_get_object( builder, "menu_outside" ) );
	gMain->menu_selection = GTK_WIDGET( gtk_builder_get_object( builder, "menu_selection" ) );
	gMain->menu_selection_mod = GTK_WIDGET( gtk_builder_get_object( builder, "menu_selection_mod" ) );
	gMain->menu_edit_mod = GTK_WIDGET( gtk_builder_get_object( builder, "menu_edit_mod" ) );

	/* Get main widgets from main */
	gMain->action_step[0] = GTK_TOGGLE_TOOL_BUTTON( gtk_builder_get_object( builder, "tool_step_longstep" ) );
	gMain->action_step[1] = GTK_TOGGLE_TOOL_BUTTON( gtk_builder_get_object( builder, "tool_step_singlestep" ) );
	gMain->action_step[2] = GTK_TOGGLE_TOOL_BUTTON( gtk_builder_get_object( builder, "tool_step_selectstep" ) );
	gMain->action_step[3] = GTK_TOGGLE_TOOL_BUTTON( gtk_builder_get_object( builder, "tool_step_erasestep" ) );

	/* Get dialogs from UI //USER//*/
	CSetSizeDlg::Set(builder);
	CBeatSplitDlg::Set(builder);
	CChangeBPMDlg::Set(builder);
	CChangeVelDlg::Set(builder);
	CTickCountDlg::Set(builder);
	CStepOptionsDlg::Set(builder);
	CIncrStepDlg::Set(builder);
	CTypeNoteDlg::Set(builder);
	CComandOptionsDlg::Set(builder);
	CMaxSMConvChoose::Set(builder);

    /* Connect signals */
    gtk_builder_connect_signals( builder, gMain );

    // Create the notebook
    gMain->main_notebook = gtk_notebook_new ();
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (gMain->main_notebook), GTK_POS_BOTTOM);
    gtk_container_add(GTK_CONTAINER(gMain->main_alignment), gMain->main_notebook);
	g_object_ref(gMain->main_notebook);

    g_signal_connect( G_OBJECT( gMain->main_notebook ), "switch-page",
                      G_CALLBACK( cb_switch_page ),
                      NULL);

    gtk_widget_show(gMain->main_notebook);

	// Clipboard
	gMain->main_clip = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	gdk_atom_intern("MaxEditSteps", FALSE);

    // Make initial
    if(!main_window_initial()) return 0;

	// Load the images
	int nim = sizeof(im)/sizeof(char*);
	sim.resize(nim);
	for(int k = 0; k < nim; k++)
	{
		sim[k] = cairo_image_surface_create_from_png(im[k]);
		int g = cairo_surface_status(sim[k]);
		if(g == CAIRO_STATUS_FILE_NOT_FOUND) return 0;
		if(sim[k] == NULL) return (0);
	}

    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show( gMain->main_window );

	// Set current directory
	SetMaxCurrentDirectory(chCur);

    /* Start main loop */
    gtk_main();

	// Put cur directory in curdir.txt
	GetMaxCurrentDirectory(chCur);
	SetMaxCurrentDirectory(chCore);

	fs.open("curdir.txt", ios::out);
	if(fs.is_open())
	{
		fs << chCur << endl;
		fs.close();
	}

	// ******************************************************************
	// PIUMaX 2 StepSmu Delete Basics

	// ******************************************************************
	// PIUMaX 2 StepSmu Delete Basics (End)

    /* Destroy builder, since we don't need it anymore */
    g_object_unref( G_OBJECT( builder ) );
	g_object_unref(gMain->main_notebook);

    return( 1 );
}

void main_window_destroy(void)
{
    /* Free any allocated data */
    g_slice_free( SMain, gMain );
}

GtkWidget *
create_notebook_label( GtkWidget   *child,
                       const gchar *text,
                       GtkWidget   *notebook )
{
    GtkWidget *hbox,
    *label,
    *button,
    *image;

    hbox = gtk_hbox_new (/*GTK_ORIENTATION_HORIZONTAL*/FALSE, 3);

    label = gtk_label_new (text);
    gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 0);

    button = gtk_button_new();
    g_object_set_data( G_OBJECT( button ), "child", child );
    g_signal_connect( G_OBJECT( button ), "clicked",
                      G_CALLBACK( cb_close_tab ),
                      GTK_NOTEBOOK( notebook ) );
    gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);

    image = gtk_image_new_from_stock (GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU);
    gtk_container_add (GTK_CONTAINER (button), image);

    gtk_widget_show_all( hbox );

    return( hbox );
}

gint get_view_child(GtkWidget* child)
{
	size_t* cur = (size_t*)( g_object_get_data( G_OBJECT( child ), "view" ));
	return (gint)(*cur);
}

void actualizate_curview(void)
{
	GtkWidget* child;
	gint page_num = gtk_notebook_get_current_page(GTK_NOTEBOOK(gMain->main_notebook));
	if(page_num < 0) return;
	child = gtk_notebook_get_nth_page(GTK_NOTEBOOK(gMain->main_notebook), page_num);
    gMain->m_uCurView = (size_t)get_view_child(child);
	g_mvView[gMain->m_uCurView]->OnActualizate();
}

void
cb_close_tab( GtkButton   *button,
              GtkNotebook *notebook )
{
    GtkWidget *child;
    gint page;

    child = GTK_WIDGET( g_object_get_data( G_OBJECT( button ), "child" ));
    page = gtk_notebook_page_num( notebook, child );

	size_t* cur = (size_t*)( g_object_get_data( G_OBJECT( child ), "view" ));



	// closing view/document
	main_window_close_view( (*cur) );
	delete cur;
}

void                cb_switch_page                      (GtkNotebook *notebook,
                                                        GtkWidget   *page,
                                                        guint        page_num,
                                                        gpointer     user_data)
{
	GtkWidget* child;
	if(page_num < 0) return;
	child = gtk_notebook_get_nth_page(notebook, page_num);
    gMain->m_uCurView = (size_t)get_view_child(child);
	g_mvView[gMain->m_uCurView]->OnActualizate();
}

gint message_box(GtkDialogFlags flags, GtkMessageType type, GtkButtonsType buttons, const gchar *message)
{
    gint caret;
    GtkWidget* dialog = gtk_message_dialog_new (GTK_WINDOW(gMain->main_window),
                                 flags,
                                 type,
                                 buttons,
                                 message, 1);
    caret = gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
    return caret;
}

gint message_box_parent(GtkWindow* window, GtkDialogFlags flags, GtkMessageType type, GtkButtonsType buttons, const gchar *message)
{
    gint caret;
    GtkWidget* dialog = gtk_message_dialog_new (window,
                                 flags,
                                 type,
                                 buttons,
                                 message, 1);
    caret = gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
    return caret;
}

gboolean
configure (GtkWidget *da, GdkEventConfigure *event, gpointer user_data)
{
	return TRUE;
}

gboolean
expose (GtkWidget *da, GdkEventExpose *event, gpointer user_data)
{
	cairo_t *cr;

    /* Create cairo context from GdkWindow */
    cr = gdk_cairo_create( event->window );

	if(g_mvView.at(gMain->m_uCurView)->OnExpose(cr) == FALSE)
	{
		cairo_set_source_rgb(cr, 0, 1, 1);
		cairo_paint(cr);
	}

	/* Destroy cairo context */
    cairo_destroy( cr );

	return FALSE;
}

gboolean on_button_press (GtkWidget* widget,
  GdkEventButton* event)
{
	if(event->type == GDK_BUTTON_PRESS)
	{
		if (event->button == 1)
		{
			g_mvView.at(gMain->m_uCurView)->OnLButtonDown(int(event->x), int(event->y));
			return TRUE;
		}
		else if (event->button == 3)
		{
			g_mvView.at(gMain->m_uCurView)->OnRButtonDown(int(event->x), int(event->y));
			return TRUE;
		}
	}
	else if(event->type == GDK_BUTTON_RELEASE)
	{
		if (event->button == 1)
		{
			g_mvView.at(gMain->m_uCurView)->OnLButtonUp(int(event->x), int(event->y));
			return TRUE;
		}
		else if (event->button == 3)
		{
			g_mvView.at(gMain->m_uCurView)->OnRButtonUp(int(event->x), int(event->y), event);
			return TRUE;
		}
	}
	return FALSE;
}

gboolean on_notify (GtkWidget *da, GdkEventMotion *event, gpointer nothing)
{
    GdkModifierType     state;
    gint                x, y;

	if (event->is_hint)
	{
		gdk_window_get_pointer (event->window, &x, &y, &state);
		if(event->type == GDK_MOTION_NOTIFY)
		{
			g_mvView.at(gMain->m_uCurView)->OnMouseMove(int(event->x), int(event->y));
		}
	}
    return TRUE;
}

gboolean on_enter_notify (GtkWidget *da, GdkEventMotion *event, gpointer nothing)
{
	g_mvView.at(gMain->m_uCurView)->OnEnterNotify();
    return TRUE;
}

gboolean on_leave_notify (GtkWidget *da, GdkEventMotion *event, gpointer nothing)
{
	g_mvView.at(gMain->m_uCurView)->OnLeaveNotify();
    return TRUE;
}
