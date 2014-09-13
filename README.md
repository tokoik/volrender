volrender
=========

リアルタイムボリュームレンダリングの簡単なサンプルです．Oculus 
Rift DK1 を使って眺めることもできます．

##ボリュームデータ
ボリュームデータは CPU 側で作りました (main.cpp の makeVolume() 関数)．ボリュームのサイズが 32 × 32 × 32 だとデータの作成にあまり時間はかかりませんが，128 × 128 × 128 だと結構待たされます．GLSL にも noise 関数はあるので，それを使ってシェーダで作るべきなんでしょうけど，ここでは前述の以前作ったプログラムを流用しました．条件コンパイルってのは今風ではないんですけど，こらえてつかあさい．

        // 作業用メモリ
        std::vector<GLubyte> texture;
        
        // ノイズ関数を初期化する
        const Noise3 noise(5, 5, 5);
        
        // ノイズ関数を使ってテクスチャを作る
        for (GLint k = 0; k < depth; ++k)
        {
          const double z(double(k) / double(depth - 1));
          
          for (GLint j = 0; j < height; ++j)
          {
            const double y(double(j) / double(height - 1));
            
            for (GLint i = 0; i < width; ++i)
            {
              const double x(double(i) / double(width - 1));
              
    #if VOLUMETYPE == CHECKER
              texture.push_back(((i >> 4) + (j >> 4) + (k >> 4)) & 1 ? 0 : 255);
    #elif VOLUMETYPE == NOISE
              texture.push_back(GLubyte(noise.noise(x, y, z) * 255.0));
    #elif VOLUMETYPE == PERLIN
              texture.push_back(GLubyte(noise.perlin(x, y, z, 4, 0.5) * 255.0));
    #elif VOLUMETYPE == TURBULENCE
              texture.push_back(GLubyte(noise.turbulence(x, y, z, 4, 0.5) * 255.0));
    #elif VOLUMETYPE == SPHERE
              const double px(2.0 * x - 1.0), py(2.0 * y - 1.0), pz(2.0 * z - 1.0);
              texture.push_back(GLubyte(255.0 - sqrt(px * px + py * py + pz * pz) * 127.5));
    #else
              texture.push_back(255);
    #endif
            }
          }
        }

これを 3D テクスチャに突っ込みます．ポイントは GL_CLAMP_TO_BORDER を指定して，境界色のアルファ値を 0 にしておくあたりです．

        // テクスチャオブジェクトを作成して結合する
        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_3D, tex);
        
        // テクスチャを割り当てる
        glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, width, height, depth, 0,
          GL_RED, GL_UNSIGNED_BYTE, &texture[0]);
        
        // テクスチャの拡大・縮小に線形補間を用いる
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        // テクスチャからはみ出た部分には境界色を用いる
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        
        // テクスチャの境界色を設定する (ボリュームの外には何もない)
        static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, black);

##アルファブレンディング
このテクスチャをポリゴンにマッピングします．その際，ボリュームのセルの濃度をアルファ値にも設定しておきます．そしてアルファブレンディングを有効にすると，境界色のアルファ値を 0 にしているので，周囲の余計な部分が消えてボリュームテクスチャのところだけが見えるようになります．なお，アルファテストと違って，見えないところもレンダリングされています．

ポリゴンを複数枚並べるとボリューム全体を表示できそうです．アルファブレンディングを有効にすると，できそうな気がしてきます．

##スライスの描画
glDrawArraysInstanced() や glDrawElementsInstanced() を使えば，同じポリゴンを複数同時に描くことができます．ここでは，このポリゴン群をスライスと呼んでいます．

        // スライスの描画
        glBindVertexArray(slice);
        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, slices);

