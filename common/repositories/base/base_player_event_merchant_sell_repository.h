/**
 * DO NOT MODIFY THIS FILE
 *
 * This repository was automatically generated and is NOT to be modified directly.
 * Any repository modifications are meant to be made to the repository extending the base.
 * Any modifications to base repositories are to be made by the generator only
 *
 * @generator ./utils/scripts/generators/repository-generator.pl
 * @docs https://docs.eqemu.io/developer/repositories
 */

#ifndef EQEMU_BASE_PLAYER_EVENT_MERCHANT_SELL_REPOSITORY_H
#define EQEMU_BASE_PLAYER_EVENT_MERCHANT_SELL_REPOSITORY_H

#include "../../database.h"
#include "../../strings.h"
#include <ctime>

class BasePlayerEventMerchantSellRepository {
public:
	struct PlayerEventMerchantSell {
		uint64_t    id;
		uint32_t    npc_id;
		std::string merchant_name;
		uint32_t    merchant_type;
		uint32_t    item_id;
		std::string item_name;
		int32_t     charges;
		uint32_t    cost;
		uint64_t    player_money_balance;
		time_t      created_at;
	};

	static std::string PrimaryKey()
	{
		return std::string("id");
	}

	static std::vector<std::string> Columns()
	{
		return {
			"id",
			"npc_id",
			"merchant_name",
			"merchant_type",
			"item_id",
			"item_name",
			"charges",
			"cost",
			"player_money_balance",
			"created_at",
		};
	}

	static std::vector<std::string> SelectColumns()
	{
		return {
			"id",
			"npc_id",
			"merchant_name",
			"merchant_type",
			"item_id",
			"item_name",
			"charges",
			"cost",
			"player_money_balance",
			"UNIX_TIMESTAMP(created_at)",
		};
	}

	static std::string ColumnsRaw()
	{
		return std::string(Strings::Implode(", ", Columns()));
	}

	static std::string SelectColumnsRaw()
	{
		return std::string(Strings::Implode(", ", SelectColumns()));
	}

	static std::string TableName()
	{
		return std::string("player_event_merchant_sell");
	}

	static std::string BaseSelect()
	{
		return fmt::format(
			"SELECT {} FROM {}",
			SelectColumnsRaw(),
			TableName()
		);
	}

	static std::string BaseInsert()
	{
		return fmt::format(
			"INSERT INTO {} ({}) ",
			TableName(),
			ColumnsRaw()
		);
	}

	static PlayerEventMerchantSell NewEntity()
	{
		PlayerEventMerchantSell e{};

		e.id                   = 0;
		e.npc_id               = 0;
		e.merchant_name        = "";
		e.merchant_type        = 0;
		e.item_id              = 0;
		e.item_name            = "";
		e.charges              = 0;
		e.cost                 = 0;
		e.player_money_balance = 0;
		e.created_at           = 0;

		return e;
	}

	static PlayerEventMerchantSell GetPlayerEventMerchantSell(
		const std::vector<PlayerEventMerchantSell> &player_event_merchant_sells,
		int player_event_merchant_sell_id
	)
	{
		for (auto &player_event_merchant_sell : player_event_merchant_sells) {
			if (player_event_merchant_sell.id == player_event_merchant_sell_id) {
				return player_event_merchant_sell;
			}
		}

		return NewEntity();
	}

