// File Name: AppWindow.cpp
// Creator: Matteo Washington
// Date of last modification: July 8 2025
// Copyright (c) 2025 Matteo Washington
// Descrption: main file for all frames and dialogs

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
		wxTE_MULTILINE | wxHSCROLL | wxVSCROLL | wxTE_RICH
	);

	menuBar = new wxMenuBar();
	fileMenu = new wxMenu();
	editMenu = new wxMenu();
	aboutMenu = new wxMenu();

	menuBar->Append(fileMenu, "&File");
	menuBar->Append(editMenu, "&Edit");
	menuBar->Append(aboutMenu, "& About");
	SetMenuBar(menuBar);

	// box siser creation
	panelSizer = new wxBoxSizer(wxVERTICAL);

	// Add panel sizer to textctrl
	panelSizer->Add(apptextCtrl, 1, wxEXPAND | wxALL, 5);
	appPanel->SetSizer(panelSizer);

	// buttons append to file menu
	fileMenu->Append(wxID_NEW,		"&New Window\tCtrl+N",		"");
	fileMenu->Append(wxID_OPEN,		"&Open\tCtrl+O",			"");
	fileMenu->Append(wxID_SAVE,		"&Save\tCtrl+S",			"");
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
	Bind(wxEVT_CLOSE_WINDOW, &PadFrame::OnX, this);

	// buttons append to edit menu
	editMenu->Append(wxID_UNDO,		"&Undo\tCtrl+Z",			"");
	editMenu->Append(wxID_REDO,		"&Redo\tCtrl+Y",			"");
	editMenu->Append(wxID_CUT,		"&Cut\tCtrl+X",				"");
	editMenu->Append(wxID_COPY,		"&Copy\tCtrl+C",			"");
	editMenu->Append(wxID_PASTE,	"&Paste\tCtrl+P",			"");
	editMenu->Append(wxID_DELETE,	"&Delete\tDel",				"");
	editMenu->AppendSeparator();
	editMenu->Append(wxID_DIALOG,	"&Find\tCtrl+F",			"");

	Bind(wxEVT_MENU, &PadFrame::OnUndo, this, wxID_UNDO);
	Bind(wxEVT_MENU, &PadFrame::OnRedo, this, wxID_REDO);
	Bind(wxEVT_MENU, &PadFrame::OnCut, this, wxID_CUT);
	Bind(wxEVT_MENU, &PadFrame::OnCopy, this, wxID_COPY);
	Bind(wxEVT_MENU, &PadFrame::OnPaste, this, wxID_PASTE);
	Bind(wxEVT_MENU, &PadFrame::OnDelete, this, wxID_DELETE);
	Bind(wxEVT_MENU, &PadFrame::OnFind, this, wxID_DIALOG);

	// button append to about menu
	aboutMenu->Append(wxID_ABOUT,	"&About\tCtrl+Q",			"");
	aboutMenu->Append(wxID_HELP,	"&Help\tCtrl+H",			"");

	Bind(wxEVT_MENU, &PadFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &PadFrame::OnHelp, this, wxID_HELP);

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
	apptextCtrl->Bind(wxEVT_SET_CURSOR, &PadFrame::OnCursorChanged, this);

	//status bar, updated
	statusBar = CreateStatusBar(5);
	int widths[] = { 100, -1, -2, -3, -1 };
	statusBar->SetStatusWidths(5, widths);
	statusBar->SetStatusText("Ln " + Line + " Col " + Column, 0);
	statusBar->SetStatusText(charCount + " characters", 1);
	statusBar->SetStatusText("widgetsPad", 2);
	statusBar->SetStatusText("Version: " APP_RELEASE, 3);
	statusBar->SetStatusText("UTF-8", 4);
}
// PADFRAME CONSTRUCTOR


// FILE MENU BEGIN
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

	apptextCtrl->Clear();
	fileContent = "";
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
}

// Save existing file
void PadFrame::OnSave(wxCommandEvent& event)
{
	if (txt_Changes == false)
	{
		event.Skip();
		return;
	}
	if (txt_Untitled == true)
	{
		OnSaveAs(event);
		return;
	}

	// can be better
	wxFileOutputStream o_Stream(filePath);
	wxTextOutputStream o_Text(o_Stream, wxEOL_NATIVE, wxConvUTF8);
	fileContent = apptextCtrl->GetValue();
	o_Text.WriteString(fileContent);
	wxMessageBox(wxT("Changes Saved!"), "Saved", wxICON_INFORMATION);
	txt_Changes = false;
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
	wxCloseEvent closeEvent(wxEVT_CLOSE_WINDOW);
	OnX(closeEvent);
}

