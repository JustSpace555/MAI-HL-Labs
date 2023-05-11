#include <stdlib.h>
#include <stdio.h>
#include "config.h"

Config::Config()
{
    host = "127.0.0.1";
    port = "6033";
    login = "login";
    password = "password";
    database_path = "archdb";
    // host = std::getenv("DATABASE_HOST");
    // port = std::getenv("DATABASE_PORT");
    // login = std::getenv("DATABASE_LOGIN");
    // password = std::getenv("DATABASE_PASSWORD");
    // database_path = std::getenv("DATABASE_DATABASE");
}

Config &Config::get_instanse()
{
    static Config instance;
    return instance;
}

const std::string &Config::get_host()
{
    return host;
}

const std::string &Config::get_port()
{
    return port;
}

const std::string &Config::get_login()
{
    return login;
}

const std::string &Config::get_password()
{
    return password;
}

const std::string &Config::get_database_path()
{
    return database_path;
}