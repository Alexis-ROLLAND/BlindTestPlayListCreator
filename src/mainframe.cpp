#include "mainframe.hpp"
#include <wx/dir.h>
#include <wx/display.h>
#include <wx/filename.h>
#include <wx/stattext.h>

MainFrame::MainFrame(const wxString &title, const std::string &path) : wxFrame(NULL, wxID_ANY, title), startPath{path} {

    auto [width, height] = this->PrepareScreen();

    /** Menu bar preparation */
    menuBar = new wxMenuBar();

    wxMenu *fileMenu = new wxMenu();
    fileMenu->Append(wxID_EXIT, "Quitter");
    menuBar->Append(fileMenu, "File");
    SetMenuBar(menuBar);
    Bind(wxEVT_MENU, &MainFrame::OnQuit, this, wxID_EXIT);

    /** ------------------  Left Side ------------------------*/
    wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL); /** Creating a (Box) Sizer */

    m_fileTree = new wxTreeCtrl(leftPanel.get(), wxID_ANY); /** Creating a TreeCtrl (the File tree) */
    leftSizer->Add(m_fileTree, 1, wxEXPAND | wxALL, 5);     /** Adding file tree to the sizer */

    /** Creating and configuring the grid */
    m_grid = new wxGrid(leftPanel.get(), wxID_ANY);
    m_grid->CreateGrid(0, 1);
    m_grid->SetColLabelValue(0, COL_LABEL_VALUE);
    m_grid->Bind(wxEVT_GRID_CELL_CHANGED, &MainFrame::OnGridCellChanged, this); /** Bind event  */
    leftSizer->Add(m_grid, 1, wxEXPAND | wxALL, 5);                             /** Add the grid to the sizer */

    wxBoxSizer *leftbuttonSizer = new wxBoxSizer(wxHORIZONTAL);

    m_buttonPrepare = std::make_unique<wxButton>(leftPanel.get(), wxID_ANY, wxString::FromUTF8(BUTTON_LABEL_PREPARE));
    m_buttonPrepare->Enable(false);

    m_buttonUpdate = std::make_unique<wxButton>(leftPanel.get(), wxID_ANY, wxString::FromUTF8(BUTTON_LABEL_UPDATE)); /** Creates the "update" button */
    m_buttonUpdate->Enable(false);                                                                                   /** Bouton inactif par défaut */

    leftbuttonSizer->Add(m_buttonPrepare.get(), 0, wxALIGN_CENTER | wxALL, 5); /** Add button to the sizer */
    leftbuttonSizer->Add(m_buttonUpdate.get(), 0, wxALIGN_CENTER | wxALL, 5);  /** Add button to the sizer */

    leftSizer->Add(leftbuttonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);

    leftPanel->SetSizer(leftSizer); /** Assign leftSizer to left Panel */

    /** ------------------  Right Side ------------------------*/
    wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL); /** Creating a (Box) Sizer */

    // Ajouter un label pour la liste
    wxStaticText *listLabel = new wxStaticText(rightPanel.get(), wxID_ANY, wxString::FromUTF8(LIST_LABEL_FILELIST));
    rightSizer->Add(listLabel, 0, wxALL | wxCENTER, 5);

    m_playList = new wxListBox(rightPanel.get(), wxID_ANY, wxDefaultPosition, wxDefaultSize); /** Create a ListBox */
    rightSizer->Add(m_playList, 1, wxEXPAND | wxALL, 10);                                     // La liste occupe tout l'espace restant

    // Créer les boutons et les ajouter dans leur propre sizer
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    m_buttonUp = std::make_unique<wxButton>(rightPanel.get(), wxID_ANY, wxString::FromUTF8(BUTTON_LABEL_MONTER));
    m_buttonDown = std::make_unique<wxButton>(rightPanel.get(), wxID_ANY, wxString::FromUTF8(BUTTON_LABEL_DESCENDRE));
    m_buttonDelete = std::make_unique<wxButton>(rightPanel.get(), wxID_ANY, wxString::FromUTF8(BUTTON_LABEL_SUPPRIMER));
    m_buttonGenerate = std::make_unique<wxButton>(rightPanel.get(), wxID_ANY, wxString::FromUTF8(BUTTON_LABEL_GENERER));

    buttonSizer->Add(m_buttonUp.get(), 0, wxALL, 5);
    buttonSizer->Add(m_buttonDown.get(), 0, wxALL, 5);
    buttonSizer->Add(m_buttonDelete.get(), 0, wxALL, 5);
    buttonSizer->Add(m_buttonGenerate.get(), 0, wxALL, 5);

    rightSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);

    // Appliquer le sizer au panneau
    rightPanel->SetSizer(rightSizer);

    rightPanel->Layout();   // Forcer la mise en page du panneau

    m_splitter->SplitVertically(leftPanel.get(), rightPanel.get());

    m_splitter->SetMinimumPaneSize(width / 4);
    // Définir la position initiale du séparateur à 1/4 de la largeur
    m_splitter->SetSashPosition(width / 4);

    // Créer la liste des fichiers sélectionnés
    if (path.empty()) this->startPath = wxGetHomeDir();
    // std::println(std::clog, "Start Path = {}", this->startPath);
    wxString rootDir = wxString::FromUTF8(this->startPath);

    // Vérifier si le répertoire existe
    if (!wxDir::Exists(rootDir)) {
        wxMessageBox(wxString::FromUTF8("Le répertoire spécifié n'existe pas : ") + rootDir, wxString::FromUTF8("Erreur"), wxOK | wxICON_ERROR);
        Close();   // Ferme la fenêtre principale
        return;
    }

    // Ajouter le répertoire racine à l'arborescence
    wxTreeItemId rootId = m_fileTree->AddRoot(rootDir);
    PopulateFileTree(rootDir, rootId);
    m_fileTree->Expand(rootId);

    m_fileTree->Bind(wxEVT_TREE_ITEM_ACTIVATED, &MainFrame::OnTreeItemActivated, this);
    m_fileTree->Bind(wxEVT_TREE_SEL_CHANGED, &MainFrame::OnTreeSelectionChanged, this);
    m_fileTree->Bind(wxEVT_TREE_ITEM_MENU, &MainFrame::OnTreeItemMenu, this);

    // Lier les événements des boutons
    m_buttonUp->Bind(wxEVT_BUTTON, &MainFrame::OnMoveUp, this);
    m_buttonDown->Bind(wxEVT_BUTTON, &MainFrame::OnMoveDown, this);
    m_buttonDelete->Bind(wxEVT_BUTTON, &MainFrame::OnDelete, this);
    m_buttonGenerate->Bind(wxEVT_BUTTON, &MainFrame::OnGenerate, this);
    m_buttonUpdate->Bind(wxEVT_BUTTON, &MainFrame::OnUpdate, this);
    m_buttonPrepare->Bind(wxEVT_BUTTON, &MainFrame::OnPrepare, this);
}
/**------------------------------------------------------------------------------------------------*/

