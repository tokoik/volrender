#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// �e�N�X�`�����W�̕ϊ��s��
uniform mat4 mt;

// ���f���r���[�ϊ��s��
uniform mat4 mw;

// �v���W�F�N�V�����ϊ��s��
uniform mat4 mp;

// �X���C�X�̊Ԋu
uniform float spacing;

// [-0.5, 0.5] �̐����`�̒��_�ʒu
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
  t = (mat3(mt) * p.xyz) * 1.732 + 0.5;

  // ���_�ʒu�����_���W�n�Ɉڂ�
  p = mw * p;

  // ���f���r���[�v���W�F�N�V�����ϊ������Ă��烉�X�^���C�U�ɑ���
  gl_Position = mp * p;
}
