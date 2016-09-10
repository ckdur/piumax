#include "TypeNoteDlg.h"
#include "main_window.h"
#include "main_document.h"

GtkWidget *CTypeNoteDlg::m_main_dlg;

CMaxDataExchange CTypeNoteDlg::m_exc;

// ** Custom controls here
GtkTreeView* CTypeNoteDlg::m_treeview_type_note_type;
GtkComboBox* CTypeNoteDlg::m_combo_type_note;
GtkLabel* CTypeNoteDlg::m_label_descr_type_note;

// ** Data here
int CTypeNoteDlg::m_nSel;
int CTypeNoteDlg::m_nValue;
int CTypeNoteDlg::m_nArg1;
int CTypeNoteDlg::m_nArg2;
int CTypeNoteDlg::m_nType;

int CTypeNoteDlg::m_nIndexGrab;

CTypeNoteDlg::CTypeNoteDlg(void)
{
	m_nIndexGrab = -1;
}


CTypeNoteDlg::~CTypeNoteDlg(void)
{
}

void CTypeNoteDlg::Set(GtkBuilder* builder)
{
	m_main_dlg = g_slice_new( GtkWidget );
	m_main_dlg = GTK_WIDGET( gtk_builder_get_object( builder, "dialog_type_note" ) );

	// Declare controls here
	m_treeview_type_note_type = GTK_TREE_VIEW (
				gtk_builder_get_object( builder, "treeview_type_note_type" ) );
	m_combo_type_note = GTK_COMBO_BOX(gtk_builder_get_object( builder, "combo_type_note" ));
	m_label_descr_type_note = GTK_LABEL(gtk_builder_get_object( builder, "label_descr_type_note" ));

	// Declare data exchanges here
	m_exc.Add(GTK_WIDGET(m_combo_type_note), MAXCONTROL_COMBO, &m_nSel, MAXDATA_INT);
	m_nIndexGrab = (int)(m_exc.m_data.size())-1;
	m_exc.Add(GTK_WIDGET(m_treeview_type_note_type), MAXCONTROL_TREEVIEW, &m_nType, MAXDATA_INT);
	m_exc.Add(GTK_WIDGET(gtk_builder_get_object( builder, "entry_value_type_note" )), MAXCONTROL_TEXT, &m_nValue, MAXDATA_INT);
	m_exc.Add(GTK_WIDGET(gtk_builder_get_object( builder, "entry_arg1" )), MAXCONTROL_TEXT, &m_nArg1, MAXDATA_INT);
	m_exc.Add(GTK_WIDGET(gtk_builder_get_object( builder, "entry_arg2" )), MAXCONTROL_TEXT, &m_nArg2, MAXDATA_INT);

	// Connect signals here
	g_signal_connect(m_main_dlg, 
					 "response", 
					 G_CALLBACK( on_dialog_type_note_response), 
					 NULL);
	g_signal_connect(m_main_dlg, 
					 "show", 
					 G_CALLBACK( on_dialog_type_note_show), 
					 NULL);
	g_signal_connect(m_combo_type_note, 
					 "changed", 
					 G_CALLBACK( on_combo_type_note_changed), 
					 NULL);
	GtkTreeSelection* selection = gtk_tree_view_get_selection(m_treeview_type_note_type);
	g_signal_connect(selection, 
					 "changed", 
					 G_CALLBACK( on_tree_view_type_note_type_changed), 
					 NULL);

}

int CTypeNoteDlg::DoModal(void)
{
	gtk_window_set_modal( GTK_WINDOW(m_main_dlg), true);
	gint r = gtk_dialog_run ( GTK_DIALOG( m_main_dlg ) );
	return r;
}

int CTypeNoteDlg::DoNoModal(void)
{
	gtk_window_set_modal( GTK_WINDOW(m_main_dlg), false);
	gtk_widget_show_all ( GTK_WIDGET( m_main_dlg ) );
	g_signal_connect_swapped (m_main_dlg,
                             "delete_event",
                             G_CALLBACK (gtk_widget_hide_on_delete),
                             m_main_dlg);
	return 1;
}


int CTypeNoteDlg::UpdateData(bool bGet)
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

void CTypeNoteDlg::on_dialog_type_note_response(GtkDialog* dialog, 
		gint response_id, gpointer userdata)
{
	if(response_id == GTK_RESPONSE_ACCEPT)
	{
		UpdateData(true);
		//DO MAGIC
	}
	gtk_widget_hide(GTK_WIDGET(m_main_dlg));
}