/**------------------------------------------------------------------------------------------------*/
std::pair<int, int> MainFrame::PrepareScreen() {   // Obtenir la taille de l'écran principal
    wxDisplay display{};
    wxRect screenRect = display.GetClientArea();

    // Calculer les 3/4 de la largeur et de la hauteur
    int width = screenRect.GetWidth() * SCREEN_FACTOR;
    int height = screenRect.GetHeight() * SCREEN_FACTOR;

    // Définir la taille de la fenêtre
    SetSize(width, height);

    // Centrer la fenêtre sur l'écran
    Centre();

    this->m_splitter = std::make_unique<wxSplitterWindow>(this, wxID_ANY);    /** Creates two subwindows */
    leftPanel = std::make_unique<wxPanel>(this->m_splitter.get(), wxID_ANY);  /** Creates the left Panel (wxWindow specialization, graphical container) */
    rightPanel = std::make_unique<wxPanel>(this->m_splitter.get(), wxID_ANY); /** Creates the right Panel (wxWindow specialization, graphical container) */

    // Retourner les coordonnées
    return std::make_pair(width, height);
}

/**------------------------------------------------------------------------------------------------*/
void MainFrame::OnMoveUp(wxCommandEvent &event) {

    int sel = m_playList->GetSelection();
    if (sel > 0) {
        wxString item = m_playList->GetString(sel);
        m_playList->Delete(sel);
        m_playList->Insert(item, sel - 1);
        m_playList->SetSelection(sel - 1);
    }

    event.Skip(); /** Do not propagate event to parent object */
}
/**------------------------------------------------------------------------------------------------*/
void MainFrame::OnMoveDown(wxCommandEvent &event) {

    int sel = m_playList->GetSelection();
    if (sel == wxNOT_FOUND) return; /** No item selected */

    if (static_cast<unsigned int>(sel) < m_playList->GetCount() - 1) {
        wxString item = m_playList->GetString(sel);
        m_playList->Delete(sel);
        m_playList->Insert(item, sel + 1);
        m_playList->SetSelection(sel + 1);
    }
    event.Skip(); /** Do not propagate event to parent object */
}
/**------------------------------------------------------------------------------------------------*/
void MainFrame::OnDelete(wxCommandEvent &event) {

    int sel = m_playList->GetSelection();
    if (sel == wxNOT_FOUND) return; /** No item selected */

    m_playList->Delete(sel);
    if (static_cast<unsigned int>(sel) < m_playList->GetCount())
        m_playList->SetSelection(sel);
    else if (m_playList->GetCount() > 0)
        m_playList->SetSelection(m_playList->GetCount() - 1);

    event.Skip(); /** Do not propagate event to parent object */
}
/**------------------------------------------------------------------------------------------------*/
void MainFrame::OnGenerate(wxCommandEvent &event) {

    int NbTitres = m_playList->GetCount();
    if (NbTitres < 1) return;

    wxFileDialog saveFileDialog(this, "Enregistrer le fichier", "", "", "Fichiers m3u (*.m3u)|*.m3u", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_OK) {
        wxString wxfileDir = saveFileDialog.GetDirectory();
        wxString wxfileName = saveFileDialog.GetFilename();

        std::string fileDir = std::string(wxfileDir.ToUTF8()) + std::string{wxFILE_SEP_PATH};
        std::string fileName = std::string(wxfileName.ToUTF8());

        m3uPlaylist pl{fileName, fileDir};

        // std::println(std::clog, "File Name = {}", pl.getFileName());

        for (int i = 0; i < NbTitres; ++i) {
            wxString chaine = m_playList->GetString(i);
            std::string filepath = std::string(chaine.ToUTF8());
            pl.add(filepath);
        }

        try {
            pl.generate();
        } catch (const std::ios::failure &e) {
            std::println(std::cerr, "Error Creating m3u file");
        }
    }
    event.Skip(); /** Do not propagate event to parent object */
}
/**------------------------------------------------------------------------------------------------*/
void MainFrame::OnTreeItemMenu(wxTreeEvent &event) {
    wxTreeItemId item = event.GetItem();
    if (item.IsOk()) {
        wxMenu contextMenu;
        contextMenu.Append(ID_GENERATE_RANDOM_PLAYLIST_ACTION, wxString::FromUTF8("Générer Playlist Aléatoire"));
        contextMenu.Append(ID_ADD_RANDOM_TITLE_ACTION, wxString::FromUTF8("Ajouter titre aléatoire à la PlayList"));

        contextMenu.Bind(wxEVT_MENU, &MainFrame::OnGenerateRandomPlaylistAction, this, ID_GENERATE_RANDOM_PLAYLIST_ACTION);
        contextMenu.Bind(wxEVT_MENU, &MainFrame::OnAddRandomTitleAction, this, ID_ADD_RANDOM_TITLE_ACTION);

        PopupMenu(&contextMenu, event.GetPoint());
    }
}
/**------------------------------------------------------------------------------------------------*/
void MainFrame::OnGenerateRandomPlaylistAction(wxCommandEvent &event) {
    wxString itemPath = this->GetFullPath(this->m_fileTree->GetSelection());
    wxArrayString files;

    // Vérifier si c'est un répertoire ou un fichier
    if (wxFileName::DirExists(itemPath)) {
        this->m_playList->Clear();
        wxDir::GetAllFiles(itemPath, &files, "*.mp3", wxDIR_FILES);
        std::size_t NbFiles = files.GetCount();
        int NbTitlesToAdd = (NbFiles < DEFAULT_PLAYLIST_SIZE) ? NbFiles : DEFAULT_PLAYLIST_SIZE;
        // std::println(std::clog, "Files to add : {}", NbTitlesToAdd);
        std::vector<int> tabIndex(NbFiles);
        std::iota(tabIndex.begin(), tabIndex.end(), 0);
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(tabIndex.begin(), tabIndex.end(), g);
        for (int i = 0; i < NbTitlesToAdd; ++i)
            m_playList->Append(files.Item((tabIndex.at(i))));

    } else {
        wxMessageBox(wxString::FromUTF8("L'élément sélectionné n'est pas un dossier."), wxString::FromUTF8("Erreur"));
    }
    event.Skip();
}
/**------------------------------------------------------------------------------------------------*/
void MainFrame::OnAddRandomTitleAction(wxCommandEvent &event) {
    wxString itemPath = this->GetFullPath(this->m_fileTree->GetSelection());
    wxArrayString files;

    // Vérifier si c'est un répertoire ou un fichier
    if (wxFileName::DirExists(itemPath)) {

        wxDir::GetAllFiles(itemPath, &files, "*.mp3", wxDIR_FILES);
        /**
        // Affiche les fichiers trouvés
        for (const auto &file : files) {
            std::println("-> {}", std::string(file.ToUTF8()));
        }
        */

        int idSelectedFile = this->generateRandomNumber(files.GetCount() - 1);
        // std::println("Nombre de fichiers dans le dossier : {}", NbFiles);
        m_playList->Append(files.Item((idSelectedFile)));
    } else {
        wxMessageBox(wxString::FromUTF8("L'élément sélectionné n'est pas un dossier."), wxString::FromUTF8("Erreur"));
    }
    event.Skip();
}

