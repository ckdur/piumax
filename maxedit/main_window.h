#ifndef MAXEDIT_MAIN_WINDOW
#define MAXEDIT_MAIN_WINDOW



#include "stdafx.h"
#include "main_view.h"
#include "main_document.h"

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

/***************************************************************
This is my MDI interface declaration

It will use GtkNotebook and won't use separate-child windows

****************************************************************/

struct _SMain
{
	// ** Main windows
	GtkWidget  *main_window;
    GtkWidget  *main_alignment;
    GtkWidget  *main_notebook;
    GtkWidget  *main_about;

	// ** Menus
	GtkWidget	*menu_edit;
	GtkWidget	*menu_outside;
	GtkWidget	*menu_selection;
	GtkWidget	*menu_selection_mod;
	GtkWidget	*menu_edit_mod;

	// ** Toogle buttons
	GtkToggleToolButton *action_step[4];

	// ** Clipboard
	GtkClipboard* main_clip;

	// ** Info
    size_t m_uCurView;

    size_t m_uNewDocCount;
};

typedef struct _SMain SMain;

extern SMain *gMain;
//extern CMaxStepSmu *g_mssSimulator;

extern vector<CMaxDocument*> g_mdDocument;
extern vector<CMaxView*> g_mvView;

int main_window_create(void);
void main_window_destroy(void);

GtkWidget * create_notebook_label( GtkWidget   *child, const gchar *text, GtkWidget   *notebook );
gint get_view_child(GtkWidget* child);
void cb_close_tab( GtkButton   *button, GtkNotebook *notebook );
void cb_switch_page (GtkNotebook *notebook, GtkWidget *page, guint page_num, gpointer user_data);
gboolean configure (GtkWidget *da, GdkEventConfigure *event, gpointer user_data);
gboolean expose (GtkWidget *da, GdkEventExpose *event, gpointer user_data);
gboolean on_button_press (GtkWidget* widget, GdkEventButton * event);

gboolean on_notify (GtkWidget *da, GdkEventMotion *event, gpointer nothing);
gboolean on_enter_notify (GtkWidget *da, GdkEventMotion *event, gpointer nothing);
gboolean on_leave_notify (GtkWidget *da, GdkEventMotion *event, gpointer nothing);

// From main_window2.cpp

// ** Main
int main_window_initial(void);
int main_window_new_document(void);
int main_window_open_document(void);
int main_window_save_document(void);
int main_window_saveas_document(void);

int main_window_save_document(size_t cur);
int main_window_saveas_document(size_t cur);

bool main_window_is_saved_documents(void);

void main_window_close_cur_document(void);
void main_window_close_document(uint16_t uDoc);

void main_window_close_cur_view(void);
int main_window_close_view(uint16_t uView);

int main_window_new_view(void);
uint16_t main_window_get_current_document(void);

void actualizate_curview(void);

gint message_box(GtkDialogFlags flags, GtkMessageType type, GtkButtonsType buttons, const gchar *message);
gint message_box_parent(GtkWindow* window, GtkDialogFlags flags, GtkMessageType type, GtkButtonsType buttons, const gchar *message);

extern vector<cairo_surface_t*> sim;

#endif // MAXEDIT_MAIN_WINDOW
