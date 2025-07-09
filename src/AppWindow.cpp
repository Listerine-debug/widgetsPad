// File Name: AppWindow.cpp
// Creator: Matteo Washington
// Date of last modification: July 8 2025
// Copyright (c) 2025 Matteo Washington

#include "AppWindow.hpp"

// PADFRAME BEGIN
//
// PADFRAME CONSTRUCTOR
PadFrame::PadFrame(const wxString& title)
	: wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
	// Panel creation
	appPanel = new wxPanel
	(	
		this, 
		wxID_ANY,
		wxDefaultPosition, 
		wxSize(800, 600)
	);

	// text ctrl creation
	apptextCtrl = new wxTextCtrl
	(
		appPanel, 
		wxUSE_TEXTCTRL,
		wxEmptyString,
		wxDefaultPosition, 
		wxDefaultSize,
		wxTE_MULTILINE | wxHSCROLL | wxVSCROLL
	);

	menuBar = new wxMenuBar();
	fileMenu = new wxMenu();

	menuBar->Append(fileMenu, "&File");
	SetMenuBar(menuBar);

	// box siser creation
	panelSizer = new wxBoxSizer(wxVERTICAL);

	// Add panel sizer to textctrl
	panelSizer->Add(apptextCtrl, 1, wxEXPAND | wxALL, 5);
	appPanel->SetSizer(panelSizer);

	// buttons append to file menu
	fileMenu->Append(wxID_NEW,		"&New Window\tCtrl-N",		"");
	fileMenu->Append(wxID_OPEN,		"&Open\tCtrl-O",			"");
	fileMenu->Append(wxID_SAVE,		"&Save\tCtrl-S",			"");
	fileMenu->Append(wxID_SAVEAS,	"&Save As\tCtrl+Alt+S",		"");
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_CLOSE,	"&Close\tCtrl+Alt+C",		"");
	fileMenu->Append(wxID_EXIT,		"&Exit",					"");

	Bind(wxEVT_MENU, &PadFrame::OnNewWindow, this, wxID_NEW);
	Bind(wxEVT_MENU, &PadFrame::OnOpen, this, wxID_OPEN);
	Bind(wxEVT_MENU, &PadFrame::OnSave, this, wxID_SAVE);
	Bind(wxEVT_MENU, &PadFrame::OnSaveAs, this, wxID_SAVEAS);
	Bind(wxEVT_MENU, &PadFrame::OnClose, this, wxID_CLOSE);
	Bind(wxEVT_MENU, &PadFrame::OnExit, this, wxID_EXIT);

	// Font control, will update to change font, as of NOW,
	// font has been set to Consolas 13 size, as Windows 10 Notepad
	wxFont Font
	(
		13, // size
		wxFONTFAMILY_MODERN, // family
		wxFONTSTYLE_NORMAL, // style
		wxFONTWEIGHT_NORMAL, // weight
		false, // underline ?
		"Consolas" // font itself
	);

	// bind font to text plain
	apptextCtrl->SetFont(Font);
	apptextCtrl->Bind(wxEVT_TEXT, &PadFrame::OnTextChanged, this);

	//status bar, update later
	CreateStatusBar();
	SetStatusText("widgetsPad | App Version: Alpha " APP_RELEASE);
}
// PADFRAME CONSTRUCTOR

// Create New instance of APP
void PadFrame::OnNewWindow(wxCommandEvent& event)
{
	::wxExecute(wxTheApp->argv[0]);
}

// Open Existing file
void PadFrame::OnOpen(wxCommandEvent& event)
{
	// Open file explorer
	wxFileDialog openLog
	(
		this,
		"Open file",
		"",
		"",
		"All files (*.*)|*.*",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST
	);

	if (openLog.ShowModal() != wxID_OK)
	{
		return;
	}

	filePath = openLog.GetPath();
	wxFileName fn(filePath);
	fileName = fn.GetFullName();

	SetTitle(fileName + " - widgetsPad");
	wxFileInputStream Stream(filePath);
	if (!Stream.IsOk())
	{
		wxMessageBox("Cannont open file!", "Error", wxICON_ERROR); 
		return;
	}

	// READ FILE, but need to change to own function later, 
	// maybe, idk how but I will find a way
	wxTextInputStream Text(Stream, "\x09", wxConvUTF8);
	while (Stream.IsOk() && !Stream.Eof())
	{
		fileContent += Text.ReadLine();
		if (!Stream.Eof())
			fileContent += "\n";
	}
	apptextCtrl->SetValue(fileContent);
	txt_Changes = false;
	txt_Untitled = false;
	//////////////////////////////////////
}