/**------------------------------------------------------------------------------------------------*/
void MainFrame::PopulateFileTree(const wxString &path, wxTreeItemId parentId) {

    wxDir dir(path);
    wxString filename;

    if (dir.GetFirst(&filename)) {
        do {
            wxString fullPath = path + wxFILE_SEP_PATH + filename;
            wxTreeItemId itemId;

            if (wxDir::Exists(fullPath)) {
                itemId = m_fileTree->AppendItem(parentId, filename);
                PopulateFileTree(fullPath, itemId);
            } else {
                m_fileTree->AppendItem(parentId, filename);
            }
        } while (dir.GetNext(&filename));
    }
}
/**------------------------------------------------------------------------------------------------*/
void MainFrame::OnTreeItemActivated(wxTreeEvent &event) {
    wxTreeItemId itemId = event.GetItem();
    wxString path = GetFullPath(itemId);

    if (wxDir::Exists(path)) {
        if (m_fileTree->IsExpanded(itemId)) {
            m_fileTree->Collapse(itemId);
        } else {
            m_fileTree->DeleteChildren(itemId);
            PopulateFileTree(path, itemId);
            m_fileTree->Expand(itemId);
        }
    } else if (wxFileExists(path)) {
        m_playList->Append(path);
    }
    event.Skip(); /** Do not propagate event to parent object */
}
/**------------------------------------------------------------------------------------------------*/
void MainFrame::OnTreeSelectionChanged(wxTreeEvent &event) {
    wxTreeItemId itemId = event.GetItem();
    wxString path = GetFullPath(itemId);
    this->TagsChanged.clear();

    if (wxFileExists(path)) {
        UpdateGridWithFileInfo(path);
    } else {
        // Optionnel : effacer la grille si un dossier est sélectionné
        m_grid->ClearGrid();
        m_grid->ForceRefresh();
    }
    event.Skip(); /** Do not propagate event to parent object */
}
/**------------------------------------------------------------------------------------------------*/
void MainFrame::UpdateGridWithFileInfo(const wxString &filePath) {

    if (!this->TagsChanged.empty()) this->TagsChanged.clear();
    //   Effacer le contenu actuel de la grille
    m_grid->ClearGrid();
    m_grid->ClearSelection();
    if (m_grid->GetNumberRows() > 0) m_grid->DeleteRows(0, m_grid->GetNumberRows());
    m_grid->ForceRefresh();
    std::unique_ptr<tagManager> tagM;

    std::string path = std::string(filePath.ToUTF8());

    try {
        tagM = std::make_unique<tagManager>(path);
    } catch (const FileNotFoundException &e) {
        return;
    } catch (const FileErrorException &e) {
        return;
    } catch (const NoTagsInFileException &e) {
        return;
    }

    this->m_buttonPrepare->Enable(true);

    int currRow{0};
    std::string tagValue{};
    /** TITLE tag */
    if (tagM) {
        try {
            tagValue = this->removeSpecialCharacters(tagM->getTitre(false));
            m_grid->AppendRows(1);
            m_grid->SetRowLabelValue(currRow, wxString::FromUTF8(ROW_LABEL_VALUE_TITRE));
            m_grid->SetCellValue(currRow, 0, wxString::FromUTF8(tagValue));
            currRow++;

        } catch (const TagNotInTheFileException &e) {
        }
    }

    /** ARTIST tag */
    if (tagM) {
        try {
            tagValue = this->removeSpecialCharacters(tagM->getInterprete(false));
            m_grid->AppendRows(1);
            m_grid->SetRowLabelValue(currRow, wxString::FromUTF8(ROW_LABEL_VALUE_ARTIST));
            m_grid->SetCellValue(currRow, 0, wxString::FromUTF8(tagValue));
            currRow++;

        } catch (const TagNotInTheFileException &e) {
        }
    }

    /** DATE tag */
    if (tagM) {
        try {
            int Date = tagM->getDate(false);
            tagValue = std::to_string(Date);
            m_grid->AppendRows(1);
            m_grid->SetRowLabelValue(currRow, wxString::FromUTF8(ROW_LABEL_VALUE_DATE));
            m_grid->SetCellValue(currRow, 0, wxString::FromUTF8(tagValue));
            currRow++;

        } catch (const TagNotInTheFileException &e) {
        }
    }

    /** LANGUAGE tag */
    if (tagM) {
        try {
            tagManager::btLanguage Langue = tagM->getLangue(false);
            switch (Langue) {
            case tagManager::btLanguage::FRA:
                tagValue = "FRA";
                break;
            case tagManager::btLanguage::INT:
                tagValue = "INT";
                break;
            default:
                tagValue = "ERR";
                break;
            }
            m_grid->AppendRows(1);
            m_grid->SetRowLabelValue(currRow, wxString::FromUTF8(ROW_LABEL_VALUE_LANGUAGE));
            m_grid->SetCellValue(currRow, 0, wxString::FromUTF8(tagValue));
            currRow++;

        } catch (const TagNotInTheFileException &e) {
        }
    }

    /** EXTRA tag */
    if (tagM) {
        try {
            tagValue.clear();
            if (tagM->isMovieSoundTrack(false)) tagValue += EXTRA_TAG_MOVIE;
            if (tagM->isTvShow(false)) tagValue += EXTRA_TAG_TV;
            if (tagM->isMasterPiece(false)) tagValue += EXTRA_TAG_MASTERPIECE;
            if (tagM->isSbig(false)) tagValue += EXTRA_TAG_SOBADISGOOD;
            if (tagM->isDuet(false)) tagValue += EXTRA_TAG_DUET;
            if (tagM->isCover(false)) tagValue += EXTRA_TAG_COVER;
            if (tagM->isName(false)) tagValue += EXTRA_TAG_NAME;
            if (tagM->isCity(false)) tagValue += EXTRA_TAG_CITY;

            if (tagValue.empty()) tagValue = EXTRA_TAG_NONE;

            m_grid->AppendRows(1);
            m_grid->SetRowLabelValue(currRow, wxString::FromUTF8(ROW_LABEL_VALUE_EXTRA));
            m_grid->SetCellValue(currRow, 0, wxString::FromUTF8(tagValue));
            currRow++;

        } catch (const TagNotInTheFileException &e) {
        }
    }

    /** EXTRA TITLE tag */
    if (tagM) {
        try {
            tagValue = this->removeSpecialCharacters(tagM->getExtraTitle(false));
            m_grid->AppendRows(1);
            m_grid->SetRowLabelValue(currRow, wxString::FromUTF8(ROW_LABEL_VALUE_EXTRA_TITLE));
            m_grid->SetCellValue(currRow, 0, wxString::FromUTF8(tagValue));
            currRow++;

        } catch (const TagNotInTheFileException &e) {
        }
    }

    /** EXTRA ARTIST tag */
    if (tagM) {
        try {
            tagValue = this->removeSpecialCharacters(tagM->getExtraArtist(false));
            m_grid->AppendRows(1);
            m_grid->SetRowLabelValue(currRow, wxString::FromUTF8(ROW_LABEL_VALUE_EXTRA_ARTIST));
            m_grid->SetCellValue(currRow, 0, wxString::FromUTF8(tagValue));
            currRow++;

        } catch (const TagNotInTheFileException &e) {
        }
    }

    /** EXTRA DATE tag */
    if (tagM) {
        try {
            int Date = tagM->getExtraDate(false);
            tagValue = std::to_string(Date);
            m_grid->AppendRows(1);
            m_grid->SetRowLabelValue(currRow, wxString::FromUTF8(ROW_LABEL_VALUE_EXTRA_DATE));
            m_grid->SetCellValue(currRow, 0, wxString::FromUTF8(tagValue));
            currRow++;

        } catch (const TagNotInTheFileException &e) {
        }
    }
    // Redimensionner les colonnes pour s'adapter au contenu
    m_grid->AutoSizeColumns();

    // Forcer le rafraîchissement de la grille
    m_grid->ForceRefresh();
}
/**------------------------------------------------------------------------------------------------*/
void MainFrame::OnGridCellChanged(wxGridEvent &event) {
    int row = event.GetRow();
    std::string Tag{m_grid->GetRowLabelValue(row).utf8_str()};
    std::string NewValue{m_grid->GetCellValue(row, 0).utf8_str()};
    this->TagsChanged.push_back(std::make_pair(Tag, NewValue));
    // std::println(std::clog, "Tag = {} - New Value = {}", Tag, NewValue);
    m_buttonUpdate->Enable(true);
    event.Skip(); /** Do not propagate event to parent object */
}
/**------------------------------------------------------------------------------------------------*/
void MainFrame::OnUpdate(wxCommandEvent &event) {

    this->UpdateTags();
    m_buttonUpdate->Enable(false);   // Désactiver le bouton
    event.Skip();                    /** Do not propagate event to parent object */
}
/**------------------------------------------------------------------------------------------------*/
void MainFrame::OnPrepare(wxCommandEvent &event) {

    this->PrepareTags();
    m_buttonPrepare->Enable(false);   // Désactiver le bouton
    event.Skip();                     /** Do not propagate event to parent object */
}
/**------------------------------------------------------------------------------------------------*/
void MainFrame::OnQuit(wxCommandEvent &event) {
    UNUSED(event);
    Close(true);
};

