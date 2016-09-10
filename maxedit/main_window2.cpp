/*
Afix Systems (R) Written by ckdur

this file contents the code to gestionate all the document/view
events from the main window

NOTE: You MUST declare event functions in another C file and
refer to this funtions to do actions
*/

#include <string.h>
#include "StdString.h"
#include "main_window.h"
#include "libc_max_common.h"

int main_window_initial(void)
{
    gMain->m_uNewDocCount = 0;
    gMain->m_uCurView = 0;
    if(!main_window_new_document()) return 0;
    return 1;
}

int main_window_new_view_from_doc(uint16_t doc)
{
    CMaxView* var = NULL;
    var = new CMaxView();
    if(var == NULL) return 0;
	g_mvView.push_back(var);

    var->m_uDoc = doc;


    gMain->m_uCurView = g_mvView.size()-1;

    // Create the new notebook page
    GtkWidget* child, *label;
    GtkWidget* draw;
     GtkAdjustment *hadj, *vadj;
     hadj = (GtkAdjustment *)gtk_adjustment_new(0.0, 0.0, 10, 1, 1.0, 1.0);
     vadj = (GtkAdjustment *)gtk_adjustment_new(0.0, 0.0, 10, 1, 1.0, 1.0);
    child = gtk_scrolled_window_new(hadj, vadj);
    draw = gtk_drawing_area_new();

	// Set the view asociated
	size_t* cur = new size_t;
	(*cur) = gMain->m_uCurView;
	g_object_set_data( G_OBJECT( child ), "view", cur );

    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(child), draw);
    label = create_notebook_label(child, g_mdDocument.at(doc)->m_strName, gMain->main_notebook);

	gtk_widget_add_events(draw, GDK_BUTTON_PRESS_MASK);
	gtk_widget_add_events(draw, GDK_BUTTON_RELEASE_MASK);
	gtk_widget_add_events(draw, GDK_POINTER_MOTION_HINT_MASK | GDK_POINTER_MOTION_MASK);//GDK_POINTER_MOTION_MASK);
	gtk_widget_add_events(draw, GDK_ENTER_NOTIFY_MASK);
	gtk_widget_add_events(draw, GDK_LEAVE_NOTIFY_MASK);
	g_signal_connect (draw, "configure-event",
			G_CALLBACK (configure), NULL);
	g_signal_connect (draw, "expose-event",
			G_CALLBACK (expose), NULL);
	g_signal_connect (draw, "button-press-event",
		G_CALLBACK (on_button_press), NULL);
	g_signal_connect (draw, "button-release-event",
			G_CALLBACK (on_button_press), NULL);
	g_signal_connect (draw, "motion-notify-event",
			G_CALLBACK (on_notify), NULL);
	g_signal_connect (draw, "enter-notify-event",
			G_CALLBACK (on_enter_notify), NULL);
	g_signal_connect (draw, "leave-notify-event",
			G_CALLBACK (on_leave_notify), NULL);

    gtk_widget_show_all( child );

    gtk_notebook_insert_page(GTK_NOTEBOOK(gMain->main_notebook), child, label, gMain->m_uCurView);
    gtk_notebook_set_current_page(GTK_NOTEBOOK(gMain->main_notebook), gMain->m_uCurView);

	var->m_gtkDrawingArea = draw;
	var->m_gtkScrolledWindow = child;
	var->m_gtkHAdj = hadj;
	var->m_gtkVAdj = vadj;

	var->Init();

    // ** TODO: Call the custom func

    return (1);
}

int main_window_new_document(void)
{
    gMain->m_uNewDocCount++;
    CMaxDocument* varx = new CMaxDocument((int)gMain->m_uNewDocCount);
    if(varx == NULL) return 0;
	varx->New((int)gMain->m_uNewDocCount);
    g_mdDocument.push_back(varx);

    if(!main_window_new_view_from_doc(g_mdDocument.size()-1)) return 0;

    // ** TODO: Call the custom func

    return (1);
}

