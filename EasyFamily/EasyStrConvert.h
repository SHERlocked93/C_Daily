#pragma once

#include <algorithm>
#include <chrono>
#include <ctime>
#include <initializer_list>
#include <iomanip>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class EasyStrConvert {
 public:
  static std::string GbkToUtf8(std::string src_str);
  static std::string Utf8ToGbk(std::string src_str);
  static std::string UnicodeToUTF8(const std::wstring &wstr);
  static std::wstring UTF8ToUnicode(const std::string &wstr);
  static std::wstring to_wstring(const std::string &s);
  static std::string wto_string(const std::wstring &ws);
  static std::string w2c(const wchar_t *src);
  static std::string w2c(const std::wstring &src) { return w2c(src.c_str()); }
  static std::wstring c2w(const char *src);
  static std::wstring c2w(const std::string &src) { return c2w(src.c_str()); }
};

// 去除多余\0的字符串截取
inline std::string toStrPure(unsigned char *pos1, unsigned char *pos2) {
  return std::string(std::string(pos1, pos2).c_str());
}

inline std::string toStrPure(unsigned char *pos, int size_, int &idx_) {
  std::string ret =
      std::string(std::string(pos + idx_, pos + idx_ + size_).c_str());
  idx_ += size_;
  return ret;
}

//! 检查变量val是否在vals中
//! \param val [in] 要检查的值
//! \param vals [in] 要检查的容器初始化列表
//! \return val是否在vals中
//! ex:
//!   containsInVals(3, {1, 2, 3}) 返回true
//!   containsInVals(4, {1, 2, 3}) 返回false
template <typename T>
bool containsInVals(const T &val, const std::initializer_list<T> &vals) {
  for (const auto &v : vals) {
    if (val == v) return true;
  }
  return false;
}

template <typename T>
bool containsInVals(const T &val, const std::vector<T> &vals) {
  for (const auto &v : vals)
    if (val == v) return true;
  return false;
}

// 获取val的第pos位字节的值
template <typename C>
unsigned char getUcharAt(C val, int pos = 0) {
  return static_cast<unsigned char>((val >> (8 * pos)) & 0xff);
}

// 获取map中key对应的值
template <typename T>
std::string getTypeMapVal(T key,
                          const std::unordered_map<T, std::string> &keyMap,
                          const std::string &defaultPrompt = "") {
  auto iter = keyMap.find(key);
  if (iter != keyMap.end()) return iter->second;

  return defaultPrompt;  // TYPE MAP NOT FOUND!
}

// 获取map中key对应的值
template <typename keyT, typename valT>
valT getMapVal(keyT key, const std::unordered_map<keyT, valT> &keyMap) {
  auto iter = keyMap.find(key);
  if (iter != keyMap.end()) return iter->second;

  return valT{};  // TYPE MAP NOT FOUND!
}

// 获取map中值对应的key
template <typename keyT, typename valT>
keyT getMapKey(valT val, const std::unordered_map<keyT, valT> &keyMap) {
  for (const auto &[key, value] : keyMap)
    if (value == val) return key;

  return keyT{};  // TYPE MAP NOT FOUND!
}

// 十进制转化为十六进制BCD ex: 1723->0x1723
template <typename H>
H int2hexBcd(H sour) {
  H result = 0;
  int shift = 0;
  int numDigits = sizeof(H) * 2;

  for (int i = 0; i < numDigits; ++i) {
    result |= (sour % 10) << shift;
    sour /= 10;
    shift += 4;
  }

  // if (sour != 0)
  //   throw std::overflow_error("[EasyStrConvert] int2hexBcd input too large");

  return result;
}

inline unsigned long long bcd2num(const unsigned char *pos, int ptrLen,
                                  bool isRevert = false) {
  unsigned long long result = 0;
  const auto o_ = [&](int pos_) {  // 倒序
    if (!isRevert) return pos_;
    pos_ = -(pos_ - ptrLen + 1) % ptrLen;
    return pos_;
  };

  for (int i = 0; i < ptrLen; i++) {
    result = result * 100 + ((pos[o_(i)] & 0x0F) + ((pos[i] >> 4) * 10));
  }

  return result;
}

