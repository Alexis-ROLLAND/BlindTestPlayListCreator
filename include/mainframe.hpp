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

#include "ressources.hpp"

#include "Playlist.hpp"
#include "tagManager.hpp"

using TagAndValue = std::pair<std::string, std::string>;

class MainFrame : public wxFrame {
  public:
    MainFrame(const wxString &title);

  private:
    std::unique_ptr<wxSplitterWindow> m_splitter;

    wxTreeCtrl *m_fileTree;
    wxGrid *m_grid;

    wxListBox *m_playList;

    std::unique_ptr<wxPanel> leftPanel;
    std::unique_ptr<wxPanel> rightPanel;

    std::unique_ptr<wxButton> m_buttonUp;
    std::unique_ptr<wxButton> m_buttonDown;
    std::unique_ptr<wxButton> m_buttonDelete;
    std::unique_ptr<wxButton> m_buttonGenerate;

    std::unique_ptr<wxButton> m_buttonUpdate;
    std::unique_ptr<wxButton> m_buttonPrepare;

    std::vector<TagAndValue> TagsChanged{};

    /**
     * @brief Prepares the screen from start to splitter and panels (left + right) construction
     *
     * @return std::pair<int, int> : width and height of the display.
     */
    std::pair<int, int> PrepareScreen();

    void OnTreeItemActivated(wxTreeEvent &event);
    void OnTreeSelectionChanged(wxTreeEvent &event);
    void PopulateFileTree(const wxString &path, wxTreeItemId parentId);
    wxString GetFullPath(wxTreeItemId itemId);
    void UpdateGridWithFileInfo(const wxString &filePath);

    void OnMoveUp(wxCommandEvent &event);
    void OnMoveDown(wxCommandEvent &event);
    void OnDelete(wxCommandEvent &event);
    void OnGenerate(wxCommandEvent &event);
    void OnUpdate(wxCommandEvent &event);
    void OnPrepare(wxCommandEvent &event);
    void OnGridCellChanged(wxGridEvent &event);

    bool UpdateTags();
    bool PrepareTags();
    std::string removeSpecialCharacters(const std::string &input);
};

#endif /* __MAINFRAME_HPP__ */
