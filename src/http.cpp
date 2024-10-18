#include "http.h"
#include <string.h>
#include <stdio.h>
static std::string get_line(const std::string& str, int line){
 std::string res="";
 int beg=0,end;		//begin and end of the line
 end=str.find("\r\n", beg+1);
 if(end==-1)end=str.size()-1;
 for(int i=1; i<=line; i++){
  beg=end+2;
  end=str.find("\r\n", beg+1);
  if(end==-1){end=str.size()-1;break;}
 }
 res=str.substr(beg, end-beg);
 return res;
}
std::string HTTP::get_header(const char* header, const char* name){
 std::string res="";
 int name_len=strlen(name);
 int lines=0;
 std::string s_header=header;
 for(int pos=0; pos!=-1; lines++){
  pos=s_header.find("\r\n", pos+1);
 }
 
 for(int i=0; i<lines; i++){
  std::string line=get_line(header, i);
  if(line.size()>name_len&&line.substr(0, name_len)==name&&(line.c_str()[name_len]==' '||line.c_str()[name_len]==':')){
   res=line.substr(name_len+1, (line.size()-name_len-1));
   while(res.c_str()[0]==' '){res.erase(0,1);}
   break;
  }
 }
 return res;
}
std::string HTTP::remove_header(const char* header, const char* name){
 std::string res=header;
 int i=0, end=0;
 for(i=0; i!=-1; i++){
  i=res.find(name, i);
  //find if it is really a header name
  if(i==0||(i>2&&res.c_str()[i-2]=='\r'&&res.c_str()[i-1]=='\n')){
   printf("%i\n", i);
   end=res.find("\r\n", i+1);
   if(end==-1){end=res.size()-1;}		//no line break character found, so end of substring to cut will be at the end of header
   else{end+=2;}
   res.erase(i, end-i);
   break;
  }
  i++;
 }
 return res;
}
