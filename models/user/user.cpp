#include "user.h"
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
#include <future>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace models
{

    void User::init()
    {
        try
        {

            Poco::Data::Session db_session = database::Database::get_instance().create_database_session();
            Statement create_stmt(db_session);
                create_stmt << "create table if not exists `users_table`"
                            << "("
                            << "`user_id` int not null auto_increment,"
                            << "`user_first_name` varchar(256) not null,"
                            << "`user_last_name` varchar(256) not null,"
                            << "`user_login` varchar(64) not null,"
                            << "`user_password` varchar(64) not null,"
                            << "`user_email` varchar(256) null,"
                            << "`user_phone_number` varchar(11)"
                            << "`user_birth_date` varchar(10) null,"
                            << "primary key (`id`), key `user_first_name_index` (`first_name`), key `user_last_name_index` (`last_name`)"
                            << ");",
                            now;
                std::cout << create_stmt.toString() << std::endl;
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

    User User::from_json(const std::string &json)
    {
        User user;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(json);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        user.user_id = object->getValue<long>("user_id");
        user.user_first_name = object->getValue<std::string>("user_first_name");
        user.user_last_name = object->getValue<std::string>("user_last_name");
        user.user_login = object->getValue<std::string>("user_login");
        user.user_password = object->getValue<std::string>("user_password");
        user.user_email = object->getValue<std::string>("user_email");
        user.user_phone_number = object->getValue<std::string>("user_phone_number");
        user.user_birth_date = object->getValue<std::string>("user_birth_date");

        return user;
    }

    Poco::JSON::Object::Ptr User::to_json() const
    {
        Poco::JSON::Object::Ptr object = new Poco::JSON::Object();

        object->set("user_id", user_id);
        object->set("user_first_name", user_first_name);
        object->set("user_last_name", user_last_name);
        object->set("user_login", user_login);
        object->set("user_password", user_password);
        object->set("user_email", user_email);
        object->set("user_phone_number", user_phone_number);
        object->set("user_birth_date", user_birth_date);

        return object;
    }

    std::optional<User> User::get_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(session);
            User output;
            select << "select id, first_name, last_name, user_login, user_password, email, phone_number, birth_date from users_table where id=?",
                into(output.user_id),
                into(output.user_first_name),
                into(output.user_last_name),
                into(output.user_login),
                into(output.user_password),
                into(output.user_email),
                into(output.user_phone_number),
                into(output.user_birth_date),
                use(id),
                range(0, 1);

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return output;
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

    std::optional<User> User::auth_by_login(std::string &login, std::string &password)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(session);
            User output;
            select << "select id, first_name, last_name, user_login, user_password, email, phone_number, birth_date from users_table where login=? and user_password=?",
                into(output.user_id),
                into(output.user_first_name),
                into(output.user_last_name),
                into(output.user_login),
                into(output.user_password),
                into(output.user_email),
                into(output.user_phone_number),
                into(output.user_birth_date),
                use(login),
                use(password),
                range(0, 1);

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return output;
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

    std::optional<User> User::auth_by_email(std::string &email, std::string &password)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(session);
            User output;
            select << "select id, first_name, last_name, user_login, user_password, email, phone_number, birth_date from users_table where email=? and user_password=?",
                into(output.user_id),
                into(output.user_first_name),
                into(output.user_last_name),
                into(output.user_login),
                into(output.user_password),
                into(output.user_email),
                into(output.user_phone_number),
                into(output.user_birth_date),
                use(email),
                use(password),
                range(0, 1);

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return output;
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

    std::optional<User> User::auth_by_phone_number(std::string &phone_number, std::string &password)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement select(session);
            User output;
            select << "select id, first_name, last_name, user_login, user_password, email, phone_number, birth_date from users_table where phone_number=? and user_password=?",
                into(output.user_id),
                into(output.user_first_name),
                into(output.user_last_name),
                into(output.user_login),
                into(output.user_password),
                into(output.user_email),
                into(output.user_phone_number),
                into(output.user_birth_date),
                use(phone_number),
                use(password),
                range(0, 1);

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return output;
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

    std::optional<User> User::find_by_login(std::string &login)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Statement select(session);
            User user;
            login += "%";
            select << "select id, first_name, last_name, user_login, user_password, email, phone_number, birth_date from users_table where user_login like ?",
                    into(user.user_id),
                    into(user.user_first_name),
                    into(user.user_last_name),
                    into(user.user_login),
                    into(user.user_password),
                    into(user.user_email),
                    into(user.user_phone_number),
                    into(user.user_birth_date),
                    use(login),
                    range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return user;
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

    std::optional<User> User::find_by_email(std::string &email)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Statement select(session);
            User user;
            select << "select id, first_name, last_name, user_login, user_password, email, phone_number, birth_date from users_table where email like ?",
                    into(user.user_id),
                    into(user.user_first_name),
                    into(user.user_last_name),
                    into(user.user_login),
                    into(user.user_password),
                    into(user.user_email),
                    into(user.user_phone_number),
                    into(user.user_birth_date),
                    use(email),
                    range(0, 1); //  iterate over result set one row at a time
            
            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return user;
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

    std::vector<User> User::find_by_first_last_name(std::string &first_name, std::string &last_name)
    {
        try
        {
            std::vector<User> result;
            first_name += "%";
            last_name += "%";

            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Statement select(session);
            select << "select id, first_name, last_name, user_login, user_password, email, phone_number, birth_date from users_table where first_name like ? and last_name like ?",
                    use(first_name),
                    use(last_name),
                    range(0, 1);

            select.execute();
            Poco::Data::RecordSet rs(select);
            bool more = rs.moveFirst();
            while (more)
            {
                User user;
                user.user_id = rs[0].convert<long>();
                user.user_first_name = rs[1].convert<std::string>();
                user.user_last_name = rs[2].convert<std::string>();
                user.user_login = rs[3].convert<std::string>();
                user.user_password = rs[4].convert<std::string>();
                user.user_email = rs[5].convert<std::string>();
                user.user_phone_number = rs[6].convert<std::string>();
                user.user_birth_date = rs[7].convert<std::string>();
                result.push_back(user);
                more = rs.moveNext();
            }
            return result;
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

    std::optional<User> User::find_by_phone_number(std::string &phone_number)
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Statement select(session);
            User user;
            select << "select id, first_name, last_name, user_login, user_password, email, phone_number, birth_date from users_table where phone_number like ?",
                    into(user.user_id),
                    into(user.user_first_name),
                    into(user.user_last_name),
                    into(user.user_login),
                    into(user.user_password),
                    into(user.user_email),
                    into(user.user_phone_number),
                    into(user.user_birth_date),
                    use(phone_number),
                    range(0, 1); //  iterate over result set one row at a time
            
            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return user;
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

    void User::save_to_db()
    {
        try
        {
            Poco::Data::Session session = database::Database::get_instance().create_database_session();
            Poco::Data::Statement insert(session);

            insert << "insert into users_table (first_name, last_name, login, password, email, phone_number, birth_date) values(?, ?, ?, ?, ?, ?)",
                use(user_first_name),
                use(user_last_name),
                use(user_login),
                use(user_password),
                use(user_email),
                use(user_phone_number),
                use(user_birth_date);

            insert.execute();

            Poco::Data::Statement select(session);
            select << "select last_insert_id()",
                into(user_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "Inserted new user with id = " << user_id << std::endl;
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

    long User::get_id() const
    {
        return user_id;
    }

    std::string User::get_first_name() const
    {
        return user_first_name;
    }

    std::string User::get_last_name() const
    {
        return user_last_name;
    }

    std::string User::get_login() const
    {
        return user_login;
    }

    std::string User::get_password() const
    {
        return user_password;
    }

    std::string User::get_email() const
    {
        return user_email;
    }

    std::string User::get_phone_number() const
    {
        return user_phone_number;
    }

    std::string User::get_birth_date() const
    {
        return user_birth_date;
    }

    void User::set_first_name(std::string f_n)
    {
        user_first_name = f_n;
    }

    void User::set_last_name(std::string l_n)
    {
        user_last_name = l_n;
    }

    void User::set_login(std::string log)
    {
        user_login = log;
    }

    void User::set_password(std::string pswrd)
    {
        user_password = pswrd;
    }

    void User::set_email(std::string mail)
    {
        user_email = mail;
    }

    void User::set_phone_number(std::string p_n)
    {
        user_phone_number = p_n;
    }

    void User::set_birth_day(std::string b_d)
    {
        user_birth_date = b_d;
    }
}
