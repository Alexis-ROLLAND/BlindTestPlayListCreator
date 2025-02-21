#include "mainframe.hpp"
#include <wx/dir.h>
#include <wx/display.h>
#include <wx/filename.h>
#include <wx/stattext.h>

MainFrame::MainFrame(const wxString &title) : wxFrame(NULL, wxID_ANY, title) {

    // Obtenir la taille de l'écran principal
    wxDisplay display{};
    wxRect screenRect = display.GetClientArea();

    // Calculer les 3/4 de la largeur et de la hauteur
    int width = (screenRect.GetWidth() * 3) / 4;
    int height = (screenRect.GetHeight() * 3) / 4;

    // Définir la taille de la fenêtre
    SetSize(width, height);

    // Centrer la fenêtre sur l'écran
    Centre();

    m_splitter = std::make_unique<wxSplitterWindow>(this, wxID_ANY); /** Creates two subwindows */

    // Panneau gauche
    leftPanel = std::make_unique<wxPanel>(m_splitter.get(), wxID_ANY);
    leftSizer = std::make_unique<wxBoxSizer>(wxVERTICAL);

    m_fileTree = std::make_unique<wxTreeCtrl>(leftPanel.get(), wxID_ANY); /** Creates a Tree Control (wxWindow specialization) */
    leftSizer->Add(m_fileTree.get(), 1, wxEXPAND | wxALL, 5);
    m_grid = std::make_unique<wxGrid>(leftPanel.get(), wxID_ANY);
    m_grid->CreateGrid(0, 2);
    m_grid->HideRowLabels();
    m_grid->SetColLabelValue(0, "Tag");
    m_grid->SetColLabelValue(1, "Value");
    wxGridCellAttr *attr = new wxGridCellAttr();
    attr->SetReadOnly();
    m_grid->SetColAttr(0, attr);

    leftSizer->Add(m_grid.get(), 1, wxEXPAND | wxALL, 5);

    leftPanel->SetSizer(leftSizer.get());

    // Panneau pour la liste à droite
    rightPanel = std::make_unique<wxPanel>(m_splitter.get(), wxID_ANY); /** Creates a Panel (wxWindow specialization, graphical container) */
    rightSizer = std::make_unique<wxBoxSizer>(wxVERTICAL);              /** Creates a sizer (boxSizer), not a wxWindow specialization */

    // Créer la liste des fichiers sélectionnés
    m_playList = std::make_unique<wxListBox>(rightPanel.get(), wxID_ANY, wxDefaultPosition, wxDefaultSize);
    rightSizer->Add(m_playList.get(), 1, wxEXPAND | wxALL, 10);   // La liste occupe tout l'espace restant

    // Ajouter les boutons
    buttonSizer = std::make_unique<wxBoxSizer>(wxHORIZONTAL);
    m_buttonUp = std::make_unique<wxButton>(rightPanel.get(), wxID_ANY, "Monter");
    m_buttonDown = std::make_unique<wxButton>(rightPanel.get(), wxID_ANY, "Descendre");
    m_buttonDelete = std::make_unique<wxButton>(rightPanel.get(), wxID_ANY, "Supprimer");
    m_buttonGenerate = std::make_unique<wxButton>(rightPanel.get(), wxID_ANY, "Generer");

    buttonSizer->Add(m_buttonUp.get(), 0, wxALL, 5);
    buttonSizer->Add(m_buttonDown.get(), 0, wxALL, 5);
    buttonSizer->Add(m_buttonDelete.get(), 0, wxALL, 5);
    buttonSizer->Add(m_buttonGenerate.get(), 0, wxALL, 5);

    rightSizer->Add(buttonSizer.get(), 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);

    // Appliquer le sizer au panneau
    rightPanel->SetSizer(rightSizer.get());
    rightPanel->Layout();   // Forcer la mise en page du panneau

    // Configurer le splitter pour diviser la fenêtre en deux parties
    m_splitter->SplitVertically(leftPanel.get(), rightPanel.get());
    m_splitter->SetMinimumPaneSize(width / 4);
    // Définir la position initiale du séparateur à 1/4 de la largeur
    m_splitter->SetSashPosition(width / 4);

    // Obtenir le répertoire home
    wxString homeDir = wxGetHomeDir();
    wxString rootDir = homeDir + wxFILE_SEP_PATH + "Musique";   // Sous-répertoire spécifique

    // Vérifier si le répertoire existe
    if (!wxDir::Exists(rootDir)) {
        wxMessageBox("Le répertoire spécifié n'existe pas : " + rootDir, "Erreur", wxOK | wxICON_ERROR);
        Close();   // Ferme la fenêtre principale
        return;
    }

    // Ajouter le répertoire racine à l'arborescence
    wxTreeItemId rootId = m_fileTree->AddRoot(rootDir);
    PopulateFileTree(rootDir, rootId);
    m_fileTree->Expand(rootId);

    m_fileTree->Bind(wxEVT_TREE_ITEM_ACTIVATED, &MainFrame::OnTreeItemActivated, this);
    m_fileTree->Bind(wxEVT_TREE_SEL_CHANGED, &MainFrame::OnTreeSelectionChanged, this);

    // Lier les événements des boutons
    m_buttonUp->Bind(wxEVT_BUTTON, &MainFrame::OnMoveUp, this);
    m_buttonDown->Bind(wxEVT_BUTTON, &MainFrame::OnMoveDown, this);
    m_buttonDelete->Bind(wxEVT_BUTTON, &MainFrame::OnDelete, this);
    m_buttonGenerate->Bind(wxEVT_BUTTON, &MainFrame::OnGenerate, this);
}

