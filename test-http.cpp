#include <stdio.h>
#include "http.h"
int main(){
 const char input[]=
 "GET \n"
 "HOST: exapmple.com\n"
 "USER-AGENT: agent\r\n\r\n";
 printf("%s", HTTP::get_header(input, "HOST").c_str());
 
 return 0;
}
