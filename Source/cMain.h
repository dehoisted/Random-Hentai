#pragma once
#include <wx/wx.h>

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

public:
	wxMenuBar* extra = nullptr;
	wxMenu* ex_menu1 = nullptr;

	wxButton* info_text1 = nullptr;
	wxButton* change_bg = nullptr;
	wxListBox* data_log = nullptr;

	void OnChangeBackground(wxCommandEvent &evt);
	void OnResetBackground(wxCommandEvent &evt);
	void OnGetCredits(wxCommandEvent &evt);

	wxDECLARE_EVENT_TABLE();
};