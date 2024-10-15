#include <stdio.h>
#include "http.h"
int main(){
 const char* input=
 "HOST doow.cf\r\n"
 "GET /index\r\n\r\n";
 printf("%s", HTTP::get_header(input, "GET").c_str());
 return 0;
}
