#include "Playlist.hpp"


void Playlist::dump() noexcept{
    for (auto elt : this->FileList){
        std::println("# {0:}",elt);
    }

}


m3uPlaylist::m3uPlaylist(const std::string &Name,const std::string& path):Playlist(Name),path{path}{
    if (!(std::filesystem::exists(this->path) && std::filesystem::is_directory(this->path))){
        throw std::filesystem::filesystem_error("Destination Directory invalid",std::error_code{std::make_error_code(std::errc::no_such_file_or_directory)});;
    }
    this->makeFileName();

}



void m3uPlaylist::generate(){
    // Ouvrir un fichier en écriture
    std::ofstream fichier(this->getFileName());
    if (!fichier) {
        throw std::ios::failure("Impossible de créer le fichier : " + this->getFileName());
    }

    std::println(fichier,"#EXTM3U");

    for (auto elt : this->FileList){
        std::println(fichier, "{0:}",elt);
    }

    fichier.close();
}


