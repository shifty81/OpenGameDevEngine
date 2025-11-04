/**
 * Network Manager Implementation
 */

#include "ogde/networking/NetworkManager.h"

namespace ogde {
namespace networking {

bool NetworkManager::initialize() {
    return true;
}

void NetworkManager::shutdown() {
}

bool NetworkManager::connect(const std::string& address, int port) {
    return true;
}

void NetworkManager::disconnect() {
}

bool NetworkManager::startServer(int port) {
    return true;
}

void NetworkManager::stopServer() {
}

void NetworkManager::send(const void* data, size_t size) {
}

} // namespace networking
} // namespace ogde