void MainFrame::OnMoveUp(wxCommandEvent &event) {
    UNUSED(event);

    int sel = m_playList->GetSelection();
    if (sel > 0) {
        wxString item = m_playList->GetString(sel);
        m_playList->Delete(sel);
        m_playList->Insert(item, sel - 1);
        m_playList->SetSelection(sel - 1);
    }
}

void MainFrame::OnMoveDown(wxCommandEvent &event) {
    UNUSED(event);

    int sel = m_playList->GetSelection();
    if (sel != wxNOT_FOUND && sel < m_playList->GetCount() - 1) {
        wxString item = m_playList->GetString(sel);
        m_playList->Delete(sel);
        m_playList->Insert(item, sel + 1);
        m_playList->SetSelection(sel + 1);
    }
}

void MainFrame::OnDelete(wxCommandEvent &event) {
    UNUSED(event);

    int sel = m_playList->GetSelection();
    if (sel != wxNOT_FOUND) {
        m_playList->Delete(sel);
        if (sel < m_playList->GetCount())
            m_playList->SetSelection(sel);
        else if (m_playList->GetCount() > 0)
            m_playList->SetSelection(m_playList->GetCount() - 1);
    }
}

void MainFrame::OnGenerate(wxCommandEvent &event) {
    UNUSED(event);

    int NbTitres = m_playList->GetCount();
    if (NbTitres < 1) return;

    wxFileDialog saveFileDialog(this, "Enregistrer le fichier", "", "", "Fichiers m3u (*.m3u)|*.m3u", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_OK) {
        wxString wxfileDir = saveFileDialog.GetDirectory();
        wxString wxfileName = saveFileDialog.GetFilename();

        std::string fileDir = std::string(wxfileDir.ToUTF8()) + "/";
        std::string fileName = std::string(wxfileName.ToUTF8());

        m3uPlaylist pl{fileName, fileDir};

        std::println(std::clog, "File Name = {}", pl.getFileName());

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
}

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
}

void MainFrame::OnTreeSelectionChanged(wxTreeEvent &event) {
    wxTreeItemId itemId = event.GetItem();
    wxString path = GetFullPath(itemId);

    if (wxFileExists(path)) {
        UpdateGridWithFileInfo(path);
    } else {
        // Optionnel : effacer la grille si un dossier est sélectionné
        m_grid->ClearGrid();
        m_grid->ForceRefresh();
    }
}

