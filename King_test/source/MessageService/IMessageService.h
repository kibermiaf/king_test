#pragma once

#include <string>
#include <vector>

namespace Core
{
    struct Message;

    // INTERFACE
    // Addresses: "Can the code be easily extended...?" & "SOLID (Dependency Inversion)"
    // This allows you to swap in a "CloudMessageService" or "DatabaseMessageService" later
    // without breaking the User Interface.

    class IMessageService {
    public:
        virtual ~IMessageService() = default;

        [[nodiscard]] virtual bool addUser(const std::string& username) = 0;
        [[nodiscard]] virtual bool userExists(const std::string& username) const = 0;

        // Using an enum for clearer return states than just 'bool' or 'int'.
        enum class SendResult
        {
            Success,
            UserNotFound,
            InvalidContent
        };
        virtual SendResult sendMessage(const std::string& from, const std::string& to, const std::string& content) = 0;

        // Returns a vector by value. Modern C++ relies on RVO (Return Value Optimization)
        // so this does not copy the vector, making it efficient.
        [[nodiscard]] virtual std::vector<Message> retrieveMessagesForUser(const std::string& username) = 0;
    };

}