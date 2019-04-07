/**
 * Copyright 2019 Colin Doig.  Distributed under the MIT license.
 */
#ifndef DIALOG_PAGINATIONTOOLBAR_H
#define DIALOG_PAGINATIONTOOLBAR_H

#include <wx/wx.h>
#include <wx/toolbar.h>

namespace greenthumb {
namespace dialog {

wxDECLARE_EVENT(PAGE_CHANGED, wxCommandEvent);

/**
 * A toolbar with three buttons - refresh, previous page and next page.
 */
class PaginationToolbar : public wxToolBar {
    public:
        /**
         * Constructor.
         *
         * @param parent The parent window.
         * @param id An identifier for the toolbar.
         */
        PaginationToolbar(wxWindow* parent, wxWindowID id);

        /**
         * Returns the id for the refresh button.
         *
         * @return The id for the refresh button.
         */
        const wxWindowID& GetRefreshId();

        /**
         * Returns the id for the previous button.
         *
         * @return The id for the previous button.
         */
        const wxWindowID& GetPreviousId();

        /**
         * Returns the id for the next button.
         *
         * @return The id for the previous button.
         */
        const wxWindowID& GetNextId();

        /**
         * Sets whether or not there are more pages after the current one.
         *
         * @param moreAvailable True if there are more pages available else false.
         */
        void SetMoreAvailable(bool moreAvailable);

        /**
         * Returns the current page number.
         *
         * @return The current page number.
         */
        uint32_t GetCurrentPage() const;

    private:
        /** The id of the refresh button. */
        wxWindowID refreshId;
        /** The id of the previous button. */
        wxWindowID previousId;
        /** The id of the next button. */
        wxWindowID nextId;
        /** The current page number. */
        uint32_t currentPage = 1;

        /**
         * Decrement page number and emit PAGE_CHANGED event.
         *
         * @param event The click event.
         */
        void OnClickPrevious(const wxCommandEvent& event);

        /**
         * Increment page number and emit PAGE_CHANGED event.
         *
         * @param event The click event.
         */
        void OnClickNext(const wxCommandEvent& event);
};

}
}

#endif // DIALOG_PAGINATIONTOOLBAR_H
