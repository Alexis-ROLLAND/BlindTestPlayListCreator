#include "tagManager.hpp"

tagManager::tagManager(const std::string &_FileName):FileName{_FileName}{
    if (std::filesystem::exists(this->getFileName()) == false) throw FileNotFoundException{};
    
    this->file = TagLib::FileRef(this->getFileName().c_str(),false);
    if (this->getFile().isNull()) throw FileErrorException{};

    if (this->getFile().tag() == nullptr) throw NoTagsInFileException{};

    this->tags = this->getFile().properties();
    if (this->tags.isEmpty()) throw NoTagsInFileException{};
}



void    tagManager::dump() noexcept{
    std::string status{};
    for(auto tag : this->tags){
        if (this->isbtTag(tag.first.to8Bit())==true) status = "[BTtag]";
        else status="";
        std::println("{} = {} {}",tag.first.to8Bit(),tag.second.toString().to8Bit(),status);
    }
}

bool tagManager::isTagExisting(const std::string &Tag) noexcept {
    if (this->tags.find(Tag) == this->tags.end()) return false;
    else return true;
}

void    tagManager::deleteTag(const std::string &tag){
    if (this->isTagExisting(tag)){
        this->tags.erase(tag);
    }
    this->settagsHaveChanged(true);
}

std::string tagManager::getTitre(bool fc) {
    if (this->isTagExisting(tagTitre)) return this->tags.value(tagTitre).toString().to8Bit();
    
    if (fc){
        this->setTitre(DEFAULT_STRING_VALUE);
        return DEFAULT_STRING_VALUE;
    }
    else{
        throw TagNotInTheFileException(tagTitre);
    }

}

std::string tagManager::getExtraTitle(bool fc){
    if (this->isTagExisting(tagExtraTitle)) return this->tags.value(tagExtraTitle).toString().to8Bit();
    
    if (fc){
        this->setExtraTitle(DEFAULT_STRING_VALUE);
        return DEFAULT_STRING_VALUE;
    }
    else{
        throw TagNotInTheFileException(tagExtraTitle);
    }  
}

std::string tagManager::getInterprete(bool fc){
    if (this->isTagExisting(tagInterprete)) return this->tags.value(tagInterprete).toString().to8Bit();
    
    if (fc){
        this->setInterprete(DEFAULT_STRING_VALUE);
        return DEFAULT_STRING_VALUE;
    }
    else{
        throw TagNotInTheFileException(tagInterprete);
    }     
}

std::string tagManager::getExtraArtist(bool fc){
    if (this->isTagExisting(tagExtraArtist)) return this->tags.value(tagExtraArtist).toString().to8Bit();
    
    if (fc){
        this->setExtraArtist(DEFAULT_STRING_VALUE);
        return DEFAULT_STRING_VALUE;
    }
    else{
        throw TagNotInTheFileException(tagExtraArtist);
    }     
}


unsigned int    tagManager::getDate(bool fc) {
    if (this->isTagExisting(tagDate)) return std::stoi(this->tags.value(tagDate).toString().to8Bit());
    
    if (fc) {   
        this->setDate(INVALID_DATE_VALUE);    /** Set to "invalide value" */
        return INVALID_DATE_VALUE;
    }
    else{
        throw TagNotInTheFileException(tagDate); 
    }
}

unsigned int    tagManager::getExtraDate(bool fc) {
    if (this->isTagExisting(tagExtraDate)) return std::stoi(this->tags.value(tagExtraDate).toString().to8Bit());
    
    if (fc) {   
        this->setExtraDate(INVALID_DATE_VALUE);    /** Set to "invalide value" */
        return INVALID_DATE_VALUE;
    }
    else{
        throw TagNotInTheFileException(tagExtraDate); 
    }
}

tagManager::btPeriod    tagManager::toPeriod(unsigned int year) const noexcept{
    if (year == INVALID_DATE_VALUE) return btPeriod::INVALID;
    else if (year < 1970) return btPeriod::OLDIES;
    else if (year < 1980) return btPeriod::SEVENTIES; 
    else if (year < 1990) return btPeriod::EIGHTIES;
    else if (year < 2000) return btPeriod::NINETIES;
    else if (year < 2010) return btPeriod::Y2K0X;
    else if (year < 2020) return btPeriod::Y2K1X;
    else return btPeriod::Y2K2X;
}

