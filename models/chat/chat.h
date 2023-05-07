#ifndef CHAT_H
#define CHAT_H

#include <string>
#include <vector>
#include <optional>
#include <Poco/JSON/Object.h>

namespace models
{
    class Chat
    {
        private:
            long chat_id;
            long sender_id;
            long receiver_id;

        public:
            static void init();
            static Chat from_json(const std::string &json_str);
            static std::optional<Chat> select_by_id(const long id);
            static std::vector<Chat> select_all_by_sender_id(const long user_id);

            void save_to_db();
            Poco::JSON::Object::Ptr to_json() const;

            long get_id();
            long get_sender_id();
            long get_receiver_id();

            void set_sender_id(long id);
            void set_receiver_id(long id);
    };
}

#endif