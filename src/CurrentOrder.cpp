/**
* Copyright 2016 Colin Doig.  Distributed under the MIT license.
*/
#include <wx/wx.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <greentop/ExchangeApi.h>

#include "CurrentOrder.h"
#include "GreenThumb.h"

namespace greenthumb {

CurrentOrder::CurrentOrder(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size,
    long style, const wxString& name) : wxPanel(parent, id, pos, size, style, name) {

}

void CurrentOrder::SetMarket(const entity::Market& market) {
    this->market = market;
}

greentop::CurrentOrderSummary CurrentOrder::GetCurrentOrderSummary() {
    return currentOrderSummary;
}

}
