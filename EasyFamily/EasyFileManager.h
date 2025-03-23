#pragma once

#include <filesystem>
#include <regex>
#include <string>

namespace fs = std::filesystem;


class  EasyFileManager {
 public:
  //! move file to target folder
  //! \param fullPath path of file
  //! \param tarFolder path of target folder
  //! \return 0:OK, non-0: error
  static int moveFile(const std::string& fullPath,
                      const std::string& tarFolder);

  //! move file to target folder
  //! \param fullPath path of file
  //! \param tarFolder path of target folder
  //! \param [out] newPath new path after the file moved
  //! \return 0:OK, non-0: error
  static int moveFile(const std::string& fullPath, const std::string& tarFolder,
                      std::string& newPath);

  //! delete file
  //! \param fullpath
  //! \return 0:OK, non-0: error
  static int deleteFile(const std::string& fullpath);

  //! delete folder
  //! \param fullpath
  //! \return 0:OK, non-0: error
  static int deleteFolder(const std::string& fullpath);

  //! 用正则在文件夹路径中搜索文件
  //! \param dirPath 文件夹路径
  //! \param pattern 文件名正则 ex:R"(Prog\.([0-9A-Za-z]{4})\.4109.+)"
  //! \return 找到的文件路径 未找到为空字符串
  static std::string findFilepathWithPattern(const std::string& dirPath,
                                             const std::string& pattern);

  //! 用正则在文件夹路径中搜索所有满足条件的文件
  //! \param dirPath 文件夹路径
  //! \param pattern 文件名正则 ex:R"(Prog\.([0-9A-Za-z]{4})\.41.+)"
  //! \return 找到的文件名数组 未找到为空
  static std::vector<std::string> findFilesWithPattern(
      const std::string& dirPath, const std::string& pattern);

  //! 从一个路径中拆分出父路径和文件名
  //! \param fullPath ex: "/afc/abc/def/Prog.00"
  //! \return ex: ["/afc/abc/def", "Prog.00"]
  static std::pair<std::string, std::string> splitPathAndFilename(
      const std::string& fullPath);
};

