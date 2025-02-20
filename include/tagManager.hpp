#ifndef __TAG_MANAGER_HPP__
#define __TAG_MANAGER_HPP__


/** Standard inclusions     */
#include <exception>
#include <filesystem>
#include <print>
#include <set>

/** inclusions for TagLib  */
#include <tag.h>
#include <fileref.h>
#include <tpropertymap.h>

using   missingtagmask_t = uint16_t;

const   std::string   tagTitre{"TITLE"};
const   std::string   tagInterprete{"ARTIST"};
const   std::string   tagDate{"DATE"};
const   std::string   tagLangue{"LANGUAGE"};
const   std::string   tagExtra{"EXTRA"};
const   std::string   tagExtraTitle{"EXTRA_TITLE"};
const   std::string   tagExtraDate{"EXTRA_DATE"};
const   std::string   tagExtraArtist("EXTRA_ARTIST");

const   std::set<std::string>    bttTagList{tagTitre,
                                            tagInterprete,
                                            tagDate,
                                            tagLangue,
                                            tagExtra,
                                            tagExtraTitle,
                                            tagExtraDate,
                                            tagExtraArtist
                                        };

const   unsigned int  INVALID_DATE_VALUE{9999}; 
const   std::string   DEFAULT_STRING_VALUE{"default_string_do_not_use"};      

class tagManagerException : public std::exception{
    private:
        const char* Message{};
    public:
        tagManagerException(const char* msg) : Message(msg) {};
        const char* what() const noexcept override {return this->Message;};
};

class FileNotFoundException : public tagManagerException{
    public:
        FileNotFoundException() : tagManagerException("File not found."){};
};

class FileErrorException : public tagManagerException{
    public:
        FileErrorException() : tagManagerException("File error."){};
};

class NoTagsInFileException : public tagManagerException{
    public:
        NoTagsInFileException() : tagManagerException("No Tags in the file error."){};
};

class UnknownTagException : public tagManagerException{
    public:
        UnknownTagException() : tagManagerException("Tag provided is not a valid bt Tag."){};
};

class TagNotInTheFileException : public tagManagerException{
    public:
        const std::string tag{}; 
        TagNotInTheFileException(const std::string _tag) : tagManagerException("Tag provided is not in the file metadata."),tag{_tag}{};
};

class BadTagValueException : public tagManagerException{
    public:
        BadTagValueException() : tagManagerException("Tag provided is not in the file metadata."){};
};

class tagManager{
    public:
        

        enum class btPeriod{    OLDIES,     /**< Avant 1970 (DATE < 1970) */
                                SEVENTIES,  /**< DATE entre 1970 et 1979 */
                                EIGHTIES,   /**< DATE entre 1980 et 1989*/
                                NINETIES,   /**< DATE entre 1990 et 1999 */
                                Y2K0X,      /**< DATE entre 2000 et 2009 */
                                Y2K1X,      /**< DATE entre 2010 et 2019 */
                                Y2K2X,      /**< DATE entre 2020 et 2029 */ 
                                INVALID      /**< Valeur par défaut, tag non initialisé (Date = 9999)) */
        };

        enum class btLanguage{  FRA,        /**< FRAnçais */
                                INT         /**< INTernational */
        };

        enum class btExtraMask : uint8_t {    
            MOVIE_TRACK = 0x01,         /**< Title is also a movie track  */
            TV_SHOW = 0x02,             /**< Title is also a TV Show music */
            MASTERPIECE = 0x04,         /**< Title is known as a masterpiece */
            SBIG = 0x08,                /**< Title is known as a very bad song */
            DUET = 0x10,                /**< This title is a duet        */
            COVER = 0x20,               /**< This title is a cover   */
            NAME = 0x40,                /**< This title can be used for the "Name" game     */
            CITY = 0x80                 /**< This title can be used for the "Cities" game   */
        };

        enum class btMissingTagMask : uint16_t {
            TITLE = 0x0001,
            ARTIST = 0x0002,
            DATE = 0x0004,
            LANGUAGE = 0x0008,
            EXTRA = 0x0010,
            EXTRA_TITLE = 0x0020,
            EXTRA_DATE = 0x0040,
            EXTRA_ARTIST = 0x0080
        };

    private:
    
        const std::string   FileName{};
        
        TagLib::FileRef file{};
        TagLib::PropertyMap tags{};

        bool    tagsHaveChanged{false};
        void    settagsHaveChanged(bool value) {this->tagsHaveChanged = value;};

        /**
         * @brief   Getter for the file attribute, wich is a reference to the mp3 file
         * @return  TagLib::FileRef
         */
        [[nodiscard]]   TagLib::FileRef getFile() const noexcept {return this->file;};

        /**
         * @brief   Check for the tag's existence.
         * @return  true if the tag exists in the file, false otherwise.
         */
        [[nodiscard]]   bool isTagExisting(const std::string &Tag) noexcept;

        /**
         *  @brief  getter for the Extra tag value
         *  @param  bool : if this param is true, the tag (if nonexistent) will be
         *                  created (with a default, identificable) value)
         * @return  uint8_t : Tag's value 
         *    
         * @throw   TagNotInTheFileException
         * @throw   BadTagValueException
         */
        [[nodiscard]]   uint8_t getExtraTagValue(bool CreateEmptyIfNotExist = false);
        
