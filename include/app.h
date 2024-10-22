#ifndef __APP_H
#define __APP_H
#include <wx/wx.h>
#include "frame.h"
#include "server.h"
class App: public wxApp{
 public:
  bool OnInit();
  void on_server(wxSocketEvent&);
  void on_sock(wxSocketEvent&);
 private:
  enum IDs{ID_SERV=1, ID_SOCK};
//  Server s;
};
#endif
