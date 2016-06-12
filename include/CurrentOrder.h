#ifndef CURRENTORDER_H
#define CURRENTORDER_H

#include <greentop/betting/CurrentOrderSummary.h>

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include "entity/Market.h"

#include "worker/WorkerManager.h"

#include "OddsSpinCtrl.h"
#include "StakeSpinCtrl.h"

namespace greenthumb {

class CurrentOrder : public wxPanel {

    public:

        CurrentOrder(wxWindow* parent, const wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxSUNKEN_BORDER,
            const wxString& name = wxPanelNameStr);

        void SetCurrentOrderSummary(const greentop::CurrentOrderSummary& currentOrderSummary);
        greentop::CurrentOrderSummary GetCurrentOrderSummary();
        void SetMarket(const entity::Market& market);

    protected:
    private:

        wxStaticText* runnerName;
        OddsSpinCtrl* oddsSpin;
        wxStaticText* stake;
        wxStaticText* profitOrLiabilityLabel;
        wxStaticText* profitOrLiability;
        wxFlexGridSizer* sizer;
        greentop::CurrentOrderSummary currentOrderSummary;
        entity::Market market;
        wxButton* submitButton;
        worker::WorkerManager workerManager;

        void OnSpin(wxSpinDoubleEvent& event);
        void OnText(wxCommandEvent& event);
        void OnClickCancel(wxCommandEvent& event);
        void OnCancelOrders(wxThreadEvent& event);
        void OnClickSubmit(wxCommandEvent& event);
        void UpdateProfitOrLiability();
};

}

#endif // CURRENTORDER_H