inline unsigned char bcd2uchar(const unsigned char *pos) {
  return (unsigned char)bcd2num(pos, 1);
}
inline unsigned short bcd2ushort(const unsigned char *pos) {
  return (unsigned short)bcd2num(pos, 2);
}
inline unsigned int bcd2uint(const unsigned char *pos) {
  return (unsigned int)bcd2num(pos, 4);
}
inline unsigned int bcd2uint(unsigned int pos) {
  return (unsigned int)bcd2num((const unsigned char *)&pos, 4, true);
}
inline unsigned long long bcd2ulonglong(const unsigned char *pos) {
  return (unsigned long long)bcd2num(pos, 8);
}

template <typename D>
int ConvertFromByte(D &dest_, const unsigned char *buff_, const int &&len_,
                    bool isRevert = false) {
  const int size = sizeof(dest_);
  const auto o_ = [&](int pos) {  // 倒序
    if (!isRevert) return pos;
    pos = -(pos - size + 1) % size;
    return pos;
  };
  switch (size) {
    case 1:
      dest_ = *(buff_ + len_);
      break;
    case 2:
      dest_ = *(buff_ + len_ + o_(1)) << 8 | *(buff_ + len_ + o_(0));
      break;
    case 4:
      dest_ = *(buff_ + len_ + o_(3)) << 24 | *(buff_ + len_ + o_(2)) << 16 |
              *(buff_ + len_ + o_(1)) << 8 | *(buff_ + len_ + o_(0));
      break;
    case 8:
      dest_ =
          *(buff_ + (D)len_ + o_(7)) << 56 | *(buff_ + (D)len_ + o_(6)) << 48 |
          *(buff_ + (D)len_ + o_(5)) << 40 | *(buff_ + (D)len_ + o_(4)) << 32 |
          *(buff_ + (D)len_ + o_(3)) << 24 | *(buff_ + (D)len_ + o_(2)) << 16 |
          *(buff_ + (D)len_ + o_(1)) << 8 | *(buff_ + (D)len_ + o_(0));
      break;
    default:
      break;
  }

  return 0;
}

//! \param isRevert 是否倒序取出 倒序为大端序
template <typename D>
int ConvertFromByte(D &dest_, const unsigned char *buff_, int &idx_,
                    bool isRevert = false) {
  const int size = sizeof(dest_);
  ConvertFromByte(dest_, buff_, std::move(idx_), isRevert);

  idx_ += size;
  return 0;
}

template <typename D>
int ConvertFromBCD(D &dest_, const unsigned char *buff_, int &&idx_) {
  const int size = sizeof(dest_);
  switch (size) {
    case 1:
      dest_ = bcd2uchar(buff_ + idx_);
      break;
    case 2:
      dest_ = bcd2ushort(buff_ + idx_);
      break;
    case 4:
      dest_ = bcd2uint(buff_ + idx_);
      break;
    case 8:
      dest_ = bcd2ulonglong(buff_ + idx_);
      break;
    default:
      break;
  }

  return 0;
}

template <typename D>
int ConvertFromBCD(D &dest_, const unsigned char *buff_, int &idx_) {
  const int size = sizeof(dest_);
  int ret = ConvertFromBCD(dest_, buff_, std::move(idx_));
  idx_ += size;

  return ret;
}

template <typename S>
int Convert2Byte(S sour_, unsigned char *buff_, int &&idx_,
                 bool isRevert = false) {
  const int size = sizeof(sour_);
  const auto o_ = [&](int pos) {  // 倒序
    if (!isRevert) return pos;
    pos = -(pos - size + 1) % size;
    return pos;
  };

  switch (size) {
    case 1:
      *(buff_ + idx_) = sour_;
      break;
    case 2:
      *(buff_ + idx_ + o_(0)) = sour_ >> 0 & 0xff;
      *(buff_ + idx_ + o_(1)) = sour_ >> 8 & 0xff;
      break;
    case 4:
      *(buff_ + idx_ + o_(0)) = sour_ >> 0 & 0xff;
      *(buff_ + idx_ + o_(1)) = sour_ >> 8 & 0xff;
      *(buff_ + idx_ + o_(2)) = sour_ >> 16 & 0xff;
      *(buff_ + idx_ + o_(3)) = sour_ >> 24 & 0xff;
      break;
    case 8:
      *(buff_ + idx_ + o_(0)) = sour_ & 0xff;
      *(buff_ + idx_ + o_(1)) = sour_ >> 8 & 0xff;
      *(buff_ + idx_ + o_(2)) = sour_ >> 16 & 0xff;
      *(buff_ + idx_ + o_(3)) = sour_ >> 24 & 0xff;
      *(buff_ + idx_ + o_(4)) = sour_ >> 32 & 0xff;
      *(buff_ + idx_ + o_(5)) = sour_ >> 40 & 0xff;
      *(buff_ + idx_ + o_(6)) = sour_ >> 48 & 0xff;
      *(buff_ + idx_ + o_(7)) = sour_ >> 56 & 0xff;
      break;
    default:
      break;
  }
  return 0;
}

