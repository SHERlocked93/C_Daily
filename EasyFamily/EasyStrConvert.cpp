#include "EasyStrConvert.h"

#include <Windows.h>

#include <codecvt>
#include <locale>

std::wstring EasyStrConvert::to_wstring(const std::string& s) {
  int length = MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.size(), nullptr, 0);
  if (length == 0) {
    return L"";
  }
  auto* wide_buffer = new wchar_t[length]{0};
  MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.size(), wide_buffer, length);

  std::wstring ret(wide_buffer, wide_buffer + length);
  delete[] wide_buffer;
  return ret;
}
std::string EasyStrConvert::wto_string(const std::wstring& ws) {
  int length = WideCharToMultiByte(CP_ACP, 0, ws.c_str(), ws.size(), nullptr, 0, nullptr, nullptr);
  if (length == 0) {
    return "";
  }
  auto* multi_buffer = new char[length]{0};
  WideCharToMultiByte(CP_ACP, 0, ws.c_str(), ws.size(), multi_buffer, length, nullptr, nullptr);

  std::string ret(multi_buffer, multi_buffer + length);
  delete[] multi_buffer;
  return ret;
}

std::string EasyStrConvert::w2c(const wchar_t* src) {
  if (src) {
    // Convert to a char*
    size_t dstSizeInBytes = wcslen(src) * sizeof(wchar_t) + 1;
    size_t convertedChars = 0;
    char* dst = new char[dstSizeInBytes];
    _locale_t locale = _create_locale(LC_ALL, ".936");
    _wcstombs_s_l(&convertedChars  // size_t			*_PtNumOfCharConverted,
                  ,
                  dst  // char *			_Dst,
                  ,
                  dstSizeInBytes  // size_t			_DstSizeInBytes,
                  ,
                  src  // const wchar_t		*src,
                  ,
                  _TRUNCATE  // size_t			_MaxCountInBytes
                  ,
                  locale  //_locale_t _Locale
    );
    _free_locale(locale);

    std::string reVal = std::string(dst);
    delete[] dst;
    return reVal;
  } else {
    return std::string("");
  }
}

std::wstring EasyStrConvert::c2w(const char* src) {
  if (!src) {
    return std::wstring(L"");
  }

  int srcLen = static_cast<int>(strlen(src));
  int dstSizeInWords = MultiByteToWideChar(CP_ACP, 0, src, srcLen, NULL, 0);

  if (dstSizeInWords <= 0) {
    return std::wstring(L"");
  }

  std::wstring reVal(dstSizeInWords, L'\0');
  MultiByteToWideChar(CP_ACP, 0, src, srcLen, &reVal[0], dstSizeInWords);

  return reVal;
  //if (src) {
  //  size_t dstSizeInWords = strlen(src) + 1;
  //  size_t convertedChars = 0;
  //  wchar_t* dst = new wchar_t[dstSizeInWords];
  //  // mbstowcs_s(&convertedChars, dst, dstSizeInWords, src,dstSizeInWords);
  //  _locale_t locale = _create_locale(LC_ALL, ".936");
  //  _mbstowcs_s_l(&convertedChars, dst, dstSizeInWords, src, dstSizeInWords, locale);
  //  _free_locale(locale);
  //  std::wstring reVal = std::wstring(dst);
  //  delete[] dst;
  //  return reVal;
  //} else {
  //  return std::wstring(L"");
  //}
}

std::string EasyStrConvert::GbkToUtf8(std::string src_str) {
  int len = MultiByteToWideChar(CP_ACP, 0, src_str.c_str(), -1, NULL, 0);
  wchar_t* wstr = new wchar_t[len + 1];
  memset(wstr, 0, len + 1);
  MultiByteToWideChar(CP_ACP, 0, src_str.c_str(), -1, wstr, len);
  len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
  char* str = new char[len + 1];
  memset(str, 0, len + 1);
  WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
  std::string strTemp = str;
  if (wstr) delete[] wstr;
  if (str) delete[] str;
  return strTemp;
}

std::string EasyStrConvert::Utf8ToGbk(std::string src_str) {
  int len = MultiByteToWideChar(CP_UTF8, 0, src_str.c_str(), -1, NULL, 0);
  wchar_t* wszGBK = new wchar_t[len + 1];
  memset(wszGBK, 0, len * 2 + 2);
  MultiByteToWideChar(CP_UTF8, 0, src_str.c_str(), -1, wszGBK, len);
  len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
  char* szGBK = new char[len + 1];
  memset(szGBK, 0, len + 1);
  WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
  std::string strTemp(szGBK);
  if (wszGBK) delete[] wszGBK;
  if (szGBK) delete[] szGBK;
  return strTemp;
}

std::string EasyStrConvert::UnicodeToUTF8(const std::wstring& wstr) {
  // 使用 std::wstring_convert 将 wchar_t 转换为 UTF-8 编码的 std::string
  std::wstring_convert<std::codecvt_utf8<wchar_t> > wcv;
  std::string ret = wcv.to_bytes(wstr);

  // 移除多余的 \u0000
  ret.erase(std::remove(ret.begin(), ret.end(), '\0'), ret.end());

  return ret;
}

std::wstring EasyStrConvert::UTF8ToUnicode(const std::string& str) {
  std::wstring ret;

  std::wstring_convert<std::codecvt_utf8<wchar_t> > wcv;
  ret = wcv.from_bytes(str);

  return ret;
}

