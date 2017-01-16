/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#ifndef DIALOG_SETTINGS_H
#define DIALOG_SETTINGS_H

#include <wx/dialog.h>

namespace greenthumb {
namespace dialog {

/**
 * Application settings dialog.
 */
class Settings : public wxDialog {
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
        Settings(wxWindow* parent, wxWindowID id, const wxString& title,
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxDialogNameStr);

};

}
}

#endif // DIALOG_SETTINGS_H
