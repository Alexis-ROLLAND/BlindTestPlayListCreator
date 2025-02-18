#include "mainframe.hpp"
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/stattext.h>

MainFrame::MainFrame(const wxString &title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)) {
    m_splitter = new wxSplitterWindow(this, wxID_ANY);

    // Arborescence des fichiers à gauche
    m_fileTree = new wxTreeCtrl(m_splitter, wxID_ANY);

    // Panneau pour la liste à droite
    wxPanel *rightPanel = new wxPanel(m_splitter, wxID_ANY);
    wxBoxSizer *rightSizer = new wxBoxSizer(wxVERTICAL);

    // Ajouter un label au-dessus de la liste
    wxStaticText *listLabel = new wxStaticText(rightPanel, wxID_ANY, "Fichiers Sélectionnés");
    rightSizer->Add(listLabel, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);   // Marges et centrage horizontal

    // Créer la liste des fichiers sélectionnés
    m_fileList = new wxListBox(rightPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    rightSizer->Add(m_fileList, 1, wxEXPAND | wxALL, 10);   // La liste occupe tout l'espace restant

    // Ajouter les boutons
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    m_buttonUp = new wxButton(rightPanel, wxID_ANY, "Monter");
    m_buttonDown = new wxButton(rightPanel, wxID_ANY, "Descendre");
    m_buttonDelete = new wxButton(rightPanel, wxID_ANY, "Supprimer");

    buttonSizer->Add(m_buttonUp, 0, wxALL, 5);
    buttonSizer->Add(m_buttonDown, 0, wxALL, 5);
    buttonSizer->Add(m_buttonDelete, 0, wxALL, 5);

    rightSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);

    // Appliquer le sizer au panneau
    rightPanel->SetSizer(rightSizer);
    rightPanel->Layout();   // Forcer la mise en page du panneau

    // Configurer le splitter pour diviser la fenêtre en deux parties
    m_splitter->SplitVertically(m_fileTree, rightPanel);
    m_splitter->SetMinimumPaneSize(200);

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
}

void MainFrame::OnMoveUp(wxCommandEvent &event) {
    UNUSED(event);

    int sel = m_fileList->GetSelection();
    if (sel > 0) {
        wxString item = m_fileList->GetString(sel);
        m_fileList->Delete(sel);
        m_fileList->Insert(item, sel - 1);
        m_fileList->SetSelection(sel - 1);
    }
}

void MainFrame::OnMoveDown(wxCommandEvent &event) {
    UNUSED(event);

    int sel = m_fileList->GetSelection();
    if (sel != wxNOT_FOUND && sel < m_fileList->GetCount() - 1) {
        wxString item = m_fileList->GetString(sel);
        m_fileList->Delete(sel);
        m_fileList->Insert(item, sel + 1);
        m_fileList->SetSelection(sel + 1);
    }
}

void MainFrame::OnDelete(wxCommandEvent &event) {
    UNUSED(event);

    int sel = m_fileList->GetSelection();
    if (sel != wxNOT_FOUND) {
        m_fileList->Delete(sel);
        if (sel < m_fileList->GetCount())
            m_fileList->SetSelection(sel);
        else if (m_fileList->GetCount() > 0)
            m_fileList->SetSelection(m_fileList->GetCount() - 1);
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
        m_fileList->Append(path);
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
