#include "frame.h"
Frame::Frame(const wxString& title):wxFrame(NULL, -1, title, wxPoint(-1,-1), wxSize(-1,-1)){
 //Create the menu bar which will control the server state
 wxMenuBar* mb=new wxMenuBar;
 wxMenu* file=new wxMenu;
 wxMenu* server=new wxMenu;
 server->Append(ID_START, wxT("&Start\tCtrl+S"));
 server->Append(ID_STOP, wxT("S&top\tCtrl+T"));
 server->Append(ID_RESTART, wxT("Restart"));
 file->AppendSubMenu(server, wxT("&Server"));
 file->AppendSeparator();
 file->Append(wxID_EXIT, wxT("E&xit\tAlt+X"));
 mb->Append(file, wxT("&File"));
 SetMenuBar(mb);
 
 //Set the event handlers
 Bind(wxEVT_COMMAND_MENU_SELECTED, &Frame::on_start, this, ID_START);
 Bind(wxEVT_COMMAND_MENU_SELECTED, &Frame::on_stop, this, ID_STOP);
 Bind(wxEVT_COMMAND_MENU_SELECTED, &Frame::on_restart,  this, ID_RESTART);
 Bind(wxEVT_SOCKET, &Frame::on_sock, this, ID_SOCK);
 Bind(wxEVT_SOCKET, &Frame::on_server, this, ID_SERVER);
 
 //The ui of the window
 wxPanel* panel=new wxPanel(this, -1);
 wxBoxSizer* vbox=new wxBoxSizer(wxVERTICAL), *status_box=new wxBoxSizer(wxHORIZONTAL);			//status_box is the sizer what contains the status label
 
 status=new wxStaticText(panel, -1, wxT("Stopped"));
 status->SetForegroundColour(wxColour(255,0,0));
 
 resp=new wxTextCtrl(panel, -1, "", wxPoint(-1,-1),wxSize(-1,-1), wxTE_MULTILINE|wxTE_READONLY);
 
 status_box->Add(new wxStaticText(panel, -1, wxT("Status: ")), 0, wxRIGHT, 10);
 status_box->Add(status, 0,0,0);
 vbox->Add(status_box, 0, wxLEFT|wxTOP, 3);
 vbox->Add(resp, 1, wxEXPAND|wxALL, 20);
 panel->SetSizer(vbox);
 
}
void Frame::on_server(wxSocketEvent& evt){
 wxSocketBase *sock=serv.get_sock()->Accept(false);
 if(!sock->IsOk()){
  wxMessageBox(wxT("Failed to accept the connection"), wxT("Error"), wxOK|wxICON_ERROR);
  return;
 }
 sock->SetEventHandler(*this, ID_SOCK);
 sock->SetNotify(wxSOCKET_INPUT_FLAG|wxSOCKET_LOST_FLAG);
 sock->Notify(1);
}
void Frame::on_sock(wxSocketEvent& evt){
 wxSocketBase* sock=evt.GetSocket();
 switch(evt.GetSocketEvent()){
  case wxSOCKET_INPUT:{
   if(!serv.accept(sock)){
    wxMessageBox(wxT("Failed to accept the connection"), wxT("Error"), wxOK|wxICON_ERROR);
   }
  }
  case wxSOCKET_LOST:{
   
  }
  sock->Destroy();
 }
}
void Frame::on_start(wxCommandEvent& evt){		//TODO: add custom ports
 unsigned short port=8080;
 if(!serv.start(port)){
  wxMessageBox(wxT("Failed to start the server"), wxT("Error"), wxOK|wxICON_ERROR);
  return;
 }
 wxSocketServer* sock=serv.get_sock();
 
 sock->SetEventHandler(*this, ID_SERVER);
 sock->SetNotify(wxSOCKET_CONNECTION_FLAG);
 sock->Notify(true);
 status->SetForegroundColour(wxColour(0,255,0));
 status->SetLabel(wxT("Running"));
}
void Frame::on_stop(wxCommandEvent& evt){
 serv.stop();
 status->SetForegroundColour(wxColour(255,0,0));
 status->SetLabel(wxT("Stopped"));
}
void Frame::on_restart(wxCommandEvent& evt){
 
}
