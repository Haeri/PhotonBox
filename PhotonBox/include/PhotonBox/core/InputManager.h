#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <vector>

#include "PhotonBox/math/Vector2f.h"
#include "PhotonBox/core/Display.h"

class InputManager
{
public:
	enum CursorMode
	{
		HIDDEN = GLFW_CURSOR_HIDDEN,
		NORMAL = GLFW_CURSOR_NORMAL,
		DISABLED = GLFW_CURSOR_DISABLED,
	};

	static const int KEY_UNKNOWN = -1;
	static const int KEY_SPACE = 32;
	static const int KEY_APOSTROPHE = 39;	/* ' */
	static const int KEY_COMMA = 44;		/* , */
	static const int KEY_MINUS = 45;		/* - */
	static const int KEY_PERIOD = 46;		/* . */
	static const int KEY_SLASH = 47;		/* / */
	static const int KEY_0 = 48;
	static const int KEY_1 = 49;
	static const int KEY_2 = 50;
	static const int KEY_3 = 51;
	static const int KEY_4 = 52;
	static const int KEY_5 = 53;
	static const int KEY_6 = 54;
	static const int KEY_7 = 55;
	static const int KEY_8 = 56;
	static const int KEY_9 = 57;
	static const int KEY_SEMICOLON = 59;	/* ; */
	static const int KEY_EQUAL = 61;		/* = */
	static const int KEY_A = 65;
	static const int KEY_B = 66;
	static const int KEY_C = 67;
	static const int KEY_D = 68;
	static const int KEY_E = 69;
	static const int KEY_F = 70;
	static const int KEY_G = 71;
	static const int KEY_H = 72;
	static const int KEY_I = 73;
	static const int KEY_J = 74;
	static const int KEY_K = 75;
	static const int KEY_L = 76;
	static const int KEY_M = 77;
	static const int KEY_N = 78;
	static const int KEY_O = 79;
	static const int KEY_P = 80;
	static const int KEY_Q = 81;
	static const int KEY_R = 82;
	static const int KEY_S = 83;
	static const int KEY_T = 84;
	static const int KEY_U = 85;
	static const int KEY_V = 86;
	static const int KEY_W = 87;
	static const int KEY_X = 88;
	static const int KEY_Y = 89;
	static const int KEY_Z = 90;
	static const int KEY_LEFT_BRACKET = 91;	/* [ */
	static const int KEY_BACKSLASH = 92;	/* \ */
	static const int KEY_RIGHT_BRACKET = 93; /* ] */
	static const int KEY_GRAVE_ACCENT = 96;	/* ` */
	static const int KEY_WORLD_1 = 161;		/* non-US #1 */
	static const int KEY_WORLD_2 = 162;		/* non-US #2 */
	static const int KEY_ESCAPE = 256;
	static const int KEY_ENTER = 257;
	static const int KEY_TAB = 258;
	static const int KEY_BACKSPACE = 259;
	static const int KEY_INSERT = 260;
	static const int KEY_DELETE = 261;
	static const int KEY_RIGHT = 262;
	static const int KEY_LEFT = 263;
	static const int KEY_DOWN = 264;
	static const int KEY_UP = 265;
	static const int KEY_PAGE_UP = 266;
	static const int KEY_PAGE_DOWN = 267;
	static const int KEY_HOME = 268;
	static const int KEY_END = 269;
	static const int KEY_CAPS_LOCK = 280;
	static const int KEY_SCROLL_LOCK = 281;
	static const int KEY_NUM_LOCK = 282;
	static const int KEY_PRINT_SCREEN = 283;
	static const int KEY_PAUSE = 284;
	static const int KEY_F1 = 290;
	static const int KEY_F2 = 291;
	static const int KEY_F3 = 292;
	static const int KEY_F4 = 293;
	static const int KEY_F5 = 294;
	static const int KEY_F6 = 295;
	static const int KEY_F7 = 296;
	static const int KEY_F8 = 297;
	static const int KEY_F9 = 298;
	static const int KEY_F10 = 299;
	static const int KEY_F11 = 300;
	static const int KEY_F12 = 301;
	static const int KEY_F13 = 302;
	static const int KEY_F14 = 303;
	static const int KEY_F15 = 304;
	static const int KEY_F16 = 305;
	static const int KEY_F17 = 306;
	static const int KEY_F18 = 307;
	static const int KEY_F19 = 308;
	static const int KEY_F20 = 309;
	static const int KEY_F21 = 310;
	static const int KEY_F22 = 311;
	static const int KEY_F23 = 312;
	static const int KEY_F24 = 313;
	static const int KEY_F25 = 314;
	static const int KEY_KP_0 = 320;
	static const int KEY_KP_1 = 321;
	static const int KEY_KP_2 = 322;
	static const int KEY_KP_3 = 323;
	static const int KEY_KP_4 = 324;
	static const int KEY_KP_5 = 325;
	static const int KEY_KP_6 = 326;
	static const int KEY_KP_7 = 327;
	static const int KEY_KP_8 = 328;
	static const int KEY_KP_9 = 329;
	static const int KEY_KP_DECIMAL = 330;
	static const int KEY_KP_DIVIDE = 331;
	static const int KEY_KP_MULTIPLY = 332;
	static const int KEY_KP_SUBTRACT = 333;
	static const int KEY_KP_ADD = 334;
	static const int KEY_KP_ENTER = 335;
	static const int KEY_KP_EQUAL = 336;
	static const int KEY_LEFT_SHIFT = 340;
	static const int KEY_LEFT_CONTROL = 341;
	static const int KEY_LEFT_ALT = 342;
	static const int KEY_LEFT_SUPER = 343;
	static const int KEY_RIGHT_SHIFT = 344;
	static const int KEY_RIGHT_CONTROL = 345;
	static const int KEY_RIGHT_ALT = 346;
	static const int KEY_RIGHT_SUPER = 347;
	static const int KEY_MENU = 348;

	void init();
	void update();
	void pollEvents();

	static bool keyPressed(int key);
	static bool keyDown(int key);
	static bool keyReleased(int key);
	static Vector2f getMouse();
	static Vector2f getMouseDelta();
	static void setCursorMode(CursorMode mode);
	static CursorMode getCursorMode();

	static void handleKey(int key, int action);
	static void handleMouse(float x, float y);
private:
	static std::vector<int> _keyPress;
	static std::vector<int> _keyDown;
	static std::vector<int> _keyRelease;
	static float _xPos, _yPos, _xPosLast, _yPosLast;
};
#endif // INPUT_MANAGER_H
