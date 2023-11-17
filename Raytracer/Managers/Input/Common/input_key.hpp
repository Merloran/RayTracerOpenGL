#pragma once
#undef DELETE
#include "GLFW/glfw3.h"
enum class EInputKey
{
	UNKNOWN,

	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	SPACE,
	ENTER,
	ESCAPE,
	BACKSPACE,
	DELETE,
	TAB,
	LEFT_CONTROL,
	RIGHT_CONTROL,
	LEFT_SHIFT,
	RIGHT_SHIFT,
	LEFT_ALT,
	RIGHT_ALT,
	UP,
	DOWN,
	LEFT,
	RIGHT,

	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,

	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_MIDDLE
};

static EInputKey s_glfw_mouse_button_to_input_key(const Int32 button) {
	switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
		{
			return EInputKey::MOUSE_LEFT;
		}
		case GLFW_MOUSE_BUTTON_RIGHT:
		{
			return EInputKey::MOUSE_RIGHT;
		}
		case GLFW_MOUSE_BUTTON_MIDDLE:
		{
			return EInputKey::MOUSE_MIDDLE;
		}
		default:
		{
			return EInputKey::UNKNOWN;
		}
	}
}

static EInputKey s_glfw_key_to_input_key(const Int32 key) {
	switch (key) {
		case GLFW_KEY_A:
		{
			return EInputKey::A;
		}
		case GLFW_KEY_B:
		{
			return EInputKey::B;
		}
		case GLFW_KEY_C:
		{
			return EInputKey::C;
		}
		case GLFW_KEY_D:
		{
			return EInputKey::D;
		}
		case GLFW_KEY_E:
		{
			return EInputKey::E;
		}
		case GLFW_KEY_F:
		{
			return EInputKey::F;
		}
		case GLFW_KEY_G:
		{
			return EInputKey::G;
		}
		case GLFW_KEY_H:
		{
			return EInputKey::H;
		}
		case GLFW_KEY_I:
		{
			return EInputKey::I;
		}
		case GLFW_KEY_J:
		{
			return EInputKey::J;
		}
		case GLFW_KEY_K:
		{
			return EInputKey::K;
		}
		case GLFW_KEY_L:
		{
			return EInputKey::L;
		}
		case GLFW_KEY_M:
		{
			return EInputKey::M;
		}
		case GLFW_KEY_N:
		{
			return EInputKey::N;
		}
		case GLFW_KEY_O:
		{
			return EInputKey::O;
		}
		case GLFW_KEY_P:
		{
			return EInputKey::P;
		}
		case GLFW_KEY_Q:
		{
			return EInputKey::Q;
		}
		case GLFW_KEY_R:
		{
			return EInputKey::R;
		}
		case GLFW_KEY_S:
		{
			return EInputKey::S;
		}
		case GLFW_KEY_T:
		{
			return EInputKey::T;
		}
		case GLFW_KEY_U:
		{
			return EInputKey::U;
		}
		case GLFW_KEY_V:
		{
			return EInputKey::V;
		}
		case GLFW_KEY_W:
		{
			return EInputKey::W;
		}
		case GLFW_KEY_X:
		{
			return EInputKey::X;
		}
		case GLFW_KEY_Y:
		{
			return EInputKey::Y;
		}
		case GLFW_KEY_Z:
		{
			return EInputKey::Z;
		}
		case GLFW_KEY_SPACE:
		{
			return EInputKey::SPACE;
		}
		case GLFW_KEY_ENTER:
		{
			return EInputKey::ENTER;
		}
		case GLFW_KEY_ESCAPE:
		{
			return EInputKey::ESCAPE;
		}
		case GLFW_KEY_BACKSPACE:
		{
			return EInputKey::BACKSPACE;
		}
		case GLFW_KEY_DELETE:
		{
			return EInputKey::DELETE;
		}
		case GLFW_KEY_TAB:
		{
			return EInputKey::TAB;
		}
		case GLFW_KEY_LEFT_CONTROL:
		{
			return EInputKey::LEFT_CONTROL;
		}
		case GLFW_KEY_RIGHT_CONTROL:
		{
			return EInputKey::RIGHT_CONTROL;
		}
		case GLFW_KEY_LEFT_SHIFT:
		{
			return EInputKey::LEFT_SHIFT;
		}
		case GLFW_KEY_RIGHT_SHIFT:
		{
			return EInputKey::RIGHT_SHIFT;
		}
		case GLFW_KEY_LEFT_ALT:
		{
			return EInputKey::LEFT_ALT;
		}
		case GLFW_KEY_RIGHT_ALT:
		{
			return EInputKey::RIGHT_ALT;
		}
		case GLFW_KEY_UP:
		{
			return EInputKey::UP;
		}
		case GLFW_KEY_DOWN:
		{
			return EInputKey::DOWN;
		}
		case GLFW_KEY_LEFT:
		{
			return EInputKey::LEFT;
		}
		case GLFW_KEY_RIGHT:
		{
			return EInputKey::RIGHT;
		}
		case GLFW_KEY_F1:
		{
			return EInputKey::F1;
		}
		case GLFW_KEY_F2:
		{
			return EInputKey::F2;
		}
		case GLFW_KEY_F3:
		{
			return EInputKey::F3;
		}
		case GLFW_KEY_F4:
		{
			return EInputKey::F4;
		}
		case GLFW_KEY_F5:
		{
			return EInputKey::F5;
		}
		case GLFW_KEY_F6:
		{
			return EInputKey::F6;
		}
		case GLFW_KEY_F7:
		{
			return EInputKey::F7;
		}
		case GLFW_KEY_F8:
		{
			return EInputKey::F8;
		}
		case GLFW_KEY_F9:
		{
			return EInputKey::F9;
		}
		case GLFW_KEY_F10:
		{
			return EInputKey::F10;
		}
		case GLFW_KEY_F11:
		{
			return EInputKey::F11;
		}
		case GLFW_KEY_F12:
		{
			return EInputKey::F12;
		}
		default:
		{
			return EInputKey::UNKNOWN;
		}
	}
}