int main_window_open_document(void)
{
    // Classic code to open file from GTK dialog
    GtkWidget *dialog;
    GtkFileFilter *filter;

    dialog = gtk_file_chooser_dialog_new ("Open File",
                                          GTK_WINDOW(gMain->main_window),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);

	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, ALL_DOCUMENT_FILTER_NAME);
	gtk_file_filter_add_pattern(filter, MAIN_DOCUMENT_FILTER_PATTERN);
	gtk_file_filter_add_pattern(filter, SKSF_DOCUMENT_FILTER_PATTERN);
	gtk_file_filter_add_pattern(filter, SM_DOCUMENT_FILTER_PATTERN);
	gtk_file_filter_add_pattern(filter, SMA_DOCUMENT_FILTER_PATTERN);
	gtk_file_filter_add_pattern(filter, KSF_DOCUMENT_FILTER_PATTERN);
	gtk_file_filter_add_pattern(filter, UCS_DOCUMENT_FILTER_PATTERN);
	gtk_file_filter_add_pattern(filter, NX10_DOCUMENT_FILTER_PATTERN);
	gtk_file_filter_add_pattern(filter, STX_DOCUMENT_FILTER_PATTERN);
	gtk_file_filter_add_pattern(filter, NOT_DOCUMENT_FILTER_PATTERN);
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

	filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, MAIN_DOCUMENT_FILTER_NAME);
    gtk_file_filter_add_pattern(filter, MAIN_DOCUMENT_FILTER_PATTERN);
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

	filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, SKSF_DOCUMENT_FILTER_NAME);
    gtk_file_filter_add_pattern(filter, SKSF_DOCUMENT_FILTER_PATTERN);
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

	filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, SM_DOCUMENT_FILTER_NAME);
    gtk_file_filter_add_pattern(filter, SM_DOCUMENT_FILTER_PATTERN);
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

	filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, SMA_DOCUMENT_FILTER_NAME);
    gtk_file_filter_add_pattern(filter, SMA_DOCUMENT_FILTER_PATTERN);
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

	filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, KSF_DOCUMENT_FILTER_NAME);
    gtk_file_filter_add_pattern(filter, KSF_DOCUMENT_FILTER_PATTERN);
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

	filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, UCS_DOCUMENT_FILTER_NAME);
    gtk_file_filter_add_pattern(filter, UCS_DOCUMENT_FILTER_PATTERN);
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, NX10_DOCUMENT_FILTER_NAME);
	gtk_file_filter_add_pattern(filter, NX10_DOCUMENT_FILTER_PATTERN);
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, STX_DOCUMENT_FILTER_NAME);
	gtk_file_filter_add_pattern(filter, STX_DOCUMENT_FILTER_PATTERN);
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, NOT_DOCUMENT_FILTER_NAME);
	gtk_file_filter_add_pattern(filter, NOT_DOCUMENT_FILTER_PATTERN);
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    // ** USER: Add aditional filters here

	TCHAR tchCur[MAX_PATH];
	char chCur[MAX_PATH];
	GetMaxCurrentDirectory(tchCur);
	Max_strcpy(chCur, tchCur);
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (dialog), chCur);

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;

		char *folder = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER (dialog));
		Max_strcpy(tchCur, folder);
		SetMaxCurrentDirectory(tchCur);
		g_free(folder);

        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

		char core[MAX_PATH];
		Max_strcpy_utf8(core, MAX_PATH,filename,0);
		g_free (filename);

        gtk_widget_destroy (dialog);
        // New document
        CMaxDocument* varx = new CMaxDocument();
        if(varx == NULL) return 0;
        if(!varx->Open(core)) return 0;
        g_mdDocument.push_back(varx);

        if(!main_window_new_view_from_doc(g_mdDocument.size()-1)) return 0;
    }
    else
    {
        gtk_widget_destroy (dialog);
    }



    return (1);
}

