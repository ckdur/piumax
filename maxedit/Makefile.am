## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = po

maxeditdocdir = ${prefix}/doc/maxedit
maxeditdoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = $(maxeditdoc_DATA) \
	$(INTLTOOL_FILES)

bin_PROGRAMS =  \
	maxedit

maxedit_SOURCES =  \
	BeatSplitDlg.cpp \
	ChangeBPMDlg.cpp \
	ChangeVelDlg.cpp \
	main_document.cpp \
	main_view.cpp \
	main_window.cpp \
	main_window2.cpp \
	MaxDataExchange.cpp \
	MaxDialog.cpp \
	maxedit.cpp \
	SetSizeDlg.cpp \
	signals.cpp \
	signalst.cpp \
	signalstr.cpp \
	StepOptionsDlg.cpp \
	TickCountDlg.cpp \
	TypeNoteDlg.cpp

maxedit_LDFLAGS = `pkg-config \
	--cflags \
	--libs \
	gtk+-2.0 \
	gtkglext-1.0 \
	gtkglext-x11-1.0 \
	gmodule-2.0` 

DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(maxeditdocdir)
