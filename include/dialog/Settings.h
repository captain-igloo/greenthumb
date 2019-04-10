/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#ifndef DIALOG_SETTINGS_H
#define DIALOG_SETTINGS_H

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>

namespace greenthumb {
namespace dialog {

/**
 * Application settings dialog.
 */
class Settings : public wxDialog {
    public:

        const static uint32_t ACCOUNT_PAGE_SIZE;
        const static uint32_t DEFAULT_STAKE;
        const static uint32_t REFRESH_INTERVAL;

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

    private:

        wxSpinCtrl* accountRowsPerPage = NULL;
        wxSpinCtrl* defaultStake = NULL;
        wxSpinCtrl* refreshInterval = NULL;

        void OnSave(const wxCommandEvent& event);
        void OnCancel(const wxCommandEvent& event);
};

}
}

#endif // DIALOG_SETTINGS_H
