#include "MsgDialog.hpp"

#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/statbmp.h>
#include <wx/scrolwin.h>
#include <wx/clipbrd.h>

#include "libslic3r/libslic3r.h"
#include "libslic3r/Utils.hpp"
#include "GUI.hpp"
#include "I18N.hpp"
#include "ConfigWizard.hpp"
#include "wxExtensions.hpp"
#include "GUI_App.hpp"

namespace Slic3r {
namespace GUI {


MsgDialog::MsgDialog(wxWindow *parent, const wxString &title, const wxString &headline, wxWindowID button_id) :
// 	MsgDialog(parent, title, headline, wxBitmap(from_u8(Slic3r::var("Slic3r_192px.png")), wxBITMAP_TYPE_PNG), button_id)
	MsgDialog(parent, title, headline, create_scaled_bitmap("Slic3r_192px.png"), button_id)
{}

MsgDialog::MsgDialog(wxWindow *parent, const wxString &title, const wxString &headline, wxBitmap bitmap, wxWindowID button_id) :
	wxDialog(parent, wxID_ANY, title),
	boldfont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT)),
	content_sizer(new wxBoxSizer(wxVERTICAL)),
	btn_sizer(new wxBoxSizer(wxHORIZONTAL))
{
	boldfont.SetWeight(wxFONTWEIGHT_BOLD);

	auto *topsizer = new wxBoxSizer(wxHORIZONTAL);
	auto *rightsizer = new wxBoxSizer(wxVERTICAL);

	auto *headtext = new wxStaticText(this, wxID_ANY, headline);
	headtext->SetFont(boldfont);
    headtext->Wrap(CONTENT_WIDTH*wxGetApp().em_unit());
	rightsizer->Add(headtext);
	rightsizer->AddSpacer(VERT_SPACING);

	rightsizer->Add(content_sizer, 1, wxEXPAND);

	if (button_id != wxID_NONE) {
		auto *button = new wxButton(this, button_id);
		button->SetFocus();
		btn_sizer->Add(button);
	}

	rightsizer->Add(btn_sizer, 0, wxALIGN_CENTRE_HORIZONTAL);

	auto *logo = new wxStaticBitmap(this, wxID_ANY, std::move(bitmap));

	topsizer->Add(logo, 0, wxALL, BORDER);
	topsizer->Add(rightsizer, 1, wxALL | wxEXPAND, BORDER);

	SetSizerAndFit(topsizer);
}

MsgDialog::~MsgDialog() {}


// ErrorDialog

ErrorDialog::ErrorDialog(wxWindow *parent, const wxString &msg)
	: MsgDialog(parent, _(L("Slic3r error")), _(L("Slic3r has encountered an error")),
// 		wxBitmap(from_u8(Slic3r::var("Slic3r_192px_grayscale.png")), wxBITMAP_TYPE_PNG),
        create_scaled_bitmap("Slic3r_192px_grayscale.png"),
		wxID_NONE)
	, msg(msg)
{
	auto *panel = new wxScrolledWindow(this);
	auto *p_sizer = new wxBoxSizer(wxVERTICAL);
	panel->SetSizer(p_sizer);

	auto *text = new wxStaticText(panel, wxID_ANY, msg);
	text->Wrap(CONTENT_WIDTH*wxGetApp().em_unit());
	p_sizer->Add(text, 1, wxEXPAND);

    panel->SetMinSize(wxSize(CONTENT_WIDTH*wxGetApp().em_unit(), 0));
	panel->SetScrollRate(0, 5);

	content_sizer->Add(panel, 1, wxEXPAND);

	auto *btn_copy = new wxButton(this, wxID_ANY, _(L("Copy to clipboard")));
	btn_copy->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
		if (wxTheClipboard->Open()) {
			wxTheClipboard->SetData(new wxTextDataObject(this->msg));   // Note: the clipboard takes ownership of the pointer
			wxTheClipboard->Close();
		}
	});

	auto *btn_ok = new wxButton(this, wxID_OK);
	btn_ok->SetFocus();

	btn_sizer->Add(btn_copy, 0, wxRIGHT, HORIZ_SPACING);
	btn_sizer->Add(btn_ok);

    SetMaxSize(wxSize(-1, CONTENT_MAX_HEIGHT*wxGetApp().em_unit()));
	Fit();
}

ErrorDialog::~ErrorDialog() {}



}
}
