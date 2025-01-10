#include <print>


#include "lib_btplcreator.hpp"



int main(int argc, const char *argv[]){
    cxxopts::Options options("btplcreator", "Automated Blind Test Playlist Creator");

    options.add_options()
        ("source_dir", "The source directory", cxxopts::value<std::string>())
        ("dest_dir", "The destination directory", cxxopts::value<std::string>())
        ("playlist_name", "The Playlist's name", cxxopts::value<std::string>());

    options.parse_positional({"source_dir", "dest_dir", "playlist_name"});

    auto result = options.parse(argc, argv);

    std::string     sourceDir = result["source_dir"].as<std::string>();
    std::string     destDir = result["dest_dir"].as<std::string>();
    std::string     plName = result["playlist_name"].as<std::string>();

    std::println("Source directory : {0:}",sourceDir);
    std::println("Destination directory : {0:}",destDir);
    std::println("Playlist name : {0:}",plName);

    if (! isdDirectoryValid(sourceDir)) {
        std::println("Error : {0:} is not a valid directory",sourceDir);
        return EXIT_FAILURE;
    }

    std::size_t nbfiles;
    nbfiles = lsDir(sourceDir);
    std::println("Number of files is source dir = {0:}",nbfiles);

    std::vector<std::string>    FileList{};

    FileList = makeFileList(sourceDir);

    std::println("FileList size = {0:}",FileList.size());

    for(auto path : FileList){
        println("File path = {0:}",path);
    }

    

    m3uPlaylist myPlayList{plName,destDir};
    std::println("PlayList Full Filename = {0:}",myPlayList.getFileName());

    makePlayList(FileList,myPlayList);

    myPlayList.dump();

    myPlayList.generate();

    return EXIT_SUCCESS;
}