//! \param isRevert 是否倒序存入 倒序为大端序
template <typename S>
int Convert2Byte(S sour_, unsigned char *buff_, int &idx_,
                 bool isRevert = false) {
  const int size = sizeof(sour_);
  Convert2Byte(sour_, buff_, std::move(idx_), isRevert);
  idx_ += size;
  return 0;
}

//! \param isRevert 是否倒序存入 倒序为大端序
template <typename S>
int Convert2Byte(S sour_, std::vector<unsigned char> &buff_, int &idx_,
                 bool isRevert = false) {
  const int size = sizeof(sour_);
  if (buff_.size() < idx_ + size) buff_.resize(idx_ + size);
  const auto o_ = [&](int pos) {  // 倒序
    if (!isRevert) return pos;
    pos = -(pos - size + 1) % size;
    return pos;
  };

  switch (size) {
    case 1:
      buff_[idx_] = sour_;
      break;
    case 2:
      buff_[idx_ + o_(0)] = sour_ & 0xff;
      buff_[idx_ + o_(1)] = (sour_ >> 8) & 0xff;
      break;
    case 4:
      buff_[idx_ + o_(0)] = sour_ & 0xff;
      buff_[idx_ + o_(1)] = (sour_ >> 8) & 0xff;
      buff_[idx_ + o_(2)] = (sour_ >> 16) & 0xff;
      buff_[idx_ + o_(3)] = (sour_ >> 24) & 0xff;
      break;
    case 7:
      buff_[idx_ + o_(0)] = sour_ & 0xff;
      buff_[idx_ + o_(1)] = (sour_ >> 8) & 0xff;
      buff_[idx_ + o_(2)] = (sour_ >> 16) & 0xff;
      buff_[idx_ + o_(3)] = (sour_ >> 24) & 0xff;
      buff_[idx_ + o_(4)] = (sour_ >> 32) & 0xff;
      buff_[idx_ + o_(5)] = (sour_ >> 40) & 0xff;
      buff_[idx_ + o_(6)] = (sour_ >> 48) & 0xff;
      break;
    case 8:
      buff_[idx_ + o_(0)] = sour_ & 0xff;
      buff_[idx_ + o_(1)] = (sour_ >> 8) & 0xff;
      buff_[idx_ + o_(2)] = (sour_ >> 16) & 0xff;
      buff_[idx_ + o_(3)] = (sour_ >> 24) & 0xff;
      buff_[idx_ + o_(4)] = (sour_ >> 32) & 0xff;
      buff_[idx_ + o_(5)] = (sour_ >> 40) & 0xff;
      buff_[idx_ + o_(6)] = (sour_ >> 48) & 0xff;
      buff_[idx_ + o_(7)] = (sour_ >> 56) & 0xff;
      break;
    default:
      break;
  }
  idx_ += size;
  return 0;
}

inline void CopyFromBuff(unsigned char *dest_, const unsigned char *sour_,
                         int size_, int &idx_) {
  memcpy(dest_, sour_ + idx_, size_);
  idx_ += size_;
}

template <typename T>
void Copy2Buff(const T *sour_, unsigned char *dest_, int size_, int &&idx_,
               bool isRevert = false) {
  if (isRevert) {
    for (int i = 0; i < size_; ++i)
      dest_[idx_ + i] = *((unsigned char *)sour_ + size_ - 1 - i);
  } else {
    memcpy(dest_ + idx_, sour_, size_);
  }
}
//! \param isRevert 是否倒序存入
template <typename T>
void Copy2Buff(const T *sour_, unsigned char *dest_, int size_, int &idx_,
               bool isRevert = false) {
  Copy2Buff(sour_, dest_, size_, std::move(idx_), isRevert);
  idx_ += size_;
}

//! 大小端序互转
inline void bigEndian2littleEndian(const unsigned char *srcArr,
                                   unsigned char *dstArr, const int srcLen) {
  for (int i = 0; i < srcLen / 2; i++) {
    const unsigned char temp = srcArr[i];
    dstArr[i] = srcArr[srcLen - i - 1];
    dstArr[srcLen - i - 1] = temp;
  }
}

