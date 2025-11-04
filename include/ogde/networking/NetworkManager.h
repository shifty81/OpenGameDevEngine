/**
 * @file NetworkManager.h
 * @brief Network communication for multiplayer
 */

#ifndef OGDE_NETWORKING_NETWORKMANAGER_H
#define OGDE_NETWORKING_NETWORKMANAGER_H

#include <string>

namespace ogde {
namespace networking {

/**
 * @class NetworkManager
 * @brief Network communication system
 */
class NetworkManager {
public:
    /**
     * @brief Initialize the network manager
     * @return true if initialization was successful
     */
    bool initialize();

    /**
     * @brief Shutdown the network manager
     */
    void shutdown();

    /**
     * @brief Connect to a server
     * @param address Server address
     * @param port Server port
     * @return true if connection was successful
     */
    bool connect(const std::string& address, int port);

    /**
     * @brief Disconnect from server
     */
    void disconnect();

    /**
     * @brief Start hosting a server
     * @param port Port to listen on
     * @return true if server started successfully
     */
    bool startServer(int port);

    /**
     * @brief Stop the server
     */
    void stopServer();

    /**
     * @brief Send data to server or clients
     * @param data Data to send
     * @param size Size of data
     */
    void send(const void* data, size_t size);
};

} // namespace networking
} // namespace ogde

#endif // OGDE_NETWORKING_NETWORKMANAGER_H