// very very bac BUT it works :3
void PadFrame::OnX(wxCloseEvent& event)
{
	if (txt_Changes == false)
	{
		wxWindow::Destroy();
	}
	else
	{
		SaveDialog* t_saveDialog = new SaveDialog(this, fileName);
		if (t_saveDialog->ShowModal() == wxID_OK);
		// wxMessageBox(wxT("Failed event!"), "Error", wxICON_ERROR); return;
	}
}

// check if the text in the area has been changed
void PadFrame::OnUpdateStatusBar()
{
	size_t c_charCount = apptextCtrl->GetValue().Length();
	charCount = wxString::Format("%zu", c_charCount);
	SetStatusText(charCount + " characters", 1);

	apptextCtrl->PositionToXY(apptextCtrl->GetInsertionPoint(), &x, &y);
	Line = wxString::Format("%d", y + 1);
	Column = wxString::Format("%d", x + 1);
	statusBar->SetStatusText("Ln " + Line + " Col " + Column, 0);
}

void PadFrame::OnTextChanged(wxCommandEvent& event)
{
	// wxMessageBox(wxT("OnTextCalled")); debug
	currentText = apptextCtrl->GetValue();
	txt_Changes = (currentText != fileContent);
	OnUpdateStatusBar();
	event.Skip();
}

void  PadFrame::OnCursorChanged(wxSetCursorEvent& event)
{
	OnUpdateStatusBar();
	event.Skip();
}
// FILE MENU END

// EDIT MENU BEGIN
void PadFrame::OnUndo(wxCommandEvent& event)
{
	if (apptextCtrl->CanUndo())
	{
		apptextCtrl->Undo();
	}
}

void PadFrame::OnRedo(wxCommandEvent& event)
{
	if (apptextCtrl->CanRedo())
	{
		apptextCtrl->Redo();
	}
}

void PadFrame::OnCut(wxCommandEvent& event)
{
	if (apptextCtrl->CanCut())
	{
		apptextCtrl->Cut();
	}
}

void PadFrame::OnCopy(wxCommandEvent& event)
{
	if (apptextCtrl->CanCopy())
	{
		apptextCtrl->Copy();
	}
}

void PadFrame::OnPaste(wxCommandEvent& event)
{
	if (apptextCtrl->CanPaste())
	{
		apptextCtrl->Paste();
	}
}

void PadFrame::OnDelete(wxCommandEvent& event)
{
	long int begin, end;
	apptextCtrl->GetSelection(&begin, &end);
	apptextCtrl->Remove((long int)begin, (long int)end);
}

void PadFrame::OnFind(wxCommandEvent& event)
{
	FindFrame* t_FindFrame = new FindFrame(this, wxT("Find"));
	t_FindFrame->Show(true);
	//if (t_FindDialog->ShowModal() == wxID_OK);
}
// EDIT MENU END

// ABOUT MENU BEGIN

void PadFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox(wxT("OnAbout dialog"));
}

void PadFrame::OnHelp(wxCommandEvent& event)
{
	wxMessageBox(wxT("OnHelp dialog"));
}

// ABOUT MENU END


// PADFRAME END

///////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////

// FIND DIALOG BEGIN

FindFrame::FindFrame(PadFrame* parentFrame, const wxString& title)
	: wxFrame(parentFrame, wxID_ANY, "Find", wxDefaultPosition, wxSize(400, 200),
		wxDEFAULT_DIALOG_STYLE & ~wxRESIZE_BORDER), 
	m_parentFrame(parentFrame)
{
	findPanel = new wxPanel
	(
		this,
		wxID_ANY,
		wxDefaultPosition,
		wxSize(400, 200)
	);
	
	findtextCtrl = new wxTextCtrl
	(
		findPanel,
		wxUSE_TEXTCTRL,
		wxEmptyString,
		wxPoint(15, 20),
		wxSize(350, 30)
	);


	wxFont Font
	(
		13, // size
		wxFONTFAMILY_MODERN, // family
		wxFONTSTYLE_NORMAL, // style
		wxFONTWEIGHT_NORMAL, // weight
		false, // underline ?
		"Consolas" // font itself
	);
	
	findtextCtrl->SetFont(Font);
	rollback.SetTextColour(*wxBLACK);
	rollback.SetBackgroundColour(*wxWHITE);
	finding.SetTextColour(*wxBLUE);
	finding.SetBackgroundColour(*wxWHITE);
	highlightStyle.SetTextColour(*wxBLUE);
	highlightStyle.SetBackgroundColour(*wxLIGHT_GREY);

	wxButton* findBtn = new wxButton
	(
		findPanel,
		wxID_FIND,
		"Find",
		wxPoint(250, 65),
		wxSize(100, 25)
	);

	wxButton* nextBtn = new wxButton
	(
		findPanel,
		wxID_NEXT,
		"Next",
		wxPoint(250, 95),
		wxSize(100, 25)
	);

	wxButton* prevBtn = new wxButton
	(
		findPanel,
		wxID_PREV,
		"Prev",
		wxPoint(250, 125),
		wxSize(100, 25)
	);

	findBtn->Bind(wxEVT_BUTTON, &FindFrame::OnFindFrame, this, wxID_FIND);
	nextBtn->Bind(wxEVT_BUTTON, &FindFrame::OnNextFrame, this, wxID_NEXT);
	prevBtn->Bind(wxEVT_BUTTON, &FindFrame::OnPrevFrame, this, wxID_PREV);
	Bind(wxEVT_CLOSE_WINDOW, &FindFrame::OnCloseFrame, this);

	text1 = new wxStaticText
	(
		findPanel,
		wxID_STATIC,
		"", // place holder
		wxPoint(70, 90),
		wxSize(70, 30)
	);

	text1->SetFont(Font);
}

