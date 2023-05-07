#include "post.h"
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

    void Post::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Statement create_stmt(session);
            create_stmt << "create table if not exists `posts_table`"
                        << "("
                        << "`id` int not null auto_increment,"
                        << "`user_id` int(100) not null,"
                        << "`content` varchar(256) not null"
                        << "`creation_date` varchar(10) not null"
                        << "primary key (`id`), key `post_user_id_index` (`user_id`)"
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

    Poco::JSON::Object::Ptr Post::to_json() const
    {
        Poco::JSON::Object::Ptr object = new Poco::JSON::Object();

        object->set("id", post_id);
        object->set("user_id", user_id);
        object->set("content", content);
        object->set("creation_date", creation_date);

        return object;
    }

    Post Post::from_json(const std::string &json)
    {
        Post post;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(json);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        post.post_id = object->getValue<long>("post_id");
        post.user_id = object->getValue<long>("user_id");
        post.content = object->getValue<std::string>("content");
        post.creation_date = object->getValue<std::string>("creation_date");

        return post;
    }

    std::optional<Post> Post::get_post_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(session);
            Post post;
            select << "select post_id, user_id, content, creation_date from posts_table where id=?",
                    into(post.post_id),
                    into(post.user_id),
                    into(post.content),
                    into(post.creation_date),
                    use(id),
                    range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return post;
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

        return {};
    }

    std::vector<Post> Post::get_all_posts_by_user_id(long id)
    {
        try
        {
            Poco::Data::Session db_session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(db_session);
            std::vector<Post> output_posts;
            Post post;

            select << "select post_id, user_id, content, creation_date from posts_table where user_id=?",
                    into(post.post_id),
                    into(post.user_id),
                    into(post.content),
                    into(post.creation_date),
                    use(id),
                    range(0, 1);

            while (!select.done())
            {
                if (select.execute()) output_posts.push_back(post);
            }

            return output_posts;
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

    void Post::save_to_db()
    {

        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement insert(session);

            insert << "insert into posts_table (user_id, content, creation_date) VALUES(?, ?, ?)",
                    use(user_id),
                    use(content),
                    use(creation_date);

            insert.execute();

            Poco::Data::Statement select(session);
            select << "select last_insert_id()",
                    into(post_id),
                    range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "Inserted new post with id = " << post_id << std::endl;

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
    }

    long Post::get_post_id() const
    {
        return post_id;
    }

    long Post::get_user_id() const
    {
        return user_id;
    }

    std::string Post::get_content() const
    {
        return content;
    }

    std::string Post::get_creation_date() const
    {
        return creation_date;
    }

    void Post::set_user_id(long id)
    {
        user_id = id;
    }

    void Post::set_content(std::string c)
    {
        content = c;
    }

    void Post::set_creation_date(std::string date)
    {
        creation_date = date;
    }
}
