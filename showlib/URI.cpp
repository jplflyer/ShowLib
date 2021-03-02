#include <iostream>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <cctype>
#include <functional>

#include "URI.h"

using namespace std;
using namespace ShowLib;

/**
 * Parse the string into constituent pieces.
 *
 * Note: this method uses C++ std algorithms and less-often-used methods such as:
 *
 * 		search(range_low, range_high, search_for_beginning, search_for_ending)
 * 		std::string::reserve(size) -- this simply allocates the space more efficiently than possibly doing it a char at a time.
 * 		transform(from_low, from_high, inserter, transformation) -- used to copy from our input url_s to the various
 * 					strings, forcing lower as we do it.
 * 		advance(iterator, amount) -- pushes an iterator forward a bit, used to skip over things like :// in the URL.
 */
void
URI::parse(const string& url_s)
{
    wellFormed = false;
    urlFailureReason = "";

    //--------------------------------------------------
    // Handle the protocol such as http://. At the end
    // of this section, protocol_ will have the http
    // portion, and we ignore ://.
    // prot_i will point to the first part of the host.
    //--------------------------------------------------
    const string prot_end("://");
    string::const_iterator prot_i = search(url_s.begin(), url_s.end(), prot_end.begin(), prot_end.end());

    protocol_.reserve(distance(url_s.begin(), prot_i));
    transform(url_s.begin(), prot_i,
              back_inserter(protocol_),
              ::tolower);

    if( prot_i == url_s.end() ) {
        urlFailureReason = "No protocol";
        return;
    }

    advance(prot_i, prot_end.length());

    //--------------------------------------------------
    // Find the start of the path within the host.
    // For http://foo:1234/b1/b1?flippy, we're grabbing
    // the /b1/b2 part.
    //--------------------------------------------------
    string::const_iterator path_i = find(prot_i, url_s.end(), '/');

    if (path_i == url_s.end()) {
        path_i = find(prot_i, url_s.end(), '?');
    }

    host_.reserve(distance(prot_i, path_i));

    transform(prot_i, path_i,
              back_inserter(hostWithPort_),
              ::tolower);

    // We do this in case there is no path portion.
    if (path_i == url_s.end()) {
        path_i = prot_i;
    }

    string::const_iterator query_i = find(path_i, url_s.end(), '?');

    path_.assign(path_i, query_i);
    if( query_i != url_s.end() ) {
        ++query_i;
    }
    query_.assign(query_i, url_s.end());

    // Parse host and port.
    int index = static_cast<int>(hostWithPort_.find(':'));
    if (index > 0) {
    	host_ = hostWithPort_.substr(0, index);
    	port_ = atoi(hostWithPort_.substr(index + 1).c_str());
    }
    else {
        host_ = hostWithPort_;
    }

    wellFormed = protocol_.length() > 0 && host_.length() > 0;
    if (!wellFormed) {
        if (protocol_.length() == 0) {
            urlFailureReason = "Protocol is blank: " + protocol_;
        }
        else if (host_.length() == 0) {
            urlFailureReason = "Host is blank: " + host_;
        }
    }
}
