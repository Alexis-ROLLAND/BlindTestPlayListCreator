#ifndef __RESSOURCES_HPP__
#define __RESSOURCES_HPP__

#include <string>

#ifndef UNUSED
#define UNUSED(x) static_cast<void>(x)
#endif

constexpr double SCREEN_FACTOR{3.0 / 4.0}; /** MainFrame will occupy 3/4 of the screen surface */

const std::string COL_LABEL_TAG{"Tag"};
const std::string COL_LABEL_VALUE{"Value"};

const std::string ROW_LABEL_VALUE_TITRE{"Titre"};
const std::string ROW_LABEL_VALUE_ARTIST{"Interprète"};
const std::string ROW_LABEL_VALUE_DATE{"Date"};
const std::string ROW_LABEL_VALUE_LANGUAGE{"Langue"};
const std::string ROW_LABEL_VALUE_EXTRA{"Extra"};
const std::string ROW_LABEL_VALUE_EXTRA_TITLE{"Extra Titre"};
const std::string ROW_LABEL_VALUE_EXTRA_ARTIST{"Extra Artist"};
const std::string ROW_LABEL_VALUE_EXTRA_DATE{"Extra Date"};

const std::string LIST_LABEL_FILELIST{"Fichiers sélectionnés"};

const std::string BUTTON_LABEL_UPDATE{"Update"};
const std::string BUTTON_LABEL_PREPARE{"Prepare File"};
const std::string BUTTON_LABEL_MONTER{"Monter"};
const std::string BUTTON_LABEL_DESCENDRE{"Descendre"};
const std::string BUTTON_LABEL_SUPPRIMER{"Supprimer"};
const std::string BUTTON_LABEL_GENERER{"Générer"};

const int BAD_YEAR_VALUE{9999};

const std::string EXTRA_TAG_MOVIE{"Mv"};
const std::string EXTRA_TAG_TV{"Tv"};
const std::string EXTRA_TAG_MASTERPIECE{"Mp"};
const std::string EXTRA_TAG_SOBADISGOOD{"Sb"};
const std::string EXTRA_TAG_DUET{"Dt"};
const std::string EXTRA_TAG_COVER{"Cv"};
const std::string EXTRA_TAG_NAME{"Na"};
const std::string EXTRA_TAG_CITY{"Cy"};
const std::string EXTRA_TAG_NONE{"None"};

#endif /* __RESSOURCES_HPP__ */
