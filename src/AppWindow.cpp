// File Name: AppWindow.cpp
// Creator: Matteo Washington
// Date of last modification: July 6 2025
// Copyright (c) 2025 Matteo Washington

#include "AppWindow.hpp"
#include <wx/wfstream.h> // borrow this from precomp
#include <wx/txtstrm.h> //  borrow this from precomp
#include <wx/filename.h> // borrow this from precomp

PadFrame::PadFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
    panelSizer->Add(ApptextCtrl, 1, wxEXPAND | wxALL, 5);
    AppPanel->SetSizer(panelSizer);

    fileMenu->Append(wxID_NEW, "&New Window\tCtrl-N", "Create new instance");
    fileMenu->Append(wxID_OPEN, "&Open\tCtrl-O", "Open file");
    fileMenu->Append(wxID_SAVE, "&Save\tCtrl-S", "Save file");
    fileMenu->Append(wxID_SAVEAS, "&Save As\tCtrl-Alt-S", "Save file as");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_CLOSE, "&Close\tCtrl-C", "Close window");
    fileMenu->Append(wxID_EXIT, "&Exit\tCtrl-X", "Exit application");

    menuBar->Append(fileMenu, "&File");
    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &PadFrame::OnNewWindow, this, wxID_NEW);
    Bind(wxEVT_MENU, &PadFrame::OnOpen, this, wxID_OPEN);
    Bind(wxEVT_MENU, &PadFrame::OnSave, this, wxID_SAVE);
    Bind(wxEVT_MENU, &PadFrame::OnSaveAs, this, wxID_SAVEAS);
    Bind(wxEVT_MENU, &PadFrame::OnClose, this, wxID_CLOSE);
    Bind(wxEVT_MENU, &PadFrame::OnExit, this, wxID_EXIT);

    wxFont font(13, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas");
    ApptextCtrl->SetFont(font);

    ApptextCtrl->Bind(wxEVT_TEXT, &PadFrame::TextChanged, this);


    CreateStatusBar(2);
    SetStatusText("widgetsPad | App Verison: Alpha " APP_RELEASE);
}

saveDialog::saveDialog(PadFrame* parentFrame, const wxString& title)
    : wxDialog(parentFrame, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300),
        wxDEFAULT_DIALOG_STYLE & ~wxRESIZE_BORDER & ~wxCLOSE_BOX & ~wxCAPTION)
{

}

// PadFrame button and interactions
void PadFrame::TextChanged(wxCommandEvent& event)
{
    if (!ApptextCtrl->GetValue().IsEmpty())
    {
        changesMade = true;
    }
    else
    {
        changesMade = false;
    }
    event.Skip();
}

void PadFrame::OnNewWindow(wxCommandEvent& event)
{
    ::wxExecute(wxTheApp->argv[0]);
}

void PadFrame::OnOpen(wxCommandEvent& event)
{
    wxFileDialog fdlog(this, "Open file", "", "", "All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (fdlog.ShowModal() != wxID_OK)
        return;
    
    untitled = false;

    wxString filePath = fdlog.GetPath();
    currfilePath = filePath;
    SetTitle(wxFileName(currfilePath).GetFullName() + " - widgetsPad");
    wxFileInputStream input(filePath);
    if (!input.IsOk()) 
    {
        wxMessageBox("Cannot open file.", "Error", wxICON_ERROR);
        return;
    }

    wxTextInputStream text(input, "\x09", wxConvUTF8);
    wxString fileContent;
    while (input.IsOk() && !input.Eof()) 
    {
        fileContent += text.ReadLine();
        if (!input.Eof())
            fileContent += "\n";
    }
    ApptextCtrl->SetValue(fileContent);
    changesMade = false;
}

void PadFrame::OnSave(wxCommandEvent& event)
{
    if (changesMade == false)
    {
        event.Skip();
    }
    else
    {
        if (untitled == true)
        {
            OnSaveAs(event);
        }
        else
        {
            wxFileOutputStream outputstream(currfilePath);
            wxString content = ApptextCtrl->GetValue();
            wxTextOutputStream text(outputstream, wxEOL_NATIVE, wxConvUTF8);
            text.WriteString(content);
            changesMade = false;
            wxMessageBox(wxT("Changes saved"));
        }
    }
}

void PadFrame::OnSaveAs(wxCommandEvent& event)
{
    wxFileDialog saveFile(this, _("Save TXT file"), "", "", "TXT files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFile.ShowModal() == wxID_CANCEL)
        return;

    wxString file = saveFile.GetPath();
    currfilePath = file;
    SetTitle(wxFileName(currfilePath).GetFullName() + " - widgetsPad");
    wxFileOutputStream outputstream(saveFile.GetPath());
    wxString content = ApptextCtrl->GetValue();
    wxTextOutputStream text(outputstream, wxEOL_NATIVE, wxConvUTF8);
    text.WriteString(content);

    // decide, do you destroy the frame or keep it?
    // keep frame for now
    changesMade = false;
    untitled = false;
}

void PadFrame::OnClose(wxCommandEvent& event)
{
    Iconize(true);
}

void PadFrame::OnExit(wxCommandEvent& event)
{
    if (changesMade == false)
    {
        wxWindow::Destroy();
    }
    else
    {
        saveDialog* dialog = new saveDialog(this, wxT(""));
        if (dialog->ShowModal() == wxID_OK);
    }
}
// PadFrame buttons and interactions


// saveDialog button and interactions

// saveDialog button and interactions