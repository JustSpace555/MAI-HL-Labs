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
            std::string method = request.getMethod();
            std::istream& stream = request.stream();

            std::ostringstream oss;
            Poco::StreamCopier::copyStream(stream, oss);
            std::string requestBody = oss.str();
            std::cout << "Request body = " + requestBody << std::endl;

            HTMLForm form(request, stream);

            try 
            {
                if (path == "/user" && method == HTTPRequest::HTTP_GET)
                {
                    int user_id = stoi(form.get("id", "0"));
                    bool apply_cache = request.get("Cache-Control", "no-cache") == "cache";

                    if (user_id == 0)
                    {
                        send_not_found_exception("Missing param id", "/user", response);
                        return;
                    }

                    std::optional<models::User> result;
                    if (apply_cache)
                        result = models::User::from_cache(user_id);
                    else
                        result = models::User::get_by_id(user_id);

                    if (result)
                    {
                        if (apply_cache) result->save_to_cache();
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(result->to_json(), ostr);
                    }
                    else
                    {
                        send_not_found_exception("User with id = " + std::to_string(user_id) + " was not found", "/user", response);
                    }

                    return;
                }

                else if (path == "/user" && method == HTTPRequest::HTTP_POST)
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
                    std::ostream& stream = response.send();

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
                        Poco::Dynamic::Var result = parser.parse(requestBody);
                        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
                        first_name = object->getValue<std::string>("first_name");
                        last_name = object->getValue<std::string>("last_name");
                        login = object->getValue<std::string>("login");
                        password = object->getValue<std::string>("password");
                        email = object->getValue<std::string>("email");
                        phone_number = object->getNullableValue<std::string>("phone_number");
                        birth_day = object->getNullableValue<std::string>("birth_day");
                        
                    } catch (std::exception &e) {
                        stream << "{\"error_message\":\"" + (std::string)e.what() + "\"}";
                        return;
                    }

                    user.set_first_name(first_name);
                    user.set_last_name(last_name);
                    user.set_login(login);
                    user.set_password(password);
                    user.set_email(email);
                    if (!phone_number.isNull()) user.set_phone_number(phone_number.value());
                    if (!birth_day.isNull()) user.set_birth_day(birth_day.value());

                    try
                    {
                        int i = 0;
                        user.send_to_queue();
                        std::cout << "Send to queue: " << std::to_string(++i) << std::endl;
                        stream << "{\"result\":true}";
                        return;
                    }
                    catch(const std::exception& e)
                    {
                        stream << "{\"error_message\":\"" + (std::string)e.what() + "\"}";
                        return;
                    }
                    return;
                }

                else if (path == "/user/auth" && method == HTTPRequest::HTTP_GET)
                {
                    std::string by = form.get("by", "");
                    std::string param = form.get("param", "");
                    std::string password = form.get("password", "");

                    if (by.empty())
                    {
                        send_not_found_exception("Missing `by` param", "/user/auth", response);
                        return;
                    }
                    else if (param.empty())
                    {
                        send_not_found_exception("Missing `param`", "/user/auth", response);
                        return;
                    }
                    else if (password.empty())
                    {
                        send_not_found_exception("Missing `password` param", "user/auth", response);
                        return;
                    }

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
                        send_not_found_exception("User with " + by + " = " + param + " was not found", "/user", response);
                    }

                    return;
                }

                else if (path == "/user/find" && method == HTTPRequest::HTTP_POST)
                {
                    std::string by = form.get("by", "");

                    if (by.empty())
                    {
                        send_not_found_exception("Missing `by` param", "/user/find", response);
                        return;
                    }

                    Poco::JSON::Object::Ptr object;
                    try
                    {
                        Poco::JSON::Parser parser;
                        Poco::Dynamic::Var result = parser.parse(requestBody);
                        object = result.extract<Poco::JSON::Object::Ptr>();                        
                    }
                    catch (std::exception &e)
                    {
                        std::cout << "Error parsing request body: " << e.what() << std::endl;
                        response.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
                        response.send();
                        return;
                    }

                    std::vector<models::User> result;
                    if (by == "login")
                    {
                        Poco::Nullable<std::string> login = object->getNullableValue<std::string>("login");
                        if (login.isNull())
                        {
                            send_not_found_exception("Missing `login` param", "/user/auth", response);
                            return;
                        }
                        std::optional<models::User> user = models::User::find_by_login(login.value());
                        if (user.has_value()) result.push_back(user.value());
                    }
                    else if (by == "email")
                    {
                        Poco::Nullable<std::string> email = object->getNullableValue<std::string>("email");
                        if (email.isNull())
                        {
                            send_not_found_exception("Missing `login` param", "/user/auth", response);
                            return;
                        }
                        std::optional<models::User> user = models::User::find_by_email(email.value());
                        if (user.has_value()) result.push_back(user.value());
                    }
                    else if (by == "phone_number")
                    {
                        Poco::Nullable<std::string> phone_number = object->getNullableValue<std::string>("phone_number");
                        if (phone_number.isNull())
                        {
                            send_not_found_exception("Missing `login` param", "/user/auth", response);
                            return;
                        }
                        std::optional<models::User> user = models::User::find_by_phone_number(phone_number.value());
                        if (user.has_value()) result.push_back(user.value());
                    }
                    else if (by == "name")
                    {
                        Poco::Nullable<std::string> first_name = object->getNullableValue<std::string>("first_name"); 
                        Poco::Nullable<std::string> last_name = object->getNullableValue<std::string>("last_name");
                        if (first_name.isNull() || last_name.isNull())
                        {
                            send_not_found_exception("Missing `first_name` or `last_name` param", "/user/auth", response);
                            return;
                        }
                        result = models::User::find_by_first_last_name(first_name, last_name);
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

                    if (!result.empty())
                    {
                        Poco::JSON::Array output;
                        for (models::User user : result)
                            output.add(user.to_json());

                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        std::ostream &ostr = response.send();
                        Poco::JSON::Stringifier::stringify(output, ostr);
                    }
                    else
                    {
                        send_not_found_exception("User was not found", "/user", response);
                    }
                    return;
                }
            }
            catch (const std::exception &e)
            {
                std::string what = e.what();
                std::cout << "Exception: " + what << std::endl;
            }

            send_not_found_exception("Request receiver with path: " + path + " not found", "user", response);
        }

    private:
        std::string _format;

};

#endif
