//
// BCrypt-Password pw1 pw2 ...
//
// This is a front end to encrypt passwords. The standard bcrypt program is
// for files, which is annoying.
//

#include <iostream>

#include <bcrypt/BCrypt.hpp>
#include <showlib/OptionHandler.h>

using std::cout;
using std::endl;
using std::string;

using namespace ShowLib;

int main(int argc, char **argv)
{
    int workLoad = 12;
    OptionHandler::ArgumentVector args;

    args.addArg("workload", [&](const char *value) { workLoad = atoi(value); }, "workload", string{"A value [4..31], default is: "} + std::to_string(workLoad) );
    OptionHandler::handleOptions(argc, argv, args);

    for (int index = optind; index < argc; ++index) {
        std::string password { argv[index] };
        std::string hash = BCrypt::generateHash(password, workLoad);
        cout << hash << endl;
    }

    return 0;
}
