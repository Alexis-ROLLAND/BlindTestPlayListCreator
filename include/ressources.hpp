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

#endif /* __RESSOURCES_HPP__ */
