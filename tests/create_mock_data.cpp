#include <string>
#include <iostream>
#include <fstream>

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include "../models/user/user.h"
#include "../database/database.h"

int main()
{

    Poco::Data::MySQL::Connector::registerConnector();

    std::string db_connection_string;
    db_connection_string += "host=127.0.0.1;";
    db_connection_string += "port=6033;";
    db_connection_string += "user=login;";
    db_connection_string += "password=password;";
    db_connection_string += "db=archdb";
    std::cout << "Database connection string:" << db_connection_string << std::endl;

    std::vector<std::string> shards;
    for(int i = 0; i <= 2; i++)
    {
        std::string shard_name = "-- sharding:";
        shard_name += std::to_string(i);
        shards.push_back(shard_name);
    }


    Poco::Data::Session session(
        Poco::Data::SessionFactory::instance().create(Poco::Data::MySQL::Connector::KEY, db_connection_string)
    );
    std::cout << "Session created" << std::endl;

    try
    {
        for ([[maybe_unused]] std::string &shard : shards) {
            session << "drop table `users_table`";
            Poco::Data::Statement create_stmt(session);
            create_stmt << "create table if not exists `users_table`"
                        << "("
                        << "`user_id` int not null auto_increment,"
                        << "`user_first_name` varchar(256) not null,"
                        << "`user_last_name` varchar(256) not null,"
                        << "`user_login` varchar(64) not null,"
                        << "`user_password` varchar(64) not null,"
                        << "`user_email` varchar(256) null,"
                        << "`user_phone_number` varchar(11) null, "
                        << "`user_birth_date` varchar(10) null,"
                        << "primary key (`user_id`), key `user_first_name_index` (`user_first_name`), key `user_last_name_index` (`user_last_name`)"
                        << ");";
            create_stmt.execute();
        }
        std::cout << "Users table created" << std::endl;

        Poco::Data::Statement truncate_stmt(session);
        truncate_stmt << "truncate table `users_table`;";
        truncate_stmt.execute();

        std::string json;
        std::ifstream is("tests/mock_data.json");
        std::istream_iterator<char> eos;
        std::istream_iterator<char> iit(is);
        while (iit != eos)
            json.push_back(*(iit++));
        is.close();

        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(json);
        Poco::JSON::Array::Ptr arr = result.extract<Poco::JSON::Array::Ptr>();

        for (int i = 0; i < (int) arr->size(); i++)
        {
            Poco::JSON::Object::Ptr object = arr->getObject(i);
            std::string first_name = object->getValue<std::string>("first_name");
            std::string last_name = object->getValue<std::string>("last_name");
            std::string email = object->getValue<std::string>("email");
            std::string login = email;
            std::string password;
            std::string birth_date = object->getValue<std::string>("birth_date");
            std::string phone_number;

            std::string key;

            std::string select_str = "insert into users_table (user_first_name, user_last_name, user_login, user_password, user_email, user_phone_number, user_birth_date) values(?, ?, ?, ?, ?, ?, ?)";
            std::cout << select_str << std::endl;

            Poco::Data::Statement insert(session);
            insert << select_str,
                    Poco::Data::Keywords::use(first_name),
                    Poco::Data::Keywords::use(last_name),
                    Poco::Data::Keywords::use(login),
                    Poco::Data::Keywords::use(password),
                    Poco::Data::Keywords::use(email),
                    Poco::Data::Keywords::use(phone_number),
                    Poco::Data::Keywords::use(birth_date);

            insert.execute();
            
            long inserted_id;
            Poco::Data::Statement select(session);
            select << "select last_insert_id()",
                    Poco::Data::Keywords::into(inserted_id),
                    Poco::Data::Keywords::range(0, 1);

            if (!select.done()) select.execute();

            std::cout << "Inserted 1 new user with id = " << inserted_id << std::endl;
        }

        std::cout << "Inserted " << arr->size() << " records" << std::endl;

    }
    catch (Poco::Data::MySQL::ConnectionException &e)
    {
        std::cout << "Connection exception: " << e.message() << std::endl;
    }
    catch (Poco::Data::MySQL::StatementException &e)
    {

        std::cout << "Statement exception: " << e.message() << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    return 1;
}