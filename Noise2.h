#pragma once
//
// 二次元のノイズ関数
//

#include "Noise1.h"

// 二次元のノイズ関数
class Noise2
{
  int m;                                          // 節点数
  Noise1 **p;                                     // 各節点の値
  void erase();                               // データの消去
  void copy(const Noise2 &noise);                 // データのコピー

public:

  // コンストラクタ
  Noise2() { p = 0; }
  Noise2(int n, int m);
  Noise2(const Noise2 &noise) { copy(noise); }

  // デストラクタ
  virtual ~Noise2() { erase(); }

  // 代入演算子
  Noise2 &operator=(const Noise2 &noise);

  // 節点 (i, j) の値を得る
  double point(int i, int j) const { return p[j]->point(i); }

  // (x, y) における補間値を求める (0 ≦ x ≦ n, 0 ≦ y ≦ m)
  double noise(double x, double y) const;

  // (x, y) における o オクターブの Perlin ノイズを求める (オクターブ間の減衰率 a)
  double perlin(double p, double q, int o, double a) const;

  // (x, y) における o オクターブの Turbulence ノイズを求める (オクターブ間の減衰率 a)
  double turbulence(double x, double y, int o, double a) const;
};
