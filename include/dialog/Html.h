/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */
#ifndef DIALOG_HTML_H
#define DIALOG_HTML_H

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/wxhtml.h>

namespace greenthumb {
namespace dialog {

class Html : public wxDialog {

    public:

        /**
         * Constructor.
         *
         * @param parent Pointer to the parent window.
         * @param id The identifier of the dialog.
         * @param title The title of the dialog.
         * @param pos The dialog position.
         * @param size The dialog size.
         * @param style The window style.
         * @param name The dialog name.
         */
        Html(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos=wxDefaultPosition,
            const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER,
            const wxString& name=wxDialogNameStr);

        /**
         * Sets the HTML to display in the dialog.
         *
         * @param page The HTML to display.
         */
        void SetPage(const wxString& page);

    private:
        wxHtmlWindow* htmlWindow = NULL;

        void OnLinkClicked(wxHtmlLinkEvent& event);
};

}
}

#endif // DIALOG_RULES_H
