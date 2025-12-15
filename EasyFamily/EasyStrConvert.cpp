#include "EasyStrConvert.h"

#include <algorithm>
#include <codecvt>
#include <cstring>
#include <locale>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#else
#include <iconv.h>
#endif

namespace common_util {

std::wstring to_wstring(const std::string& s) {
  if (s.empty()) {
    return L"";
  }

  try {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(s);
  } catch (...) {
    return L"";
  }
}

std::string wto_string(const std::wstring& ws) {
  if (ws.empty()) {
    return "";
  }

  try {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(ws);
  } catch (...) {
    return "";
  }
}

std::string w2c(const wchar_t* src) {
  if (!src) {
    return "";
  }

  return wto_string(std::wstring(src));
}

std::wstring c2w(const char* src) {
  if (!src) {
    return std::wstring(L"");
  }

#ifdef _WIN32
  int srcLen = static_cast<int>(strlen(src));
  int dstSizeInWords = MultiByteToWideChar(CP_ACP, 0, src, srcLen, NULL, 0);

  if (dstSizeInWords <= 0) {
    return std::wstring(L"");
  }

  std::wstring reVal(dstSizeInWords, L'\0');
  MultiByteToWideChar(CP_ACP, 0, src, srcLen, &reVal[0], dstSizeInWords);

  return reVal;
#else
  return to_wstring(std::string(src));
#endif
}

std::string UnicodeToUTF8(const std::wstring& wstr) {
  // Use std::wstring_convert to convert wchar_t to UTF-8 encoded std::string
  std::wstring_convert<std::codecvt_utf8<wchar_t>> wcv;
  try {
    std::string ret = wcv.to_bytes(wstr);
    // Remove any \u0000
    ret.erase(std::remove(ret.begin(), ret.end(), '\0'), ret.end());
    return ret;
  } catch (...) {
    return "";
  }
}

std::wstring UTF8ToUnicode(const std::string& str) {
  std::wstring ret;
  std::wstring_convert<std::codecvt_utf8<wchar_t>> wcv;
  try {
    ret = wcv.from_bytes(str);
  } catch (...) {
    return L"";
  }
  return ret;
}

std::string GbkToUtf8(std::string src_str) {
  if (src_str.empty()) {
    return "";
  }

#ifdef _WIN32
  // Windows 平台使用 Windows API
  int len = MultiByteToWideChar(CP_ACP, 0, src_str.c_str(), -1, NULL, 0);
  if (len <= 0) return "";

  std::vector<wchar_t> wstr(len);
  MultiByteToWideChar(CP_ACP, 0, src_str.c_str(), -1, wstr.data(), len);

  len = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, NULL, 0, NULL, NULL);
  if (len <= 0) {
    return "";
  }

  std::vector<char> strTemp(len);
  WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, strTemp.data(), len, NULL, NULL);

  // Remove null terminator if present in the count, though std::string handles it
  if (len > 0 && strTemp[len - 1] == '\0') {
    return std::string(strTemp.data());
  }
  return std::string(strTemp.data(), len);

#else
  // Linux/Unix 平台使用 iconv
  iconv_t cd = iconv_open("UTF-8", "GBK");
  if (cd == (iconv_t)-1) {
    return src_str;  // 转换失败，返回原字符串
  }

  size_t inlen = src_str.length();
  size_t outlen = inlen * 4;  // UTF-8 最多需要 4 倍空间

  std::vector<char> outbuf(outlen);
  char* inbuf = const_cast<char*>(src_str.c_str());
  char* outptr = outbuf.data();

  if (iconv(cd, &inbuf, &inlen, &outptr, &outlen) == (size_t)-1) {
    iconv_close(cd);
    return src_str;
  }

  std::string result(outbuf.data());
  iconv_close(cd);
  return result;
#endif
}

std::string Utf8ToGbk(std::string src_str) {
  if (src_str.empty()) {
    return "";
  }

#ifdef _WIN32
  // Windows 平台使用 Windows API
  int len = MultiByteToWideChar(CP_UTF8, 0, src_str.c_str(), -1, NULL, 0);
  if (len <= 0) return "";

  std::vector<wchar_t> wszGBK(len);
  MultiByteToWideChar(CP_UTF8, 0, src_str.c_str(), -1, wszGBK.data(), len);

  len = WideCharToMultiByte(CP_ACP, 0, wszGBK.data(), -1, NULL, 0, NULL, NULL);
  if (len <= 0) {
    return "";
  }

  std::vector<char> szGBK(len);
  WideCharToMultiByte(CP_ACP, 0, wszGBK.data(), -1, szGBK.data(), len, NULL, NULL);

  if (len > 0 && szGBK[len - 1] == '\0') {
    return std::string(szGBK.data());
  }
  return std::string(szGBK.data(), len);

#else
  // Linux/Unix 平台使用 iconv
  iconv_t cd = iconv_open("GBK", "UTF-8");
  if (cd == (iconv_t)-1) {
    return src_str;  // 转换失败，返回原字符串
  }

  size_t inlen = src_str.length();
  size_t outlen = inlen * 2;  // GBK 最多需要 2 倍空间

  std::vector<char> outbuf(outlen);
  char* inbuf = const_cast<char*>(src_str.c_str());
  char* outptr = outbuf.data();

  if (iconv(cd, &inbuf, &inlen, &outptr, &outlen) == (size_t)-1) {
    iconv_close(cd);
    return src_str;
  }

  std::string result(outbuf.data());
  iconv_close(cd);
  return result;
#endif
}

} // namespace common_util
