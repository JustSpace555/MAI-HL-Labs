#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <optional>
#include <vector>
#include "Poco/JSON/Object.h"

namespace models
{

    class Message
    {
        private:
            long message_id;
            long chat_id;
            long user_sender_id;
            std::string content;

        public:
            static void init();
            static Message from_json(std::string &json);
            static std::optional<Message> get_by_id(long id);
            static std::vector<Message> get_all_messages_by_chat_id(long id);

            void save_to_db();
            Poco::JSON::Object::Ptr to_json() const;

            long get_id() const;
            long get_chat_id() const;
            long get_sender_id() const;
            std::string get_content() const;

            void set_chat_id(long id);
            void set_sender_id(long id);
            void set_content(std::string c);
    };
}

#endif