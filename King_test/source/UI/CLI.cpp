#include "CLI.h"
#include "Core/Message.h"
#include <iostream>
#include <limits> // For std::numeric_limits


namespace UI {

CLI::CLI(Core::IMessageService &service) : _service(service) {
  // Bind commands to functions using lambdas
  _actionMap["1"] = [this]() { handleCreateUser(); };
  _actionMap["2"] = [this]() { handleSendMessage(); };
  _actionMap["3"] = [this]() { handleReceiveMessages(); };
  // add more commands here as needed like "delete user", etc.
}

void CLI::run() {
  bool running = true;
  while (running) {
    showMenu();

    std::string input = promptLine("> ");

    if (input == "4") {
      running = false;
      std::cout << "Quitting..." << std::endl;
      continue;
    }

    // Check if the command exists in our map
    auto it = _actionMap.find(input);
    if (it != _actionMap.end()) {
      // EXECUTE the stored function
      it->second();
    } else {
      std::cout << "Invalid option.\n";
    }

    if (running) {
      std::cout << "\nPress Enter to continue...";
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
}

void CLI::showMenu() const {
  // No more "80 lines of newlines" hack.
  // We just print a clean header.
  std::cout << "\n=========================\n";
  std::cout << "   MESSAGING SERVICE     \n";
  std::cout << "=========================\n";
  std::cout << "1. Create User\n";
  std::cout << "2. Send Message\n";
  std::cout << "3. Receive All Messages For User\n";
  std::cout << "4. Quit\n";
}

void CLI::handleCreateUser() {
  std::string name = promptLine("Enter username: ");

  if (_service.userExists(name)) {
    std::cout << "[ERROR] User '" << name << "' already exists!\n";
    return;
  }

  if (_service.addUser(name)) {
    std::cout << "[SUCCESS] User '" << name << "' added.\n";
  } else {
    std::cout << "[ERROR] Failed to add user (name might be empty).\n";
  }
}

void CLI::handleSendMessage() {
  std::string from = promptLine("From: ");
  std::string to = promptLine("To: ");
  std::string msg = promptLine("Message: ");

  auto result = _service.sendMessage(from, to, msg);

  switch (result) {
  case Core::IMessageService::SendResult::Success:
    std::cout << "[SUCCESS] Message sent!\n";
    break;
  case Core::IMessageService::SendResult::UserNotFound:
    std::cout << "[ERROR] One or both users do not exist.\n";
    break;
  case Core::IMessageService::SendResult::InvalidContent:
    std::cout << "[ERROR] Message content cannot be empty.\n";
    break;
  }
}

void CLI::handleReceiveMessages() {
  std::string user = promptLine("Enter username to check inbox: ");

  // Check user existence first to give a nice error message
  if (!_service.userExists(user)) {
    std::cout << "[ERROR] User '" << user << "' does not exist.\n";
    return;
  }

  auto messages = _service.retrieveMessagesForUser(user);

  std::cout << "\n===== INBOX (" << messages.size() << ") =====\n";

  if (messages.empty()) {
    std::cout << "No messages found.\n";
  } else {
    int count = 1;
    for (const auto &msg : messages) {
      std::cout << "--- Message " << count++ << " ---\n";
      std::cout << "From:    " << msg.from << "\n";
      std::cout << "Content: " << msg.content << "\n";
    }
  }
  std::cout << "============================\n";
}

std::string CLI::promptLine(const std::string &promptText) {
  std::cout << promptText;
  std::string input;
  std::getline(std::cin, input);
  return input;
}
} // namespace UI