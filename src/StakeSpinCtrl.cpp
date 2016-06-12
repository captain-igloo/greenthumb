#include <iostream>

#include "StakeSpinCtrl.h"

namespace greenthumb {

StakeSpinCtrl::StakeSpinCtrl(wxWindow* parent, wxWindowID id, const wxString &value,
    const wxPoint& pos, const wxSize& size, long style, double min, double max,
    double initial, double increment, const wxString& name) :
    wxSpinCtrlDouble(parent, id, value, pos, size, style, min, max, initial, increment, name) {
    previousValue = initial;
    SetSnapToTicks(false);

    Bind(wxEVT_SPINCTRLDOUBLE, &StakeSpinCtrl::OnSpin, this, wxID_ANY);

}

void StakeSpinCtrl::OnSpin(wxSpinDoubleEvent& spinEvent) {

    double value = GetValue();

    if (value < previousValue) {
        // spin down

        if (value <= 10) {
            SetIncrement(1.0);
        } else if (value <= 25) {
            SetIncrement(5.0);
        } else if (value <= 50) {
            SetIncrement(25.0);
        } else if (value <= 200) {
            SetIncrement(50.0);
        } else {
            SetIncrement(100.0);
        }

    } else if (value > previousValue) {

        // spin up
        if (value < 10) {
            SetIncrement(1.0);
        } else if (value < 25) {
            SetIncrement(5.0);
        } else if (value < 50) {
            SetIncrement(25.0);
        } else if (value < 200) {
            SetIncrement(50.0);
        } else {
            SetIncrement(100.0);
        }

    }

    previousValue = value;

    spinEvent.Skip();
}

StakeSpinCtrl::~StakeSpinCtrl(){
    //dtor
}


}
