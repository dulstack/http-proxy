#ifndef __HTTP_H
#define __HTTP_H
#include <string>
//The http parser
namespace HTTP{
 std::string get_header(const char* header, const char* name);
 std::string remove_header(const char* header, const char* name);
};
#endif
