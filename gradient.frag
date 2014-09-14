#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// �e�N�X�`���̃T���v��
uniform sampler3D tex;

// �e�N�X�`�����W
in vec3 t;

// �t���[���o�b�t�@�ɏo�͂���f�[�^
layout (location = 0) out vec4 fc;

void main(void)
{
  vec3 g = vec3(
    textureOffset(tex, t, ivec3(-1, 0, 0)).r - textureOffset(tex, t, ivec3(1, 0, 0)).r,
    textureOffset(tex, t, ivec3(0, -1, 0)).r - textureOffset(tex, t, ivec3(0, 1, 0)).r,
    textureOffset(tex, t, ivec3(0, 0, -1)).r - textureOffset(tex, t, ivec3(0, 0, 1)).r
  );

  // �t���O�����g�Ɍ��z���i�[����
  fc = vec4(normalize(g) * 0.5 + 0.5, length(g));
}
