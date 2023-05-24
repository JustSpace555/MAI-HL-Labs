#include <stdlib.h>
#include <stdio.h>
#include "config.h"

Config::Config()
{
    host = std::getenv("DATABASE_HOST");
    port = std::getenv("DATABASE_PORT");
    login = std::getenv("DATABASE_LOGIN");
    password = std::getenv("DATABASE_PASSWORD");
    database_name = std::getenv("DATABASE_DATABASE");
    cache_server = std::getenv("CACHE");
    queue_host = std::getenv("QUEUE_HOST");
    queue_topic = std::getenv("QUEUE_TOPIC");
    queue_group_id  = std::getenv("QUEUE_GROUP_ID");
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

const std::string &Config::get_database_name()
{
    return database_name;
}

const std::string &Config::get_cache_server()
{
    return cache_server;
}

const std::string &Config::get_queue_group_id()
{
    return queue_group_id;
}

const std::string &Config::get_queue_topic()
{
    return queue_topic;
}

const std::string &Config::get_queue_host()
{
    return queue_host;
}