同じところに複数のポリゴンを描いても仕方ないので，バーテックスシェーダで描画するインスタンスごとにポリゴンの位置をずらします (slice.vert)．インスタンスは GLSL の組み込み変数 gl_InstanceID で識別できますから，これをもとにポリゴンの z 値を決定します．spacing はポリゴンの間隔で，1 / (ポリゴン数 - 1) です．gl_InstanceID に 0.5 を足しているのはテクスチャのサンプリング位置をセルの中心にするためで，ポリゴン群の中心 xy 平面上に移すために最後に 0.5 を引いています．

    #version 150 core
    #extension GL_ARB_explicit_attrib_location : enable
     
    // テクスチャ座標の変換行列
    uniform mat4 mt;
     
    // モデルビュー変換行列
    uniform mat4 mw;
     
    // プロジェクション変換行列
    uniform mat4 mp;
     
    // スライスの間隔
    uniform float spacing;
     
    // [-0.5, 0.5] の正方形の頂点位置
    layout (location = 0) in vec2 pv;
     
    // スライスの頂点位置
    out vec4 p;
     
    // スライスのテクスチャ座標
    out vec3 t;
     
    void main()
    {
      // スライスを gl_InstanceID でずらす
      p = vec4(pv, (float(gl_InstanceID) + 0.5) * spacing - 0.5, 1.0);

テクスチャ座標はスライスの頂点座標をもとに決定します．ボリュームの回転表示はスライスを回転させるのではなく，テクスチャ座標の方を回転します．回転によってボリュームがスライスからはみ出ないように，回転後のテクスチャ座標を √3 倍 (すなわちマッピングされるボリュームのサイズを 1 / √3 倍) します．

      // スライスのテクスチャ座標はスライスの中心を基準に √3 倍に拡大してから回転する
      t = (mat3(mt) * p.xyz) * 1.732 + 0.5;
      
      // 頂点位置を視点座標系に移す
      p = mw * p;
      
      // モデルビュープロジェクション変換をしてからラスタライザに送る
      gl_Position = mp * p;
    }
    
しかし，アルファブレンディングを有効にしても，並べるポリゴンの枚数が多くなると，結局中身が見えなくなってしまいます．
画像の説明
フラグメントの破棄
そこで，閾値を決めて，それを下回るアルファ値 (濃度) がマッピングされたフラグメントをフラグメントシェーダで破棄するようにします (slice.frag)．
    #version 150 core
    #extension GL_ARB_explicit_attrib_location : enable
     
    ... (中略) ...
     
    // テクスチャのサンプラ
    uniform sampler3D volume;
     
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

##濃度勾配を求める
破棄しなかったフラグメントに関しては，そのフラグメントにマッピングされるセルの位置における濃度勾配を求めます．

      // 濃度の勾配を求める
      vec4 g = vec4(
        textureOffset(volume, t, ivec3(-1, 0, 0)).r - textureOffset(volume, t, ivec3(1, 0, 0)).r,
        textureOffset(volume, t, ivec3(0, -1, 0)).r - textureOffset(volume, t, ivec3(0, 1, 0)).r,
        textureOffset(volume, t, ivec3(0, 0, -1)).r - textureOffset(volume, t, ivec3(0, 0, 1)).r,
        0.0
      );

これを正規化したあと，0.5 倍して 0.5 を足して [0, 1] の範囲に直してフラグメントの色に使ってみます．

      // 勾配をそのままフラグメントの色に使う
      fc = vec4(normalize(g.xyz) * 0.5 + 0.5, v);

##陰影付け
濃度勾配はそのまま法線ベクトルとして使えるので，それを使って陰影付けを行います．

      vec3 l = normalize((pl * p.w - p * pl.w).xyz);  // 光線ベクトル
      vec3 n = normalize(g.xyz * mat3(mt));           // 法線ベクトル
      vec3 h = normalize(l - normalize(p.xyz));       // 中間ベクトル
      
      // 拡散反射光＋環境光の反射光
      vec4 idiff = max(dot(n, l), 0.0) * kdiff * ldiff + kamb * lamb;
      
      // 鏡面反射光
      vec4 ispec = pow(max(dot(n, h), 0.0), kshi) * kspec * lspec;
      
      // フラグメントの色
      fc = vec4((idiff + ispec).rgb, v);
    }

キーボードの B のキーでアルファブレンディングの有効／無効を切り替えられます
