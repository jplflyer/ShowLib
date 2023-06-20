#pragma once

#include <memory>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <vector>
#include <pqxx/pqxx>

namespace ShowLib {
    class ConnectionPool;
}

/**
 * This provides a pool of connections to our PostreSQL database.
 */
class ShowLib::ConnectionPool
{
public:
    typedef std::shared_ptr<pqxx::connection> ConnectionPtr;
    using Pointer = std::shared_ptr<ConnectionPool>;

    class PoolEntry {
    public:
        ConnectionPool * pool = nullptr;
        ConnectionPtr connection;

        PoolEntry(ConnectionPool * _pool, ConnectionPtr _conn);
        PoolEntry(PoolEntry &&orig);
        virtual ~PoolEntry();

        pqxx::connection & operator*() { return *connection; }
    };

    ConnectionPool();
    ConnectionPool & setMaxConnections(size_t);

    ConnectionPool & setURL(const std::string &value) { dbURL = value; return *this; }
    ConnectionPool & setHost(const std::string &);
    ConnectionPool & setPort(int);
    ConnectionPool & setDatabaseName(const std::string &);
    ConnectionPool & setUsername(const std::string &);
    ConnectionPool & setPassword(const std::string &);

    PoolEntry getConnection();
    void releaseConnection(PoolEntry &entry);

private:
    std::vector<ConnectionPtr> totalPool;
    std::queue<ConnectionPtr> availablePool;
    std::mutex myMutex;
    std::condition_variable condVar;

    size_t maxConnections = 10;

    std::string dbURL;
    std::string host = "localhost";
    int port = 5444;
    std::string databaseName;
    std::string username;
    std::string password;
};

