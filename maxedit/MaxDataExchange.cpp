#include "MaxDataExchange.h"


CMaxDataExchange::CMaxDataExchange(void)
{
}


CMaxDataExchange::~CMaxDataExchange(void)
{
	DeleteAll();
}

int CMaxDataExchange::DoDataExchange(bool bGet)
{
	for(size_t i = 0; i < m_data.size(); i++)
	{
		if(bGet)
		{
			switch(m_data[i].type_control)
			{
			case MAXCONTROL_TEXT:
				{
					const gchar* text =
						gtk_entry_get_text(GTK_ENTRY(m_data[i].control));
					if(!Assign(text, i)) return i;
				}
				break;
			case MAXCONTROL_SPIN:
				{
					gdouble data = gtk_spin_button_get_value(
						GTK_SPIN_BUTTON(m_data[i].control));
					if(!Assign(data, i)) return i;
				}
				break;
			case MAXCONTROL_CHECK:
			case MAXCONTROL_RADIO:
			case MAXCONTROL_TOGGLE:
				{
					gint data = GTK_TOGGLE_BUTTON (m_data[i].control)->active;
					if(!Assign(data, i)) return i;
				}
				break;
			case MAXCONTROL_COMBO:
				{
					GtkTreeIter   iter;
					gint       data = -1;
					GtkTreeModel *model;

					/* Obtain currently selected item from combo box.
					 * If nothing is selected, do nothing. */
					if( gtk_combo_box_get_active_iter(
						GTK_COMBO_BOX( m_data[i].control ), &iter ) )
					{
						model = gtk_combo_box_get_model(
							GTK_COMBO_BOX( m_data[i].control ));

						/* Some nasty things can be done in Glade */
						if(gtk_tree_model_get_column_type(model, 0) == G_TYPE_INT)
							gtk_tree_model_get( model, &iter, 0, &data, -1 );
					}
					if(!Assign(data, i)) return i;
				}
				break;
			case MAXCONTROL_TREEVIEW:
				{
					GtkTreeIter   iter;
					gint       data = -1;
					GtkTreeModel *model;
					model = gtk_tree_view_get_model(GTK_TREE_VIEW( m_data[i].control ));
					GtkTreeSelection *selection = 
						gtk_tree_view_get_selection(GTK_TREE_VIEW( m_data[i].control ));

					/* Obtain currently selected item from view.
					 * If nothing is selected, do nothing. */
					if( gtk_tree_selection_get_selected(selection, &model, &iter) &&
						gtk_tree_model_get_column_type(model, 0) == G_TYPE_INT)
					{
						gtk_tree_model_get( model, &iter, 0, &data, -1);
					}
					if(!Assign(data, i)) return i;
				}
				break;
			case MAXCONTROL_SCALE:
			case MAXCONTROL_SCROLL:
				{
					GtkAdjustment* adj;
					adj = gtk_range_get_adjustment(GTK_RANGE(m_data[i].control));
					if(!Assign(adj->value, i)) return i;
				}
				break;
			default: return i; break;
			}
		}
		else
		{
			switch(m_data[i].type_control)
			{
			case MAXCONTROL_TEXT:
				{
					gchar text[255] = "\0";
					RessignChar(text, i);
					gtk_entry_set_text(GTK_ENTRY(m_data[i].control), text);
				}
				break;
			case MAXCONTROL_SPIN:
				{
					gdouble data;
					data = RessignDouble(i);
					gtk_spin_button_set_value(
						GTK_SPIN_BUTTON(m_data[i].control), data);
					if(!Assign(data, i)) return i;
				}
				break;
			case MAXCONTROL_CHECK:
			case MAXCONTROL_RADIO:
			case MAXCONTROL_TOGGLE:
				{
					gboolean data;
					data = RessignInt(i);
					gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (m_data[i].control), data);
				}
				break;
			case MAXCONTROL_COMBO:
				// OH WAIT, WE CAN DO IT!
				{
					GtkTreeIter   iter;
					gint       data = RessignInt(i), data2;
					GtkTreeModel *model;

					model = gtk_combo_box_get_model(
						GTK_COMBO_BOX( m_data[i].control ));

					if( gtk_tree_model_get_iter_first(
						model, &iter ) && 
						gtk_tree_model_get_column_type(model, 0) == G_TYPE_INT)
					{
						do
						{
							gtk_tree_model_get( model, &iter, 0, &data2, -1 );
							if(data == data2)
							{
								gtk_combo_box_set_active_iter(
									GTK_COMBO_BOX( m_data[i].control ),
									&iter);
								break;
							}
						} while (gtk_tree_model_iter_next(model, &iter));
					}
					if(!Assign(data, i)) return i;
				}
				break;
			case MAXCONTROL_TREEVIEW:
				{
					GtkTreeIter   iter;
					gint       data = RessignInt(i), data2;
					GtkTreeModel *model;

					model =gtk_tree_view_get_model(GTK_TREE_VIEW( m_data[i].control ));

					if( gtk_tree_model_get_iter_first(
						model, &iter ) && 
						gtk_tree_model_get_column_type(model, 0) == G_TYPE_INT)
					{
						do
						{
							gtk_tree_model_get( model, &iter, 0, &data2, -1 );
							if(data == data2)
							{
								GtkTreePath* path = 
									gtk_tree_model_get_path(model, 
									&iter);
								gtk_tree_view_set_cursor(
									GTK_TREE_VIEW( m_data[i].control ),
									path,
									NULL, 
									FALSE);
								break;
							}
						} while (gtk_tree_model_iter_next(model, &iter));
					}
					if(!Assign(data, i)) return i;
				}
				break;
			case MAXCONTROL_SCALE:
			case MAXCONTROL_SCROLL:
				{
					gdouble data;
					data = RessignDouble(i);
					GtkAdjustment* adj;
					adj = gtk_range_get_adjustment(GTK_RANGE(m_data[i].control));
					gtk_adjustment_set_value(adj, data);
					gtk_range_set_adjustment(GTK_RANGE(m_data[i].control), adj);
				}
				break;
			default: return i; break;
			}
		}
	}
	return -1;
}

