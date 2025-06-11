#ifndef _XORSTR_H_
#define _XORSTR_H_

#include <string>
#include <array>
#include <utility>
#include <cstdarg>

namespace wlib {

#ifdef _MSC_VER
#define XORSTR_INLINE __forceinline
#else
#define XORSTR_INLINE inline
#endif

// 辅助函数：将字符转换为整数
constexpr int char_to_int(char c) {
  return static_cast<int>(c);
}

// 根据当前编译时间计算种子值
constexpr auto compile_time = __TIME__;
constexpr auto compile_time_seed = char_to_int(compile_time[7]) +
                                   char_to_int(compile_time[6]) * 10 +
                                   char_to_int(compile_time[4]) * 60 +
                                   char_to_int(compile_time[3]) * 600 +
                                   char_to_int(compile_time[1]) * 3600 +
                                   char_to_int(compile_time[0]) * 36000;

// 使用 Park-Miller 算法生成伪随机数的模板
template <int Iteration>
struct random_generator {
private:
  static constexpr unsigned multiplier = 16807;  // 乘数常量 (7^5)
  static constexpr unsigned modulus = 2147483647;  // 模数常量 (2^31 - 1)
  static constexpr unsigned previous_value = random_generator<Iteration - 1>::value;  // 上一个随机值
  static constexpr unsigned low_bits = multiplier * (previous_value & 0xFFFF);  // 乘以低 16 位
  static constexpr unsigned high_bits = multiplier * (previous_value >> 16);  // 乘以高 16 位
  static constexpr unsigned combined_low = low_bits + ((high_bits & 0x7FFF) << 16);  // 合并低位和高位
  static constexpr unsigned combined_high = high_bits >> 15;  // 丢弃低 15 位
  static constexpr unsigned final_value = combined_low + combined_high;  // 最终计算

public:
  static constexpr unsigned max_value = modulus;  // 最大值
  static constexpr unsigned value = final_value > modulus ? final_value - modulus : final_value;  // 确保值在范围内
};

// 随机生成器的基例特化
template <>
struct random_generator<0> {
  static constexpr unsigned value = compile_time_seed;  // 使用种子作为初始值
};

// 用于生成指定范围内随机整数的模板
template <int Iteration, int Range>
struct random_int {
  static constexpr auto value = random_generator<Iteration + 1>::value % Range;  // 取模操作以限制范围
};

// 用于生成随机字符的模板
template <int Iteration>
struct random_char {
  static const char value = static_cast<char>(1 + random_int<Iteration, 0x7F - 1>::value);  // 确保在 ASCII 范围内
};

// 用于加密和解密字符串的模板
template <size_t Length, int KeyIndex>
struct xor_string {
private:
  const char encryption_key_;  // 用于 XOR 加密的随机密钥
  std::array<char, Length + 1> encrypted_string_;  // 存储加密后的字符串

  // 使用 XOR 加密字符
  constexpr char encrypt_char(char c) const {
    return c ^ encryption_key_;
  }

  // 使用 XOR 解密字符
  char decrypt_char(char c) const {
    return c ^ encryption_key_;
  }

public:
  // 构造函数，用于加密输入字符串
  template <size_t... Indices>
  constexpr XORSTR_INLINE xor_string(const char* str, std::index_sequence<Indices...>) :
    encryption_key_(random_char<KeyIndex>::value), encrypted_string_{ { encrypt_char(str[Indices])... } } {}

  // 解密字符串并返回
  XORSTR_INLINE decltype(auto) decrypt() {
    for (size_t i = 0; i < Length; ++i) {
      encrypted_string_[i] = decrypt_char(encrypted_string_[i]);
    }
    encrypted_string_[Length] = '\0';  // 添加字符串结束符
    return encrypted_string_.data();
  }
};

#undef XORSTR_INLINE

}  // namespace wlib

// 简化字符串加密/解密使用的宏
#define xorstr(s) (wlib::xor_string<sizeof(s) - 1, \
  __COUNTER__>(s, std::make_index_sequence<sizeof(s) - 1>()).decrypt())

#endif