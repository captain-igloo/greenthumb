#include "OddsSpinCtrl.h"

#include <iostream>

namespace greenthumb {

OddsSpinCtrl::OddsSpinCtrl(wxWindow* parent, wxWindowID id, const wxString &value,
    const wxPoint& pos, const wxSize& size, long style, double min, double max,
    double initial, double increment, const wxString& name) :
    wxSpinCtrlDouble(parent, id, value, pos, size, style, min, max, initial, increment, name) {
    previousValue = initial;
    SetSnapToTicks(true);

    Bind(wxEVT_SPINCTRLDOUBLE, &OddsSpinCtrl::onSpin, this, wxID_ANY);
}

void OddsSpinCtrl::onSpin(wxSpinDoubleEvent& spinEvent) {
    SetIncrement();
    previousValue = GetValue();
    spinEvent.Skip();
}

void OddsSpinCtrl::SetIncrement() {

    double value = GetValue();

    if (value <= previousValue) {

        // spin down
        if (value <= 2) {
            wxSpinCtrlDouble::SetIncrement(0.01);
        } else if (value <= 3) {
            wxSpinCtrlDouble::SetIncrement(0.02);
        } else if (value <= 4) {
            wxSpinCtrlDouble::SetIncrement(0.05);
        } else if (value <= 6) {
            wxSpinCtrlDouble::SetIncrement(0.1);
        } else if (value <= 10) {
            wxSpinCtrlDouble::SetIncrement(0.2);
        } else if (value <= 20) {
            wxSpinCtrlDouble::SetIncrement(0.5);
        } else if (value <= 30) {
            wxSpinCtrlDouble::SetIncrement(1);
        } else if (value <= 50) {
            wxSpinCtrlDouble::SetIncrement(2);
        } else if (value <= 100) {
            wxSpinCtrlDouble::SetIncrement(5);
        } else {
            wxSpinCtrlDouble::SetIncrement(10);
        }

    } else {
        // spin up
        if (value < 2) {
            wxSpinCtrlDouble::SetIncrement(0.01);
        } else if (value < 3) {
            wxSpinCtrlDouble::SetIncrement(0.02);
        } else if (value < 4) {
            wxSpinCtrlDouble::SetIncrement(0.05);
        } else if (value < 6) {
            wxSpinCtrlDouble::SetIncrement(0.1);
        } else if (value < 10) {
            wxSpinCtrlDouble::SetIncrement(0.2);
        } else if (value < 20) {
            wxSpinCtrlDouble::SetIncrement(0.5);
        } else if (value < 30) {
            wxSpinCtrlDouble::SetIncrement(1);
        } else if (value < 50) {
            wxSpinCtrlDouble::SetIncrement(2);
        } else if (value < 100) {
            wxSpinCtrlDouble::SetIncrement(5);
        } else {
            wxSpinCtrlDouble::SetIncrement(10);
        }
    }

}

void OddsSpinCtrl::SetValue(double value) {
    previousValue = value;
    wxSpinCtrlDouble::SetValue(value);
    SetIncrement();
}

OddsSpinCtrl::~OddsSpinCtrl() {
}

}
