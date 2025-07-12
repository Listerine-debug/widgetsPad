// File Name: AppMain.cpp
// Creator: Matteo Washington (Aka: Listerine-debug)
// Date of last modification: July 11 2025
// Copyright (c) 2025 Matteo Washington 
// Description: main execute 

#include "AppWindow.hpp"

DECLARE_APP(widgetsPad)
IMPLEMENT_APP(widgetsPad)

bool widgetsPad::OnInit()
{
	PadFrame* frame = new PadFrame(wxT("untitled - widgetsPad")); // filler for now, forced title 
	frame->Show(true);
	return true;
}