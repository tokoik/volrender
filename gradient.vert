#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// ���C���̈ʒu
uniform float z;

// ���_����
layout (location = 0) in vec2 pv;

// �X���C�X�̃e�N�X�`�����W
out vec3 t;

void main()
{
  t = vec3(pv, z);
  gl_Position = vec4(t * 2.0 - 1.0, 1.0);
}
