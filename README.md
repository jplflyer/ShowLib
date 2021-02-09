# showlib
Common library used by some of my other projects

# Requirements
I'm using a bcrypt C++ library from: https://github.com/trusch/libbcrypt. Build and install per the readme. This is only used for the BCrypt-Password program, so if you don't want to install this library, then edit out the Makefile. Remove "bins" from the "all" rule and take a peek at the "install" rule.
