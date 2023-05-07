#ifndef WALL_HANDLER_H
#define WALL_HANDLER_H

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

#include "../../models/post/post.h"
#include "../../helpers/send_not_found_exception.h"

class WallHandler : public HTTPRequestHandler
{

    public:
        WallHandler(const std::string &format) : _format(format) {}


        void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
        {
            response.setChunkedTransferEncoding(true);
            response.setContentType("application/json");
            Poco::URI uri = Poco::URI(request.getURI());
            std::string path = uri.getPath();
            std::cout << "Path of wall request: " + path << std::endl;

            try 
            {
                if (path == "/wall" && request.getMethod() == HTTPRequest::HTTP_GET)
                {
                    int user_id = stoi(request.get("id", "0"));
                    std::vector<models::Post> result = models::Post::get_all_posts_by_user_id(user_id);
                    Poco::JSON::Array output;

                    for (models::Post post: result) output.add(post.to_json());
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                    std::ostream &ostr = response.send();
                    Poco::JSON::Stringifier::stringify(output, ostr);
                    return;
                }

                else if (path == "/wall/post" && request.getMethod() == HTTPRequest::HTTP_GET)
                {
                    int post_id = stoi(request.get("receiver_id", "0"));
                    std::optional<models::Post> result = models::Post::get_post_by_id(post_id);
                    
                    if (result)
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                    }
                    else
                    {
                        send_not_fount_exception("Post with id" + std::to_string(post_id) + "was not found", "/wall", response);
                    }

                    return;
                }

                else if (path == "/wall/post" && request.getMethod() == HTTPRequest::HTTP_POST)
                {
                    int user_id = stoi(request.get("user_id", "0"));
                    std::string content = "";
                    std::string creation_date = "";
                    models::Post post;

                    try
                    {
                        Poco::JSON::Parser parser;
                        Poco::Dynamic::Var result = parser.parse(request.stream());
                        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
                        content = object->getValue<std::string>("content");
                        creation_date = object->getValue<std::string>("creation_date");
                    } catch (std::exception &e) {
                        std::cout << "Error parsing request body: " << e.what() << std::endl;
                        response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                        return;
                    }

                    post.set_user_id(user_id);
                    post.set_content(content);
                    post.set_creation_date(creation_date);

                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                    response.send() << post.get_post_id();
                    
                    return;
                }
            }
            catch (...) {}

            send_not_fount_exception("Request receiver with path: " + path + " not found", "", response);
        }

    private:
        std::string _format;

};

#endif
