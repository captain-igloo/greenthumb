#include "PriceButton.h"

namespace greenthumb {

PriceButton::PriceButton(wxWindow* parent, wxWindowID id, const wxString& label, const wxPoint& pos,
    const wxSize& size, long style, const wxValidator& validator, const wxString& name) :
    wxButton(parent, id, label, pos, size, style, validator, name) {

}

void PriceButton::SetSide(const greentop::Side& side) {
    placeInstruction.setSide(side);
}

const greentop::Side& PriceButton::GetSide() {
    return placeInstruction.getSide();
}

void PriceButton::SetSelectionId(const uint64_t selectionId) {
    placeInstruction.setSelectionId(selectionId);
}

void PriceButton::SetPrice(const double price) {
    greentop::LimitOrder limitOrder(0.0, price, greentop::PersistenceType());
    placeInstruction.setLimitOrder(limitOrder);
}

const greentop::PlaceInstruction& PriceButton::GetPlaceInstruction() {
    return placeInstruction;
}

}