tagManager::btLanguage      tagManager::getLangue(bool fc){
    if (this->isTagExisting(tagLangue)) {
        std::string strLanguage = this->tags.value(tagLangue).toString().to8Bit();    
        if (strLanguage == "FRA") return tagManager::btLanguage::FRA;
        else if (strLanguage == "INT") return tagManager::btLanguage::INT;
        else throw BadTagValueException{}; 
    }
    
    if (fc) {   
        this->setLangue(tagManager::btLanguage::FRA);    /** Set to "FRA" by default */
        return tagManager::btLanguage::FRA;
    }
    else{
        throw TagNotInTheFileException(tagLangue); 
    }

}

void    tagManager::setLangue(tagManager::btLanguage Langue){
    TagLib::String Value;
    switch (Langue){
        case btLanguage::FRA : Value = "FRA";break;
        case btLanguage::INT : Value = "INT";break;
        default : Value = "ERR";break;
    }
    
    if (this->isTagExisting(tagLangue) == false) this->tags.insert(tagLangue,Value);   
    else this->tags.replace(tagLangue,Value);

    this->settagsHaveChanged(true);
}

void    tagManager::setDate(unsigned int year){
    TagLib::String Value = std::to_string(year);
    if (this->isTagExisting(tagDate) == false) this->tags.insert(tagDate,Value);   
    else this->tags.replace(tagDate,Value); 
    this->settagsHaveChanged(true);
} 

void    tagManager::setExtraDate(unsigned int year){
    TagLib::String Value = std::to_string(year);
    if (this->isTagExisting(tagExtraDate) == false) this->tags.insert(tagExtraDate,Value);   
    else this->tags.replace(tagExtraDate,Value);  
    this->settagsHaveChanged(true);  
}

void    tagManager::setInterprete(std::string artist){
    TagLib::String Value = artist;
    if (this->isTagExisting(tagInterprete) == false) this->tags.insert(tagInterprete,Value);   
    else this->tags.replace(tagInterprete,Value);    
    this->settagsHaveChanged(true);
}

void    tagManager::setExtraArtist(std::string artist){
    TagLib::String Value = artist;
    if (this->isTagExisting(tagExtraArtist) == false) this->tags.insert(tagExtraArtist,Value);   
    else this->tags.replace(tagExtraArtist,Value);    
    this->settagsHaveChanged(true);
}

void    tagManager::setTitre(std::string titre){
    TagLib::String Value = titre;
    if (this->isTagExisting(tagTitre) == false) this->tags.insert(tagTitre,Value);   
    else this->tags.replace(tagTitre,Value);    
    this->settagsHaveChanged(true);
}

void    tagManager::setExtraTitle(std::string titre){
    TagLib::String Value = titre;
    if (this->isTagExisting(tagExtraTitle) == false) this->tags.insert(tagExtraTitle,Value);   
    else this->tags.replace(tagExtraTitle,Value);  
    this->settagsHaveChanged(true);     
}

uint8_t tagManager::getExtraTagValue(bool CreateEmptyIfNotExist) {
    uint8_t Byte;

    if (this->isTagExisting(tagExtra) == false) {
        if (CreateEmptyIfNotExist == false) throw TagNotInTheFileException{tagExtra}; 
        else this->setExtraTagValue(0x00);
    }
    
    std::string strExtra = this->tags.value(tagExtra).toString().to8Bit(); /** The 2 digits hexadecimal value is copied into strExtra */

    if (strExtra.length() != 2) throw BadTagValueException{}; /** String size MUST be 2 (digits) */

    auto [ptr, ec] = std::from_chars(strExtra.data(), strExtra.data() + strExtra.size(), Byte, 16);
    if ( ec != std::errc() ) throw BadTagValueException{}; 

    return Byte;
}

void    tagManager::setExtraTagValue(uint8_t Byte){
    TagLib::String Value = std::format("{:02X}", Byte);
    if (this->isTagExisting(tagExtra) == false) this->tags.insert(tagExtra,Value);   
    else this->tags.replace(tagExtra,Value);
    this->settagsHaveChanged(true);
}

void    tagManager::setMovieSoundTrackFlag(bool isMovieSt){
    uint8_t Byte;
    try{
        Byte = this->getExtraTagValue();
    }
    catch (const TagNotInTheFileException &e){
        Byte = 0x00;
    }

    uint8_t Mask = std::to_underlying(btExtraMask::MOVIE_TRACK);

    if (isMovieSt) Byte |= Mask;    /** Sets the "Movie Soundtrack" flag  */
    else Byte &= ~Mask;

    this->setExtraTagValue(Byte);
}

void    tagManager::setTvShowFlag(bool isTvs){
    uint8_t Byte;
    try{
        Byte = this->getExtraTagValue();
    }
    catch (const TagNotInTheFileException &e){
        Byte = 0x00;
    }

    uint8_t Mask = std::to_underlying(btExtraMask::TV_SHOW);

    if (isTvs) Byte |= Mask;    
    else Byte &= ~Mask;

    this->setExtraTagValue(Byte);    
}

