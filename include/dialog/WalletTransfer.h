/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#ifndef DIALOG_WALLETTRANSFER_H
#define DIALOG_WALLETTRANSFER_H

#include <wx/dialog.h>
#include <wx/radiobut.h>
#include <wx/stattext.h>

#include "worker/TransferFunds.h"
#include "worker/WorkerManager.h"

namespace greenthumb {
namespace dialog {

/**
 * Transfer funds between wallets.
 */
class WalletTransfer : public wxDialog {
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
        WalletTransfer(wxWindow* parent, wxWindowID id, const wxString& title,
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER, const wxString& name = wxDialogNameStr);

    protected:
    private:

        wxTextCtrl* amount;
        wxWindow* okButton;
        wxWindow* cancelButton;
        wxStaticText* ukBalance;
        wxStaticText* ausBalance;
        bool transferFundsRunning;
        worker::TransferFunds* transferFundsWorker;
        worker::WorkerManager workerManager;
        wxRadioButton* srcUkWallet;
        wxStaticText* destinationWalletText;

        virtual bool Validate();
        void GetAccountFunds();
        void OnGetAccountFundsUk(const wxThreadEvent& event);
        void OnGetAccountFundsAus(const wxThreadEvent& event);
        void OnGetAccountFunds(const wxThreadEvent& event, wxStaticText* balance);
        void OnOk(wxCommandEvent& event);
        void OnTransfer(wxThreadEvent& event);
        void OnChangeWallet(wxCommandEvent& event);
        void OnTextChange(wxCommandEvent& event);
};

}
}

#endif // DIALOG_WALLETTRANSFER_H
