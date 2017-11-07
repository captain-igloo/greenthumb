/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */

#include "dialog/Html.h"

namespace greenthumb {
namespace dialog {

Html::Html(wxWindow* parent, wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
    wxDialog(parent, id, title, pos, size, style, name) {

    htmlWindow = new wxHtmlWindow(this);
    Bind(wxEVT_HTML_LINK_CLICKED, &Html::OnLinkClicked, this, wxID_ANY);
}

void Html::SetPage(const wxString& page) {
    htmlWindow->SetPage(page);
}

void Html::OnLinkClicked(wxHtmlLinkEvent& event) {
    // wxLaunchDefaultBrowser(event.GetLinkInfo().GetHref());
}

}
}
