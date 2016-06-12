#ifndef STAKESPINCTRL_H
#define STAKESPINCTRL_H

#include <wx/spinctrl.h>

namespace greenthumb {

class StakeSpinCtrl : public wxSpinCtrlDouble {
    public:

        StakeSpinCtrl(
            wxWindow* parent, wxWindowID id,
            const wxString &value = wxEmptyString,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxSize(75, -1),
            long style = 0,
            double min = 1.0, double max = 10000.0,
            double initial = 100.0,
            double increment = 50.0,
            const wxString& name = _T("wxSpinCtrlDouble")
        );

        virtual ~StakeSpinCtrl();

    protected:

    private:
        double previousValue;

        void OnSpin(wxSpinDoubleEvent& spinEvent);
};

}

#endif // STAKESPINCTRL_H
