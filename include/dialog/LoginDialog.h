/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#ifndef DIALOG_LOGINDIALOG_H
#define DIALOG_LOGINDIALOG_H

#include <wx/dialog.h>
#include <wx/thread.h>
#include <wx/wx.h>

#include "worker/WorkerManager.h"

namespace greenthumb {
namespace dialog {

/**
 * Login dialog.  Log in to the betfair API.
 */
class LoginDialog : public wxDialog {
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
         * @param name Used to associate a name with the window, allowing the application
         *               user to set Motif resource values for individual dialog boxes.
         */
        LoginDialog(wxWindow* parent, wxWindowID id, const wxString& title,
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxDialogNameStr);

    protected:
    private:
        /** The betfair application key text control. */
        wxTextCtrl* appKey = NULL;
        /** The betfair username text control. */
        wxTextCtrl* username = NULL;
        /** The betfair password text control. */
        wxTextCtrl* password = NULL;
        /** OK button. */
        wxWindow* okButton = NULL;
        /** Cancel button. */
        wxWindow* cancelButton = NULL;
        /** Whether or not to save the username and password */
        wxCheckBox* remember = NULL;
        /** Manages worker threads. */
        worker::WorkerManager workerManager;

        /**
         * Submit login request.
         *
         * @param event The click event.
         */
        void OnOk(const wxCommandEvent& event);

        /**
         * Check validity of the form and enable / disable buttons accordingly.
         */
        void OnTextChange();

        /**
         * Check validity of the form and enable / disable buttons accordingly.
         *
         * @param event The text change event.
         */
        void OnTextChange(const wxCommandEvent& event);

        /**
         * Close the dialog and disable buttons.
         *
         * @param event The worker event.
         */
        void OnLogin(const wxThreadEvent& event);

        DECLARE_NO_COPY_CLASS(LoginDialog)
};

}
}

#endif // DIALOG_LOGINDIALOG_H
