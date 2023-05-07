#ifndef CHAT_HANDLER_H
#define CHAT_HANDLER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/JSON/Parser.h"
#include "Poco/URI.h"
#include <iostream>
#include <iostream>
#include <fstream>

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPRequest;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "../../models/chat/chat.h"
#include "../../models/message/message.h"
#include "../../helpers/send_not_found_exception.h"

class ChatHandler : public HTTPRequestHandler
{

    public:
        ChatHandler(const std::string &format) : _format(format) {}


        void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
        {
            response.setChunkedTransferEncoding(true);
            response.setContentType("application/json");
            Poco::URI uri = Poco::URI(request.getURI());
            std::string path = uri.getPath();
            std::cout << "Path of chat request: " + path << std::endl;

            try 
            {
                if (path == "/chat" && request.getMethod() == HTTPRequest::HTTP_GET)
                {
                    int chatId = stoi(request.get("id", "0"));
                    std::optional<models::Chat> result = models::Chat::select_by_id(chatId);

                    if (result)
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                    }
                    else
                    {
                        send_not_fount_exception("Chat with id = " + std::to_string(chatId) + "not found", "/chat", response);
                    }
                    return;
                }

                else if (path == "/chat" && request.getMethod() == HTTPRequest::HTTP_POST)
                {
                    int senderId = stoi(request.get("sender_id", "0"));
                    int receiverId = stoi(request.get("receiver_id", "0"));

                    models::Chat chat;
                    chat.set_sender_id(senderId);
                    chat.set_receiver_id(receiverId);
                    chat.save_to_db();

                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                    response.send() << chat.get_id();

                    return;
                }

                else if (path == "/chat/get_all" && request.getMethod() == HTTPRequest::HTTP_GET)
                {
                    int senderId = stoi(request.get("id", "0"));
                    std::vector<models::Chat> result = models::Chat::select_all_by_sender_id(senderId);
                    Poco::JSON::Array output;

                    for (models::Chat chat : result) output.add(chat.to_json());

                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(output, ostr);

                    return;
                }

                else if (path == "/chat/message" && request.getMethod() == HTTPRequest::HTTP_GET)
                {
                    int messageId = stoi(request.get("id", "0"));
                    std::optional<models::Message> result = models::Message::get_by_id(messageId);

                    if (result)
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                    }
                    else
                    {
                        send_not_fount_exception("Message with id = " + std::to_string(messageId) + "not found", "/chat", response);
                    }
                    return;
                }

                else if (path == "/chat/message" && request.getMethod() == HTTPRequest::HTTP_POST)
                {
                    int chatId = stoi(request.get("chat_id", "0"));
                    int senderId = stoi(request.get("sender_id", "0"));
                    std::string content = "";
                    models::Message message;

                    try
                    {
                        Poco::JSON::Parser parser;
                        Poco::Dynamic::Var result = parser.parse(request.stream());
                        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
                        content = object->getValue<std::string>("content");
                    } catch (std::exception &e) {
                        std::cout << "Error parsing request body: " << e.what() << std::endl;
                        response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                        return;
                    }

                    message.set_chat_id(chatId);
                    message.set_sender_id(senderId);
                    message.set_content(content);
                    message.save_to_db();

                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                    response.send() << message.get_id();
                    
                    return;
                }

                else if (path == "/chat/message/get_all" && request.getMethod() == HTTPRequest::HTTP_GET)
                {
                    int chat_id = stoi(request.get("id", "0"));
                    std::vector<models::Message> result = models::Message::get_all_messages_by_chat_id(chat_id);
                    Poco::JSON::Array output;

                    for (models::Message message : result) output.add(message.to_json());

                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(output, ostr);

                    return;
                }
            }
            catch (...) {}

            send_not_fount_exception("Request receiver with path: " + path + "not found", "", response);
        }
    
    private:
        std::string _format;
};

#endif
