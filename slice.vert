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
  p = vec4(pv, (float(gl_InstanceID) + 0.5) * spacing - 0.5, 1.0);

  // �X���C�X�̃e�N�X�`�����W�̓X���C�X�̒��S����� ��3 �{�Ɋg�債�Ă����]����
  t = (mt * p).xyz * 1.732 + 0.5;

  // ���f���r���[�v���W�F�N�V�����ϊ������Ă��烉�X�^���C�U�ɑ���
  gl_Position = mc * p;
}
