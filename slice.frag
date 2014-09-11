#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// テクスチャのサンプラ
uniform sampler3D vtex;

// テクスチャの勾配テクスチャのサンプラ
uniform sampler3D gtex;

// 閾値
uniform float threshold;

// テクスチャ座標
in vec3 t;

// フレームバッファに出力するデータ
layout (location = 0) out vec4 fc;

void main()
{
  // 元のボリュームの濃度と閾値の差
  float v = texture(vtex, t).a - threshold;

  // 濃度が閾値を下回っていたらフラグメントを捨てる
  if (v < 0.0) discard;

  // ボリュームの勾配
  vec4 n = texture(gtex, t);

  // 勾配をそのままフラグメントの色に使う
  fc = vec4(n.rgb, v);
}
