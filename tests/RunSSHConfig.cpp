#include <iostream>

#include <showlib/SSHConfiguration.h>

using std::cout;
using std::endl;
using std::string;

using namespace ShowLib;
using namespace ShowLib::SSH;

int main(int, char **) {
    Configuration config;

    config.load();
    const Host::Vector & hosts = config.getHosts();
    cout << "Number of hosts: " << hosts.size() << endl;

    for (const Host::Pointer &host: hosts) {
        cout << "Host: " << host->getHost()
             << " to remote host: " << host->getHostName()
             << " uses file " << host->getIdentityFile()
             << endl;
    }
}