//! 数组转化为UINT 大端序
// ex: UcharArr2UintBigEndian([0x12, 0x22, 0x44, 0x45])
//! \return 0x12224445
inline unsigned int UcharArr2UintBigEndian(const unsigned char *arr) {
  return static_cast<unsigned int>(arr[0]) << 24 |
         static_cast<unsigned int>(arr[1]) << 16 |
         static_cast<unsigned int>(arr[2]) << 8 |
         static_cast<unsigned int>(arr[3]);
}
// ex: UcharArr2UintBigEndian({0x12, 0x22, 0x44, 0x45})
//! \return 0x12224445
inline unsigned int UcharArr2UintBigEndian(
    const std::initializer_list<unsigned char> &arr) {
  unsigned int result = 0;
  int shift = 24;
  for (auto val : arr) {
    result |= static_cast<unsigned int>(val) << shift;
    shift -= 8;
  }
  return result;
}

// 数组转化为UINT 小端序
inline unsigned int UcharArr2UintLittleEndian(const unsigned char *arr) {
  return static_cast<unsigned int>(arr[0]) |
         static_cast<unsigned int>(arr[1]) << 8 |
         static_cast<unsigned int>(arr[2]) << 16 |
         static_cast<unsigned int>(arr[3]) << 24;
}
inline unsigned int UcharArr2UintLittleEndian(
    const std::initializer_list<unsigned char> &arr) {
  unsigned int result = 0;
  int shift = 0;
  for (auto val : arr) {
    result |= static_cast<unsigned int>(val) << shift;
    shift += 8;
  }
  return result;
}

//! 转化为7位BCD，YYYYMMDDHH24MISS 格式
//! \param [in] timestamp getUTCtime()的返回值
//! \param [out] res 7位长度指针
//! ex：
//!  XDateTime time = XDateTime::UtcNow();
//!  BYTE arr[7]{0};
//!  getUTCtimeBcd(time.UtcSecondsSince1970(), arr);
inline void getUTCtimeBcd(time_t timestamp, unsigned char *res) {
  struct tm timeinfo {};
  localtime_s(&timeinfo, &timestamp);

  const int year = timeinfo.tm_year + 1900;
  const unsigned char month = timeinfo.tm_mon + 1;
  const unsigned char day = timeinfo.tm_mday;
  const unsigned char hour = timeinfo.tm_hour;
  const unsigned char minute = timeinfo.tm_min;
  const unsigned char second = timeinfo.tm_sec;

  const int bcdYear = ((year / 1000) << 12) | (((year / 100) % 10) << 8) |
                      (((year / 10) % 10) << 4) | (year % 10);

  res[0] = (unsigned char)(bcdYear >> 8) & 0xFF;
  res[1] = (unsigned char)bcdYear & 0xFF;
  res[2] = int2hexBcd(month);
  res[3] = int2hexBcd(day);
  res[4] = int2hexBcd(hour);
  res[5] = int2hexBcd(minute);
  res[6] = int2hexBcd(second);
}

//! 转化为4位BCD，YYYYMMDD 格式
//! \param [in] timestamp getUTCtime()的返回值
//! \param [out] res 4位长度指针
//! ex：
//!  XDateTime time = XDateTime::UtcNow();
//!  BYTE arr[4]{0};
//!  getUTCtimeBcd(time.UtcSecondsSince1970(), arr);
inline void getUTCdateBcd(time_t timestamp, unsigned char *res) {
  struct tm timeinfo {};
  localtime_s(&timeinfo, &timestamp);

  const int year = timeinfo.tm_year + 1900;
  const unsigned char month = timeinfo.tm_mon + 1;
  const unsigned char day = timeinfo.tm_mday;

  const int bcdYear = ((year / 1000) << 12) | (((year / 100) % 10) << 8) |
                      (((year / 10) % 10) << 4) | (year % 10);

  res[0] = (unsigned char)(bcdYear >> 8) & 0xFF;
  res[1] = (unsigned char)bcdYear & 0xFF;
  res[2] = int2hexBcd(month);
  res[3] = int2hexBcd(day);
}

inline std::string str2lowerCase(const std::string &str) {
  std::string res;
  res.reserve(str.size());
  transform(str.begin(), str.end(), back_inserter(res), tolower);
  return res;
}

inline std::string str2upperCase(const std::string &str) {
  std::string res;
  res.reserve(str.size());
  transform(str.begin(), str.end(), back_inserter(res), toupper);
  return res;
}

