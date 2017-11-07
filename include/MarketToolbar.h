/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */
#ifndef MARKETTOOLBAR_H
#define MARKETTOOLBAR_H

#include <wx/wx.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/stattext.h>

namespace greenthumb {

/**
 * Toolbar for market panels.  Shows the market's name and status, and has buttons to show current orders, refresh or
 * close.
 */
class MarketToolbar : public wxPanel {
    public:

        /**
         * Constructor.
         *
         * @param parent The parent window.
         * @param id An identifier for the panel.
         * @param pos The panel position.
         * @param size The panel size.
         * @param style The window style.
         * @param name Window name.
         */
        MarketToolbar(wxWindow* parent, const wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxBORDER_NONE, const wxString& name = wxPanelNameStr);

        /**
         * Sets the market name.
         *
         * @param marketName The market name.
         */
        void SetMarketName(const wxString& marketName);

        /**
         * Sets the market status eg "CLOSED" or "SUSPENDED".  Displayed beside the market name.
         *
         * @param marketStatus The market status.
         */
        void SetMarketStatus(const wxString& marketStatus);

        /**
         * Gets the close button id.
         *
         * @return The close button id.
         */
        const wxWindowID& GetCloseButtonId() const;

        /**
         * Gets the refresh button id.
         *
         * @return The refresh button id.
         */
        const wxWindowID& GetRefreshButtonId() const;

        /**
         * Gets the current orders button id.
         *
         * @return the current orders button id.
         */
        const wxWindowID& GetCurrentOrdersButtonId() const;

        /**
         * Gets the rules button id.
         *
         * @return The rules button id.
         */
        const wxWindowID& GetRulesButtonId() const;

        /**
         * Set whether or not the market is in play.  If set true, a green tick is displayed, else a grey tick is
         * displayed.
         *
         * @param inPlay The current in play status.
         */
        void SetInPlay(bool inPlay);

    private:
        /** The close button id. */
        wxWindowID closeButtonId;
        /** The refresh button id. */
        wxWindowID refreshButtonId;
        /** The current orders button id. */
        wxWindowID currentOrdersButtonId;
        /** The rules button id. */
        wxWindowID rulesButtonId;
        /** The in-play status.*/
        wxButton* inPlayButton;
        /** Displays the market name. */
        wxStaticText* marketName;
        /** Displays the market status. */
        wxStaticText* marketStatus;
        /** Click to open the current orders dialog. */
        wxButton* currentOrdersButton;
        /** Click to open the market's rules dialog. */
        wxButton* rulesButton;

    DECLARE_NO_COPY_CLASS(MarketToolbar)
};

}

#endif // MARKETTOOLBAR_H