void CTypeNoteDlg::on_dialog_type_note_show(GtkDialog* dialog,
										  gpointer userdata)
{
	// INITIAL UPDATE
	if(m_nIndexGrab >= 0) 
		gtk_widget_grab_focus(m_exc.m_data.at(m_nIndexGrab).control);

	UpdateData(false);
}

void CTypeNoteDlg::on_combo_type_note_changed (GtkComboBox *widget, gpointer user_data)
{
	UpdateData(true);
	GtkListStore* store;
	if((store = GTK_LIST_STORE(gtk_tree_view_get_model(m_treeview_type_note_type))) == NULL)
	{
		store = gtk_list_store_new(2, G_TYPE_INT, G_TYPE_STRING);
		gtk_tree_view_set_model(m_treeview_type_note_type, GTK_TREE_MODEL(store));
	}
	GtkTreeIter iter;

	GtkTreeViewColumn* col =
		gtk_tree_view_get_column(m_treeview_type_note_type, 0);
	if(col == NULL)
	{
		GtkCellRenderer* renderer = gtk_cell_renderer_text_new ();
		col = gtk_tree_view_column_new_with_attributes ("Atributos", renderer,
														"text", 1,
														NULL);
		gtk_tree_view_insert_column (m_treeview_type_note_type, col, 0);
	}

	gtk_list_store_clear(store);

	if(m_nSel == 21)	// THIS IS THE CODE FOR BRAIN SHOWER
	{
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[MEM] Memorizacion"	, 0, 0, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[MEM] Parejas"		, 0, 1, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[MEM] MemEx. 5"		, 0, 2, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[MEM] Sec. Flechas"	, 0, 3, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[MEM] Ocul. Flechas"	, 0, 4, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[OBS] Minimo Ap"		, 0, 5, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[OBS] Maximo Ap"		, 0, 6, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[OBS] Ordenar"		, 0, 7, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[OBS] Encontrar"		, 0, 8, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[OBS] Interrogante"	, 0, 9, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[MAT] O/X"			, 0, 10, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[MAT] Resultado"		, 0, 11, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[MAT] Operacion"		, 0, 12, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[MAT] Ordenar"		, 0, 13, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[MAT] Juzgar"			, 0, 14, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[ANA] Silueta"		, 0, 15, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[ANA] Contar"			, 0, 16, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[ANA] Secuencia"		, 0, 17, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[ANA] Gir. 1 Flecha"	, 0, 18, -1);
		gtk_list_store_append(store, &iter); gtk_list_store_set(store, &iter, 1, "[ANA] Gir. Sec. Fle."	, 0, 19, -1);
	}
	else if(m_nSel > 22)		// REFER TO LUA
	{
	}

	// CODE FOR DESCRIPTIONS
	switch(m_nSel)
	{
		case 1:		gtk_label_set_text(m_label_descr_type_note, "Tap: Flecha comun. Valor = Veces que cuenta"); break;
		case 2:		gtk_label_set_text(m_label_descr_type_note, "Top Hold: Hold superior. Valor = Veces que cuenta"); break;
		case 3:		gtk_label_set_text(m_label_descr_type_note, "Body Hold: Hold en el medio. Valor = Veces que cuenta"); break;
		case 4:		gtk_label_set_text(m_label_descr_type_note, "Bottom Hold: Hold Inferior. Valor = Veces que cuenta"); break;
		case 5:		gtk_label_set_text(m_label_descr_type_note, "Mine Item: Mina que baja la barra de vida. Valor = Veces que baja (si negativo, cuenta 1/Veces)"); break;
		case 6:		gtk_label_set_text(m_label_descr_type_note, "Potion Item: Pocion que sube la barra de vida. Valor = Veces que sube (si negativo, cuenta 1/Veces)"); break;
		case 7:		gtk_label_set_text(m_label_descr_type_note, "Heart Item: Corazon que baja el judgament de la barra de vida. Valor = Veces que cuenta (si negativo, cuenta 1/Veces)"); break;
		case 8:		gtk_label_set_text(m_label_descr_type_note, "Flash Item: Item que induce el efecto 'Flash'. Valor no afecta"); break;
		case 9:		gtk_label_set_text(m_label_descr_type_note, "Back to normal: Item que anula todos los efectos visuales causados por items. Valor no afecta"); break;
		case 10:	gtk_label_set_text(m_label_descr_type_note, "x1 Velocity: Item de velocidad x1. Valor = Numero en Beats que se produce la transicion (si negativo, cuenta 1/Veces)"); break;
		case 11:	gtk_label_set_text(m_label_descr_type_note, "x2 Velocity: Item de velocidad x2. Valor = Numero en Beats que se produce la transicion (si negativo, cuenta 1/Veces)"); break;
		case 12:	gtk_label_set_text(m_label_descr_type_note, "x3 Velocity: Item de velocidad x3. Valor = Numero en Beats que se produce la transicion (si negativo, cuenta 1/Veces)"); break;
		case 13:	gtk_label_set_text(m_label_descr_type_note, "x4 Velocity: Item de velocidad x4. Valor = Numero en Beats que se produce la transicion (si negativo, cuenta 1/Veces)"); break;
		case 14:	gtk_label_set_text(m_label_descr_type_note, "x5 Velocity: Item de velocidad x5. Valor = Numero en Beats que se produce la transicion (si negativo, cuenta 1/Veces)"); break;
		case 15:	gtk_label_set_text(m_label_descr_type_note, "x8 Velocity: Item de velocidad x8. Valor = Numero en Beats que se produce la transicion (si negativo, cuenta 1/Veces)"); break;
		case 16:	gtk_label_set_text(m_label_descr_type_note, "Drop Item: Item que coloca las flechas al reves. Valor no afecta"); break;
		case 17:	gtk_label_set_text(m_label_descr_type_note, "Under Attack Item: Item que induce el ataque 'Under' que hace que las flechas vayan hacia abajo. Valor no afecta"); break;
		case 18:	gtk_label_set_text(m_label_descr_type_note, "Left Attack Item: Item que induce el ataque 'Left' que hace que las flechas vayan hacia la izquierda. Valor no afecta"); break;
		case 19:	gtk_label_set_text(m_label_descr_type_note, "Right Attack Item: Item que induce el ataque 'Right' que hace que las flechas vayan hacia la derecha. Valor no afecta"); break;
		case 20:	gtk_label_set_text(m_label_descr_type_note, "Snake Item: Item que hace el efecto 'Snake'. Valor no afecta"); break;
		case 21:	gtk_label_set_text(m_label_descr_type_note, "Brain Shower Question: Pregunta de Brain Shower. Selecciona un tipo de pregunta para la ayuda."); break;
		case 22:	gtk_label_set_text(m_label_descr_type_note, "Brain Shower Answer: Respuesta de Brain Shower. Dependiendo de la pregunta anterior debe colocar un set de respuestas. Valor no afecta"); break;
		case 23:	gtk_label_set_text(m_label_descr_type_note, "Brain Shower Pop-up: Pop-up de Brain Shower. Valor = ID del mensaje"); break;
		case 24:	gtk_label_set_text(m_label_descr_type_note, "DynVanish: (Despreciado) Vanish dinamico. Coloca las flechas desde este momento en un vanish. Arg1 = Vanish superior o inferior, Arg2 = Valor Vanish x10 (%)"); break;
		case 25:	gtk_label_set_text(m_label_descr_type_note, "DynAttack: Fuerza un ataque, lo cual hace que las flechas se dirijan a algun lugar. Valor = Direccion (0 a 3)"); break;
		case 26:	gtk_label_set_text(m_label_descr_type_note, "DynZigZag: (Despreciado) Induce el efecto de ZigZag. Valor = Confusion (0 a 8)"); break;
	}
}