int main_window_new_view(void)
{
    uint16_t doc = 0;

    // we'll put the zero document if this is the first view
    // Else, the current document in the current view
    if(g_mvView.size() == 0)
    {
        doc = 0;
    }
    else
    {
        doc = g_mvView.at(gMain->m_uCurView)->m_uDoc;
    }

    main_window_new_view_from_doc(doc);

    return (1);
}

int main_window_save_document(void)
{
    size_t cur = main_window_get_current_document();
    return main_window_save_document(cur);
}

int main_window_save_document(size_t cur)
{
    if(!g_mdDocument.at(cur)->m_bFiled) main_window_saveas_document(cur);
    else g_mdDocument.at(cur)->Save();
    return (1);
}

int main_window_saveas_document(void)
{
	size_t cur = main_window_get_current_document();
    return main_window_saveas_document(cur);
}

int main_window_saveas_document(size_t cur)
{
    // Classic code to open file from GTK dialog
    GtkWidget *dialog;
    GtkFileFilter *filter;

    dialog = gtk_file_chooser_dialog_new ("Save File",
                                          GTK_WINDOW(gMain->main_window),
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                          NULL);

    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, MAIN_DOCUMENT_FILTER_NAME);
    gtk_file_filter_add_pattern(filter, MAIN_DOCUMENT_FILTER_PATTERN);
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

	char strExt[] = MAIN_DOCUMENT_FILTER_EXTENSION;
	g_object_set_data(G_OBJECT(filter), "extension", strExt);

	filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, SKSF_DOCUMENT_FILTER_NAME);
    gtk_file_filter_add_pattern(filter, SKSF_DOCUMENT_FILTER_PATTERN);
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

	char strExtSksf[] = SKSF_DOCUMENT_FILTER_EXTENSION;
	g_object_set_data(G_OBJECT(filter), "extension", strExtSksf);

	TCHAR tchCur[MAX_PATH];
	char chCur[MAX_PATH];
	GetMaxCurrentDirectory(tchCur);
	Max_strcpy(chCur, tchCur);
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (dialog), chCur);
    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

    // ** TODO: Add aditional filters here

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
		char *chExt;

		char *folder = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER (dialog));
		Max_strcpy(tchCur, folder);
		SetMaxCurrentDirectory(tchCur);
		g_free(folder);

        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

		char core[MAX_PATH];
		Max_strcpy_utf8(core,MAX_PATH,filename,0);
		g_free (filename);

		filter = gtk_file_chooser_get_filter(GTK_FILE_CHOOSER (dialog));
		chExt = (char*)g_object_get_data(G_OBJECT(filter), "extension");
		CStdStringA file = core;
		if(file.Find('.') == -1) file += chExt;
        gtk_widget_destroy (dialog);
        // New document
        g_mdDocument.at(cur)->Save(file);

		gint page = gtk_notebook_get_current_page (GTK_NOTEBOOK(gMain->main_notebook));
		GtkWidget* child = gtk_notebook_get_nth_page(GTK_NOTEBOOK(gMain->main_notebook), page);
		GtkWidget* label = gtk_notebook_get_tab_label(GTK_NOTEBOOK(gMain->main_notebook), child);
		GList* list = gtk_container_get_children(GTK_CONTAINER(label));
		GtkWidget* tex = GTK_WIDGET (g_list_nth_data (list,0));
		gtk_label_set_text(GTK_LABEL(tex), g_mdDocument.at(cur)->m_strName);
    }
    else
    {
        gtk_widget_destroy (dialog);
		return (0);
    }
    return (1);
}

void main_window_close_cur_document(void)
{
    main_window_close_document(main_window_get_current_document());
}

