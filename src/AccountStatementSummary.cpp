#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/toolbar.h>

#include <greentop/ExchangeApi.h>

#include "dialog/Settings.h"
#include "entity/AccountStatementMarket.h"
#include "entity/Config.h"
#include "AccountStatementSummary.h"
#include "Util.h"

namespace greenthumb {

AccountStatementSummary::AccountStatementSummary(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style, const wxString& name) :
    PaginatedGrid(parent, id, pos, size, style, name) {

    GetNumberPages();

    grid = new wxGrid(this, wxID_ANY);
    grid->CreateGrid(0, 4);

    grid->SetColLabelValue(0, "Date");
    grid->SetColSize(0, 150);
    grid->SetColLabelValue(1, "Description");
    grid->SetColSize(1, 450);

    grid->SetColLabelValue(2, "Amount");
    grid->SetColSize(2, 75);
    grid->SetColFormatFloat(2, -1, 2);

    grid->SetColLabelValue(3, "Balance");
    grid->SetColSize(3, 100);
    grid->SetColFormatFloat(3, -1, 2);

    grid->HideRowLabels();

    GetSizer()->Add(grid, 1, wxEXPAND, 0);

    Render();

}

void AccountStatementSummary::Render() {

    UpdateToolbar();

    int pageSize = entity::Config::GetConfigValue<int>(
        entity::Config::KEY_ACCOUNT_PAGE_SIZE,
        dialog::Settings::ACCOUNT_PAGE_SIZE
    );
    uint32_t fromRecord = (currentPage - 1) * pageSize;

    std::vector<entity::AccountStatementMarket> ases =
        entity::AccountStatementMarket::FetchAccountStatementMarketPage(fromRecord, pageSize);

    grid->BeginBatch();
    grid->ClearGrid();
    if (grid->GetNumberRows() > 0) {
        grid->DeleteRows(0, grid->GetNumberRows());
    }

    int count = 0;
    for (auto i = ases.begin(); i != ases.end(); ++i) {

        grid->AppendRows();

        char itemDate[20];
        std::tm itemDateTm = i->GetItemDate();
        time_t itemDateTime = timegm(&itemDateTm);
        std::strftime(itemDate, 20, "%Y-%m-%d %H:%M:%S", std::localtime(&itemDateTime));

        grid->SetCellValue(count, 0, itemDate);
        grid->SetCellValue(count, 1, i->GetFullMarketName());

        grid->SetCellValue(count, 2, wxString::Format(wxT("%f"), i->GetAmount()));
        grid->SetCellValue(count, 3, wxString::Format(wxT("%f"), i->GetTotalBalance()));
        count++;
    }

    grid->EndBatch();

}

void AccountStatementSummary::OnGetAccountStatement() {
    GetNumberPages();
    Render();
}

void AccountStatementSummary::GetNumberPages() {
    int pageSize = entity::Config::GetConfigValue<int>(
        entity::Config::KEY_ACCOUNT_PAGE_SIZE,
        dialog::Settings::ACCOUNT_PAGE_SIZE
    );
    numberPages = ceil((double) entity::AccountStatementMarket::GetNumberRows() / (double) pageSize);
}


}
