#include <stdio.h>
#include "http.h"
int main(){
 const char input[]=
 "GET \n"
 "HOST: https://exapmple.com:1024 o\n"
 "USER-AGENT: agent\r\n\r\n";
 printf("%s\n", HTTP::get_addr(HTTP::get_header(input, "HOST")).c_str());
 
 return 0;
}
