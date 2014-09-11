#pragma once
//
// 三次元のノイズ関数
//

#include "Noise2.h"

// 三次元のノイズ関数
class Noise3
{
  int l;                                          // 節点数
  Noise2 **p;                                     // 各節点の値
  void erase();                               // データの消去
  void copy(const Noise3 &noise);                 // データのコピー

public:

  // コンストラクタ
  Noise3() { p = 0; }
  Noise3(int n, int m, int l);
  Noise3(const Noise3 &noise) { copy(noise); }

  // デストラクタ
  virtual ~Noise3() { erase(); }

  // 代入演算子
  Noise3 &operator=(const Noise3 &noise);

  // 節点 (i, j, k) の値を得る 
  double point(int i, int j, int k) const { return p[k]->point(i, j); }

  // (x, y, z) における補間値を求める (0 ≦ x ≦ n, 0 ≦ y ≦ m, 0 ≦ z ≦ l)
  double noise(double x, double y, double z) const;

  // (x, y, z) における o オクターブの Perlin ノイズを求める (オクターブ間の減衰率 a)
  double perlin(double x, double y, double z, int o, double a) const;

  // (x, y, z) における o オクターブの Turbulence ノイズを求める (オクターブ間の減衰率 a)
  double turbulence(double x, double y, double z, int o, double a) const;
};