// Save existing file
void PadFrame::OnSave(wxCommandEvent& event)
{
	if (txt_Changes == false)
	{
		event.Skip();
	}
	else
	{
		if (txt_Untitled == true)
		{
			OnSaveAs(event);
		}
		else
		{
			// can be better
			wxFileOutputStream o_Stream(filePath);
			wxTextOutputStream o_Text(o_Stream, wxEOL_NATIVE, wxConvUTF8);
			fileContent = apptextCtrl->GetValue();
			o_Text.WriteString(fileContent);
			wxMessageBox(wxT("Changes Saved!"), "Saved", wxICON_INFORMATION);
			txt_Changes = false;
		}
	}
}

// Save file as new path, etc.
void PadFrame::OnSaveAs(wxCommandEvent& event)
{
	wxFileDialog saveLog
	(
		this,
		_("Save as TXT file"),
		"",
		"",
		"TXT files (*.txt)|*.txt",
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT
	);

	if (saveLog.ShowModal() != wxID_OK)
	{
		return;
	}

	filePath = saveLog.GetPath();
	wxFileName fn(filePath);
	fileName = fn.GetFullName();

	// can be better
	SetTitle(fileName + " - widgetsPad");
	wxFileOutputStream o_Stream(filePath);
	wxTextOutputStream o_Text(o_Stream, wxEOL_NATIVE, wxConvUTF8);
	fileContent = apptextCtrl->GetValue();
	o_Text.WriteString(fileContent);
	wxMessageBox(wxT("File saved!"), "Saved", wxICON_INFORMATION);

	txt_Changes = false;
	txt_Untitled = false;
}

// icon the app
void PadFrame::OnClose(wxCommandEvent& event)
{
	Iconize(true);
}

// close and destroy application
void PadFrame::OnExit(wxCommandEvent& event)
{
	if (txt_Changes == false)
	{
		wxWindow::Destroy();
	}
	else
	{
		SaveDialog* t_saveDialog = new SaveDialog(this, fileName);
		if (t_saveDialog->ShowModal() != wxID_OK)
			wxMessageBox(wxT("Failed event!"), "Error", wxICON_ERROR); return;
	}
		
}

// check if the text in the area has been changed
void PadFrame::OnTextChanged(wxCommandEvent& event)
{
	if (!apptextCtrl->GetValue().IsEmpty())
	{
		txt_Changes = true;
	}
	else
	{
		txt_Changes = false;
	}
	event.Skip();
}
// PADFRAME END

///////////////////////////////////////
// SAVED DIALOG BEGIN
SaveDialog::SaveDialog(PadFrame* parentFrame, const wxString& fileName)
	: wxDialog(parentFrame, wxID_ANY, "", wxDefaultPosition, wxSize(350, 150),
		wxDEFAULT_DIALOG_STYLE & ~wxRESIZE_BORDER & ~wxCLOSE_BOX & ~wxCAPTION),
	m_parentFrame(parentFrame)
{
	CenterOnParent();

	if (fileName.IsEmpty())
	{
		wxStaticText* Text1 = new wxStaticText
		(
			this,
			wxID_ANY,
			"Do you want to save changes to untitled?", // forced tag
			wxPoint(20, 20)
		);
	}
	else
	{
		wxStaticText* Text2 = new wxStaticText
		(
			this,
			wxID_ANY,
			"Do you want to save changes to " + fileName,
			wxPoint(20, 20)
		);
	}


	// buttons
	wxButton* saveBtn = new wxButton
	(
		this,
		wxID_SAVE,
		"Save",
		wxPoint(15, 75),
		wxSize(100,50)
	);

	wxButton* dontBtn = new wxButton
	(
		this,
		wxID_EXIT,
		"Don't Save",
		wxPoint(125,75),
		wxSize(100,50)
	);

	wxButton* cancelBtn = new wxButton
	(
		this,
		wxID_CANCEL,
		"Cancel",
		wxPoint(235,75),
		wxSize(100,50)
	);

	saveBtn->Bind(wxEVT_BUTTON, &SaveDialog::OnSaveDialog, this, wxID_SAVE);
	dontBtn->Bind(wxEVT_BUTTON, &SaveDialog::OnDontSave, this, wxID_EXIT);
	cancelBtn->Bind(wxEVT_BUTTON, &SaveDialog::OnCancel, this, wxID_CANCEL);
}

void SaveDialog::OnSaveDialog(wxCommandEvent& event)
{
	m_parentFrame->OnSave(event);
	EndModal(wxID_OK);
}

void SaveDialog::OnDontSave(wxCommandEvent& event)
{
	wxDialog::Destroy();
	m_parentFrame->txt_Changes = false;
	m_parentFrame->OnExit(event);
}

void SaveDialog::OnCancel(wxCommandEvent& event)
{
	wxDialog::Destroy();
}

// SAVED DIALOG END