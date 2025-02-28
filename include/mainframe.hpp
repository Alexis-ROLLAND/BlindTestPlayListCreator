#ifndef __MAINFRAME_HPP__
#define __MAINFRAME_HPP__

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/app.h>
#include <wx/grid.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/wx.h>

#include <memory>

#include "ressources.hpp"

#include "Playlist.hpp"
#include "tagManager.hpp"

using TagAndValue = std::pair<std::string, std::string>;

const int ID_EXECUTE_ACTION{wxNewId()};
const int ID_SECOND_ACTION = wxNewId();

class MainFrame : public wxFrame {
  public:
    /**
     * @brief Construct a new Main Frame object
     *
     * @param title : Window Title
     * @param path  : path to the "root" directory (music files)
     */
    MainFrame(const wxString &title, const std::string &path);
    virtual ~MainFrame() = default;

  private:
    std::unique_ptr<wxSplitterWindow> m_splitter; /**< Main splitter windows */

    wxMenuBar *menuBar; /**< Menu bar */

    wxTreeCtrl *m_fileTree; /**< File tree   */
    wxGrid *m_grid;         /**< Tags & values grid  */

    wxListBox *m_playList; /**< Playlist list */

    std::unique_ptr<wxPanel> leftPanel;  /**< left side panel */
    std::unique_ptr<wxPanel> rightPanel; /**< right side panel */

    /** Buttons */
    std::unique_ptr<wxButton> m_buttonUp;
    std::unique_ptr<wxButton> m_buttonDown;
    std::unique_ptr<wxButton> m_buttonDelete;
    std::unique_ptr<wxButton> m_buttonGenerate;

    std::unique_ptr<wxButton> m_buttonUpdate;
    std::unique_ptr<wxButton> m_buttonPrepare;

    std::vector<TagAndValue> TagsChanged{}; /**< List of tags & values to update */

    std::string startPath{}; /**< root directory (music files) */

    /**
     * @brief Prepares the screen from start to splitter and panels (left + right) construction
     *
     * @return std::pair<int, int> : width and height of the display.
     */
    [[nodiscard]] std::pair<int, int> PrepareScreen();

    void PopulateFileTree(const wxString &path, wxTreeItemId parentId);
    [[nodiscard]] wxString GetFullPath(wxTreeItemId itemId);
    void UpdateGridWithFileInfo(const wxString &filePath);

    /** Event handlers */
    void OnMoveUp(wxCommandEvent &event);
    void OnMoveDown(wxCommandEvent &event);
    void OnDelete(wxCommandEvent &event);
    void OnGenerate(wxCommandEvent &event);
    void OnUpdate(wxCommandEvent &event);
    void OnPrepare(wxCommandEvent &event);
    void OnGridCellChanged(wxGridEvent &event);
    void OnTreeItemActivated(wxTreeEvent &event);
    void OnTreeSelectionChanged(wxTreeEvent &event);
    void OnTreeItemMenu(wxTreeEvent &event);
    void OnQuit(wxCommandEvent &event);
    void OnExecuteAction(wxCommandEvent &event);
    void OnSecondAction(wxCommandEvent &event);

    bool UpdateTags();
    bool PrepareTags();
    [[nodiscard]] std::string removeSpecialCharacters(const std::string &input);
    [[nodiscard]] int StringToInt(const std::string &str);
};

#endif /* __MAINFRAME_HPP__ */
