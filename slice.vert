#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// �e�N�X�`�����W�̕ϊ��s��
uniform mat4 mt;

// �N���b�s���O���W�n�ւ̕ϊ��s��
uniform mat4 mc;

// �X���C�X�̊Ԋu
uniform float spacing;

// ���_����
layout (location = 0) in vec2 pv;

// �X���C�X�̒��_�ʒu
out vec4 p;

// �X���C�X�̃e�N�X�`�����W
out vec3 t;

void main()
{
  // �X���C�X�� gl_InstanceID �ł��炷
  vec4 p = vec4(pv, float(gl_InstanceID * 2 + 1) * spacing - 1.0, 1.0);

  // ���[���h���W�n��̍��W�l�����߂�
  vec4 q = mt * inverse(mc) * p;

  // �e�N�X�`�����W�ɒ���
  t = q.xyz / q.w * 0.5 + 0.5;

  // �X���C�X�̓N���b�s���O��Ԃ� xy ���ʂ����ς��̑傫���� z �����ɓ��Ԋu�ɕ��ׂ�
  gl_Position = p;
}
