#pragma once

#include "Core/IMessageService.h"
#include <functional>
#include <unordered_map>

// Namespace for UI components
namespace UI {

class CLI {
public:
  // Dependency Injection:
  // We pass the service by reference (or pointer).
  // The UI doesn't "own" the service, it just uses it.
  // This is crucial for the "Extensibility" criteria.
  explicit CLI(Core::IMessageService &service);

  // The main loop of the application
  void run();

private:
  void showMenu() const;
  void handleCreateUser();
  void handleSendMessage();
  void handleReceiveMessages();

  // Helper to get a line of text cleanly
  std::string promptLine(const std::string &promptText);

  Core::IMessageService &_service;

  // Map a string command ("1", "2") to a void function
  std::unordered_map<std::string, std::function<void()>> _actionMap;
};

} // namespace UI