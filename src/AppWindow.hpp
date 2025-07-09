// File Name: AppWindow.hpp
// Creator: Matteo Washington
// Date of last modification: July 8 2025
// Copyright (c) 2025 Matteo Washington
// Description: 

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/frame.h>
	#include <wx/sizer.h>
	#include <wx/panel.h>
	#include <wx/statbmp.h>
	#include <wx/image.h>
#endif
	#include <wx/wfstream.h> 
	#include <wx/txtstrm.h> 
	#include <wx/filename.h>

#define MAJOR "1"
#define MINOR "2"
#define PATCH "0"
#define APP_RELEASE MAJOR "." MINOR "." PATCH

// App declare
class widgetsPad : public wxApp
{
public:
	virtual bool OnInit();
};

// Main frame declare // PadFrame
class PadFrame : public wxFrame
{
public:
	PadFrame(const wxString& title);

	// file stuff
	wxString filePath;
	wxString fileName;
	wxString fileContent;
	// wxFileName fn;
	bool txt_Changes = false;
	bool txt_Untitled = true;

	// main panel and text area declare
	wxPanel* appPanel;
	wxTextCtrl* apptextCtrl;
	wxBoxSizer* panelSizer;

	// menu bar stuff
	wxMenuBar* menuBar;
	wxMenu* fileMenu;
	wxMenu* editMenu;
	wxMenu* aboutMenu;

	// File menu buttons
	void OnNewWindow(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnSaveAs(wxCommandEvent& event);
	// void OnPrint(wxCommandEvent& event); still a maybe
	void OnClose(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);

	void OnTextChanged(wxCommandEvent& event);
	// void OnRead();

	// Edit menu buttons
	// 
	// void OnUndo(wxCommandEvent& event);
	// void OnCut(wxCommandEvent& event);
	// void OnCopy(wxCommandEvent& event);
	// void OnPaste(wxCommandEvent& event);
	// void OnDelete(wxCommandEvent& event);
	
	// void OnFind(wxCommandEvent& event);
	// void OnFindNext(wxCommandEvent& event);
	// void OnFindPrev(wxCommandEvent& event);
	// void OnReplace(wxCommandEvent& event);
	// void OnGoTo(wxCommandEvent& event);
	// void OnFont(wxCommandEvent& event);

	// About menu buttons
	// 
	// void OnAbout(wxCommandEvent& event);
	// void OnHelp(wxCommandEvent& event);
};


class SaveDialog : public wxDialog
{
public:
	SaveDialog(PadFrame* parentFrame, const wxString& fileName);
private:
	PadFrame* m_parentFrame;

	// buttons
	void OnSaveDialog(wxCommandEvent& event);
	void OnDontSave(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
};

/*
class aboutDialog : public wxDialog
{
public:
	aboutDialog(PadFrame& parentFrame);
private:
	PadFrame& m_ParentFrame;
};
*/
