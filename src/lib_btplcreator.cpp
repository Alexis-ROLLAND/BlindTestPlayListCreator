#include "lib_btplcreator.hpp"


bool    isdDirectoryValid(const std::string& filename){
    return ( std::filesystem::exists(filename) && std::filesystem::is_directory(filename) );
}

std::size_t    lsDir(const std::string& filename){
    std::size_t compteur = 0;
    // Itération à travers les éléments du répertoire
    for (const auto& entry : std::filesystem::directory_iterator(filename)){
        if (std::filesystem::is_regular_file(entry)) { // Vérifie si c'est un fichier
            ++compteur;
            std::cout << "Fichier : " << entry.path().filename() << std::endl;
        }
    }

    return compteur;

}


std::vector<std::string> makeFileList(const std::string& filename){
    std::vector<std::string> FileList{};
    for (const auto& entry : std::filesystem::directory_iterator(filename)){
        if (std::filesystem::is_regular_file(entry)) { // Vérifie si c'est un fichier
            FileList.push_back(entry.path().c_str());
        }
    }

    return FileList;
}

void    makePlayList(const std::vector<std::string> fileList,Playlist &pl){
    std::random_device rd{}; // Générateur de nombre aléatoire basé sur le matériel
    std::mt19937 gen{rd()}; // Générateur de nombres pseudo-aléatoires
    
    std::vector<int> indices(fileList.size());
    std::iota(indices.begin(), indices.end(), 0); // Génère les indices 0, 1, 2, ..., vec.size() - 1

    std::shuffle(indices.begin(), indices.end(), gen); // Mélange les indices

    for (int i=0;i<PLAYLIST_SIZE;++i){
        println("Choosen file = {0:}", fileList.at(indices.at(i)) );
        pl.add(fileList.at(indices.at(i)));
    }

    

}