	static PlayerEventMerchantSell FindOne(
		Database& db,
		int player_event_merchant_sell_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {} = {} LIMIT 1",
				BaseSelect(),
				PrimaryKey(),
				player_event_merchant_sell_id
			)
		);

		auto row = results.begin();
		if (results.RowCount() == 1) {
			PlayerEventMerchantSell e{};

			e.id                   = row[0] ? strtoull(row[0], nullptr, 10) : 0;
			e.npc_id               = row[1] ? static_cast<uint32_t>(strtoul(row[1], nullptr, 10)) : 0;
			e.merchant_name        = row[2] ? row[2] : "";
			e.merchant_type        = row[3] ? static_cast<uint32_t>(strtoul(row[3], nullptr, 10)) : 0;
			e.item_id              = row[4] ? static_cast<uint32_t>(strtoul(row[4], nullptr, 10)) : 0;
			e.item_name            = row[5] ? row[5] : "";
			e.charges              = row[6] ? static_cast<int32_t>(atoi(row[6])) : 0;
			e.cost                 = row[7] ? static_cast<uint32_t>(strtoul(row[7], nullptr, 10)) : 0;
			e.player_money_balance = row[8] ? strtoull(row[8], nullptr, 10) : 0;
			e.created_at           = strtoll(row[9] ? row[9] : "-1", nullptr, 10);

			return e;
		}

		return NewEntity();
	}

	static int DeleteOne(
		Database& db,
		int player_event_merchant_sell_id
	)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {} = {}",
				TableName(),
				PrimaryKey(),
				player_event_merchant_sell_id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int UpdateOne(
		Database& db,
		const PlayerEventMerchantSell &e
	)
	{
		std::vector<std::string> v;

		auto columns = Columns();

		v.push_back(columns[1] + " = " + std::to_string(e.npc_id));
		v.push_back(columns[2] + " = '" + Strings::Escape(e.merchant_name) + "'");
		v.push_back(columns[3] + " = " + std::to_string(e.merchant_type));
		v.push_back(columns[4] + " = " + std::to_string(e.item_id));
		v.push_back(columns[5] + " = '" + Strings::Escape(e.item_name) + "'");
		v.push_back(columns[6] + " = " + std::to_string(e.charges));
		v.push_back(columns[7] + " = " + std::to_string(e.cost));
		v.push_back(columns[8] + " = " + std::to_string(e.player_money_balance));
		v.push_back(columns[9] + " = FROM_UNIXTIME(" + (e.created_at > 0 ? std::to_string(e.created_at) : "null") + ")");

		auto results = db.QueryDatabase(
			fmt::format(
				"UPDATE {} SET {} WHERE {} = {}",
				TableName(),
				Strings::Implode(", ", v),
				PrimaryKey(),
				e.id
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static PlayerEventMerchantSell InsertOne(
		Database& db,
		PlayerEventMerchantSell e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.id));
		v.push_back(std::to_string(e.npc_id));
		v.push_back("'" + Strings::Escape(e.merchant_name) + "'");
		v.push_back(std::to_string(e.merchant_type));
		v.push_back(std::to_string(e.item_id));
		v.push_back("'" + Strings::Escape(e.item_name) + "'");
		v.push_back(std::to_string(e.charges));
		v.push_back(std::to_string(e.cost));
		v.push_back(std::to_string(e.player_money_balance));
		v.push_back("FROM_UNIXTIME(" + (e.created_at > 0 ? std::to_string(e.created_at) : "null") + ")");

		auto results = db.QueryDatabase(
			fmt::format(
				"{} VALUES ({})",
				BaseInsert(),
				Strings::Implode(",", v)
			)
		);

		if (results.Success()) {
			e.id = results.LastInsertedID();
			return e;
		}

		e = NewEntity();

		return e;
	}

	static int InsertMany(
		Database& db,
		const std::vector<PlayerEventMerchantSell> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.id));
			v.push_back(std::to_string(e.npc_id));
			v.push_back("'" + Strings::Escape(e.merchant_name) + "'");
			v.push_back(std::to_string(e.merchant_type));
			v.push_back(std::to_string(e.item_id));
			v.push_back("'" + Strings::Escape(e.item_name) + "'");
			v.push_back(std::to_string(e.charges));
			v.push_back(std::to_string(e.cost));
			v.push_back(std::to_string(e.player_money_balance));
			v.push_back("FROM_UNIXTIME(" + (e.created_at > 0 ? std::to_string(e.created_at) : "null") + ")");

			insert_chunks.push_back("(" + Strings::Implode(",", v) + ")");
		}

		std::vector<std::string> v;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} VALUES {}",
				BaseInsert(),
				Strings::Implode(",", insert_chunks)
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static std::vector<PlayerEventMerchantSell> All(Database& db)
	{
		std::vector<PlayerEventMerchantSell> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{}",
				BaseSelect()
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			PlayerEventMerchantSell e{};

			e.id                   = row[0] ? strtoull(row[0], nullptr, 10) : 0;
			e.npc_id               = row[1] ? static_cast<uint32_t>(strtoul(row[1], nullptr, 10)) : 0;
			e.merchant_name        = row[2] ? row[2] : "";
			e.merchant_type        = row[3] ? static_cast<uint32_t>(strtoul(row[3], nullptr, 10)) : 0;
			e.item_id              = row[4] ? static_cast<uint32_t>(strtoul(row[4], nullptr, 10)) : 0;
			e.item_name            = row[5] ? row[5] : "";
			e.charges              = row[6] ? static_cast<int32_t>(atoi(row[6])) : 0;
			e.cost                 = row[7] ? static_cast<uint32_t>(strtoul(row[7], nullptr, 10)) : 0;
			e.player_money_balance = row[8] ? strtoull(row[8], nullptr, 10) : 0;
			e.created_at           = strtoll(row[9] ? row[9] : "-1", nullptr, 10);

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static std::vector<PlayerEventMerchantSell> GetWhere(Database& db, const std::string &where_filter)
	{
		std::vector<PlayerEventMerchantSell> all_entries;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} WHERE {}",
				BaseSelect(),
				where_filter
			)
		);

		all_entries.reserve(results.RowCount());

		for (auto row = results.begin(); row != results.end(); ++row) {
			PlayerEventMerchantSell e{};

			e.id                   = row[0] ? strtoull(row[0], nullptr, 10) : 0;
			e.npc_id               = row[1] ? static_cast<uint32_t>(strtoul(row[1], nullptr, 10)) : 0;
			e.merchant_name        = row[2] ? row[2] : "";
			e.merchant_type        = row[3] ? static_cast<uint32_t>(strtoul(row[3], nullptr, 10)) : 0;
			e.item_id              = row[4] ? static_cast<uint32_t>(strtoul(row[4], nullptr, 10)) : 0;
			e.item_name            = row[5] ? row[5] : "";
			e.charges              = row[6] ? static_cast<int32_t>(atoi(row[6])) : 0;
			e.cost                 = row[7] ? static_cast<uint32_t>(strtoul(row[7], nullptr, 10)) : 0;
			e.player_money_balance = row[8] ? strtoull(row[8], nullptr, 10) : 0;
			e.created_at           = strtoll(row[9] ? row[9] : "-1", nullptr, 10);

			all_entries.push_back(e);
		}

		return all_entries;
	}

	static int DeleteWhere(Database& db, const std::string &where_filter)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"DELETE FROM {} WHERE {}",
				TableName(),
				where_filter
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int Truncate(Database& db)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"TRUNCATE TABLE {}",
				TableName()
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int64 GetMaxId(Database& db)
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"SELECT COALESCE(MAX({}), 0) FROM {}",
				PrimaryKey(),
				TableName()
			)
		);

		return (results.Success() && results.begin()[0] ? strtoll(results.begin()[0], nullptr, 10) : 0);
	}

	static int64 Count(Database& db, const std::string &where_filter = "")
	{
		auto results = db.QueryDatabase(
			fmt::format(
				"SELECT COUNT(*) FROM {} {}",
				TableName(),
				(where_filter.empty() ? "" : "WHERE " + where_filter)
			)
		);

		return (results.Success() && results.begin()[0] ? strtoll(results.begin()[0], nullptr, 10) : 0);
	}

	static std::string BaseReplace()
	{
		return fmt::format(
			"REPLACE INTO {} ({}) ",
			TableName(),
			ColumnsRaw()
		);
	}

	static int ReplaceOne(
		Database& db,
		const PlayerEventMerchantSell &e
	)
	{
		std::vector<std::string> v;

		v.push_back(std::to_string(e.id));
		v.push_back(std::to_string(e.npc_id));
		v.push_back("'" + Strings::Escape(e.merchant_name) + "'");
		v.push_back(std::to_string(e.merchant_type));
		v.push_back(std::to_string(e.item_id));
		v.push_back("'" + Strings::Escape(e.item_name) + "'");
		v.push_back(std::to_string(e.charges));
		v.push_back(std::to_string(e.cost));
		v.push_back(std::to_string(e.player_money_balance));
		v.push_back("FROM_UNIXTIME(" + (e.created_at > 0 ? std::to_string(e.created_at) : "null") + ")");

		auto results = db.QueryDatabase(
			fmt::format(
				"{} VALUES ({})",
				BaseReplace(),
				Strings::Implode(",", v)
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}

	static int ReplaceMany(
		Database& db,
		const std::vector<PlayerEventMerchantSell> &entries
	)
	{
		std::vector<std::string> insert_chunks;

		for (auto &e: entries) {
			std::vector<std::string> v;

			v.push_back(std::to_string(e.id));
			v.push_back(std::to_string(e.npc_id));
			v.push_back("'" + Strings::Escape(e.merchant_name) + "'");
			v.push_back(std::to_string(e.merchant_type));
			v.push_back(std::to_string(e.item_id));
			v.push_back("'" + Strings::Escape(e.item_name) + "'");
			v.push_back(std::to_string(e.charges));
			v.push_back(std::to_string(e.cost));
			v.push_back(std::to_string(e.player_money_balance));
			v.push_back("FROM_UNIXTIME(" + (e.created_at > 0 ? std::to_string(e.created_at) : "null") + ")");

			insert_chunks.push_back("(" + Strings::Implode(",", v) + ")");
		}

		std::vector<std::string> v;

		auto results = db.QueryDatabase(
			fmt::format(
				"{} VALUES {}",
				BaseReplace(),
				Strings::Implode(",", insert_chunks)
			)
		);

		return (results.Success() ? results.RowsAffected() : 0);
	}
};

#endif //EQEMU_BASE_PLAYER_EVENT_MERCHANT_SELL_REPOSITORY_H
