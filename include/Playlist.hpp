#ifndef __PLAYLIST_HPP__
#define __PLAYLIST_HPP__

#include <filesystem>
#include <fstream>
#include <print>
#include <string>
#include <vector>

/**
 * @brief
 *
 */
class Playlist {
  private:
    std::string Name{}; /**< Playlist Name  */

  protected:
    std::vector<std::string> FileList{}; /**< List of files into the playlist */

  public:
    Playlist() = delete;           /**< No default Ctor */
    virtual ~Playlist() = default; /**< Defaulted Dtor */

    /**
     * @brief Construct a new Playlist object
     *
     * @param _Name
     */
    Playlist(const std::string &_Name) : Name{_Name} {};

    /**
     * @brief   Name property getter
     *
     */
    [[nodiscard]] std::string getName() noexcept { return this->Name; };

    /**
     * @brief Adds a file to the playlist
     *
     * @param file
     */
    void add(const std::string &file) noexcept { this->FileList.push_back(file); };

    /**
     * @brief
     *
     */
    void dump() noexcept;

    virtual void generate() = 0;
};

//-----------------------------------------------------------------------------
const std::string defaultExt{"m3u"};

/**
 * @brief
 *
 */
class m3uPlaylist : public Playlist {
  private:
    const std::string ext{defaultExt};
    std::string targetDir{}; /**< Target DIRECTORY */

    std::string FileName{}; /** playlist full name : path\name.ext - created by
                               the object  */

    void makeFileName() { this->FileName = this->getpath() + this->getName() + "." + this->ext; };

  public:
    m3uPlaylist() = delete;
    virtual ~m3uPlaylist() = default;

    m3uPlaylist(const std::string &Name, const std::string &path);

    [[nodiscard]] std::string getpath() noexcept { return this->targetDir; };

    [[nodiscard]] std::string getFileName() noexcept { return this->FileName; };

    virtual void generate() override;
};

#endif /*  __PLAYLIST_HPP__    */
