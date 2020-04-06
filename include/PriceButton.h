/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#ifndef PRICEBUTTON_H
#define PRICEBUTTON_H

#include <greentop/sport/PlaceInstruction.h>
#include <greentop/sport/enum/Side.h>

#include <wx/button.h>

namespace greenthumb {

/**
 * Button showing price and size.  Click to open dialog to place a bet.
 */
class PriceButton : public wxButton {
    public:

        /**
         * Constructor.
         *
         * @param parent Parent window. Must not be NULL.
         * @param id Button identifier. A value of wxID_ANY indicates a default value.
         * @param label Text to be displayed on the button.
         * @param pos Button position.
         * @param size Button size. If the default size is specified then the button is sized appropriately for the
         *        text.
         * @param style Window style.
         * @param validator Window validator.
         * @param name Window name.
         */
        PriceButton(wxWindow *parent, wxWindowID id = wxID_ANY,
            const wxString& label = wxEmptyString, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxSize(priceButtonWidth, priceButtonHeight), long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxButtonNameStr);

        /**
         * Sets the side, either back or lay.
         *
         * @param side The side.
         */
        void SetSide(const greentop::sport::Side& side);

        /**
         * Gets the side, either back or lay.
         *
         * @return The side.
         */
        const greentop::sport::Side& GetSide() const;

        /**
         * Sets the selection id.
         *
         * @param selectionId The selection id.
         */
        void SetSelectionId(const uint64_t selectionId);

        /**
         * Sets the price.
         *
         * @param price The price.
         */
        void SetPrice(const double price);

        /**
         * Sets the handicap.
         *
         * @param handicap The handicap.
         */
        void SetHandicap(const double handicap);

        /**
         * Gets the place instruction.
         *
         * @return The place instruction.
         */
        const greentop::sport::PlaceInstruction& GetPlaceInstruction() const;

    protected:
    private:

        const static int priceButtonWidth = 60;
        const static int priceButtonHeight = 40;

        greentop::sport::PlaceInstruction placeInstruction;

};

}

#endif // PRICEBUTTON_H
