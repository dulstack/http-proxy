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
 printf("Header: %s\n", cl_rq.header.c_str());
 printf("Content: %s\n", cl_rq.cont.c_str());
 return true;
}

Request Server::get_request(wxSocketBase* sock){
 Request res={"", ""};
 char* data=(char*)malloc(8195);
 sock->Read(data, 8192);
 int len=strlen(data);
 int i=0;
 for(i=0; i<len; i++){  //get the request header
  if(data[i]=='\r'&&i+1<len&&data[i+1]=='\n'&& i+2 <len&&data[i+2]=='\r'&&i+3<len&&data[i+1]=='\n'){
   //end of the header found. Stop the loop
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
  free(data);
  send_file(sock, "400.html", 400);
  res.header="";
  return res;
 }
 std::string s_cont_len=HTTP::get_header(res.header.c_str(), "CONTENT-LENGTH:");
 char first_len_c;
 if(s_cont_len.size()==0){free(data); return res;}
 first_len_c=s_cont_len.c_str()[0];
 if(first_len_c<'0'||first_len_c>'9'){free(data); return res;}
 int cont_len=stoi(s_cont_len), cont_read=0;	//cont_len is content length, cont_read is the number of bytes of content that was read
 if(i+4<len){						//store the remaing characters
  cont_read=len-i-4;
  for(i=i+4; i<len; i++){
   res.cont+=data[i];
  }
 }
 //now [cont_len-cont_read] left to read
 free(data);
 data=(char*)malloc(cont_len-cont_read);
 i=0;
 for(i=0;i<cont_len-cont_read; i++){
  res.cont+=data[i];
 }
 free(data);
 return res;
}

void Server::send_file(wxSocketBase* sock, const char* path, int status){
 FILE* fp=fopen(path, "r");
 if(!fp)return;
 int size;				//the size of file
 fseek(fp, 0, SEEK_END);
 size=ftell(fp);
 fseek(fp, 0, SEEK_SET);
 char* buf=(char*)malloc(size);
 if(buf){
  fread(buf, size, 1, fp);
  std::string resp="HTTP/1.1 "+std::to_string(status)+"\n"
  "CONTENT-TYPE: text/html\n"
  "CONTENT-LENGTH: "+std::to_string(size)+"\n\n"+buf;
  sock->Write(resp.c_str(), resp.size());
  free(buf);
 }
 fclose(fp);
}
