#include <iostream>
#include <string>

#include <showlib/CommonUsing.h>
#include <showlib/ConnectionPool.h>

using namespace ShowLib;

/**
 * Constructor.
 */
ConnectionPool::ConnectionPool()
{
}

/**
 * Set the maximum number of connections we're willing to open.
 */
ConnectionPool &
ConnectionPool::setMaxConnections(size_t value) {
    maxConnections = value;
    return *this;
}

/**
 * DB Server name.
 */
ConnectionPool & ConnectionPool::setHost(const string & value) {
    host = value;
    return *this;
}

/**
 * DB Port.
 */
ConnectionPool & ConnectionPool::setPort(int value) {
    port = value;
    return *this;
}

/**
 * Database to connect to.
 */
ConnectionPool & ConnectionPool::setDatabaseName(const string & value) {
    databaseName = value;
    return *this;
}

/**
 * DB Username to use.
 */
ConnectionPool & ConnectionPool::setUsername(const string & value) {
    username = value;
    return *this;
}

/**
 * DB Password to use.
 */
ConnectionPool & ConnectionPool::setPassword(const string & value) {
    password = value;
    return *this;
}

ConnectionPool::PoolEntry ConnectionPool::getConnection() {
    std::unique_lock<std::mutex> lock(myMutex);
    while (availablePool.size() == 0) {
        if (totalPool.size() < maxConnections) {
            //postgresql://[user[:password]@][netloc][:port][/dbname][?param1=value1&...]
            string url = dbURL;

            if ( url.empty()) {
                string delim = "";

                url = "postgresql://";
                if (username.length() > 0) {
                    url += username;
                    delim ="@";
                }
                if (password.length() > 0) {
                    url += string{":"} + password;
                    delim ="@";
                }

                url += delim + host + ":" + std::to_string(port);

                if (databaseName.length() > 0) {
                    url += string{"/"} + databaseName;
                }
            }

            ConnectionPtr conn = std::make_shared<pqxx::connection>(url);
            totalPool.push_back(conn);
            availablePool.push(conn);
            break;
        }

        condVar.wait(lock);
    }

    ConnectionPtr connection = availablePool.front();
    availablePool.pop();
    PoolEntry entry(this, connection);
    return entry;
}

void ConnectionPool::releaseConnection(ConnectionPool::PoolEntry &entry) {
    availablePool.push(entry.connection);
}

/**
 * Initial constructor.
 */
ConnectionPool::PoolEntry::PoolEntry(ConnectionPool * _pool, ConnectionPtr _conn)
    : pool(_pool), connection(_conn)
{
}

/**
 * Move constructor.
 */
ConnectionPool::PoolEntry::PoolEntry(PoolEntry &&orig) {
    std::swap(pool, orig.pool);
    std::swap(connection, orig.connection);
}

/**
 * Destructor.
 */
ConnectionPool::PoolEntry::~PoolEntry() {
    if (pool != nullptr) {
        pool->releaseConnection(*this);
    }
}
