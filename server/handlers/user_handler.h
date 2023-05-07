#ifndef USER_HANDLER_H
#define USER_HANDLER_H

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

#include "../../models/user/user.h"
#include "../../helpers/send_not_found_exception.h"

class UserHandler : public HTTPRequestHandler
{

    public:
        UserHandler(const std::string &format) : _format(format) {}


        void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
        {
            response.setChunkedTransferEncoding(true);
            response.setContentType("application/json");
            Poco::URI uri = Poco::URI(request.getURI());
            std::string path = uri.getPath();
            std::cout << "Path of user request: " + path << std::endl;

            try 
            {
                if (path == "/user" && request.getMethod() == HTTPRequest::HTTP_GET)
                {
                    int user_id = stoi(request.get("id", "0"));
                    std::optional<models::User> result = models::User::get_by_id(user_id);

                    if (result)
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                    }
                    else
                    {
                        send_not_fount_exception("User with id" + std::to_string(user_id) + "was not found", "/wall", response);
                    }

                    return;
                }

                else if (path == "/user" && request.getMethod() == HTTPRequest::HTTP_POST)
                {
                    models::User user;
                    std::string first_name = "";
                    std::string last_name = "";
                    std::string login = "";
                    std::string password = "";
                    std::string email = "";
                    Poco::Nullable<std::string> phone_number;
                    Poco::Nullable<std::string> birth_day;

                    try
                    {
                        Poco::JSON::Parser parser;
                        Poco::Dynamic::Var result = parser.parse(request.stream());
                        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
                        first_name = object->getValue<std::string>("first_name");
                        last_name = object->getValue<std::string>("last_name");
                        login = object->getValue<std::string>("login");
                        password = object->getValue<std::string>("password");
                        email = object->getValue<std::string>("email");
                        phone_number = object->getNullableValue<std::string>("phone_number");
                        birth_day = object->getNullableValue<std::string>("birth_day");
                        
                    } catch (std::exception &e) {
                        std::cout << "Error parsing request body: " << e.what() << std::endl;
                        response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                        return;
                    }

                    user.set_first_name(first_name);
                    user.set_last_name(last_name);
                    user.set_login(login);
                    user.set_password(password);
                    user.set_email(email);
                    if (!phone_number.isNull()) user.set_phone_number(phone_number.value());
                    if (!birth_day.isNull()) user.set_birth_day(birth_day.value());

                    user.save_to_db();

                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                    response.send() << user.get_id();
                }

                else if (path == "/user/auth" && request.getMethod() == HTTPRequest::HTTP_GET)
                {
                    std::string by = request.get("by", "");
                    std::string param = request.get("param", "");
                    std::string password = request.get("password", "");

                    std::optional<models::User> result;
                    if (by == "login")
                    {
                        result = models::User::auth_by_login(param, password);
                    }
                    else if (by == "email")
                    {
                        result = models::User::auth_by_email(param, password);
                    }
                    else if (by == "phone_number")
                    {
                        result = models::User::auth_by_phone_number(param, password);
                    }
                    else
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_ACCEPTABLE);
                        Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
                        json->set("title", "Internal exception");
                        json->set("status", "406");
                        json->set("detail", "Unallowes authorize method: " + by);
                        json->set("instance", "/user");
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(json, ostr);
                        return;
                    }

                    if (result)
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                    }
                    else
                    {
                        send_not_fount_exception("User with " + by + " = " + param + "was not found", "/user", response);
                    }
                }

                else if (path == "/user/find" && request.getMethod() == HTTPRequest::HTTP_POST)
                {
                    std::string by = request.get("by", "");
                    std::string param = request.get("param", "");

                    std::optional<models::User> result;
                    if (by == "login")
                    {
                        try
                        {
                            Poco::JSON::Parser parser;
                            Poco::Dynamic::Var result = parser.parse(request.stream());
                            Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
                            std::string login = object->getValue<std::string>("login");
                            result = models::User::find_by_login(login);
                        } catch (std::exception &e) {
                            std::cout << "Error parsing request body: " << e.what() << std::endl;
                            response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                            return;
                        }
                    }
                    else if (by == "email")
                    {
                        try
                        {
                            Poco::JSON::Parser parser;
                            Poco::Dynamic::Var result = parser.parse(request.stream());
                            Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
                            std::string email = object->getValue<std::string>("email");
                            result = models::User::find_by_email(email);
                        } catch (std::exception &e) {
                            std::cout << "Error parsing request body: " << e.what() << std::endl;
                            response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                            return;
                        }
                    }
                    else if (by == "phone_number")
                    {
                        try
                        {
                            Poco::JSON::Parser parser;
                            Poco::Dynamic::Var result = parser.parse(request.stream());
                            Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
                            std::string phone_number = object->getValue<std::string>("phone_number");
                            result = models::User::find_by_phone_number(phone_number);
                        } catch (std::exception &e) {
                            std::cout << "Error parsing request body: " << e.what() << std::endl;
                            response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                            return;
                        }
                    }
                    else if (by == "name")
                    {
                        try
                        {
                            Poco::JSON::Parser parser;
                            Poco::Dynamic::Var result = parser.parse(request.stream());
                            Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
                            std::string first_name = object->getValue<std::string>("first_name");
                            std::string last_name = object->getValue<std::string>("last_name");
                            result = models::User::find_by_first_last_name(first_name, last_name);
                        } catch (std::exception &e) {
                            std::cout << "Error parsing request body: " << e.what() << std::endl;
                            response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                            return;
                        }
                    }
                    else
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_ACCEPTABLE);
                        Poco::JSON::Object::Ptr json = new Poco::JSON::Object();
                        json->set("title", "Internal exception");
                        json->set("status", "406");
                        json->set("detail", "Unallowes find method: " + by);
                        json->set("instance", "/user");
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(json, ostr);
                        return;
                    }

                    if (result)
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                    }
                    else
                    {
                        send_not_fount_exception("User with " + by + " = " + param + "was not found", "/user", response);
                    }
                }
            }
            catch (...) {}

            send_not_fount_exception("Request receiver with path: " + path + " not found", "", response);
        }

    private:
        std::string _format;

};

#endif
