#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// 光源
const vec4 lamb = vec4(0.4, 0.4, 0.4, 1.0);         // 環境光成分
const vec4 ldiff = vec4(0.8, 0.8, 0.8, 1.0);        // 拡散反射光成分
const vec4 lspec = vec4(0.8, 0.8, 0.8, 1.0);        // 鏡面反射光成分
const vec4 pl = vec4(0.0, 0.5, 1.0, 0.0);           // 位置

// 材質
const vec4 kamb = vec4(0.8, 0.8, 0.4, 1.0);         // 環境光の反射係数
const vec4 kdiff = vec4(0.6, 0.6, 0.3, 1.0);        // 拡散反射係数
const vec4 kspec = vec4(0.4, 0.4, 0.2, 1.0);        // 鏡面反射係数
const float kshi = 50.0;                               // 輝き係数

// テクスチャのサンプラ
uniform sampler3D volume;

// テクスチャ座標の変換行列
uniform mat4 mt;

// 閾値
uniform float threshold;

// スライスの表面上の位置
in vec4 p;

// テクスチャ座標
in vec3 t;

// フレームバッファに出力するデータ
layout (location = 0) out vec4 fc;

void main()
{
  // 元のボリュームの濃度と閾値の差
  float v = texture(volume, t).r - threshold;

  // 濃度が閾値以下ならフラグメントを捨てる
  if (v <= 0.0) discard;

  // 濃度の勾配を求める
  vec3 g = vec3(
    textureOffset(volume, t, ivec3(-1, 0, 0)).r - textureOffset(volume, t, ivec3(1, 0, 0)).r,
    textureOffset(volume, t, ivec3(0, -1, 0)).r - textureOffset(volume, t, ivec3(0, 1, 0)).r,
    textureOffset(volume, t, ivec3(0, 0, -1)).r - textureOffset(volume, t, ivec3(0, 0, 1)).r
  );

#if 1
  vec3 l = normalize((pl * p.w - p * pl.w).xyz);  // 光線ベクトル
  vec3 n = normalize(g * mat3(mt));               // 法線ベクトル
  vec3 h = normalize(l - normalize(p.xyz));       // 中間ベクトル

  // 拡散反射光＋環境光の反射光
  vec4 idiff = max(dot(n, l), 0.0) * kdiff * ldiff + kamb * lamb;

  // 鏡面反射光
  vec4 ispec = pow(max(dot(n, h), 0.0), kshi) * kspec * lspec;

  // フラグメントの色
  fc = vec4((idiff + ispec).rgb, v);
#else
  // 勾配をそのままフラグメントの色に使う
  fc = vec4(normalize(g) * 0.5 + 0.5, v);
#endif
}
