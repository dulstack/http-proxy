#include "app.h"
#include <stdio.h>
wxIMPLEMENT_APP(App);
bool App::OnInit(){
 if(!wxApp::OnInit())return false;
 Frame* frame=new Frame(wxT("Untitled"));
 frame->Show(true);
 return true;
}
