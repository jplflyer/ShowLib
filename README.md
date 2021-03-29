# showlib
Common library used by some of my other projects

# Requirements
## bcrypt
I'm using a bcrypt C++ library from: https://github.com/trusch/libbcrypt. Build and install per the readme. This is only used for the BCrypt-Password program, so if you don't want to install this library, then edit out the Makefile. Remove "bins" from the "all" rule and take a peek at the "install" rule.

## poco
Poco::Net comes from git@github.com:pocoproject/poco.git. Their readme describes the build process. It's slightly atypical:

    cd poco
    mkdir cmake-build
    cd cmake-build
    cmake .. -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl
    cmake --build . --config Release
    sudo cmake --build . --target install

That's on Mac. You can get rid of the -DOPENSSL part on other environments.

## My GitLib plus libgit2
You'll need my gitlib repo: git@github.com:jplflyer/GitLib.git. And that requires libgit2:

    brew install libgit2


The GitLib repo is just:

    make && sudo make install
