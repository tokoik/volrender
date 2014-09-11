#pragma once
//
// 一次元のノイズ関数
//

// 3 次 Catmull-Rom Spline 補間
extern double catmull_rom(double x0, double x1, double x2, double x3, double t);

// 一次元のノイズ関数
class Noise1
{
  int n;                                          // 節点数
  double *p;                                      // 各節点の値
  void copy(const Noise1 &noise);                 // データのコピー

public:

  // コンストラクタ
  Noise1() { p = 0; }
  Noise1(int n);
  Noise1(const Noise1 &noise) { copy(noise); }

  // デストラクタ
  virtual ~Noise1() { delete[] p; }

  // 代入演算子
  Noise1 &operator=(const Noise1 &noise);

  // 節点 i の値を得る
  double point(int i) const { return p[i]; }

  // x における補間値を求める (0 ≦ x ≦ n)
  double noise(double x) const;

  // x における o オクターブの Perlin ノイズを求める (オクターブ間の減衰率 a)
  double perlin(double x, int o, double a) const;

  // x における o オクターブの Turbulence ノイズを求める (オクターブ間の減衰率 a)
  double turbulence(double x, int o, double a) const;
};