int CMaxDataExchange::Assign(const gchar* text, size_t i)
{
	switch(m_data[i].type_data)
	{
	case MAXDATA_TEXT:
		{
			if(text == NULL) return 0;
			char** data = (char**)m_data[i].var;
			strcpy((*data), text);
		}
		break;
	case MAXDATA_INT:
		{
			int* data = (int*)m_data[i].var;
			int asg = sscanf(text, "%d", data);
			if(asg!=1) return 0;
		}
		break;
	case MAXDATA_FLOAT:
		{
			float* data = (float*)m_data[i].var;
			if(sscanf(text, "%g", data)!=1) return 0;
		}
		break;
	case MAXDATA_DOUBLE:
		{
			double* data = (double*)m_data[i].var;
			if(sscanf(text, "%lg", data)!=1) return 0;
		}
		break;
	default: return 0; break;
	}
	return 1;
}

int CMaxDataExchange::Assign(const gint num, size_t i)
{
	switch(m_data[i].type_data)
	{
	case MAXDATA_TEXT:
		{
			char** data = (char**)m_data[i].var;
			sprintf((*data), "%d", num);
		}
		break;
	case MAXDATA_INT:
		{
			int* data = (int*)m_data[i].var;
			(*data) = num;
		}
		break;
	case MAXDATA_FLOAT:
		{
			float* data = (float*)m_data[i].var;
			(*data) = float(num);
		}
		break;
	case MAXDATA_DOUBLE:
		{
			double* data = (double*)m_data[i].var;
			(*data) = double(num);
		}
		break;
	case MAXDATA_BOOL:
		{
			bool* data = (bool*)m_data[i].var;
			(*data) = num?true:false;
		}
		break;
	default: return 0; break;
	}
	return 1;
}

int CMaxDataExchange::Assign(const gfloat num, size_t i)
{
	switch(m_data[i].type_data)
	{
	case MAXDATA_TEXT:
		{
			char** data = (char**)m_data[i].var;
			sprintf((*data), "%g", num);
		}
		break;
	case MAXDATA_INT:
		{
			int* data = (int*)m_data[i].var;
			(*data) = int(num);
		}
		break;
	case MAXDATA_FLOAT:
		{
			float* data = (float*)m_data[i].var;
			(*data) = num;
		}
		break;
	case MAXDATA_DOUBLE:
		{
			double* data = (double*)m_data[i].var;
			(*data) = double(num);
		}
		break;
	case MAXDATA_BOOL:
		{
			bool* data = (bool*)m_data[i].var;
			(*data) = num!=0.0f?true:false;
		}
		break;
	default: return 0; break;
	}
	return 1;
}

