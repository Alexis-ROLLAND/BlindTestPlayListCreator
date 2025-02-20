#ifndef __MAINFRAME_HPP__
#define __MAINFRAME_HPP__

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/grid.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/wx.h>

#include <memory>

#include "tagManager.hpp"

#define UNUSED(x) static_cast<void>(x)

class MainFrame : public wxFrame {
  public:
    MainFrame(const wxString &title);

  private:
    std::unique_ptr<wxSplitterWindow> m_splitter;

    std::unique_ptr<wxTreeCtrl> m_fileTree;
    std::unique_ptr<wxGrid> m_grid;

    std::unique_ptr<wxListBox> m_playList;

    std::unique_ptr<wxPanel> leftPanel;
    std::unique_ptr<wxBoxSizer> leftSizer;

    std::unique_ptr<wxPanel> rightPanel;
    std::unique_ptr<wxBoxSizer> rightSizer;

    std::unique_ptr<wxButton> m_buttonUp;
    std::unique_ptr<wxButton> m_buttonDown;
    std::unique_ptr<wxButton> m_buttonDelete;
    std::unique_ptr<wxButton> m_buttonGenerate;

    std::unique_ptr<wxBoxSizer> buttonSizer;

    void OnTreeItemActivated(wxTreeEvent &event);
    void OnTreeSelectionChanged(wxTreeEvent &event);
    void PopulateFileTree(const wxString &path, wxTreeItemId parentId);
    wxString GetFullPath(wxTreeItemId itemId);
    void UpdateGridWithFileInfo(const wxString &filePath);

    void OnMoveUp(wxCommandEvent &event);
    void OnMoveDown(wxCommandEvent &event);
    void OnDelete(wxCommandEvent &event);
    void OnGenerate(wxCommandEvent &event);
};

#endif /* __MAINFRAME_HPP__ */