/**------------------------------------------------------------------------------------------------*/
bool MainFrame::PrepareTags() {
    wxTreeItemId itemId = m_fileTree->GetFocusedItem();
    wxString tree_path = GetFullPath(itemId);

    std::unique_ptr<tagManager> tagM;

    std::string path = std::string(tree_path.ToUTF8());

    // std::println(std::clog, "File to update : {}", path);

    try {
        tagM = std::make_unique<tagManager>(path);
    } catch (const FileNotFoundException &e) {
        return false;
    } catch (const FileErrorException &e) {
        return false;
    } catch (const NoTagsInFileException &e) {
        return false;
    }

    if (tagM) {
        tagM->prepareFile(true);
        this->UpdateGridWithFileInfo(tree_path);
        return true;
    }
    return false;
}
/**------------------------------------------------------------------------------------------------*/
bool MainFrame::UpdateTags() {
    if (this->TagsChanged.empty()) return false;

    wxTreeItemId itemId = m_fileTree->GetFocusedItem();
    wxString tree_path = GetFullPath(itemId);

    std::unique_ptr<tagManager> tagM;

    std::string path = std::string(tree_path.ToUTF8());

    // std::println(std::clog, "File to update : {}", path);

    try {
        tagM = std::make_unique<tagManager>(path);
    } catch (const FileNotFoundException &e) {
        return false;
    } catch (const FileErrorException &e) {
        return false;
    } catch (const NoTagsInFileException &e) {
        return false;
    }

    if (!tagM) return false;

    for (auto item : this->TagsChanged) {
        // std::println(std::clog, "Tag = {} - New Value = {}", item.first, item.second);
        if (item.first == ROW_LABEL_VALUE_TITRE)
            tagM->setTitre(item.second);
        else if (item.first == ROW_LABEL_VALUE_ARTIST)
            tagM->setInterprete(item.second);
        else if (item.first == ROW_LABEL_VALUE_EXTRA_TITLE)
            tagM->setExtraTitle(item.second);
        else if (item.first == ROW_LABEL_VALUE_EXTRA_ARTIST)
            tagM->setExtraArtist(item.second);
        else if (item.first == ROW_LABEL_VALUE_DATE) {
            int date = this->StringToInt(item.second);
            tagM->setDate(date);
        } else if (item.first == ROW_LABEL_VALUE_EXTRA_DATE) {
            int date = this->StringToInt(item.second);
            tagM->setExtraDate(date);
        } else if (item.first == ROW_LABEL_VALUE_EXTRA) {
            std::string str = item.second;
            tagM->setMovieSoundTrackFlag(str.contains(EXTRA_TAG_MOVIE));
            tagM->setTvShowFlag(str.contains(EXTRA_TAG_TV));
            tagM->setMasterPieceFlag(str.contains(EXTRA_TAG_MASTERPIECE));
            tagM->setSbigFlag(str.contains(EXTRA_TAG_SOBADISGOOD));
            tagM->setDuetFlag(str.contains(EXTRA_TAG_DUET));
            tagM->setCoverFlag(str.contains(EXTRA_TAG_COVER));
            tagM->setNameFlag(str.contains(EXTRA_TAG_NAME));
            tagM->setCityFlag(str.contains(EXTRA_TAG_CITY));
        } else if (item.first == ROW_LABEL_VALUE_LANGUAGE) {
            std::string str = item.second;
            if (str == "FRA")
                tagM->setLangue(tagManager::btLanguage::FRA);
            else if (str == "INT")
                tagM->setLangue(tagManager::btLanguage::INT);
        }
    }
    if (tagM->update()) this->UpdateGridWithFileInfo(tree_path);

    return true;
}

