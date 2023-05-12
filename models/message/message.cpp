#include "message.h"
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

    void Message::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Statement create_stmt(session);
            create_stmt << "create table if not exists `messages_table`"
                        << "("
                        << "`id` int not null auto_increment,"
                        << "`chat_id` int(100) not null,"
                        << "`user_sender_id` int(100) not null,"
                        << "`content` varchar(256) not null,"
                        << "primary key (`id`), key `message_chat_id_index` (`chat_id`)"
                        << ");",
                        now;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "Database connection exception:" << e.message() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "Database statement exception:" << e.message() << std::endl;
            throw;
        }
    }

    Poco::JSON::Object::Ptr Message::to_json() const
    {
        Poco::JSON::Object::Ptr object = new Poco::JSON::Object();

        object->set("message_id", message_id);
        object->set("chat_id", chat_id);
        object->set("content", content);
        object->set("user_sender_id", user_sender_id);

        return object;
    }

    Message Message::from_json(std::string &json)
    {
        Message message;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(json);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        message.message_id = object->getValue<long>("message_id");
        message.chat_id = object->getValue<long>("chat_id");
        message.content = object->getValue<std::string>("content");
        message.user_sender_id = object->getValue<long>("user_sender_id");

        return message;
    }

    std::optional<Message> Message::get_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(session);
            Message message;
            select << "select id, chat_id, user_sender_id, content from messages_table where id=?",
                    into(message.message_id),
                    into(message.chat_id),
                    into(message.user_sender_id),
                    into(message.content),
                    use(id),
                    range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return message;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "Database connection exception:" << e.message() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "Database statement exception:" << e.message() << std::endl;
            throw;
        }

        return {};
    }

    std::vector<Message> Message::get_all_messages_by_chat_id(long id)
    {
        try
        {
            Poco::Data::Session db_session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(db_session);
            std::vector<Message> output_messages;
            Message message;

            select << "select id, chat_id, user_sender_id, content from messages_table where chat_id=?",
                    into(message.message_id),
                    into(message.chat_id),
                    into(message.user_sender_id),
                    into(message.content),
                    use(id),
                    range(0, 1);

            while (!select.done())
            {
                if (select.execute()) output_messages.push_back(message);
            }

            return output_messages;
        }
        catch(Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "Connection exception:" << e.message() << std::endl;
            throw;
        }
        catch(Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "Database statement exception:" << e.message() << std::endl;
            throw;
        }
        
        return {};
    }

    void Message::save_to_db()
    {

        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement insert(session);

            insert << "insert into messages_table (chat_id, user_sender_id, content) VALUES(?, ?, ?)",
                    use(chat_id),
                    use(user_sender_id),
                    use(content);

            insert.execute();

            Poco::Data::Statement select(session);
            select << "select last_insert_id()",
                    into(message_id),
                    range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "Inserted new message with id = " << message_id << std::endl;

        }
        catch(Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "Connection exception:" << e.message() << std::endl;
            throw;
        }
        catch(Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "Database statement exception:" << e.message() << std::endl;
            throw;
        }
    }

    long Message::get_id() const
    {
        return message_id;
    }

    long Message::get_chat_id() const
    {
        return chat_id;
    }

    long Message::get_sender_id() const
    {
        return user_sender_id;
    }

    std::string Message::get_content() const
    {
        return content;
    }

    void Message::set_chat_id(long id)
    {
        chat_id = id;
    }

    void Message::set_sender_id(long id)
    {
        user_sender_id = id;
    }

    void Message::set_content(std::string c)
    {
        content = c;
    }
}
