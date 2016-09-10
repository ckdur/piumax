#ifndef MAXDataExchange
#define MAXDataExchange

#include <gtk/gtk.h>
#include <vector>
#include <cstring>

using namespace std;

enum MAXDATA_TYPE{
	MAXDATA_NULL,
	MAXDATA_INT,
	MAXDATA_FLOAT,
	MAXDATA_DOUBLE,
	MAXDATA_TEXT,
	MAXDATA_BOOL,
	MAXDATA_SIZE
};

enum MAXCONTROL_TYPE{
	MAXCONTROL_NULL,
	MAXCONTROL_TEXT,
	MAXCONTROL_SPIN,
	MAXCONTROL_CHECK,
	MAXCONTROL_RADIO,
	MAXCONTROL_TOGGLE,
	MAXCONTROL_COMBO,
	MAXCONTROL_TREEVIEW,
	MAXCONTROL_SCALE,
	MAXCONTROL_SCROLL,
	MAXCONTROL_SIZE

};

struct _MaxData{
	GtkWidget* control;
	void* var;
	MAXDATA_TYPE type_data;
	MAXCONTROL_TYPE type_control;
};

// ** Macro declaration

class CMaxDataExchange
{
public:
	CMaxDataExchange(void);
	~CMaxDataExchange(void);

	vector<_MaxData> m_data;
	int DoDataExchange(bool bGet);
	int Add(GtkWidget* control, MAXCONTROL_TYPE type_control,
		void* var, MAXDATA_TYPE type_data);
	void DeleteAll(void);

private:
	int Assign(const gchar* text, size_t i);
	int Assign(const gint num, size_t i);
	int Assign(const gfloat num, size_t i);
	int Assign(const gdouble num, size_t i);
	void RessignChar(gchar* ch, size_t i);
	gfloat RessignFloat(size_t i);
	gdouble RessignDouble(size_t i);
	gint RessignInt(size_t i);
};

#endif // MAXDataExchange
