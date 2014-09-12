#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// テクスチャ座標の変換行列
uniform mat4 mt;

// クリッピング座標系への変換行列
uniform mat4 mc;

// スライスの間隔
uniform float spacing;

// 頂点属性
layout (location = 0) in vec2 pv;

// スライスの頂点位置
out vec4 p;

// スライスのテクスチャ座標
out vec3 t;

void main()
{
  // スライスを gl_InstanceID でずらす
  vec4 p = vec4(pv, float(gl_InstanceID * 2 + 1) * spacing - 1.0, 1.0);

  // ワールド座標系上の座標値を求める
  vec4 q = mt * inverse(mc) * p;

  // テクスチャ座標に直す
  t = q.xyz / q.w * 0.5 + 0.5;

  // スライスはクリッピング空間の xy 平面いっぱいの大きさで z 方向に等間隔に並べる
  gl_Position = p;
}
