#include <iostream>
#include <fstream>
#include <filesystem>
#include <pwd.h>
#include <unistd.h>

#include "SSHConfiguration.h"
#include "StringUtils.h"

using std::cout;
using std::endl;
using std::string;
using namespace ShowLib;
using namespace ShowLib::SSH;

static Configuration * s_singleton = nullptr;
static std::mutex globalMutex;

/**
 * Return our singleton, which may not yet be loaded.
 */
Configuration &
Configuration::singleton() {
    std::unique_lock<std::mutex> lock(globalMutex);
    if (s_singleton == nullptr) {
        s_singleton = new Configuration();
    }
    return *s_singleton;
}

/**
 * Constructor.
 */
Configuration::Configuration()
{
}

/**
 * Load it up.
 */
void
Configuration::load(string pathIn) {
    string homeDir = getpwuid(getuid())->pw_dir;
    path = pathIn;

    if (path.length() == 0) {
        path = homeDir + "/.ssh/config";
    }

    std::ifstream ifs {path};
    string line;
    Host::Pointer currentEntry = nullptr;

    //--------------------------------------------------
    // Parse lines. We assume that any # starts a
    // a comment (no # inside a quoted string or
    // something like that).
    //--------------------------------------------------
    while (std::getline(ifs, line)) {
        size_t commentPos = line.find("#");
        if (commentPos != line.npos) {
            line = line.substr(0, commentPos);
        }

        string trimmed = trim(line);
        if (trimmed.length() == 0) {
            continue;
        }

        if (startsWith(line, "Host ")) {
            currentEntry = std::make_shared<Host>(line.substr(5));
            hosts.push_back(currentEntry);
        }
        else if (currentEntry != nullptr) {
            currentEntry->parse(trimmed);
        }
    }

    for (Host::Pointer &host: hosts) {
        string identityFile = host->getIdentityFile();
        if (identityFile[0] == '~') {
            host->setIdentityFile(homeDir + identityFile.substr(1));
        }
    }

    loaded = true;
}

/**
 * Look it up.
 */
Host::Pointer
Configuration::getHost(const std::string &host) const {
    Host::Pointer rv = nullptr;

    for (const Host::Pointer &ptr: hosts) {
        if (ptr->matchHost(host)) {
            rv = ptr;
            break;
        }
    }

    return rv;
}

//======================================================================
// Individual keys.
//======================================================================

/**
 * Constructor
 */
Host::Host(const std::string &hostIn)
    : host(hostIn)
{
}

Host::~Host() {
}

/**
 * Parse this line. It's already been trimmed.
 */
void Host::parse(const std::string &line) {
    auto parts = splitAtWhitespace(trim(line));
    string key = toLower(parts.first);

    if (key == "hostname") {
        hostName = parts.second;
    }
    else if (key == "user") {
        user = parts.second;
    }
    else if (key == "identityfile") {
        identityFile = parts.second;
    }
    else if (key == "port") {
        port = stoi(parts.second);
    }
}

bool
Host::matchHost(const std::string &hostIn) const {
    return (host == "*") || (hostIn == host);
}
