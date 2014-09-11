//
// 三次元のノイズ関数
//

#include <math.h>
#include <memory.h>

#include "Noise3.h"

// データの消去
void Noise3::erase()
{
  if (p)
  {
    for (int i = 0; i < l; ++i) delete p[i];
    delete[] p;
  }
}

// データのコピー
void Noise3::copy(const Noise3 &noise)
{
  erase();

  l = noise.l;
  p = new Noise2 *[l];

  for (int i = 0; i < l; ++i)
  {
    p[i] = new Noise2;
    *p[i] = *noise.p[i];
  }
}

// コンストラクタ (節点数 n, m, l)
Noise3::Noise3(int n, int m, int l)
{
  if (l > 0)
  {
    this->l = l;
    this->p = new Noise2 *[l];

    for (int i = 0; i < l; ++i)
      this->p[i] = new Noise2(n, m);
  }
  else
  {
    this->p = 0;
  }
}

// 代入演算子
Noise3 &Noise3::operator=(const Noise3 &noise)
{
  if (&noise != this) copy(noise);

  return *this;
}

// (x, y, z) における補間値を求める (0 ≦ x ≦ n, 0 ≦ y ≦ m, 0 ≦ z ≦ l)
double Noise3::noise(double x, double y, double z) const
{
  const double lz(static_cast<double>(l) * (z - floor(z)));
  const int i(static_cast<int>(lz));
  const double t(lz - static_cast<double>(i));

  if (i == 0)
    return catmull_rom(p[l - 1]->noise(x, y), p[0]->noise(x, y),
    p[1]->noise(x, y), p[2]->noise(x, y), t);
  if (i < l - 2)
    return catmull_rom(p[i - 1]->noise(x, y), p[i]->noise(x, y),
    p[i + 1]->noise(x, y), p[i + 2]->noise(x, y), t);
  if (i == l - 2)
    return catmull_rom(p[l - 3]->noise(x, y), p[l - 2]->noise(x, y),
    p[l - 1]->noise(x, y), p[0]->noise(x, y), t);
  return catmull_rom(p[l - 2]->noise(x, y), p[l - 1]->noise(x, y),
    p[0]->noise(x, y), p[1]->noise(x, y), t);
}

// (x, y, z) における o オクターブの Perlin ノイズを求める (オクターブ間の減衰率 a)
double Noise3::perlin(double x, double y, double z, int o, double a) const
{
  double v(noise(x, y, z));
  double w(a);

  while (--o > 0)
  {
    v += noise(x *= 2.0, y *= 2.0, z *= 2.0) * w;
    w *= a;
  }

  return v;
}

// (x, y, z) における o オクターブの Turbulence ノイズを求める (オクターブ間の減衰率 a)
double Noise3::turbulence(double x, double y, double z, int o, double a) const
{
  double v(fabs(noise(x, y, z)));
  double w(a);

  while (--o > 0)
  {
    v += fabs(noise(x *= 2.0, y *= 2.0, z *= 2.0)) * w;
    w *= a;
  }

  return v;
}
