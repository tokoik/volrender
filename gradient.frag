#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// �e�N�X�`���̃T���v��
uniform sampler3D tex;

// �e�N�X�`�����W
in vec3 t;

// �t���[���o�b�t�@�ɏo�͂���f�[�^
layout (location = 0) out vec4 fc;

void main()
{
  vec3 v;
  v.x = textureOffset(tex, t, ivec3(-1, 0, 0)).a - textureOffset(tex, t, ivec3(1, 0, 0)).a;
  v.y = textureOffset(tex, t, ivec3(0, -1, 0)).a - textureOffset(tex, t, ivec3(0, 1, 0)).a;
  v.z = textureOffset(tex, t, ivec3(0, 0, -1)).a - textureOffset(tex, t, ivec3(0, 0, 1)).a;

  fc = vec4(normalize(v) * 0.5 + 0.5, length(v));
}
