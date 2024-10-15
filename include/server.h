#ifndef __SERVER_H
#define __SERVER_H
#include <wx/socket.h>
#include <string.h>
//TODO: add the error enum for error processing
typedef struct Request{
 std::string header;
 std::string cont;
}Request;
class Server{
 public:
  Server();
  ~Server();
  bool start(unsigned short port);
  Request get_request(wxSocketBase* sock);			//will read from socket
  bool accept(wxSocketBase* sock);
  void stop();
  wxSocketServer* get_sock();
 private:
  wxSocketServer* serv;
  
};
#endif
