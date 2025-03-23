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

  //! ���������ļ���·���������ļ�
  //! \param dirPath �ļ���·��
  //! \param pattern �ļ������� ex:R"(Prog\.([0-9A-Za-z]{4})\.4109.+)"
  //! \return �ҵ����ļ�·�� δ�ҵ�Ϊ���ַ���
  static std::string findFilepathWithPattern(const std::string& dirPath,
                                             const std::string& pattern);

  //! ���������ļ���·�����������������������ļ�
  //! \param dirPath �ļ���·��
  //! \param pattern �ļ������� ex:R"(Prog\.([0-9A-Za-z]{4})\.41.+)"
  //! \return �ҵ����ļ������� δ�ҵ�Ϊ��
  static std::vector<std::string> findFilesWithPattern(
      const std::string& dirPath, const std::string& pattern);

  //! ��һ��·���в�ֳ���·�����ļ���
  //! \param fullPath ex: "/afc/abc/def/Prog.00"
  //! \return ex: ["/afc/abc/def", "Prog.00"]
  static std::pair<std::string, std::string> splitPathAndFilename(
      const std::string& fullPath);
};