void    tagManager::setMasterPieceFlag(bool isMstpce){
    uint8_t Byte;
    try{
        Byte = this->getExtraTagValue();
    }
    catch (const TagNotInTheFileException &e){
        Byte = 0x00;
    }

    uint8_t Mask = std::to_underlying(btExtraMask::MASTERPIECE);

    if (isMstpce) Byte |= Mask;    
    else Byte &= ~Mask;

    this->setExtraTagValue(Byte); 
}

void    tagManager::setSbigFlag(bool isSb){
    uint8_t Byte;
    try{
        Byte = this->getExtraTagValue();
    }
    catch (const TagNotInTheFileException &e){
        Byte = 0x00;
    }

    uint8_t Mask = std::to_underlying(btExtraMask::SBIG);

    if (isSb) Byte |= Mask;    
    else Byte &= ~Mask;

    this->setExtraTagValue(Byte); 
}

void    tagManager::setDuetFlag(bool isSb){
    uint8_t Byte;
    try{
        Byte = this->getExtraTagValue();
    }
    catch (const TagNotInTheFileException &e){
        Byte = 0x00;
    }

    uint8_t Mask = std::to_underlying(btExtraMask::DUET);

    if (isSb) Byte |= Mask;    
    else Byte &= ~Mask;

    this->setExtraTagValue(Byte); 
}

void    tagManager::setCoverFlag(bool isSb){
    uint8_t Byte;
    try{
        Byte = this->getExtraTagValue();
    }
    catch (const TagNotInTheFileException &e){
        Byte = 0x00;
    }

    uint8_t Mask = std::to_underlying(btExtraMask::COVER);

    if (isSb) Byte |= Mask;    
    else Byte &= ~Mask;

    this->setExtraTagValue(Byte); 
}

void    tagManager::setNameFlag(bool isSb){
    uint8_t Byte;
    try{
        Byte = this->getExtraTagValue();
    }
    catch (const TagNotInTheFileException &e){
        Byte = 0x00;
    }

    uint8_t Mask = std::to_underlying(btExtraMask::NAME);

    if (isSb) Byte |= Mask;    
    else Byte &= ~Mask;

    this->setExtraTagValue(Byte); 
}

void    tagManager::setCityFlag(bool isSb){
    uint8_t Byte;
    try{
        Byte = this->getExtraTagValue();
    }
    catch (const TagNotInTheFileException &e){
        Byte = 0x00;
    }

    uint8_t Mask = std::to_underlying(btExtraMask::CITY);

    if (isSb) Byte |= Mask;    
    else Byte &= ~Mask;

    this->setExtraTagValue(Byte); 
}

bool    tagManager::isMovieSoundTrack(bool fc) {
    uint8_t Byte = this->getExtraTagValue(fc);

    if ((Byte & std::to_underlying(btExtraMask::MOVIE_TRACK)) == std::to_underlying(btExtraMask::MOVIE_TRACK) ) return true;
    else return false;

}

bool    tagManager::isTvShow(bool fc) {
    uint8_t Byte = this->getExtraTagValue(fc);

    if ((Byte & std::to_underlying(btExtraMask::TV_SHOW)) == std::to_underlying(btExtraMask::TV_SHOW) ) return true;
    else return false;    
}

bool    tagManager::isMasterPiece(bool fc) {
    uint8_t Byte = this->getExtraTagValue(fc);

    if ((Byte & std::to_underlying(btExtraMask::MASTERPIECE)) == std::to_underlying(btExtraMask::MASTERPIECE) ) return true;
    else return false;     
}

bool    tagManager::isSbig(bool fc) {
    uint8_t Byte = this->getExtraTagValue(fc);

    if ((Byte & std::to_underlying(btExtraMask::SBIG)) == std::to_underlying(btExtraMask::SBIG) ) return true;
    else return false;      
}

bool    tagManager::isDuet(bool fc) {
    uint8_t Byte = this->getExtraTagValue(fc);

    if ((Byte & std::to_underlying(btExtraMask::DUET)) == std::to_underlying(btExtraMask::DUET) ) return true;
    else return false;      
}

bool    tagManager::isCover(bool fc) {
    uint8_t Byte = this->getExtraTagValue(fc);

    if ((Byte & std::to_underlying(btExtraMask::COVER)) == std::to_underlying(btExtraMask::COVER) ) return true;
    else return false;      
}

bool    tagManager::isName(bool fc) {
    uint8_t Byte = this->getExtraTagValue(fc);

    if ((Byte & std::to_underlying(btExtraMask::NAME)) == std::to_underlying(btExtraMask::NAME) ) return true;
    else return false;      
}

