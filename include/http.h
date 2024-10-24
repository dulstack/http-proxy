#ifndef __HTTP_H
#define __HTTP_H
#include <string>
//The http parser
namespace HTTP{
 void prepare_str(std::string& str);
 std::string get_header(const char* header, const char* name);
 std::string remove_header(const char* header, const char* name);
 unsigned short get_port(const std::string& addr);
 std::string get_addr(const std::string& line);
};
#endif