//! 指针转化为字符串形式
//! @param bytes 指针 ex:{0x98, 0xE2}
//! @param length 长度 ex:2
//! @param isUpperCase 是否大写，默认小写
//! @param hasDivide 是否有空格分隔符 默认无
//! @return string"98e2" or "98 e2"
inline std::string bytes2hexStr(const unsigned char *bytes, int length,
                                bool isUpperCase = false,
                                bool hasDivide = false) {
  if (bytes == NULL) {
    return "";
  }
  std::string str;
  const int len = length;
  for (int j = 0; j < len; j++) {
    int high = bytes[j] / 16, low = bytes[j] % 16;
    str += (high < 10) ? ('0' + high) : ('a' + high - 10);
    str += (low < 10) ? ('0' + low) : ('a' + low - 10);
    if (hasDivide && j != len - 1) {
      str += " ";
    }
  }
  if (isUpperCase) str = str2upperCase(str);
  return str;
}

//! 将十六进制字符串转换vector
//! \param hexStr 十六进制字符串 ex:"0220240719184242338B5D"
//! \return {02 20 24 07 19 18 42 42 33 8b 5d}
inline std::vector<unsigned char> hexStr2Vector(const std::string &hexStr) {
  // 将十六进制字符转换为无符号字符（unsigned char）
  auto hexCharToByte = [](const char ch) -> unsigned char {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;

    throw std::invalid_argument("Invalid hex character");
  };

  std::vector<unsigned char> result;
  size_t length = hexStr.length();

  // 长度是奇数，忽略最后一个字符
  if (length % 2 != 0) length -= 1;

  for (size_t i = 0; i < length; i += 2) {
    unsigned char byte =
        (hexCharToByte(hexStr[i]) << 4) | hexCharToByte(hexStr[i + 1]);
    result.push_back(byte);
  }

  return result;
}

inline std::string str2HexStr(const std::string &str, bool isUpperCase = false,
                              bool hasDivide = false) {
  return bytes2hexStr((const unsigned char *)str.c_str(), str.size(),
                      isUpperCase, hasDivide);
}

//! 将十进制数转成ASCii同时前补0
//! \param num 转前数字 ex:123
//! \param len 确保长度，不够前需补0 ex:6
//! \return ex: "000123"
inline std::string int2str(unsigned long num, int len = 4) {
  std::string result;

  // 不断取余数并将结果添加到字符串中，直到number为0
  while (num > 0) {
    int digit = num % 10;           // 取当前最低位
    result.push_back('0' + digit);  // 将数字转换为字符并添加到结果字符串中
    num /= 10;                      // 去掉已经处理的最低位
  }
  int templ = result.size();
  if (templ < len) {
    for (int i = 0; i < (len - templ); i++) {
      result.push_back('0');
    }
  }
  // 反转字符串，因为我们是从低位到高位处理的
  std::reverse(result.begin(), result.end());

  return result;
}

//! int转16进制格式string
//! \param num 转前数字 ex:1234
//! \param len 确保长度，为0则不需补0，不够前需补0 ex:4
//! \return "4d2" or "04d2"
inline std::string int2hexStr(unsigned long num, int len) {
  std::stringstream ss;

  if (len)
    ss << std::hex << std::setfill('0') << std::setw(len) << num;
  else
    ss << std::hex << num;
  return ss.str();
}
//! long long转16进制格式string
inline std::string longLongToString(long long value, int width) {
  std::ostringstream stream;
  // 设置流的宽度，并左对齐
  if (width)
    stream << std::internal << std::setfill('0') << std::setw(width) << std::hex
           << value;
  else
    stream << std::internal << std::hex << value;
  return stream.str();
}
// !long long转十进制进制格式string
inline std::string longLongToDecString(long long value, int width) {
  std::ostringstream stream;
  // 设置流的宽度，并左对齐
  if (width)
    stream << std::internal << std::setfill('0') << std::setw(width) << std::dec
           << value;
  else
    stream << std::internal << std::dec << value;
  return stream.str();
}
// 将16进制string 转换成int
inline int hexStringToInt(const std::string &hexStr) {
  std::stringstream ss;
  ss << std::hex << hexStr;
  unsigned int result;
  ss >> result;
  return static_cast<int>(result);
}
//! 指针转化为字符串形式
//! @param bytes 指针 ex:{0x20, 0x24, 0x12, 0x03, 0x12, 0x59, 0x59}
//! @param flag 格式化形式 ex:7
//! @return string"2024-12-03 12:59:59"
inline std::string BCDTime2formatStr(unsigned char *bytes, int flag = 7) {
  if (bytes == NULL) {
    return "";
  }
  char temp[100] = {0};
  // auto t = bytes[6];
  if (flag == 7) {
    sprintf_s(temp, sizeof(temp), "%02x%02x-%02x-%02x %02x:%02x:%02x", bytes[0],
              bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], bytes[6]);
  }
  if (flag == 4) {
    sprintf_s(temp, sizeof(temp), "%02x%02x-%02x-%02x", bytes[0], bytes[1],
              bytes[2], bytes[3]);
  }
  if (flag == 5) {
    sprintf_s(temp, sizeof(temp), "%02x-%02x-%02x %02x:%02x", bytes[0],
              bytes[1], bytes[2], bytes[3], bytes[4]);
  }
  if (flag == 6) {
    sprintf_s(temp, sizeof(temp), "%02x-%02x-%02x %02x:%02x:%02x", bytes[0],
              bytes[1], bytes[2], bytes[3], bytes[4], bytes[5]);
  }
  std::string buff = temp;
  return buff;
}

