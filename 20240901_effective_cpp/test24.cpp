#include <iostream>

// 有理数类
class Rational {
public:
  Rational(int numerator = 0, int denominator = 1) : numerator_(numerator), denominator_(denominator) {
  }

  // 有理数相乘，分子x分子，分母x分母
  // const Rational operator*(const Rational& rhs) const { return Rational(numerator_ * rhs.numerator(), denominator_ * rhs.denominator()); }

  int numerator() const { return numerator_; } // 分子
  int denominator() const { return denominator_; } // 分母

private:
  int numerator_ = 0;
  int denominator_ = 1;
};

const Rational operator*(const Rational& lhs, const Rational& rhs) {
  return Rational(lhs.numerator() * rhs.numerator(), lhs.denominator() * rhs.denominator());
}


int main() {
  Rational oneEighth(1, 8); // 有理数 1/8
  Rational oneHalf(1, 2); // 有理数 1/2

  Rational result = oneHalf * 2;
  Rational result2 = 2 * oneHalf * 2;

  std::cout << result.numerator() << "/" << result.denominator();
  std::cout << " " << result2.numerator() << "/" << result2.denominator() << std::endl;
}
