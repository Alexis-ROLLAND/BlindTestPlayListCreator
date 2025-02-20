#include "mainframe.hpp"

class MyApp : public wxApp {
  public:
    virtual bool OnInit();
};

bool MyApp::OnInit() {
    MainFrame *frame = new MainFrame("Blind Test PlayList Creator");
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);
