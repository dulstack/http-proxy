#include <stdio.h>
#include "http.h"
int main(){
 const char* input=
 "GET /\r\n"
 "HOST: exapmple.com\r\n"
 "USER-AGENT: agent\r\n\r\n";
 printf("%s", HTTP::get_header(input, "HOST").c_str());
 return 0;
}
