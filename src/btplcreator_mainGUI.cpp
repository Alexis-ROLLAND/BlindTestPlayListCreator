#include "mainframe.hpp"

class MyApp : public wxApp {
  public:
    virtual bool OnInit();
};

bool MyApp::OnInit() {

    std::string path{};
    if (this->argc == 2) path = std::string(this->argv[1].ToUTF8());

    MainFrame *frame = new MainFrame("Blind Test PlayList Creator", path);

    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);
