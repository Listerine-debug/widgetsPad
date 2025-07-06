// File Name: AppWindow.hpp
// Creator: Matteo Washington
// Date of last modification: July 6 2025
// Copyright (c) 2025 Matteo Washington

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/frame.h>
	#include <wx/sizer.h>
	#include <wx/panel.h>
#endif

#include <wx/sound.h> // for now keep out of precomp

#define MAJOR "1"
#define MINOR "0"
#define PATCH "0"
#define APP_RELEASE MAJOR "." MINOR "." PATCH

class widgetsPad : public wxApp
{
public:
	virtual bool OnInit();
};


class PadFrame : public wxFrame
{
public:
	PadFrame(const wxString& title);

	// sounds stuff ig
	// wxSound mouseclick; later installment thingy I will do

	// file stuff ig
	wxString currfilePath;
	// document stuff
	bool changesMade = false;
	bool untitled = true;

	// App panel + App textCtrl
	wxPanel* AppPanel = new wxPanel(this, wxID_ANY, 
		wxDefaultPosition, wxSize(800, 600));

	wxTextCtrl* ApptextCtrl = new wxTextCtrl(AppPanel, wxUSE_TEXTCTRL,
		wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE 
	| wxHSCROLL | wxVSCROLL);

	wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);


	// Main Menu bar
	wxMenuBar* menuBar = new wxMenuBar();

	// file menu
	wxMenu* fileMenu = new wxMenu;

	void OnNewWindow(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnSaveAs(wxCommandEvent& event);
	// void OnPrint(wxCommandEvent& event); // Maybe
	void OnClose(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);

	void TextChanged(wxCommandEvent& event);
};

class saveDialog : public wxDialog
{
public:
	saveDialog(PadFrame* parentFrame, const wxString& title);

private:
	PadFrame* m_parentFrame;
};