//
// �ꎟ���̃m�C�Y�֐�
//

#include <math.h>

#include "Noise1.h"

// 3 �� Catmull-Rom Spline ���
double catmull_rom(double x0, double x1, double x2, double x3, double t)
{
  const double v1((x2 - x0) * 0.5), v2((x3 - x1) * 0.5);

  return (((2.0 * x1 - 2.0 * x2 + v1 + v2) * t
    - 3.0 * x1 + 3.0 * x2 - 2.0 * v1 - v2) * t
    + v1) * t + x1;
}

// ��l�������� (Xorshift �@)
double xor128()
{
  static unsigned int x(123456789);
  static unsigned int y(362436069);
  static unsigned int z(521288629);
  static unsigned int w(88675123);

  unsigned int t = x ^ x << 11;

  x = y;
  y = z;
  z = w;

  return double(w ^= w >> 19 ^ t ^ t >> 8) * 0.0000000002328306437080797375;
}

// �f�[�^�̃R�s�[
void Noise1::copy(const Noise1 &noise)
{
  delete[] p;

  n = noise.n;
  p = new double[n];

  for (int i = 0; i < n; ++i)
    p[i] = noise.p[i];
}

// �R���X�g���N�^ (�ߓ_�� n)
Noise1::Noise1(int n)
{
  if (n > 0)
  {
    this->n = n;
    this->p = new double[n];

    for (int i = 0; i < n; ++i)
      this->p[i] = xor128() - 0.5;
  }
  else
  {
    this->p = 0;
  }
}

// ������Z�q
Noise1 &Noise1::operator=(const Noise1 &noise)
{
  if (&noise != this) copy(noise);

  return *this;
}

// x �ɂ������Ԓl�����߂� (0 �� x �� n)
double Noise1::noise(double x) const
{
  const double nx(static_cast<double>(n) * (x - floor(x)));
  const int i(static_cast<int>(nx));
  const double t(nx - static_cast<double>(i));

  if (i == 0)
    return catmull_rom(p[n - 1], p[0], p[1], p[2], t);
  if (i < n - 2)
    return catmull_rom(p[i - 1], p[i], p[i + 1], p[i + 2], t);
  if (i == n - 2)
    return catmull_rom(p[n - 3], p[n - 2], p[n - 1], p[0], t);
  return catmull_rom(p[n - 2], p[n - 1], p[0], p[1], t);
}

// x �ɂ����� o �I�N�^�[�u�� Perlin �m�C�Y�����߂� (�I�N�^�[�u�Ԃ̌����� a)
double Noise1::perlin(double x, int o, double a) const
{
  double v(noise(x));
  double w(a);

  while (--o > 0)
  {
    v += noise(x *= 2.0) * w;
    w *= a;
  }

  return v;
}

// x �ɂ����� o �I�N�^�[�u�� Turbulence �m�C�Y�����߂� (�I�N�^�[�u�Ԃ̌����� a)
double Noise1::turbulence(double x, int o, double a) const
{
  double v(fabs(noise(x)));
  double w(a);

  while (--o > 0)
  {
    v += fabs(noise(x *= 2.0)) * w;
    w *= a;
  }

  return v;
}
