#include "app.h"
#include <stdio.h>
wxIMPLEMENT_APP(App);
bool App::OnInit(){
 if(!s.start(8080)){
  fprintf(stderr, "Failed to create the socket\n");
  return false;
 }
 wxSocketServer* sock=s.get_sock();
 if(!sock){return 0;}
 Bind(wxEVT_SOCKET, &App::on_server, this, ID_SERV);
 Bind(wxEVT_SOCKET, &App::on_sock, this, ID_SOCK);
 sock->SetEventHandler(*this, ID_SERV);
 sock->SetNotify(wxSOCKET_CONNECTION_FLAG);
 sock->Notify(true);
 return 1;
}

void App::on_server(wxSocketEvent& evt){
 wxSocketBase* sock=s.get_sock()->Accept(false);
 if(!sock){fprintf(stderr, "Failed to accept the connection\n");return;}
 sock->SetEventHandler(*this, ID_SOCK);
 sock->SetNotify(wxSOCKET_INPUT_FLAG|wxSOCKET_LOST_FLAG);
 sock->Notify(true);
}
void App::on_sock(wxSocketEvent& evt){
 wxSocketBase* sock=evt.GetSocket();
 switch(evt.GetSocketEvent()){
  case wxSOCKET_INPUT:{
   s.accept(sock);
   break;
  }
  case wxSOCKET_LOST:{
   fprintf(stderr, "Connection lost\n");
   s.stop();
   break;
  }
 }
 sock->Destroy();
 s.stop();
}
