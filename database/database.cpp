#include "database.h"
#include "../config/config.h"

namespace database
{
    Database::Database()
    {
        Config &config = Config::get_instanse();

        connection += "host=";
        connection += config.get_host();

        connection += ";port=";
        connection += config.get_port();
        
        connection += ";user=";
        connection += config.get_login();

        connection += ";password=";
        connection += config.get_password();

        connection += ";db=";
        connection += config.get_database_name();

        std::cout << "Database connection parameters:" << connection << std::endl;

        Poco::Data::MySQL::Connector::registerConnector();
        pool = std::make_unique<Poco::Data::SessionPool>(Poco::Data::MySQL::Connector::KEY, connection);
    }

    Database &Database::get_instance() {
        static Database instance;
        return instance;
    }

    Poco::Data::Session Database::create_database_session()
    {
        return Poco::Data::Session(pool->get());
    }

    std::vector<std::string> Database::get_all_shardings()
    {
        std::vector<std::string> output;
        std::string shard = "-- sharding:";
        for (int i = 0; i <= MAX_SHARDINGS; i++)
            output.push_back(shard + std::to_string(i));
        return output;
    }

    std::string Database::get_sharding(long id)
    {
        std::string key = std::to_string(id);
        int shard_number = std::hash<std::string>{}(key) % MAX_SHARDINGS;
        return "-- sharding:" + std::to_string(shard_number);
    }
}