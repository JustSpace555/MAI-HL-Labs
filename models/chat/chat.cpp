#include "chat.h"
#include "../../database/database.h"
#include "../../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace models
{
    void Chat::init()
    {
        try
        {
            Poco::Data::Session db_session = database::Database::get_instance().create_database_session();
            Statement create_stmt(db_session);
            create_stmt << "create table if not exists `chats_table`"
                        << "("
                        << "`id` int not null auto_increment,"
                        << "`user_first_id` int(100) not null,"
                        << "`user_second_id` int(100) not null,"
                        << "primary key (`id`),"
                        << "key `user_first_id_index` (`user_first_id`)"
                        << "key `user_second_id_index` (`user_second_id`)"
                        << ");",
                        now;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "Database connection exception:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "Database statement exception:" << e.what() << std::endl;
            throw;
        }
    }

    Chat Chat::from_json(const std::string &json_str)
    {
        Chat chat;
        Poco::JSON::Parser json_parser;
        Poco::Dynamic::Var result = json_parser.parse(json_str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        chat.chat_id = object->getValue<long>("id");
        chat.sender_id = object->getValue<long>("sender_id");
        chat.receiver_id = object->getValue<long>("receiver_id");

        return chat;
    }

    Poco::JSON::Object::Ptr Chat::to_json() const
    {
        Poco::JSON::Object::Ptr json = new Poco::JSON::Object();

        json->set("id", chat_id);
        json->set("sender_id", sender_id);
        json->set("receiver_id", receiver_id);

        return json;
    }

    std::optional<Chat> Chat::select_by_id(long id)
    {
        try
        {
            Poco::Data::Session db_session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(db_session);
            Chat chat;

            select << "select id, sender_id, receiver_id fromt chats_table where id=?",
                    into(chat.chat_id),
                    into(chat.sender_id),
                    into(chat.receiver_id),
                    use(id),
                    range(0, 1);
            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return chat;
        }
        catch(Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "Connection exception:" << e.what() << std::endl;
        }
        catch(Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "Database statement exception:" << e.what() << std::endl;
        }
        
        return {};
    }

    std::vector<Chat> Chat::select_all_by_sender_id(long id)
    {
        try
        {
            Poco::Data::Session db_session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(db_session);
            std::vector<Chat> output_chats;
            Chat chat;

            select << "select id, sender_id, receiver_id from chats_table where sender_id=?",
                    into(chat.chat_id),
                    into(chat.sender_id),
                    into(chat.receiver_id),
                    use(id),
                    range(0, 1);

            while (!select.done())
            {
                if (select.execute()) output_chats.push_back(chat);
            }

            return output_chats;
        }
        catch(Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "Connection exception:" << e.what() << std::endl;
            throw;
        }
        catch(Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "Database statement exception:" << e.what() << std::endl;
            throw;
        }
        
        return {};
    }

    void Chat::save_to_db()
    {
        try
        {
            Poco::Data::Session db_session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement insert(db_session);

            insert << "insert into chats_table (sender_id, receiver_id) values(?, ?)",
                    use(sender_id),
                    use(receiver_id);
            insert.execute();

            Poco::Data::Statement select(db_session);
            select << "select last_insert_id()",
                    into(chat_id),
                    range(0, 1);

            if (!select.done()) select.execute();

            std::cout << "Inserted 1 new chat with id = " << chat_id << std::endl;
        }
        catch(Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "Connection exception:" << e.what() << std::endl;
            throw;
        }
        catch(Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "Database statement excpetion:" << e.what() << std::endl;
            throw;
        }
        
    }

    long Chat::get_id()
    {
        return chat_id;
    }

    long Chat::get_sender_id()
    {
        return sender_id;
    }

    long Chat::get_receiver_id()
    {
        return receiver_id;
    }

    void Chat::set_sender_id(long id)
    {
        sender_id = id;
    }

    void Chat::set_receiver_id(long id)
    {
        receiver_id = id;
    }
}