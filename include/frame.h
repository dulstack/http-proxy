#ifndef __FRAME_H
#define  __FRAME_H
#include "app.h"
#include "server.h"
//TODO: add custom ports, closing confirmation, quit by choosing quit on menu
class Frame: public wxFrame{
 public:
  Frame(const wxString& title);
  void log(const wxString& message);
  void update_status();
  void on_server(wxSocketEvent&);
  void on_sock(wxSocketEvent&);
  void on_start(wxCommandEvent&);
  void on_stop(wxCommandEvent&);
  void on_restart(wxCommandEvent&);
 private:
  enum IDs{
   ID_START=1,
   ID_STOP,
   ID_RESTART,
   ID_SERVER,
   ID_SOCK
  };
  wxStaticText *status;
  wxTextCtrl *tc_log;
  Server serv;
};
#endif