int CMaxDataExchange::Assign(const gdouble num, size_t i)
{
	switch(m_data[i].type_data)
	{
	case MAXDATA_TEXT:
		{
			char** data = (char**)m_data[i].var;
			sprintf((*data), "%g", num);
		}
		break;
	case MAXDATA_INT:
		{
			int* data = (int*)m_data[i].var;
			(*data) = int(num);
		}
		break;
	case MAXDATA_FLOAT:
		{
			float* data = (float*)m_data[i].var;
			(*data) = float(num);
		}
		break;
	case MAXDATA_DOUBLE:
		{
			double* data = (double*)m_data[i].var;
			(*data) = num;
		}
		break;
	case MAXDATA_BOOL:
		{
			bool* data = (bool*)m_data[i].var;
			(*data) = num!=0.0?true:false;
		}
		break;
	default: return 0; break;
	}
	return 1;
}

 void CMaxDataExchange::RessignChar(gchar* ch, size_t i)
{
	switch(m_data[i].type_data)
	{
	case MAXDATA_TEXT:
		{
			char** data = (char**)m_data[i].var;
			strcpy(ch, (*data));
		}
		break;
	case MAXDATA_INT:
		{
			int* data = (int*)m_data[i].var;
			sprintf(ch, "%d", *data);
		}
		break;
	case MAXDATA_FLOAT:
		{
			float* data = (float*)m_data[i].var;
			sprintf(ch, "%g", *data);
		}
		break;
	case MAXDATA_DOUBLE:
		{
			double* data = (double*)m_data[i].var;
			sprintf(ch, "%g", *data);
		}
		break;
	default: return; break;
	}
}

gfloat CMaxDataExchange::RessignFloat(size_t i)
{
	switch(m_data[i].type_data)
	{
	case MAXDATA_TEXT:
		{
			char** data = (char**)m_data[i].var;
			gfloat a;
			if(sscanf(*data, "%g", &a)!=1) return 0.0f;
			return a;
		}
		break;
	case MAXDATA_INT:
		{
			int* data = (int*)m_data[i].var;
			return (gfloat)(*data);
		}
		break;
	case MAXDATA_FLOAT:
		{
			float* data = (float*)m_data[i].var;
			return (*data);
		}
		break;
	case MAXDATA_DOUBLE:
		{
			double* data = (double*)m_data[i].var;
			return (gfloat)(*data);
		}
		break;
	default: return 0.0f; break;
	}
	return 0.0f;
}

gdouble CMaxDataExchange::RessignDouble(size_t i)
{
	switch(m_data[i].type_data)
	{
	case MAXDATA_TEXT:
		{
			char** data = (char**)m_data[i].var;
			gdouble a;
			if(sscanf(*data, "%lg", &a)!=1) return 0.0;
			return a;
		}
		break;
	case MAXDATA_INT:
		{
			int* data = (int*)m_data[i].var;
			return (gdouble)(*data);
		}
		break;
	case MAXDATA_FLOAT:
		{
			float* data = (float*)m_data[i].var;
			return (gdouble)(*data);
		}
		break;
	case MAXDATA_DOUBLE:
		{
			double* data = (double*)m_data[i].var;
			return (*data);
		}
		break;
	default: return 0.0; break;
	}
	return 0.0;
}

gint CMaxDataExchange::RessignInt(size_t i)
{
	switch(m_data[i].type_data)
	{
	case MAXDATA_TEXT:
		{
			char** data = (char**)m_data[i].var;
			gint a;
			if(sscanf(*data, "%d", &a)!=1) return 0;
			return a;
		}
		break;
	case MAXDATA_BOOL:
		{
			bool* data = (bool*)m_data[i].var;
			return (*data)?1:0;
		}
		break;
	case MAXDATA_INT:
		{
			int* data = (int*)m_data[i].var;
			return (*data);
		}
		break;
	case MAXDATA_FLOAT:
		{
			float* data = (float*)m_data[i].var;
			return (gint)(*data);
		}
		break;
	case MAXDATA_DOUBLE:
		{
			double* data = (double*)m_data[i].var;
			return (gint)(*data);
		}
		break;
	default: return 0; break;
	}
}

int CMaxDataExchange::Add(GtkWidget* control, MAXCONTROL_TYPE type_control,
		void* var, MAXDATA_TYPE type_data)
{
	_MaxData data;
	data.control = control;
	data.type_control = type_control;
	data.var = var;
	data.type_data = type_data;
	m_data.push_back(data);
	return 1;
}

void CMaxDataExchange::DeleteAll(void)
{
	m_data.clear();
}
