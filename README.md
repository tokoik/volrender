# volrender - 3D テクスチャによるボリュームレンダリング

## 1. 概要

本プログラムは、3D テクスチャのスライスおよび勾配計算を用いて、プロシージャルノイズ（パーリンノイズ、タービュランスなど）の 3 次元空間データをリアルタイムにボリュームレンダリングするサンプルプログラムです。

- 移行元ブログ記事:
  - [ボリュームレンダリング (volrender) - 床井研究室](https://tokoik.github.io/blog/2014-09-13.html)

## 2. 対応環境

- **Windows**: Visual Studio 2019 / 2022 / 2026 (CMake 経由で GLFW 3.4 を自動ダウンロード)
- **macOS**: Xcode (GLFW 3.4 を自動ダウンロード、OpenGL Framework を使用)
- **Ubuntu Linux**: GCC / Make (システム標準の libglfw3-dev, libgl1-mesa-dev を使用)

## 3. ビルド手順

### Windows (Visual Studio)

```pwsh
cmake -B build -S .
cmake --build build --config Release
```

### macOS (Xcode)

```bash
cmake -B build -G Xcode
cmake --build build --config Release
```

### Ubuntu Linux (Makefile)

```bash
sudo apt-get update
sudo apt-get install -y libglfw3-dev libgl1-mesa-dev
cmake -B build -S .
cmake --build build
```

## 4. 起動方法

ビルド完了後、生成された実行ファイルを実行します。

- **Windows**: `build/Release/volrender.exe`
- **macOS**: `build/Release/volrender.app`
- **Linux**: `build/volrender`

## 5. 操作方法

- **マウス左ドラッグ**: ボリュームの回転
- **マウス右ドラッグ**: 視点の平行移動
- **マウスホイール**: ズーム
- **[q] / [Q] / [ESC]**: プログラムの終了

## 6. プログラムの解説

`Noise1.cpp`〜`Noise3.cpp` でプロシージャルに生成した 3D テクスチャを `slice.vert` および `slice.frag` で視線方向に垂直な平面群としてスライス描画し、ボリュームレンダリングを実現しています。
