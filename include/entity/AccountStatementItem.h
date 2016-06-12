#ifndef ENTITY_ACCOUNTSTATEMENTITEM_H
#define ENTITY_ACCOUNTSTATEMENTITEM_H

#include <iostream>

#include <ctime>
#include <soci.h>
#include <string>

namespace greenthumb {
namespace entity {

class AccountStatementItem {
    public:

        static std::tm GetMostRecentItemDate();

        static std::vector<AccountStatementItem> FetchAccountStatementItemPage(uint32_t fromRecord, uint32_t pageSize);

        static std::vector<AccountStatementItem> FetchTotalBalanceNull();

        static double GetTotalBalanceForEvent(uint64_t eventId);

        static uint32_t GetNumberRows();

        bool Insert();

        double GetPreviousItemBalance();

        void Update();

        void SetId(const uint64_t id);
        const uint64_t GetId() const;

        void SetExchangeId(const uint32_t exchangeId);
        const uint32_t GetExchangeId() const;

        void SetRefId(const std::string& refId);
        const std::string& GetRefId() const;

        void SetItemDate(const std::tm& itemDate);
        const std::tm& GetItemDate() const;

        void SetAmount(const double amount);
        const double GetAmount() const;

        void SetBalance(const double balance);
        const double GetBalance() const;

        void SetAvgPrice(const double avgPrice);
        const double GetAvgPrice() const;

        void SetBetSize(const double betSize);
        const double GetBetSize() const;

        void SetBetType(const std::string& betType);
        const std::string& GetBetType() const;

        void SetEventId(const uint64_t eventId);
        const uint64_t GetEventId() const;

        void SetEventTypeId(const uint64_t eventTypeId);
        const uint64_t GetEventTypeId() const;

        void SetFullMarketName(const std::string& fullMarketName);
        const std::string& GetFullMarketName() const;

        void SetGrossBetAmount(const double grossBetAmount);
        const double GetGrossBetAmount() const;

        void SetMarketName(const std::string& marketName);
        const std::string& GetMarketName() const;

        void SetPlacedDate(const std::tm& placedDate);
        const std::tm& GetPlacedDate() const;

        void SetSelectionId(const uint64_t selectionId);
        const uint64_t GetSelectionId() const;

        void SetSelectionName(const std::string& selectionName);
        const std::string& GetSelectionName() const;

        void SetStartDate(const std::tm& startDate);
        const std::tm& GetStartDate() const;

        void SetTransactionType(const std::string& transactionType);
        const std::string& GetTransactionType() const;

        void SetWinLose(const std::string& winLose);
        const std::string& GetWinLose() const;

        void SetTotalBalance(const double totalBalance);
        const double GetTotalBalance() const;

    protected:
    private:
        uint64_t id;
        uint32_t exchangeId;
        std::string refId;
        std::tm itemDate;
        double amount;
        double balance;
        double avgPrice;
        double betSize;
        std::string betType;
        uint64_t eventId;
        uint64_t eventTypeId;
        std::string fullMarketName;
        double grossBetAmount;
        std::string marketName;
        std::tm placedDate;
        uint64_t selectionId;
        std::string selectionName;
        std::tm startDate;
        std::string transactionType;
        std::string winLose;
        double totalBalance;
};

}
}

namespace soci {

    template<>
    struct type_conversion<greenthumb::entity::AccountStatementItem> {

        typedef values base_type;

        static void from_base(const values& v, indicator ind, greenthumb::entity::AccountStatementItem& asi) {

            asi.SetId(v.get<int>("id"));
            asi.SetExchangeId(v.get<int>("exchange_id"));
            asi.SetRefId(v.get<std::string>("ref_id"));
            asi.SetItemDate(v.get<std::tm>("item_date"));
            asi.SetAmount(v.get<double>("amount"));
            asi.SetBalance(v.get<double>("balance"));
            asi.SetAvgPrice(v.get<double>("avg_price"));
            asi.SetBetSize(v.get<double>("bet_size"));
            asi.SetBetType(v.get<std::string>("bet_type"));
            asi.SetEventId(v.get<long long>("event_id"));
            asi.SetEventTypeId(v.get<long long>("event_type_id"));
            asi.SetFullMarketName(v.get<std::string>("full_market_name"));
            asi.SetGrossBetAmount(v.get<double>("gross_bet_amount"));
            asi.SetMarketName(v.get<std::string>("market_name"));
            asi.SetPlacedDate(v.get<std::tm>("placed_date"));
            asi.SetSelectionId(v.get<long long>("selection_id"));
            asi.SetSelectionName(v.get<std::string>("selection_name"));
            asi.SetStartDate(v.get<std::tm>("start_date"));
            asi.SetTransactionType(v.get<std::string>("transaction_type"));
            asi.SetWinLose(v.get<std::string>("win_lose"));
            asi.SetTotalBalance(v.get<double>("total_balance", 0));

        }

        static void to_base(const greenthumb::entity::AccountStatementItem& asi, values& v, indicator& ind) {

            v.set("id", asi.GetId());
            v.set("exchange_id", asi.GetExchangeId());
            v.set("ref_id", asi.GetRefId());
            v.set("item_date", asi.GetItemDate());
            v.set("amount", asi.GetAmount());
            v.set("balance", asi.GetBalance());
            v.set("avg_price", asi.GetAvgPrice());
            v.set("bet_size", asi.GetBetSize());
            v.set("bet_type", asi.GetBetType());
            v.set("event_id", asi.GetEventId());
            v.set("event_type_id", asi.GetEventTypeId());
            v.set("full_market_name", asi.GetFullMarketName());
            v.set("gross_bet_amount", asi.GetGrossBetAmount());
            v.set("market_name", asi.GetMarketName());
            v.set("placed_date", asi.GetPlacedDate());
            v.set("selection_id", asi.GetSelectionId());
            v.set("selection_name", asi.GetSelectionName());
            v.set("start_date", asi.GetStartDate());
            v.set("transaction_type", asi.GetTransactionType());
            v.set("win_lose", asi.GetWinLose());
            if (asi.GetTotalBalance() > 0) {
                v.set("total_balance", asi.GetTotalBalance());
            }

            ind = i_ok;
        }
    };
}

#endif // ENTITY_ACCOUNTSTATEMENTITEM_H