void CTypeNoteDlg::on_tree_view_type_note_type_changed (GtkTreeView *tree_view, gpointer user_data)
{
	UpdateData(true);
	if(m_nSel == 21)	// THIS IS THE CODE FOR BRAIN SHOWER
	{
		switch(m_nType)
		{
			case 0:		gtk_label_set_text(m_label_descr_type_note, "Memorizacion: Pregunta de memoria. Se muestra una secuencia de items y luego debe volverlas a pisar en el orden correcto. Arg1 = Velocidad & Tipo de item. Arg2 = Numero de respuestas."); break;
			case 1:		gtk_label_set_text(m_label_descr_type_note, "Parejas: Pregunta de memoria. Se muestra un panel con 9 objetos al comienzo. Debe encontrar los pares a medida que se muestren. Arg1 = Tipo de item."); break;
			case 2:		gtk_label_set_text(m_label_descr_type_note, "MemEx. 5: Pregunta de memoria. Se muestra una secuencia de 5 items y luego debe pisar sobre el panel en el orden que aparecio. Arg1 = Tipo de item."); break;
			case 3:		gtk_label_set_text(m_label_descr_type_note, "Secuencia Flechas: Pregunta de memoria. Se muestra una secuencia de flechas que debera pisar (adicionalmente se muestra en un recuadro) y luego debe volverlas a pisar en el orden correcto. Arg1 = Numero de flechas."); break;
			case 4:		gtk_label_set_text(m_label_descr_type_note, "Ocultar flechas: Pregunta de memoria. Se ocultan las flechas en signos de ?. Arg1 = Numero de flechas. Arg2 = Nivel de desaparicion (1-10)."); break;

			case 5:		gtk_label_set_text(m_label_descr_type_note, "Minimo aparecido: Pregunta de observacion. Se muestra una secuencia de items y luego debe pisar el que mas veces aparecio. Arg1 = Velocidad & Tipo de item. Arg2 = Numero de items."); break;
			case 6:		gtk_label_set_text(m_label_descr_type_note, "Maximo aparecido: Pregunta de observacion. Se muestra una secuencia de items y luego debe pisar el que menos veces aparecio. Arg1 = Velocidad & Tipo de item. Arg2 = Numero de items."); break;
			case 7:		gtk_label_set_text(m_label_descr_type_note, "Ordenar: Pregunta de observacion. Se muestra una secuencia de numeros y luego debe pisar en el orden correcto. Arg1 = (1) Menor a mayor, (0) Mayor a menor."); break;
			case 8:		gtk_label_set_text(m_label_descr_type_note, "Encontrar frecuencia: Pregunta de observacion. Se muestran items en pantalla y luego debe pisar el mas o menos aparecido. Arg1 = (1) Mas aparecido, (0) Menos aparecido. Arg2 = Tipo de item."); break;
			case 9:		gtk_label_set_text(m_label_descr_type_note, "Interrogante: Pregunta de observacion. Se muestran items en pantalla y luego debe pisar el que se oculto en el ?. Arg1 = Tipo de item. Arg2 = Numero de items diferentes."); break;

			case 10:	gtk_label_set_text(m_label_descr_type_note, "Verdadero/Falso: Pregunta de matematicas. Se muestra una expresion y tiene que marcar si es verdadero o falso. Arg1 = Numero de terminos. Arg2 = Numero de terminos a la izquierda."); break;
			case 11:	gtk_label_set_text(m_label_descr_type_note, "Respuesta: Pregunta de memoria. Se muestra una expresion numerica y luego debe pisar la respuesta. Arg1 = Numero de terminos. Arg2 = Maximo resultado."); break;
			case 12:	gtk_label_set_text(m_label_descr_type_note, "Encontrar operacion: Pregunta de matematicas. Se muestra una expresion y luego debe pisar cual es el signo aritmetico correcto para que la expresion sea correcta. Arg1 = Numero de terminos. Arg2 = Maximo resultado."); break;
			case 13:	gtk_label_set_text(m_label_descr_type_note, "Ordenar: Pregunta de matematicas. Se muestra en el panel numeros que indican sumas y debe pisar el orden correcto. Arg1 = (1) Menor a mayor, (0) Mayor a menor. Arg2 = Maximo sumando."); break;
			case 14:	gtk_label_set_text(m_label_descr_type_note, "Juzgar: Pregunta de matematicas. Se muestra una expresion y luego debe pisar si lo de la izquierda es mayor, menor o igual que la derecha. Arg1 = Numero de terminos. Arg2 = Numero de terminos a la izquierda."); break;

			case 15:	gtk_label_set_text(m_label_descr_type_note, "Silueta: Pregunta de analisis. Se muestra una silueta de un item y luego debe pisar el item correcto. Arg1 = Tipo de item."); break;
			case 16:	gtk_label_set_text(m_label_descr_type_note, "Contar: Pregunta de analisis. Se muestran items en pantalla con formas de tetris y luego pisar el numero de la cantidad que hay. Arg1 = Tipo de item. Arg2 = Numero de conjuntos."); break;
			case 17:	gtk_label_set_text(m_label_descr_type_note, "Secuencia: Pregunta de analisis. Se muestra una secuencia de items y luego debe buscar el item en ? (Razonamiento abstracto). Arg1 = Velocidad & Tipo de item. Arg2 = Numero de items."); break;
			case 18:	gtk_label_set_text(m_label_descr_type_note, "Girar 1 flecha: Pregunta de analisis. Se muestra una flecha y un angulo, y luego debe pisar la flecha resultante. Argumentos no aplican."); break;
			case 19:	gtk_label_set_text(m_label_descr_type_note, "Girar Secuencia de flechas: Pregunta de analisis. Se muestra una secuencia de flechas y un angulo, y luego debe pisar la flecha resultante. Arg1 = Numero de flechas a memorizar."); break;
		}
	}
}