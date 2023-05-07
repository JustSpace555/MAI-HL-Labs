#ifndef USER_H
#define USER_H

#include <string>
#include <optional>
#include <vector>
#include "Poco/JSON/Object.h"

namespace models
{
    class User
    {
        private:
            long user_id;
            std::string user_first_name;
            std::string user_last_name;
            std::string user_login;
            std::string user_password;
            std::string user_email;
            std::string user_phone_number;
            std::string user_birth_date;

        public:
            static void init();
            static User from_json(const std::string &json);

            static std::optional<User> get_by_id(long id);
            static std::optional<User> auth_by_login(std::string &login, std::string &password);
            static std::optional<User> auth_by_email(std::string &email, std::string &password);
            static std::optional<User> auth_by_phone_number(std::string &phone_number, std::string &password);
            static std::optional<User> find_by_login(std::string &login);
            static std::optional<User> find_by_email(std::string &email);
            static std::vector<User> find_by_first_last_name(std::string &first_name, std::string &last_name);
            static std::optional<User> find_by_phone_number(std::string &phone_number);

            Poco::JSON::Object::Ptr to_json() const;
            void save_to_db();

            long get_id() const;
            std::string get_first_name() const;
            std::string get_last_name() const;
            std::string get_login() const;
            std::string get_password() const;
            std::string get_email() const;
            std::string get_phone_number() const;
            std::string get_birth_date() const;

            void set_first_name(std::string f_n);
            void set_last_name(std::string l_n);
            void set_login(std::string log);
            void set_password(std::string pswrd);
            void set_email(std::string mail);
            void set_phone_number(std::string p_n);
            void set_birth_day(std::string b_d);
    };
}

#endif