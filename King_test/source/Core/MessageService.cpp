#include "MessageService.h"

namespace Core 
{

    bool InMemoryMessageService::addUser(const std::string& username) 
    {
        if (username.empty()) 
        {
            return false;
        }

        auto [iter, success] = _users.insert(username);
        
        return success;
    }

    bool InMemoryMessageService::userExists(const std::string& username) const 
    {
        return _users.find(username) != _users.end();
    }

    IMessageService::SendResult InMemoryMessageService::sendMessage(
        const std::string& from,
        const std::string& to,
        const std::string& content) 
    {
        if (!userExists(from) || !userExists(to)) 
        {
            return SendResult::UserNotFound;
        }
        
        if (content.empty())
        {
            return SendResult::InvalidContent;
        }

        _userInboxes[to].emplace_back(Message {from, to, content });

        return SendResult::Success;
    }

    std::vector<Message> InMemoryMessageService::retrieveMessagesForUser(const std::string& username) 
    {
        if (!userExists(username)) 
        {
            return {};
        }

        // In the original code, messages were deleted after being viewed.
        // We replicate this behavior by "moving" the messages out of the map 
        // and returning them to the caller. The inbox becomes empty.
        
        // Check if they have an inbox entry
        auto it = _userInboxes.find(username);
        if (it == _userInboxes.end())
        {
            return {};
        }

        // Move the vector out of the map. This is very efficient (pointer swap), no copying of data.
        std::vector<Message> messages = std::move(it->second);
        
        // Clear the entry in the map since the user has "received" them.
        _userInboxes.erase(it);

        return messages;
    }

}