bool    tagManager::isCity(bool fc) {
    uint8_t Byte = this->getExtraTagValue(fc);

    if ((Byte & std::to_underlying(btExtraMask::CITY)) == std::to_underlying(btExtraMask::CITY) ) return true;
    else return false;      
}


missingtagmask_t    tagManager::checkTags(){
    uint16_t res{};

    if (!this->isTagExisting(tagTitre)) res += std::to_underlying(btMissingTagMask::TITLE);
    if (!this->isTagExisting(tagInterprete)) res += std::to_underlying(btMissingTagMask::ARTIST);
    if (!this->isTagExisting(tagDate)) res += std::to_underlying(btMissingTagMask::DATE);
    if (!this->isTagExisting(tagLangue)) res += std::to_underlying(btMissingTagMask::LANGUAGE);
    if (!this->isTagExisting(tagExtra)) res += std::to_underlying(btMissingTagMask::EXTRA);
    if (!this->isTagExisting(tagExtraTitle)) res += std::to_underlying(btMissingTagMask::EXTRA_TITLE);
    if (!this->isTagExisting(tagExtraDate)) res += std::to_underlying(btMissingTagMask::EXTRA_DATE);
    if (!this->isTagExisting(tagExtraArtist)) res += std::to_underlying(btMissingTagMask::EXTRA_ARTIST);

   return res;
}

bool tagManager::ContainsNonBtTags(){
    auto it = this->tags.begin();

    while(it != this->tags.end()){
        std::string curTag = it->first.to8Bit();
        //std::println("Checking tag {}",curTag);
        if (this->isbtTag(it->first.to8Bit())==false) return true;
        else ++it;
    }

    return false;
}

void tagManager::prepareFile(bool eraseUnusedTags){
    if (eraseUnusedTags == true){
        
        while (this->ContainsNonBtTags()){
            auto it = this->tags.begin();
            
            while(this->isbtTag(it->first.to8Bit())==true){
                ++it;
            }
            this->deleteTag(it->first.to8Bit());

        }
        
    }
    

    if (!this->isTagExisting(tagTitre)) this->setTitre(DEFAULT_STRING_VALUE);
    if (!this->isTagExisting(tagInterprete)) this->setInterprete(DEFAULT_STRING_VALUE);
    if (!this->isTagExisting(tagDate)) this->setDate(INVALID_DATE_VALUE);
    if (!this->isTagExisting(tagLangue)) this->setLangue(btLanguage::FRA);
    if (!this->isTagExisting(tagExtra)) this->setExtraTagValue(0x00);
    if (!this->isTagExisting(tagExtraTitle)) this->setExtraTitle(DEFAULT_STRING_VALUE);
    if (!this->isTagExisting(tagExtraDate)) this->setExtraDate(INVALID_DATE_VALUE);
    if (!this->isTagExisting(tagExtraArtist)) this->setExtraArtist(DEFAULT_STRING_VALUE);   
    if(this->update())std::println("OK");
    else std::println("ERRROR");
}

std::string     tagManager::makeFileName() {
    std::string Name{};
    tagManager::btPeriod    Periode;

    if ( (this->isMovieSoundTrack(false)) || (this->isTvShow(false))) Periode = this->getExtraPeriod(false);
    else    Periode = this->getPeriod(false);

    switch (Periode){
        case tagManager::btPeriod::OLDIES : Name += "o"; break;
        case tagManager::btPeriod::SEVENTIES : Name += "7"; break;
        case tagManager::btPeriod::EIGHTIES : Name += "8"; break;
        case tagManager::btPeriod::NINETIES : Name += "9"; break;
        case tagManager::btPeriod::Y2K0X : Name += "0";break;
        case tagManager::btPeriod::Y2K1X : Name += "1";break;
        case tagManager::btPeriod::Y2K2X : Name += "2";break;
        default: Name += "x";break;
    }
  
    tagManager::btLanguage Langue = this->getLangue(false);
    switch(Langue){
        case tagManager::btLanguage::FRA : Name+="F";break;
        case tagManager::btLanguage::INT : Name+="I";break;
        default : Name += "x";
    }

    Name += std::format("{:02X}", this->getExtraTagValue());
    Name += "_";
    Name += this->getInterprete(false);
    Name += "_";
    Name += this->getTitre(false);

    if ( (this->isMovieSoundTrack(false)) || (this->isTvShow(false)) ) {
        Name += "_";
        Name += this->getExtraTitle(false); 
    }

    Name += ".mp3";
    
    return Name;
}