/**------------------------------------------------------------------------------------------------*/
wxString MainFrame::GetFullPath(wxTreeItemId itemId) {
    wxString path = m_fileTree->GetItemText(itemId);
    wxTreeItemId parent = m_fileTree->GetItemParent(itemId);

    while (parent.IsOk() && parent != m_fileTree->GetRootItem()) {
        path = m_fileTree->GetItemText(parent) + wxFILE_SEP_PATH + path;
        parent = m_fileTree->GetItemParent(parent);
    }

    if (parent == m_fileTree->GetRootItem()) {
        path = m_fileTree->GetItemText(parent) + wxFILE_SEP_PATH + path;
    }

    return path;
}
/**------------------------------------------------------------------------------------------------*/
std::string MainFrame::removeSpecialCharacters(const std::string &input) {
    std::string result = input;
    std::replace_if(result.begin(), result.end(), [](unsigned char c) { return c > 127; }, '_');
    return result;
}
/**------------------------------------------------------------------------------------------------*/
int MainFrame::StringToInt(const std::string &str) {
    int result = 0;
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);

    if (ec == std::errc() && ptr == str.data() + str.size()) {
        return result;
    } else {
        return BAD_YEAR_VALUE;
    }
}
/**------------------------------------------------------------------------------------------------*/
int MainFrame::generateRandomNumber(int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, max);
    return distrib(gen);
}
/**------------------------------------------------------------------------------------------------*/