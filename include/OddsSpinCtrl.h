#ifndef ODDSSPINCTRL_H
#define ODDSSPINCTRL_H

#include <iostream>

#include <wx/spinctrl.h>

namespace greenthumb {

class OddsSpinCtrl : public wxSpinCtrlDouble {
    public:

        OddsSpinCtrl(
            wxWindow* parent, wxWindowID id,
            const wxString &value = wxEmptyString,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxSize(75, -1),
            long style = 0,
            double min = 1.0, double max = 1000.0,
            double initial = 1.01,
            double increment = 0.01,
            const wxString& name = _T("wxSpinCtrlDouble")
        );

        void SetValue(double value);
        void SetIncrement();

        /* OddsSpinCtrl(
            wxWindow* parent, wxWindowID id,
            const wxString &value = wxEmptyString,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxSize(75, -1),
            long style = 0,
            double min = 1.0, double max = 1000.0,
            double initial = 1.01,
            double increment = 0.01,
            const wxString& name = _T("wxSpinCtrlDouble")
        ) : wxSpinCtrlDouble(parent, id, value, pos, size, style, min, max, initial, increment, name) {
            previousValue = initial;
            SetSnapToTicks(true);
        }*/


        /*OddsSpinCtrl(
            wxWindow *parent, wxWindowID id = wxID_ANY,
            const wxString &value = wxEmptyString,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxSize(75, -1),
            long style = 0,
            int min = 1, int max = 1000,
            int initial = 0,
            const wxString& name = _T("wxSpinCtrlDbl")
        ) : wxSpinCtrlDbl(parent, id, value, pos, size, style, min, max, initial, name) {

            SetIncrement(0.01);
            SetRange(1.01, 1000);
            SetSnapToTicks(true);
        }*/

        virtual ~OddsSpinCtrl();
    protected:
        void onSpin(wxSpinDoubleEvent& spinEvent);
        // void OnSpinDown(wxSpinEvent& spinEvent);

    private:

        double previousValue;

        // DECLARE_EVENT_TABLE()
};

}

#endif // ODDSSPINCTRL_H
