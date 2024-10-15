#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <http.h>
Server::Server(){
 serv=NULL;
}
Server::~Server(){
 this->stop();
}
void Server::stop(){
 if(serv){
  serv->Close();
  serv->Destroy();
 }
 serv=NULL;
}
bool Server::start(unsigned short port){
 wxIPV4address addr;
 addr.Service(port);
 serv=new wxSocketServer(addr);
 if(!serv->Ok()){
  serv=NULL;return false;
 }
 return true;
}

wxSocketServer* Server::get_sock(){
 return this->serv;
}

bool Server::accept(wxSocketBase* sock){
 Request cl_rq=get_request(sock);
 printf("%s\n", cl_rq.header.c_str());
 return true;
}

Request Server::get_request(wxSocketBase* sock){
 Request res={"", ""};
 char* data=(char*)malloc(8195);
 sock->Read(data, 8192);
 int len=strlen(data);
 int i=0;
 for(i=0; i<len; i++){
  if(data[i]=='\r'&&i+1<len&&data[i+1]=='\n'&& i+2 <len&&data[i+2]=='\r'&&i+3<len&&data[i+1]=='\n'){
   //end of the header found. Stop the loop
   printf("Found the end of header\n");
   break;
  }
  //make the header uppercase, if it is lowercase
  if(data[i]>='a'&&data[i]<='z'){
   data[i]=data[i]-0x20;		//'a'-'A'=0x61-0x41=0x20
  }
  
  res.header=res.header+data[i];
 }
 if(i>=len){
  //The end of header was not found, fail
  const char* res_body=
   "<html>\n"
   "<head><title>400 bad request</title></head>"
   "<body><h2>Bad request</h2></body>";
   std::string res=
   "HTTP/1.1 400 Bad Request\n"
   "Content-Type:text/html\n"
   "Content-Length:";
   res+=std::to_string(strlen(res_body))+std::string("\n\n")+res_body;
   sock->Write(res.c_str(), res.size());
 }
 return res;
}
