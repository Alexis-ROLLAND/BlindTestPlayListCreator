#ifndef __PLAYLIST_HPP__
#define __PLAYLIST_HPP__

#include <filesystem>
#include <fstream>
#include <print>
#include <string>
#include <vector>

class Playlist {
  private:
    std::string Name{};

  protected:
    std::vector<std::string> FileList{};

  public:
    Playlist() = delete;
    virtual ~Playlist() = default;

    Playlist(const std::string &_Name) : Name{_Name} {};

    [[nodiscard]] std::string getName() noexcept { return this->Name; };

    void add(const std::string file) noexcept {
        this->FileList.push_back(file);
    };

    void dump() noexcept;

    virtual void generate() = 0;
};

class m3uPlaylist : public Playlist {
  private:
    const std::string ext{"m3u"};
    std::string path{}; /**< Target DIRECTORY */

    std::string FileName{}; /** playlist full name : path\name.ext - created by
                               the object  */

    void makeFileName() {
        this->FileName = this->getpath() + this->getName() + "." + this->ext;
    };

  public:
    m3uPlaylist() = delete;
    virtual ~m3uPlaylist() = default;

    m3uPlaylist(const std::string &Name, const std::string &path);

    [[nodiscard]] std::string getpath() noexcept { return this->path; };

    [[nodiscard]] std::string getFileName() noexcept { return this->FileName; };

    virtual void generate() override;
};

#endif /*  __PLAYLIST_HPP__    */
