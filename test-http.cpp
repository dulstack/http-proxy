#include <stdio.h>
#include "http.h"
int main(){
 const char input[]=
 "GET \n"
 "HOST: https://exapmple.com:8080\r\n"
 "USER-AGENT: agent\r\n\r\n";
 std::string addr=HTTP::get_addr(HTTP::get_header(input,"HOST"));
 printf("%i\n", HTTP::get_port(addr));
 
 return 0;
}
