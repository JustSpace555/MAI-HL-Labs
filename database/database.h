#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <Poco/Data/SessionPool.h>
#include <vector>

namespace database
{
    class Database
    {
        private:
            std::string connection;
            std::unique_ptr<Poco::Data::SessionPool> pool;
            Database();

        public:
            static const int MAX_SHARDINGS = 2;
            static Database &get_instance();
            static std::vector<std::string> get_all_shardings();
            static std::string get_sharding(long id);

            Poco::Data::Session create_database_session();

    };
}

#endif