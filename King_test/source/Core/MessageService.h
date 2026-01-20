#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "IMessageService.h"
#include "Message.h"

namespace Core 
{
    // Addresses: "Appropriate data structures" & "Scalability"
    class InMemoryMessageService : public IMessageService 
    {
    public:
        bool addUser(const std::string& username) override;
        bool userExists(const std::string& username) const override;
        SendResult sendMessage(const std::string& from, const std::string& to, const std::string& content) override;
        std::vector<Message> retrieveMessagesForUser(const std::string& username) override;

    private:
        // Addresses: "Appropriate data structures"
        // Original code used std::vector for users (O(N) search time).
        // std::unordered_set provides O(1) average search time
        std::unordered_set<std::string> _users;

        // Addresses: "Scalability"
        // Original code iterated through *every* message to find one user's inbox.
        // A map allows O(1) access to a specific user's inbox.
        // Key = Receiver's Username, Value = List of Messages
        std::unordered_map<std::string, std::vector<Message>> _userInboxes;
    };

}

