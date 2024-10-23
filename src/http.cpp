#include "http.h"
#include <string.h>
#include <stdio.h>
void HTTP::prepare_str(std::string& arg){
 for(int i=0; i!=-1; i=arg.find("\n", i+1)){
  if(i>1&&arg.c_str()[i-1]!='\r'){arg.replace(i, 1, "\r\n");}
  i++;
 }
}
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
 HTTP::prepare_str(s_header);
 for(int pos=0; pos!=-1; lines++){
  pos=s_header.find("\r\n", pos+1);
 }
 
 for(int i=0; i<lines; i++){
  std::string line=get_line(s_header, i);
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
 HTTP::prepare_str(res);
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
std::string HTTP::get_addr(const std::string& line){
 //get the port from the line(possibly will not work with multiple lines)
 unsigned short port=-1;
 std::string s_addr=line;
 //convert the line to upper case
 for(int i=0; i<s_addr.size(); i++){
  const char c=s_addr[i];
  if(c>='a'&&c<='z'){s_addr[i]=c-('a'-'A');}
 }
 //if there is http:// or https:// in string, then remove it
 int i=s_addr.find("HTTP://");
 if(i==-1){
  i=s_addr.find("HTTPS://");
  if(i!=-1){
   s_addr.erase(i, 8);
  }
 }
 else{
  s_addr.erase(i, 7);
 }
 while(s_addr[0]==' '){s_addr.erase(0,1);}		//remove the spaces
 i=s_addr.find(":");
 if(i==-1){
  //consider the first word as the address
  i=s_addr.find(" ");
  if(i!=-1)s_addr=s_addr.substr(0, i);
  return s_addr;
 }
 //get the last word before ':' character
 int start=i, end=i;
 for(start=i; start>0&&s_addr[start]!=' '; start--);
 if(start!=0){start++;}
 end=s_addr.find(" ", i+1);
 if(end==-1)end=s_addr.size()-1;
 s_addr=s_addr.substr(start, end-start);
 return s_addr;
}
