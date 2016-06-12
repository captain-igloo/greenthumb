/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#ifndef ENTITY_ACCOUNTSTATEMENTMARKET_H
#define ENTITY_ACCOUNTSTATEMENTMARKE_H

#include <ctime>
#include <soci.h>
#include <string>

namespace greenthumb {
namespace entity {

/**
 * Represents a row in account_statement_market.
 */
class AccountStatementMarket {
    public:

        /**
         * Populate account_statement_market from account_statement_item.
         */
        static void Populate();

        /**
         * Retrieve a page of results from account_statement_market.
         *
         * @param fromRecord The offset.
         * @param pageSize The number of rows to return.
         */
        static std::vector<AccountStatementMarket> FetchAccountStatementMarketPage(uint32_t fromRecord, uint32_t pageSize);

        /**
         * The total number of rows in account_statement_market.
         */
        static uint32_t GetNumberRows();

        /**
         * Insert a row in account_statement_market.
         */
        void Insert();

        /**
         * Sets the id
         *
         * @param id The id.
         */
        void SetId(const uint64_t id);

        /**
         * Gets the id.
         */
        const uint64_t GetId() const;

        /**
         * Sets the exchange id.
         *
         * @param exchangeId The exchange id.
         */
        void SetExchangeId(const uint32_t exchangeId);

        /**
         * Gets the exchange id.
         */
        const uint32_t GetExchangeId() const;

        /**
         * Sets the item date.
         *
         * @param The item date.
         */
        void SetItemDate(const std::tm& itemDate);

        /**
         * Gets the item date.
         */
        const std::tm& GetItemDate() const;

        /**
         * Sets the total balance of AUS and UK wallets.
         *
         * @param The balance.
         */
        void SetBalance(const double balance);

        /**
         * Gets the total balance of AUS and UK wallets.
         */
        const double GetBalance() const;

        /**
         * Sets the profit / loss made on the market.
         *
         * @param amount The amount.
         */
        void SetAmount(const double amount);

        /**
         * Gets the profit / loss made on the market.
         */
        const double GetAmount() const;

        void SetEventId(const uint64_t eventId);
        const uint64_t GetEventId() const;

        void SetFullMarketName(const std::string& fullMarketName);
        const std::string& GetFullMarketName() const;

        void SetTotalBalance(const double totalBalance);
        const double GetTotalBalance() const;

    protected:
    private:
        uint64_t id;
        uint32_t exchangeId;
        std::tm itemDate;
        double balance;
        double amount;
        uint64_t eventId;
        std::string fullMarketName;
        double totalBalance;
};

}
}

namespace soci {

    template<>
    struct type_conversion<greenthumb::entity::AccountStatementMarket> {

        typedef values base_type;

        static void from_base(const values& v, indicator ind, greenthumb::entity::AccountStatementMarket& ase) {

            ase.SetId(v.get<int>("id"));
            ase.SetExchangeId(v.get<int>("exchange_id"));
            ase.SetItemDate(v.get<std::tm>("item_date"));
            ase.SetBalance(v.get<double>("balance"));
            ase.SetAmount(v.get<double>("amount"));
            ase.SetEventId(v.get<long long>("event_id"));
            ase.SetFullMarketName(v.get<std::string>("full_market_name"));
            ase.SetTotalBalance(v.get<double>("total_balance"));

        }

        static void to_base(const greenthumb::entity::AccountStatementMarket& ase, values& v, indicator& ind) {

            v.set("id", ase.GetId());
            v.set("exchange_id", ase.GetExchangeId());
            v.set("item_date", ase.GetItemDate());
            v.set("balance", ase.GetBalance());
            v.set("amount", ase.GetAmount());
            v.set("event_id", ase.GetEventId());
            v.set("full_market_name", ase.GetFullMarketName());
            v.set("total_balance", ase.GetTotalBalance());

            ind = i_ok;
        }
    };
}

#endif // ENTITY_ACCOUNTSTATEMENTMARKET_H
