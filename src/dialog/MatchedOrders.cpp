/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */

#include "dialog/MatchedOrders.h"
#include "worker/ListCurrentOrders.h"
#include "MatchedOrder.h"

namespace greenthumb {
namespace dialog {

MatchedOrders::MatchedOrders(wxWindow* parent) : CurrentOrdersPage(parent) {
    if (noOrdersMessage != NULL) {
        noOrdersMessage->SetLabel("There are no matched orders.");
    }
}

CurrentOrder* MatchedOrders::CreateOrder() {
    return new MatchedOrder(this, wxID_ANY);
}

greentop::sport::OrderProjection MatchedOrders::GetOrderProjection() const {
    return greentop::sport::OrderProjection::ALL;
}

}
}
