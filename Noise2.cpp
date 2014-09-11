//
// �񎟌��̃m�C�Y�֐�
//

#include <math.h>
#include <memory.h>

#include "Noise2.h"

// �f�[�^�̏���
void Noise2::erase()
{
  if (p)
  {
    for (int i = 0; i < m; ++i) delete p[i];
    delete[] p;
  }
}

// �f�[�^�̃R�s�[
void Noise2::copy(const Noise2 &noise)
{
  erase();

  m = noise.m;
  p = new Noise1 *[m];

  for (int i = 0; i < m; ++i)
  {
    p[i] = new Noise1;
    *p[i] = *noise.p[i];
  }
}

// �R���X�g���N�^ (�ߓ_�� n, m)
Noise2::Noise2(int n, int m)
{
  if (m > 0)
  {
    this->m = m;
    this->p = new Noise1 *[m];

    for (int i = 0; i < m; ++i)
      this->p[i] = new Noise1(n);
  }
  else
  {
    this->p = 0;
  }
}

// ������Z�q
Noise2 &Noise2::operator=(const Noise2 &noise)
{
  if (&noise != this) copy(noise);

  return *this;
}

// (x, y) �ɂ������Ԓl�����߂� (0 �� x �� n, 0 �� y �� m)
double Noise2::noise(double x, double y) const
{
  const double my(static_cast<double>(m) * (y - floor(y)));
  const int i(static_cast<int>(my));
  const double t(my - static_cast<double>(i));

  if (i == 0)
    return catmull_rom(p[m - 1]->noise(x), p[0]->noise(x),
    p[1]->noise(x), p[2]->noise(x), t);
  if (i < m - 2)
    return catmull_rom(p[i - 1]->noise(x), p[i]->noise(x),
    p[i + 1]->noise(x), p[i + 2]->noise(x), t);
  if (i == m - 2)
    return catmull_rom(p[m - 3]->noise(x), p[m - 2]->noise(x),
    p[m - 1]->noise(x), p[0]->noise(x), t);
  return catmull_rom(p[m - 2]->noise(x), p[m - 1]->noise(x),
    p[0]->noise(x), p[1]->noise(x), t);
}

// (x, y) �ɂ����� o �I�N�^�[�u�� Perlin �m�C�Y�����߂� (�I�N�^�[�u�Ԃ̌����� a)
double Noise2::perlin(double x, double y, int o, double a) const
{
  double v(noise(x, y));
  double w(a);

  while (--o > 0)
  {
    v += noise(x *= 2.0, y *= 2.0) * w;
    w *= a;
  }

  return v;
}

// (x, y) �ɂ����� o �I�N�^�[�u�� Turbulence �m�C�Y�����߂� (�I�N�^�[�u�Ԃ̌����� a)
double Noise2::turbulence(double x, double y, int o, double a) const
{
  double v(fabs(noise(x, y)));
  double w(a);

  while (--o > 0)
  {
    v += fabs(noise(x *= 2.0, y *= 2.0)) * w;
    w *= a;
  }

  return v;
}
