/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */
#include "OddsSpinCtrl.h"
#include "Util.h"

namespace greenthumb {

/** The increment ranges.  Below 2 the increment is 0.01, between 2 and 3 the increment is 0.02 etc. */
const std::map<unsigned, std::pair<double, double> > OddsSpinCtrl::ranges = {
    { 2, { 0.01, 0.02 } },
    { 3, { 0.02, 0.05 } },
    { 4, { 0.05, 0.1 } },
    { 6, { 0.1, 0.2 } },
    { 10, { 0.2, 0.5 } },
    { 20, { 0.5, 1 } },
    { 30, { 1, 2 } },
    { 50, { 2, 5 } },
    { 100, { 5, 10 } },
    { 1000, { 10, -1 } }
};

OddsSpinCtrl::OddsSpinCtrl(wxWindow* parent, wxWindowID id, const wxString &value,
    const wxPoint& pos, const wxSize& size, long style, double min, double max,
    double initial, double increment, const wxString& name) :
    wxSpinCtrlDouble(parent, id, value, pos, size, style, min, max, initial, increment, name) {
    previousValue = initial;
    SetSnapToTicks(true);

    Bind(wxEVT_TEXT, &OddsSpinCtrl::OnTextChange, this, wxID_ANY);
    Bind(wxEVT_SPINCTRLDOUBLE, &OddsSpinCtrl::OnSpin, this, wxID_ANY);
}

void OddsSpinCtrl::OnTextChange(wxCommandEvent& event) {
    SetIncrement();
    event.Skip();
}

void OddsSpinCtrl::OnSpin(wxSpinDoubleEvent& spinEvent) {
    SetIncrement();
    AdjustValue();
    previousValue = GetValue();
    spinEvent.Skip();
}

/**
 * The spin event is received after the value has changed, making it difficult to set the increment
 * correctly.  Here we detect that the value has not changed correctly and fix it.
 */
void OddsSpinCtrl::AdjustValue() {
    std::map<unsigned, std::pair<double, double> >::const_iterator it;
    for (it = ranges.begin(); it != ranges.end(); ++it) {
        if (DoubleEquals(previousValue, it->first)) {
            if (DoubleEquals(GetValue(), it->first - it->second.second)) {
                // eg previous value == 2, new value 1.98, adjust to 1.99
                SetValue(it->first - it->second.first);
            } else if (DoubleEquals(GetValue(), it->first + it->second.first)) {
                // eg previous value == 2, new value 2.01, adjust to 2.02
                SetValue(it->first + it->second.second);
            }
        }
    }
}

void OddsSpinCtrl::SetIncrement() {
    double value = GetValue();

    std::map<unsigned, std::pair<double, double> >::const_iterator it;
    for (it = ranges.begin(); it != ranges.end(); ++it) {
        if (value <= previousValue) {
            // spin down
            if (value <= it->first) {
                wxSpinCtrlDouble::SetIncrement(it->second.first);
                break;
            }
        } else {
            // spin up
            if (value < it->first) {
                wxSpinCtrlDouble::SetIncrement(it->second.first);
                break;
            }
        }
    }
}

void OddsSpinCtrl::SetValue(double value) {
    previousValue = value;
    wxSpinCtrlDouble::SetValue(value);
    SetIncrement();
}

}
