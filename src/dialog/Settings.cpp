#include <wx/stattext.h>

#include "dialog/Settings.h"

namespace greenthumb {
namespace dialog {

Settings::Settings(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size,
    long style, const wxString& name) : wxDialog(parent, id, title, pos, size, style, name) {


    wxStaticText* blah = new wxStaticText(this, wxID_ANY, "asdf adf");
}

}
}
