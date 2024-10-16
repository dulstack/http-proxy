#include <stdio.h>
#include "http.h"
int main(){
 const char* input=
 "ACCEPT */*\r\n"
 "HOST doow.cf\r\n"
 "GET /index\r\n\r\n";
 printf("%s", HTTP::remove_header(input, "HOST").c_str());
 return 0;
}
