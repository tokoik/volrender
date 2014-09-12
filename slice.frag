#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// 光源
const vec4 lamb = vec4(0.4f, 0.4f, 0.4f, 1.0f);         // 環境光成分
const vec4 ldiff = vec4(0.8f, 0.8f, 0.8f, 1.0f);        // 拡散反射光成分
const vec4 lspec = vec4(0.8f, 0.8f, 0.8f, 1.0f);        // 鏡面反射光成分
const vec4 pl = vec4(0.0f, 0.5f, 1.0f, 0.0f);           // 位置

// 材質
const vec4 kamb = vec4(0.8f, 0.8f, 0.4f, 1.0f);         // 環境光の反射係数
const vec4 kdiff = vec4(0.6f, 0.6f, 0.3f, 1.0f);        // 拡散反射係数
const vec4 kspec = vec4(0.4f, 0.4f, 0.2f, 1.0f);        // 鏡面反射係数
const float kshi = 50.0f;                               // 輝き係数

// テクスチャのサンプラ
uniform sampler3D volume;

// テクスチャ座標の変換行列
uniform mat4 mt;

// クリッピング座標系への変換行列
uniform mat4 mc;

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
  vec4 g = vec4(
    textureOffset(volume, t, ivec3(-1, 0, 0)).r - textureOffset(volume, t, ivec3(1, 0, 0)).r,
    textureOffset(volume, t, ivec3(0, -1, 0)).r - textureOffset(volume, t, ivec3(0, 1, 0)).r,
    textureOffset(volume, t, ivec3(0, 0, -1)).r - textureOffset(volume, t, ivec3(0, 0, 1)).r,
    0.0
  );

#if 1
  vec3 e = normalize((p * mc[3].w - mc[3] * p.w).xyz);  // 視線ベクトル
  vec3 l = normalize((pl * p.w - p * pl.w).xyz);        // 光線ベクトル
  vec3 n = normalize((g * mt).xyz);                     // 法線ベクトル
  vec3 h = normalize(l - e);                            // 中間ベクトル

  // 拡散反射光＋環境光の反射光
  vec4 idiff = max(dot(n, l), 0.0) * kdiff * ldiff + kamb * lamb;

  // 鏡面反射光
  vec4 ispec = pow(abs(dot(n, h)), kshi) * kspec * lspec;

  // フラグメントの色
  fc = vec4((idiff + ispec).rgb, v);
#else
  // 勾配をそのままフラグメントの色に使う
  fc = vec4(g.rgb, v);
#endif
}