void MainFrame::UpdateGridWithFileInfo(const wxString &filePath) {

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

    int currRow{0};
    std::string tagValue{};
    /** TITLE tag */
    if (tagM) {
        try {
            tagValue = this->removeSpecialCharacters(tagM->getTitre(false));
            m_grid->AppendRows(1);
            m_grid->SetCellValue(currRow, 0, "Titre");

            m_grid->SetCellValue(currRow, 1, wxString::FromUTF8(tagValue));
            currRow++;

        } catch (const TagNotInTheFileException &e) {
        }
    }

    /** ARTIST tag */
    if (tagM) {
        try {
            tagValue = this->removeSpecialCharacters(tagM->getInterprete(false));
            m_grid->AppendRows(1);
            m_grid->SetCellValue(currRow, 0, wxT("Interprète"));
            m_grid->SetCellValue(currRow, 1, wxString::FromUTF8(tagValue));
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
            m_grid->SetCellValue(currRow, 0, wxT("Date"));
            m_grid->SetCellValue(currRow, 1, wxString::FromUTF8(tagValue));
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
            m_grid->SetCellValue(currRow, 0, wxT("Langue"));
            m_grid->SetCellValue(currRow, 1, wxString::FromUTF8(tagValue));
            currRow++;

        } catch (const TagNotInTheFileException &e) {
        }
    }

    /** EXTRA tag */
    if (tagM) {
        try {
            tagValue.clear();
            if (tagM->isMovieSoundTrack(false)) tagValue += "Mv";
            if (tagM->isTvShow(false)) tagValue += "Tv";
            if (tagM->isMasterPiece(false)) tagValue += "Mp";
            if (tagM->isSbig(false)) tagValue += "Sb";
            if (tagM->isDuet(false)) tagValue += "Dt";
            if (tagM->isCover(false)) tagValue += "Cv";
            if (tagM->isName(false)) tagValue += "Na";
            if (tagM->isCity(false)) tagValue += "Ci";

            if (tagValue.empty()) tagValue = "None";

            m_grid->AppendRows(1);
            m_grid->SetCellValue(currRow, 0, wxT("Extra"));
            m_grid->SetCellValue(currRow, 1, wxString::FromUTF8(tagValue));
            currRow++;

        } catch (const TagNotInTheFileException &e) {
        }
    }

    /** EXTRA TITLE tag */
    if (tagM) {
        try {
            tagValue = this->removeSpecialCharacters(tagM->getExtraTitle(false));
            m_grid->AppendRows(1);
            m_grid->SetCellValue(currRow, 0, "Extra Titre");

            m_grid->SetCellValue(currRow, 1, wxString::FromUTF8(tagValue));
            currRow++;

        } catch (const TagNotInTheFileException &e) {
        }
    }

    /** EXTRA ARTIST tag */
    if (tagM) {
        try {
            tagValue = this->removeSpecialCharacters(tagM->getExtraArtist(false));
            m_grid->AppendRows(1);
            m_grid->SetCellValue(currRow, 0, wxT("Extra Artist"));
            m_grid->SetCellValue(currRow, 1, wxString::FromUTF8(tagValue));
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
            m_grid->SetCellValue(currRow, 0, wxT("Extra Date"));
            m_grid->SetCellValue(currRow, 1, wxString::FromUTF8(tagValue));
            currRow++;

        } catch (const TagNotInTheFileException &e) {
        }
    }
    // Redimensionner les colonnes pour s'adapter au contenu
    m_grid->AutoSizeColumns();

    // Forcer le rafraîchissement de la grille
    m_grid->ForceRefresh();
}

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

std::string MainFrame::removeSpecialCharacters(const std::string &input) {
    std::string result = input;
    std::replace_if(result.begin(), result.end(), [](unsigned char c) { return c > 127; }, '_');
    return result;
}
