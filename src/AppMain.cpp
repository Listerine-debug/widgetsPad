// File Name: AppMain.cpp
// Creator: Matteo Washington
// Date of last modification: July 6 2025
// Copyright (c) 2025 Matteo Washington

#include "AppWindow.hpp"

DECLARE_APP(widgetsPad)
IMPLEMENT_APP(widgetsPad)

bool widgetsPad::OnInit()
{
	PadFrame* frame = new PadFrame(wxT("untitled - widgetsPad")); // filler for now forced title 
	frame->Show(true);
	return true;
}