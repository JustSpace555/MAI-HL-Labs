#ifndef POST_H
#define POST_H

#include <string>
#include <optional>
#include <vector>
#include "Poco/JSON/Object.h"

namespace models
{
    class Post
    {
        private:
            long post_id;
            long user_id;
            std::string content;
            std::string creation_date;

        public:
            static void init();
            static Post from_json(const std::string &json);
            static std::optional<Post> get_post_by_id(long id);
            static std::vector<Post> get_all_posts_by_user_id(long id);

            void save_to_db();

            long get_post_id() const;
            long get_user_id() const;
            std::string get_content() const;
            std::string get_creation_date() const;
            Poco::JSON::Object::Ptr to_json() const;

            void set_user_id(long id);
            void set_content(std::string c);
            void set_creation_date(std::string date);
    };
}

#endif