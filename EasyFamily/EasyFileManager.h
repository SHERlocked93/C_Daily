#pragma once

#include <filesystem>
#include <regex>
#include <string>

namespace fs = std::filesystem;

namespace common_util {

class EasyFileManager {
 public:
  /// move file to target folder
  /// \param fullPath path of file
  /// \param tarFolder path of target folder
  /// \return 0:OK, non-0: error
  static int moveFile(const std::string& fullPath, const std::string& tarFolder);

  /// move file to target folder
  /// \param fullPath path of file
  /// \param tarFolder path of target folder
  /// \param [out] newPath new path after the file moved
  /// \return 0:OK, non-0: error
  static int moveFile(const std::string& fullPath, const std::string& tarFolder, std::string& newPath);

  /// delete file
  /// \param fullpath
  /// \return 0:OK, non-0: error
  static int deleteFile(const std::string& fullpath);

  /// delete folder
  /// \param fullpath
  /// \return 0:OK, non-0: error
  static int deleteFolder(const std::string& fullpath);

  /// Find file in specified folder path matching the pattern
  /// \param dirPath Folder path
  /// \param pattern File pattern ex:R"(Prog\.([0-9A-Za-z]{4})\.4109.+)"
  /// \return Found file path, empty string if not found
  static std::string findFilepathWithPattern(const std::string& dirPath, const std::string& pattern);

  /// Find all files in specified folder path matching the pattern
  /// \param dirPath Folder path
  /// \param pattern File pattern ex:R"(Prog\.([0-9A-Za-z]{4})\.41.+)"
  /// \return Found file paths, empty if not found
  static std::vector<std::string> findFilesWithPattern(const std::string& dirPath, const std::string& pattern);

  /// Extract path and filename from a full path
  /// \param fullPath ex: "/afc/abc/def/Prog.00"
  /// \return ex: ["/afc/abc/def", "Prog.00"]
  static std::pair<std::string, std::string> splitPathAndFilename(const std::string& fullPath);
};

}  // namespace common_util
