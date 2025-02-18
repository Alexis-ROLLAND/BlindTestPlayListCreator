#ifndef __MAINFRAME_HPP__
#define __MAINFRAME_HPP__

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/wx.h>

#define UNUSED(x) static_cast<void>(x)

class MainFrame : public wxFrame {
  public:
    MainFrame(const wxString &title);

  private:
    wxSplitterWindow *m_splitter;
    wxTreeCtrl *m_fileTree;
    wxListBox *m_fileList;
    wxButton *m_buttonUp;
    wxButton *m_buttonDown;
    wxButton *m_buttonDelete;

    void OnTreeItemActivated(wxTreeEvent &event);
    void PopulateFileTree(const wxString &path, wxTreeItemId parentId);
    wxString GetFullPath(wxTreeItemId itemId);

    void OnMoveUp(wxCommandEvent &event);
    void OnMoveDown(wxCommandEvent &event);
    void OnDelete(wxCommandEvent &event);
};

#endif /* __MAINFRAME_HPP__ */