//! 将IP地址转换十六进制数
//! \param ipAddress ex:"192.168.0.1"
//! \return ex:0xc0a80001
inline unsigned int ipStr2uint(const std::string &ipAddress) {
  std::stringstream ss(ipAddress);
  std::string segment;
  unsigned int ret = 0;
  unsigned int shift = 24;  // 位移

  while (std::getline(ss, segment, '.')) {
    unsigned int num = std::stoi(segment);
    ret |= (num << shift);
    shift -= 8;
  }

  return ret;
}

//! 将十六进制数IP转换为转换string格式
//! \param ipUint ex:0xc0a80001
//! \return ex:"192.168.0.1"
inline std::string ipUint2str(const unsigned int ipUint) {
  std::string ret;
  for (int i = 3; i >= 0; --i) {
    ret += std::to_string((ipUint >> (i * 8)) & 0xFF);
    if (i > 0) ret += ".";
  }
  return ret;
}

//! 查找某个字符串有没有对应子串
//! \param str 字符串
//! \param subStr 子串
//! \return false未找到 true找到
inline bool containsInStr(const std::string &str, const std::string &subStr) {
  return str.find(subStr) != std::string::npos;
}

//! 将一个序列化后的字符串转化为向量
//! \tparam T 单个数组值类型
//! \param str 字符串 ex:0,1,2
//! \param splitStr 分隔符 ex:,
//! \return 转化为的向量 ex:<0,1,2>
template <typename T>
inline std::vector<T> str2vec(const std::string &str,
                              const std::string &splitStr = ",") {
  std::vector<T> result;
  size_t start = 0;
  size_t end = 0;

  while (end != std::string::npos) {
    end = str.find(splitStr, start);
    std::string token = str.substr(start, end - start);

    if (!token.empty()) {
      try {
        result.push_back(static_cast<T>(std::stoi(token)));
      } catch (const std::invalid_argument &) {}
    }
    start = end + splitStr.length();
  }

  return result;
}

//! 向量拼接为字符串
//! \tparam T 单个数组值类型
//! \param vec 数组 ex:<0,1,2>
//! \param splitStr 数组中间插入的分隔符 ex:","
//! \return 转化为的字符串 ex:0,1,2
template <typename T>
std::string vec2str(const std::vector<T> &vec,
                    const std::string &splitStr = ",") {
  std::stringstream ss;
  for (int i = 0; i < vec.size(); ++i) {
    ss << vec[i];
    if (i != vec.size() - 1) ss << splitStr;
  }
  return ss.str();
}

//! 替换字符串中的正则表达式
//! \param inputStr 待替换字符串 "c:/3/4\\1\\2"
//! \param regStr 正则表达式传参 ex:R"(\\)"
//! \param replacedStr 替换字符串 ex:"/"
//! \return 替换后的字符串 ex: "c:/3/4/1/2"
inline std::string replaceWithStr(const std::string &inputStr,
                                  const std::string &regStr,
                                  const std::string &replacedStr) {
  std::regex backslash(regStr);
  return std::regex_replace(inputStr, backslash, replacedStr);
}

//! 数字分转化为元 ex:1123->11.23
inline std::string getYuanStr(int valFen) {
  double amount = static_cast<double>(valFen) / 100.0;

  std::ostringstream oss;
  oss << std::fixed << std::setprecision(2) << amount;
  return oss.str();
}
