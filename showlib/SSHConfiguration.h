#pragma once

//======================================================================
// This provides reading ~/.ssh/config (or any other config file).
// Use of the singleton is not enforced but supported for performance
// reasons.
//======================================================================

#include <memory>
#include <mutex>

#include "JSONSerializable.h"

namespace ShowLib {
    namespace SSH {
        class Configuration;
        class Host;
    }
}

/**
 * This represents one entry in the config file. We manage a subset of
 * stuff we might encounter.
 */
class ShowLib::SSH::Host {
public:
    typedef std::shared_ptr<Host> Pointer;
    typedef std::vector<Pointer> Vector;

    Host(const std::string &hostIn);
    virtual ~Host();

    void parse(const std::string &line);
    bool matchHost(const std::string &) const;

    const std::string & getHost() const { return host; }
    const std::string & getHostName() const { return hostName; }
    const std::string & getUser() const { return user; }
    const std::string & getIdentityFile() const { return identityFile; }
    int getPort() const { return port; }

    Host & setIdentityFile(const std::string &value) { identityFile = value; return *this; }

private:
    std::string		host;
    std::string		hostName;
    std::string		user;
    std::string		identityFile;			// This might be a .pem file or an RSA pair.
    int				port;
};

/**
 * This represents the entire config file.
 */
class ShowLib::SSH::Configuration
{
public:
    Configuration();

    static Configuration &	singleton();

    void load(std::string pathIn = "");

    bool isLoaded() const { return loaded; }

    const Host::Vector & getHosts() const { return hosts; }
    Host::Pointer getHost(const std::string &host) const;

private:
    std::string path;
    Host::Vector	hosts;
    bool loaded = false;
};

