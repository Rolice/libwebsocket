#ifndef __HTTP_HPP__
#define __HTTP_HPP__

#include <map>
#include <string>

typedef std::map<std::string, std::string> HeaderCollection;
typedef std::map<std::string, std::string>::iterator HeaderCollectionIterator;
typedef std::pair<std::string, std::string> Header;

#define HTTP_COMPONENT_SEPARATOR       "\r\n\r\n"
#define HTTP_HEADER_SEPARATOR          "\r\n"

#define HTTP_GET                       "GET"
#define HTTP_POST                      "POST"
#define HTTP_PUT                       "PUT"
#define HTTP_DELETE                    "DELETE"
#define HTTP_HEAD                      "HEAD"
#define HTTP_OPTIONS                   "OPTIONS"
#define HTTP_TRACE                     "TRACE"
#define HTTP_CONNECT                   "CONNECT"

#endif