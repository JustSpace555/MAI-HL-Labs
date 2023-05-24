#include <iostream>
#include <csignal>
#include <cppkafka/consumer.h>
#include <cppkafka/configuration.h>
#include "config/config.h"

#include "models/user/user.h"

bool running = true;

int main()
{
    try
    {
        Config default_config = Config::get_instanse();
        cppkafka::Configuration config =
        {
            { "metadata.broker.list", default_config.get_queue_host() },
            { "group.id", default_config.get_queue_group_id() },
            { "enable.auto.commit", false }
        };

        cppkafka::Consumer consumer(config);
        consumer.set_assignment_callback(
            [](const cppkafka::TopicPartitionList &partitions)
            {
                std::cout << "Got assigned: " << partitions << std::endl;
            }
        );
        consumer.set_revocation_callback(
            [](const cppkafka::TopicPartitionList &partitions)
            {
                std::cout << "Got revoked: " << partitions << std::endl;
            }
        );
        consumer.subscribe(
            {
                default_config.get_queue_topic()
            }
        );

        std::cout << "Consuming messages from topic " << default_config.get_queue_topic() << std::endl;

        while (running)
        {
            cppkafka::Message msg = consumer.poll();
            if (msg)
            {
                if (msg.get_error())
                {
                    if (!msg.is_eof())
                    {
                        std::cout << "[+] Received error notification: " << msg.get_error() << std::endl;
                    }
                }
                else
                {
                    if (msg.get_key())
                    {
                        std::cout << msg.get_key() << " -> ";
                    }
                    std::cout << "Got message" << std::endl;
                    std::string payload = msg.get_payload();
                    std::cout << msg.get_payload() << std::endl;
                    models::User user = models::User::from_json(payload);
                    user.save_to_db();
                    consumer.commit(msg);
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}