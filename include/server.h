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
  bool start(unsigned short port=8080);
  Request get_request(wxSocketBase* sock);			//will read from socket
  Request process_rq(wxSocketBase* sock);
  void send_file(wxSocketBase* sock, const char* path, int status_code=200);
  void stop();
  bool is_started();		//TODO: implement this
  wxSocketServer* get_sock();
 private:
  wxSocketServer* serv;
};
#endif
