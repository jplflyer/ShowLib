#include <stdio.h>
#include <iostream>

#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

#include <showlib/SSHConfiguration.h>

using std::cout;
using std::endl;
using std::string;

using namespace ShowLib;
using namespace ShowLib::SSH;


int main(int argc, char ** argv) {
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

    if (argc > 1) {
        cout << endl << endl << "Now try loading the id_rsa file" << endl;
        const char * pw = argv[1];

        OpenSSL_add_all_algorithms();
        OpenSSL_add_all_ciphers();
        ERR_load_crypto_strings();

        FILE * fp = fopen("/Users/jpl/.ssh/id_rsa", "r");
        RSA * rsaKey = RSA_new();

        rsaKey = PEM_read_RSAPrivateKey(fp, NULL, NULL, (void *)pw);

        if (rsaKey == nullptr) {
            cout << "Returns NULL." << endl;
        }
        else if (RSA_check_key(rsaKey)) {
            cout << "Should be valid." << endl;
        }
        else {
            cout << "Invalid." << endl;
        }

        fclose(fp);
    }
}
