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

    m_splitter = new wxSplitterWindow(this, wxID_ANY); /** Creates two subwindows */

    // Panneau gauche
    wxPanel *leftPanel = new wxPanel(m_splitter, wxID_ANY);
    wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);
    m_fileTree = new wxTreeCtrl(leftPanel, wxID_ANY); /** Creates a Tree Control (wxWindow specialization) */
    leftSizer->Add(m_fileTree, 1, wxEXPAND | wxALL, 5);
    m_grid = new wxGrid(leftPanel, wxID_ANY);
    m_grid->CreateGrid(0, 2);
    m_grid->SetColLabelValue(0, "Tag");
    m_grid->SetColLabelValue(1, "Value");
    leftSizer->Add(m_grid, 1, wxEXPAND | wxALL, 5);

    leftPanel->SetSizer(leftSizer);

    // Panneau pour la liste à droite
    wxPanel *rightPanel =
        new wxPanel(m_splitter, wxID_ANY); /** Creates a Panel (wxWindow specialization, graphical container) */
    wxBoxSizer *rightSizer =
        new wxBoxSizer(wxVERTICAL); /** Creates a sizer (boxSizer), not a wxWindow specialization */

    // Créer la liste des fichiers sélectionnés
    m_playList = new wxListBox(rightPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    rightSizer->Add(m_playList, 1, wxEXPAND | wxALL, 10);   // La liste occupe tout l'espace restant

    // Ajouter les boutons
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    m_buttonUp = new wxButton(rightPanel, wxID_ANY, "Monter");
    m_buttonDown = new wxButton(rightPanel, wxID_ANY, "Descendre");
    m_buttonDelete = new wxButton(rightPanel, wxID_ANY, "Supprimer");
    m_buttonGenerate = new wxButton(rightPanel, wxID_ANY, "Generer");

    buttonSizer->Add(m_buttonUp, 0, wxALL, 5);
    buttonSizer->Add(m_buttonDown, 0, wxALL, 5);
    buttonSizer->Add(m_buttonDelete, 0, wxALL, 5);
    buttonSizer->Add(m_buttonGenerate, 0, wxALL, 5);

    rightSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);

    // Appliquer le sizer au panneau
    rightPanel->SetSizer(rightSizer);
    rightPanel->Layout();   // Forcer la mise en page du panneau

    // Configurer le splitter pour diviser la fenêtre en deux parties
    m_splitter->SplitVertically(leftPanel, rightPanel);
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

void MainFrame::OnGenerate(wxCommandEvent &event) { UNUSED(event); }

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
