/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */
#ifndef ODDSSPINCTRL_H
#define ODDSSPINCTRL_H

#include <iostream>

#include <wx/spinctrl.h>

namespace greenthumb {

/**
 * Control for displaying / incrementing / decrementing odds.  The amount of the increment depends on the odds, for
 * example when the odds are less than two, the increment is 0.01, and when the odds are between two and three the
 * increment is 0.02.
 */
class OddsSpinCtrl : public wxSpinCtrlDouble {
    public:

        /**
         * Constructor.
         *
         * @param The parent window.
         * @param id The window identifier.
         * @param value Default value (as text).
         * @param pos The window position.
         * @param size The window size.
         * @param style The window style.
         * @param min The minimum value.
         * @param max The maximum value.
         * @param initial The initial value.
         * @param increment The increment value.
         * @param name The window name.
         */
        OddsSpinCtrl(
            wxWindow* parent,
            wxWindowID id,
            const wxString &value = wxEmptyString,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxSize(75, -1),
            long style = 0,
            double min = 1.0,
            double max = 1000.0,
            double initial = 1.01,
            double increment = 0.01,
            const wxString& name = _T("OddsSpinCtrl")
        );

        /**
         * Sets the value of the odds control.
         *
         * @param value The value.
         */
        void SetValue(double value);

    private:
        double previousValue;

        void SetIncrement();

        void OnSpin(wxSpinDoubleEvent& spinEvent);

        void OnTextChange(wxCommandEvent& event);

        void AdjustValue();
};

}

#endif // ODDSSPINCTRL_H
