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
 
 mb->Enable(ID_STOP, 0);
 mb->Enable(ID_RESTART, 0);
 //Set the event handlers
 Bind(wxEVT_COMMAND_MENU_SELECTED, &Frame::on_start, this, ID_START);
 Bind(wxEVT_COMMAND_MENU_SELECTED, &Frame::on_stop, this, ID_STOP);
 Bind(wxEVT_COMMAND_MENU_SELECTED, &Frame::on_restart,  this, ID_RESTART);
 Bind(wxEVT_COMMAND_MENU_SELECTED, &Frame::on_quit, this, wxID_EXIT);
 Bind(wxEVT_CLOSE_WINDOW, &Frame::on_close, this);
 Bind(wxEVT_SOCKET, &Frame::on_sock, this, ID_SOCK);
 Bind(wxEVT_SOCKET, &Frame::on_server, this, ID_SERVER);
 
 //The ui of the window
 wxPanel* panel=new wxPanel(this, -1);
 wxBoxSizer* vbox=new wxBoxSizer(wxVERTICAL), *status_box=new wxBoxSizer(wxHORIZONTAL);			//status_box is the sizer what contains the status label
 
 status=new wxStaticText(panel, -1, wxT("Stopped"));
 status->SetForegroundColour(wxColour(255,0,0));
 
 tc_log=new wxTextCtrl(panel, -1, "", wxPoint(-1,-1),wxSize(-1,-1), wxTE_MULTILINE|wxTE_READONLY);
 
 status_box->Add(new wxStaticText(panel, -1, wxT("Status: ")), 0, wxRIGHT, 10);
 status_box->Add(status, 0,0,0);
 vbox->Add(status_box, 0, wxLEFT|wxTOP, 3);
 vbox->Add(tc_log, 1, wxEXPAND|wxALL, 20);
 panel->SetSizer(vbox);
 
}
void Frame::update_status(){
 wxMenuBar* mb=this->GetMenuBar();
 if(serv.is_started()){
  status->SetForegroundColour(wxColour(0,255,0));
  status->SetLabel(wxT("Started"));
 }
 else{
  status->SetForegroundColour(wxColour(255,0,0));
  status->SetLabel(wxT("Stopped"));
 }
 mb->Enable(ID_START, !serv.is_started());
 mb->Enable(ID_STOP, serv.is_started());
 mb->Enable(ID_RESTART,serv.is_started());
}
void Frame::log(const wxString& msg){
 tc_log->SetValue(tc_log->GetValue()+msg+"\n");
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
   Request rq=serv.process_rq(sock);
   if(rq.header.size()<=0){
    #ifdef DEBUG
    wxMessageBox(wxT("Failed to process the connection"), wxT("DEBUG"), wxOK|wxICON_ERROR);
    #endif
   }
   else{
    log(rq.header);
    log(rq.cont);
   }
   break;
  }
  case wxSOCKET_LOST:{
   break;
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
 update_status();
}
void Frame::on_stop(wxCommandEvent& evt){
 serv.stop();
 update_status();
}
void Frame::on_restart(wxCommandEvent& evt){
 serv.stop();
 serv.start();
}

void Frame::on_close(wxCloseEvent& evt){
 this->quit();
}

void Frame::on_quit(wxCommandEvent& evt){
 this->quit();
}
void Frame::quit(){
 if(!serv.is_started())this->Destroy();
 else{
  int response;
  wxMessageDialog question(NULL,wxT("The server is still running\nDo you really want to quit?"), wxT("Question"), wxYES_NO|wxYES_DEFAULT|wxICON_QUESTION);
  response=question.ShowModal();
  if(response==wxID_YES)this->Destroy();
 }
}