        /** 
         * @brief   setter for the Extra tag value
         * @param   uint8_t : tag value
         */
        void    setExtraTagValue(uint8_t Byte);                    

        /**
         * @brief : converts a year to a period
         * @param unsigned int : the releasing date (year) of the record
         * @return btPeriod, the Blind Test Period
         */
        [[nodiscard]]   btPeriod    toPeriod(unsigned int year) const noexcept;


        [[nodiscard]]   bool    isbtTag(const std::string &tagname){return bttTagList.contains(tagname);};
        [[nodiscard]]   bool    ContainsNonBtTags();  
    public:
        tagManager() = delete;          /** Default CTOR is deleted */
        virtual ~tagManager()=default;  /** DTOR is defaulted */

        /**
         * @brief   Standard CTOR
         * @param   std::string FileName
         * 
         * @throw   FileNotFoundException   if file doesn't exist
         * @throw   FileErrorException      file load error
         * @throw   NoTagsInFileException   if file doesn't contain any tag
         */
        tagManager(const std::string &FileName);

        /**
         * @brief returns the filename 
         */
        [[nodiscard]]   std::string getFileName()  noexcept {return this->FileName;};

        /**
         * @brief returns the number of identified tags in the file 
         */
        [[nodiscard]]   std::size_t getNbTags()  noexcept {return this->tags.size();};

        /**
         * @brief "Dumps" the tags and values to std::cout. 
         *          format : TAG = Value [BTtag]
         *          [BTtag] is added at the end of the line if the tag is used by bttManager 
         */
        void    dump()  noexcept;

        /**
         * @brief Returns the "status" of the file
         * @return bool : returns true if the tags have changed (number, values...)
         */
        [[nodiscard]]   bool gettagsHaveChanged() noexcept {return this->tagsHaveChanged;};

        /**
         * @brief   These functions return a tag value (string format) : Title, Extra Title, Artist, Extra Artist
         * @param   bool : if true, the tag wil be created if missing
         * @throw   TagNotInTheFileException if the requested tag doesn't exist in the file and fc is false
         */
        [[nodiscard]]   std::string getTitre(bool fc) ;
        [[nodiscard]]   std::string getExtraTitle(bool fc); 
        [[nodiscard]]   std::string getInterprete(bool fc);
        [[nodiscard]]   std::string getExtraArtist(bool fc);

        /**
         * @brief   These functions return a tag value (integer format) : Date, Extra Date
         * @param   bool : if true, the tag wil be created if missing
         * @throw   TagNotInTheFileException if the requested tag doesn't exist in the file and fc is false
         */
        [[nodiscard]]   unsigned int    getDate(bool fc);
        [[nodiscard]]   unsigned int    getExtraDate(bool fc);

        /**
         * @brief   These functions return a direct (getLangue) or interpreted tag value (Period, Extra Period). 
         * @param   bool : if true, the tag wil be created if missing
         * @throw   TagNotInTheFileException if the requested tag doesn't exist in the file and fc is false
         */
        [[nodiscard]]   btPeriod        getPeriod(bool fc)  {return this->toPeriod(this->getDate(fc));};
        [[nodiscard]]   btPeriod        getExtraPeriod(bool fc)  {return this->toPeriod(this->getExtraDate(fc));};
        
        [[nodiscard]]   btLanguage      getLangue(bool fc);

        void    setLangue(btLanguage Langue);
        void    setDate(unsigned int year);
        void    setExtraDate(unsigned int year);
        void    setInterprete(std::string artist);
        void    setExtraArtist(std::string artist);
        void    setTitre(std::string titre);
        void    setExtraTitle(std::string titre);

        [[nodiscard]] bool    isMovieSoundTrack(bool fc) ;
        void    setMovieSoundTrackFlag(bool isMovieSt);

        [[nodiscard]] bool    isTvShow(bool fc) ;
        void    setTvShowFlag(bool isTvs);

        [[nodiscard]] bool    isMasterPiece(bool fc) ;
        void    setMasterPieceFlag(bool isMstpce);

        [[nodiscard]] bool    isSbig(bool fc) ;
        void    setSbigFlag(bool isSb);

        [[nodiscard]] bool    isDuet(bool fc) ;
        void    setDuetFlag(bool isSb);

        [[nodiscard]] bool    isCover(bool fc) ;
        void    setCoverFlag(bool isSb);

        [[nodiscard]] bool    isName(bool fc) ;
        void    setNameFlag(bool isSb);

        [[nodiscard]] bool    isCity(bool fc) ;
        void    setCityFlag(bool isSb);


        void    deleteTag(const std::string &tag);

        missingtagmask_t    checkTags();
        void    prepareFile(bool eraseUnusedTags = false);

        [[nodiscard]]   std::string     makeFileName() ;

        [[nodiscard]]   bool    update() {this->getFile().setProperties(this->tags); return this->getFile().save();};

};

#endif  /* __TAG_MANAGER_HPP__ */