void FindFrame::OnFindFrame(wxCommandEvent& event)
{
	int m_end = m_parentFrame->apptextCtrl->GetLastPosition();
	m_parentFrame->apptextCtrl->SetStyle(0, m_end, rollback);

	wxString findText = findtextCtrl->GetValue();
	wxString fullText = m_parentFrame->apptextCtrl->GetValue();
	m_findText = findText;
	m_fullText = fullText;

	if (findText.IsEmpty()) 
	{
		wxMessageBox(wxT("Please enter text to find."), "Empty Search", wxICON_INFORMATION);
		return;
	}

	m_Count = 0;
	m_Amount = 0;
	long pos = 0;
	long total = 0;
	bool foundAny = false;

	findList.clear();

	while ((pos = fullText.find(findText, pos)) != wxNOT_FOUND) 
	{
		findList.push_back(pos);
		total += 1;
		long end = pos + findText.Length();
		m_parentFrame->apptextCtrl->SetStyle(pos, end, wxTextAttr(*wxBLUE)); // change later
		pos = end;
		foundAny = true;
	}


	m_Total = total;
	text1->SetLabel(wxString::Format("1 of " + wxString::Format("%d", total)));
	m_parentFrame->apptextCtrl->SetStyle(findList[0], findList[0] + findText.Length(), highlightStyle);
	m_parentFrame->apptextCtrl->SetInsertionPoint(findList[0]);
	m_foundAny = foundAny;

	if (!foundAny) 
	{
		wxMessageBox(wxT("No Mathces found!"), "Not Found", wxICON_INFORMATION);
	}
}

void FindFrame::OnNextFrame(wxCommandEvent& event)
{
	if (m_foundAny == false)
	{
		wxMessageBox(wxT("Please search for a word!"), "No Word", wxICON_INFORMATION);
		return;
	}
	int prevAmount = m_Amount;
	m_Amount += 1;
	m_Count += 1;
	if (m_Amount >= m_Total)
	{
		m_Amount = 0;
		m_Count = 1;
	}
	// redundacny with findText and m_findText, fix later
	// wxString findText = findtextCtrl->GetValue();
	m_parentFrame->apptextCtrl->SetStyle(findList[prevAmount], findList[prevAmount] + m_findText.Length(), finding);
	m_parentFrame->apptextCtrl->SetStyle(findList[m_Amount], findList[m_Amount] + m_findText.Length(), highlightStyle);
	m_parentFrame->apptextCtrl->SetInsertionPoint(findList[m_Amount]);
	text1->SetLabel(wxString::Format("%d of %d", m_Count, m_Total));
}

void FindFrame::OnPrevFrame(wxCommandEvent& event)
{
	if (m_foundAny == false)
	{
		wxMessageBox(wxT("Please search for a word!"), "No Word", wxICON_INFORMATION);
		return;
	}
	int prevAmount = m_Amount;
	m_Amount -= 1;
	m_Count -= 1;
	if (m_Amount < 0)
	{
		m_Amount = m_Total - 1;
		m_Count = m_Total;
	}
	// wxString findText = findtextCtrl->GetValue();
	m_parentFrame->apptextCtrl->SetStyle(findList[prevAmount], findList[prevAmount] + m_findText.Length(), finding);
	m_parentFrame->apptextCtrl->SetStyle(findList[m_Amount], findList[m_Amount] + m_findText.Length(), highlightStyle);
	m_parentFrame->apptextCtrl->SetInsertionPoint(findList[m_Amount]);
	text1->SetLabel(wxString::Format("%d of %d", m_Count, m_Total));
}

void FindFrame::OnCloseFrame(wxCloseEvent& event)
{
	if (event.CanVeto())
	{
		int m_end = m_parentFrame->apptextCtrl->GetLastPosition();
		m_parentFrame->apptextCtrl->SetStyle(0, m_end, rollback);
	}
	Destroy();
}

// FIND DIALOG END