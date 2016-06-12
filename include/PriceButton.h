#ifndef PRICEBUTTON_H
#define PRICEBUTTON_H

#include <greentop/betting/PlaceInstruction.h>

#include <wx/button.h>

namespace greenthumb {

class PriceButton : public wxButton {
    public:
        PriceButton(wxWindow *parent, wxWindowID id = wxID_ANY,
            const wxString& label = wxEmptyString, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxSize(priceButtonWidth, priceButtonHeight), long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxButtonNameStr);

        void SetSide(const greentop::Side& side);
        const greentop::Side& GetSide();
        void SetSelectionId(const uint64_t selectionId);
        void SetPrice(const double price);
        const greentop::PlaceInstruction& GetPlaceInstruction();

    protected:
    private:

        const static int priceButtonWidth = 60;
        const static int priceButtonHeight = 40;

        greentop::PlaceInstruction placeInstruction;

};

}

#endif // PRICEBUTTON_H
