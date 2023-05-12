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
#include "Poco/StreamCopier.h"
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
            std::string method = request.getMethod();
            std::istream& stream = request.stream();

            std::ostringstream oss;
            Poco::StreamCopier::copyStream(stream, oss);
            std::string requestBody = oss.str();
            std::cout << "Request body = " + requestBody << std::endl;

            HTMLForm form(request, stream);

            try 
            {
                if (path == "/chat" && method == HTTPRequest::HTTP_GET)
                {
                    int chatId = stoi(form.get("id", "0"));
                    if (chatId == 0)
                    {
                        send_not_found_exception("Missing param id", "/chat", response);
                        return;
                    }
                    std::optional<models::Chat> result = models::Chat::select_by_id(chatId);

                    if (result)
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                    }
                    else
                    {
                        send_not_found_exception("Chat with id = " + std::to_string(chatId) + " not found", "/chat", response);
                    }
                    return;
                }

                else if (path == "/chat" && method == HTTPRequest::HTTP_POST)
                {
                    int creator_id = stoi(form.get("creator_id", "0"));
                    int receiverId = stoi(form.get("receiver_id", "0"));
                    if (creator_id == 0 || receiverId == 0)
                    {
                        send_not_found_exception("Missing `creator_id` or `receiver_id` param", "/chat", response);
                        return;                        
                    }

                    models::Chat chat;
                    chat.set_creator_id(creator_id);
                    chat.set_receiver_id(receiverId);
                    chat.save_to_db();

                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                    response.send() << chat.get_id();

                    return;
                }

                else if (path == "/chat/get_all" && method == HTTPRequest::HTTP_GET)
                {
                    int creator_id = stoi(form.get("id", "0"));
                    if (creator_id == 0)
                    {
                        send_not_found_exception("Missing param `id`", "/chat/get_all", response);
                        return;                        
                    }
                    std::vector<models::Chat> result = models::Chat::select_all_by_creator_id(creator_id);
                    Poco::JSON::Array output;

                    for (models::Chat chat : result) output.add(chat.to_json());

                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(output, ostr);

                    return;
                }

                else if (path == "/chat/message" && method == HTTPRequest::HTTP_GET)
                {
                    int messageId = stoi(form.get("id", "0"));
                    if (messageId == 0)
                    {
                        send_not_found_exception("Missing param `id`", "/chat/message", response);
                        return;                        
                    }                    

                    std::optional<models::Message> result = models::Message::get_by_id(messageId);
                    if (result)
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                    }
                    else
                    {
                        send_not_found_exception("Message with id = " + std::to_string(messageId) + "not found", "/chat/message", response);
                    }
                    return;
                }

                else if (path == "/chat/message" && request.getMethod() == HTTPRequest::HTTP_POST)
                {
                    int chatId = stoi(form.get("chat_id", "0"));
                    int creator_id = stoi(form.get("creator_id", "0"));
                    if (chatId == 0 || creator_id == 0)
                    {
                        send_not_found_exception("Missing `chat_id` or `creator_id` param", "/chat/message", response);
                        return;                                                
                    }

                    std::string content = "";
                    models::Message message;
                    try
                    {
                        Poco::JSON::Parser parser;
                        Poco::Dynamic::Var result = parser.parse(requestBody);
                        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
                        content = object->getValue<std::string>("content");
                    } catch (std::exception &e) {
                        std::cout << "Error parsing request body: " << e.what() << std::endl;
                        response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                        response.send();
                        return;
                    }

                    message.set_chat_id(chatId);
                    message.set_sender_id(creator_id);
                    message.set_content(content);
                    message.save_to_db();

                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                    response.send() << message.get_id();
                    
                    return;
                }

                else if (path == "/chat/message/get_all" && method == HTTPRequest::HTTP_GET)
                {
                    int chat_id = stoi(form.get("id", "0"));
                    if (chat_id == 0)
                    {
                        send_not_found_exception("Missing `id` param", "/chat/message/get_all", response);
                        return;                           
                    }
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

            send_not_found_exception("Request receiver with path: " + path + " not found", "", response);
        }
    
    private:
        std::string _format;
};

#endif
