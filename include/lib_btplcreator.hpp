#ifndef __LIB_BT_PLCREATOR_HPP__
#define __LIB_BT_PLCREATOR_HPP__

#include <filesystem>
#include <print>
#include <string>
#include <iostream>
#include <vector>
#include <random>       /**< Inclusion des outils liés au moteur de génération de valeurs aléatoires */
#include <fstream>

#include "cxxopts.hpp"   

#include "Playlist.hpp"

const   uint8_t PLAYLIST_SIZE{15};    

bool    isdDirectoryValid(const std::string& filename);

std::size_t    lsDir(const std::string& filename);

std::vector<std::string> makeFileList(const std::string& filename);

void    makePlayList(const std::vector<std::string> fileList, Playlist &pl);

#endif  /*  __LIB_BT_PLCREATOR_HPP__ */


