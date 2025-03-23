#include "EasyFileManager.h"

#include <filesystem>
#include <thread>

#include "../Common.LogManager.Adapter/LogManager.h"

using namespace common_util;
namespace fs = std::filesystem;

int EasyFileManager::moveFile(const std::string& fullPath,
                          const std::string& tarFolder) {
  std::string m;
  return moveFile(fullPath, tarFolder, m);
}

int EasyFileManager::moveFile(const std::string& fullPath,
                          const std::string& tarFolder, std::string& newPath) {
  fs::path srcFile(fullPath);
  fs::path tarDir(tarFolder);
  fs::path tarFile = tarDir / srcFile.filename().string();
  if (!fs::exists(srcFile)) return -1;

  try {
    if (!fs::exists(tarDir)) {
      fs::create_directories(tarDir);
    }
    fs::rename(srcFile, tarFile);
  } catch (const std::exception&) {
    return -2;
  }

  newPath = tarFile.string();

  return 0;
}

int EasyFileManager::deleteFile(const std::string& fullpath) {
  fs::path file(fullpath);
  if (!fs::exists(file)) return 0;
  auto log_ = commonlogger::LogManager::getLogger("EasyFileManager");

  std::error_code err;
  auto res = fs::remove(file, err);
  if (!res)
    log_.errorFormat("remove file: %s  res: %s", fullpath.c_str(),
                     err.message().c_str());

  return res ? 0 : err.value();
}

int EasyFileManager::deleteFolder(const std::string& fullpath) {
  fs::path file(fullpath);
  if (!fs::exists(file)) return 0;

  std::error_code err;
  auto val = fs::remove_all(file, err);

  return val ? 0 : err.value();
}

std::string EasyFileManager::findFilepathWithPattern(const std::string& dirPath,
                                                 const std::string& pattern) {
  std::regex regexPattern(pattern);
  for (const auto& entry : fs::directory_iterator(dirPath)) {
    if (entry.is_regular_file()) {
      std::string filename = entry.path().filename().string();
      if (std::regex_match(filename, regexPattern)) {
        return entry.path().string();
      }
    }
  }
  return "";
}

std::vector<std::string> EasyFileManager::findFilesWithPattern(
    const std::string& dirPath, const std::string& pattern) {
  std::vector<std::string> matchedFiles;
  std::regex regexPattern(pattern);

  for (const auto& entry : fs::directory_iterator(dirPath)) {
    if (entry.is_regular_file()) {
      const std::string fileName = entry.path().filename().string();
      if (std::regex_match(fileName, regexPattern)) {
        matchedFiles.push_back(entry.path().filename().string());
      }
    }
  }

  return matchedFiles;
}

std::pair<std::string, std::string> EasyFileManager::splitPathAndFilename(
    const std::string& fullPath) {
  fs::path path(fullPath);
  std::string parent_path = path.parent_path().string();
  std::string filename = path.filename().string();
  return {parent_path, filename};
}
