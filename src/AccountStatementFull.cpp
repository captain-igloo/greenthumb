/**
* Copyright 2016 Colin Doig.  Distributed under the MIT license.
*/
#include <wx/wx.h>
#include <wx/sizer.h>
#include <greentop/ExchangeApi.h>
#include <math.h>

#include "entity/AccountStatementItem.h"
#include "AccountStatementFull.h"
#include "Util.h"

namespace greenthumb {

AccountStatementFull::AccountStatementFull(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style, const wxString& name) :
    PaginatedGrid(parent, id, pos, size, style, name) {

    GetNumberPages();

    grid = new wxGrid(this, wxID_ANY);
    grid->CreateGrid(0, 10);

    grid->SetColLabelValue(0, "Exchange");
    grid->SetColumnWidth(0, 75);
    grid->SetColLabelValue(1, "Date");
    grid->SetColumnWidth(1, 150);
    grid->SetColLabelValue(2, "Description");
    grid->SetColumnWidth(2, 450);
    grid->SetColLabelValue(3, "Back/Lay");
    grid->SetColumnWidth(3, 75);
    grid->SetColLabelValue(4, "Selection");
    grid->SetColumnWidth(4, 100);
    grid->SetColLabelValue(5, "Av. Odds");
    grid->SetColumnWidth(5, 75);
    grid->SetColFormatFloat(5, -1, 2);
    grid->SetColLabelValue(6, "Stake");
    grid->SetColumnWidth(6, 75);
    grid->SetColFormatFloat(6, -1, 2);
    grid->SetColLabelValue(7, "Result");
    grid->SetColumnWidth(7, 75);
    grid->SetColLabelValue(8, "Amount");
    grid->SetColumnWidth(8, 75);
    grid->SetColFormatFloat(8, -1, 2);
    grid->SetColLabelValue(9, "Balance");
    grid->SetColumnWidth(9, 100);
    grid->SetColFormatFloat(9, -1, 2);

    grid->HideRowLabels();

    GetSizer()->Add(grid, 1, wxEXPAND, 0);

    Render();

}

void AccountStatementFull::Render() {

    UpdateToolbar();

    uint32_t fromRecord = (currentPage - 1) * PAGE_SIZE;

    std::vector<entity::AccountStatementItem> asis = entity::AccountStatementItem::FetchAccountStatementItemPage(fromRecord, PAGE_SIZE);

    grid->BeginBatch();
    grid->ClearGrid();
    if (grid->GetNumberRows() > 0) {
        grid->DeleteRows(0, grid->GetNumberRows());
    }

    int count = 0;
    for (auto i = asis.begin(); i != asis.end(); ++i) {

        grid->AppendRows();

        char itemDate[20];
        std::tm itemDateTm = i->GetItemDate();
        time_t itemDateTime = timegm(&itemDateTm);
        std::strftime(itemDate, 20, "%F %T", std::localtime(&itemDateTime));

        if (i->GetExchangeId() == static_cast<int>(greentop::Exchange::AUS)) {
            grid->SetCellValue(count, 0, "Aus");
        } else {
            grid->SetCellValue(count, 0, "UK");
        }
        grid->SetCellValue(count, 1, itemDate);
        grid->SetCellValue(count, 2, i->GetFullMarketName());

        bool wonOrLost = false;
        if (i->GetWinLose() == "RESULT_WON") {
            grid->SetCellValue(count, 7, "Won");
            wonOrLost = true;
        } else if (i->GetWinLose() == "RESULT_LOST") {
            grid->SetCellValue(count, 7, "Lost");
            wonOrLost = true;
        }

        if (wonOrLost) {

            if (i->GetBetType() == "B") {
                grid->SetCellValue(count, 3, "Back");
            } else if (i->GetBetType() == "L") {
                grid->SetCellValue(count, 3, "Lay");
            }

            grid->SetCellValue(count, 4, i->GetSelectionName());
            grid->SetCellValue(count, 5, wxString::Format(wxT("%f"), i->GetAvgPrice()));
            grid->SetCellValue(count, 6, wxString::Format(wxT("%f"), i->GetBetSize()));

        }

        grid->SetCellValue(count, 8, wxString::Format(wxT("%f"), i->GetAmount()));
        grid->SetCellValue(count, 9, wxString::Format(wxT("%f"), i->GetTotalBalance()));
        count++;
    }

    grid->EndBatch();

}

void AccountStatementFull::GetNumberPages() {
    numberPages = ceil((double) entity::AccountStatementItem::GetNumberRows() / (double) PAGE_SIZE);
}

void AccountStatementFull::OnGetAccountStatement() {
    GetNumberPages();
    Render();
}

}