void main_window_close_document(uint16_t uDoc)
{
    for(vector<CMaxView*>::iterator it = g_mvView.begin(); it != g_mvView.end(); ++it)
    {
        /*if((*it)->m_uDoc == uDoc)
        {
            gint n = gtk_notebook_get_n_pages(GTK_NOTEBOOK(gMain->main_notebook));
			for(gint i = 0; i < n; i++)
			{
				GtkWidget *child2 = gtk_notebook_get_nth_page(GTK_NOTEBOOK(gMain->main_notebook), i);
				size_t* cura = (size_t*)( g_object_get_data( G_OBJECT( child2 ), "view" ));
				if((*cura) > size_t(it-g_mvView.begin()))
					gtk_notebook_remove_page( GTK_NOTEBOOK(gMain->main_notebook), i );
			}
            g_mvView.erase(it);
            --it;
        }
		else */if((*it)->m_uDoc > uDoc)
        {
            (*it)->m_uDoc--;
        }
    }
	g_mdDocument[uDoc]->Close();
	delete g_mdDocument[uDoc];
    g_mdDocument.erase(g_mdDocument.begin()+uDoc);
}

void main_window_close_cur_view(void)
{
    main_window_close_view(gMain->m_uCurView);
}

int main_window_close_view(uint16_t uView)
{
    uint16_t doc = g_mvView.at(uView)->m_uDoc;
    bool bMore = false;

    for(vector<CMaxView*>::iterator it = g_mvView.begin(); it != g_mvView.end(); ++it)
    {
        if((*it)->m_uDoc == doc && uView != (size_t)(it-g_mvView.begin()))
        {
            bMore = true;
            break;
        }
    }

    if(!bMore)
	{
		if(!g_mdDocument[doc]->m_bSaved)
		{
			CString str;

			str.Format(_T("El archivo '") STROP _T("' no esta guardado, le gustaria guardarlo?"), g_mdDocument[doc]->m_strName);

			char msg[300];
			Max_strcpy(msg, str);
			gint response = message_box( GTK_DIALOG_MODAL,
			GTK_MESSAGE_INFO,
            GTK_BUTTONS_YES_NO,
                msg);
			if(response == GTK_RESPONSE_YES)
			{
				int n = main_window_save_document(doc);
				if(n == 0)
				{
					return 0;
				}
			}
			else if(response != GTK_RESPONSE_NO)
			{
				return 0;
			}
		}
	}

	gint page = gtk_notebook_page_num( GTK_NOTEBOOK(gMain->main_notebook),  g_mvView.at(uView)->m_gtkScrolledWindow);
	// Switching info for notebook pages
	gint n = gtk_notebook_get_n_pages(GTK_NOTEBOOK(gMain->main_notebook));
	for(gint i = 0; i < n; i++)
	{
		GtkWidget *child2 = gtk_notebook_get_nth_page(GTK_NOTEBOOK(gMain->main_notebook), i);
		size_t* cura = (size_t*)( g_object_get_data( G_OBJECT( child2 ), "view" ));
		if((*cura) > uView) (*cura)--;
		g_object_set_data( G_OBJECT( child2 ), "view", cura );
	}


	delete g_mvView[uView];
	g_mvView.erase(g_mvView.begin()+uView);
	if(!bMore) main_window_close_document(doc);

	gtk_notebook_remove_page( GTK_NOTEBOOK(gMain->main_notebook), page );

	actualizate_curview();

	return 1;
}

uint16_t main_window_get_current_document(void)
{
    return g_mvView.at(gMain->m_uCurView)->m_uDoc;
}

bool main_window_is_saved_documents(void)
{
	bool bDone = true;
	if(g_mvView.size() == 0) return true;

	do
	{
		int n = gtk_notebook_get_n_pages(GTK_NOTEBOOK(gMain->main_notebook));
		if(n >0)
		{
			gtk_notebook_set_current_page(GTK_NOTEBOOK(gMain->main_notebook), 0);
		}
		int ncode = main_window_close_view(0);
		if(ncode == 0)
		{
			return false;
		}
		if(g_mvView.size() == 0) bDone = false;
	}
	while(bDone);
    return true;
}
