#ifndef B_INPUT_H
#define B_INPUT_H

#include "OSdetect.h"
#include <memory>
#include "Renderer_GL.h"
#include "Logger.h"
#include "View.h"
#include "Touch.h"

#ifdef __OBJC__
#import <CoreMotion/CoreMotion.h>
#endif

#ifndef __OBJC__
typedef int CMMotionManager;
#endif

namespace bRenderer
{
	const GLint INPUT_RELEASE = 0;		// The key or mouse button was pressed
	const GLint INPUT_PRESS = 1;		// The key or mouse button was released
	const GLint INPUT_REPEAT = 2;		// The key was held down until it repeated
	const GLint INPUT_UNDEFINED = -1;	// Will be returned if the OS doesn't support the input method

	const GLint LEFT_MOUSE_BUTTON = 1;
	const GLint RIGHT_MOUSE_BUTTON = 2;
	const GLint MIDDLE_MOUSE_BUTTON = 3;

	const GLint KEY_SPACE = 32;
	const GLint KEY_APOSTROPHE = 39;		// '
	const GLint KEY_COMMA = 44;				// ,
	const GLint KEY_MINUS = 45;				// -
	const GLint KEY_PERIOD = 46;			// .
	const GLint KEY_SLASH = 47;				// /
	const GLint KEY_0 = 48;
	const GLint KEY_1 = 49;
	const GLint KEY_2 = 50;
	const GLint KEY_3 = 51;
	const GLint KEY_4 = 52;
	const GLint KEY_5 = 53;
	const GLint KEY_6 = 54;
	const GLint KEY_7 = 55;
	const GLint KEY_8 = 56;
	const GLint KEY_9 = 57;
	const GLint KEY_SEMICOLON = 59;
	const GLint KEY_EQUAL = 61;
	const GLint KEY_A = 65;
	const GLint KEY_B = 66;
	const GLint KEY_C = 67;
	const GLint KEY_D = 68;
	const GLint KEY_E = 69;
	const GLint KEY_F = 70;
	const GLint KEY_G = 71;
	const GLint KEY_H = 72;
	const GLint KEY_I = 73;
	const GLint KEY_J = 74;
	const GLint KEY_K = 75;
	const GLint KEY_L = 76;
	const GLint KEY_M = 77;
	const GLint KEY_N = 78;
	const GLint KEY_O = 79;
	const GLint KEY_P = 80;
	const GLint KEY_Q = 81;
	const GLint KEY_R = 82;
	const GLint KEY_S = 83;
	const GLint KEY_T = 84;
	const GLint KEY_U = 85;
	const GLint KEY_V = 86;
	const GLint KEY_W = 87;
	const GLint KEY_X = 88;
	const GLint KEY_Y = 89;
	const GLint KEY_Z = 90;
	const GLint KEY_LEFT_BRACKET = 91;		// [
	const GLint KEY_BACKSLASH = 92;			// \ 
	const GLint KEY_RIGHT_BRACKET = 93;		// ]
	const GLint KEY_GRAVE_ACCENT = 96;		// `
	const GLint KEY_WORLD_1 = 161;			// non-US #1 
	const GLint KEY_WORLD_2 = 162;			// non-US #2
	const GLint KEY_ESCAPE = 256;
	const GLint KEY_ENTER = 257;
	const GLint KEY_TAB = 258;
	const GLint KEY_BACKSPACE = 259;
	const GLint KEY_INSERT = 260;
	const GLint KEY_DELETE = 261;
	const GLint KEY_RIGHT = 262;
	const GLint KEY_LEFT = 263;
	const GLint KEY_DOWN = 264;
	const GLint KEY_UP = 265;
	const GLint KEY_PAGE_UP = 266;
	const GLint KEY_PAGE_DOWN = 267;
	const GLint KEY_HOME = 268;
	const GLint KEY_END = 269;
	const GLint KEY_CAPS_LOCK = 280;
	const GLint KEY_SCROLL_LOCK = 281;
	const GLint KEY_NUM_LOCK = 282;
	const GLint KEY_PRINT_SCREEN = 283;
	const GLint KEY_PAUSE = 284;
	const GLint KEY_F1 = 290;
	const GLint KEY_F2 = 291;
	const GLint KEY_F3 = 292;
	const GLint KEY_F4 = 293;
	const GLint KEY_F5 = 294;
	const GLint KEY_F6 = 295;
	const GLint KEY_F7 = 296;
	const GLint KEY_F8 = 297;
	const GLint KEY_F9 = 298;
	const GLint KEY_F10 = 299;
	const GLint KEY_F11 = 300;
	const GLint KEY_F12 = 301;
	const GLint KEY_F13 = 302;
	const GLint KEY_F14 = 303;
	const GLint KEY_F15 = 304;
	const GLint KEY_F16 = 305;
	const GLint KEY_F17 = 306;
	const GLint KEY_F18 = 307;
	const GLint KEY_F19 = 308;
	const GLint KEY_F20 = 309;
	const GLint KEY_F21 = 310;
	const GLint KEY_F22 = 311;
	const GLint KEY_F23 = 312;
	const GLint KEY_F24 = 313;
	const GLint KEY_F25 = 314;
	const GLint KEY_KP_0 = 320;
	const GLint KEY_KP_1 = 321;
	const GLint KEY_KP_2 = 322;
	const GLint KEY_KP_3 = 323;
	const GLint KEY_KP_4 = 324;
	const GLint KEY_KP_5 = 325;
	const GLint KEY_KP_6 = 326;
	const GLint KEY_KP_7 = 327;
	const GLint KEY_KP_8 = 328;
	const GLint KEY_KP_9 = 329;
	const GLint KEY_KP_DECIMAL = 330;
	const GLint KEY_KP_DIVIDE = 331;
	const GLint KEY_KP_MULTIPLY = 332;
	const GLint KEY_KP_SUBTRACT = 333;
	const GLint KEY_KP_ADD = 334;
	const GLint KEY_KP_ENTER = 335;
	const GLint KEY_KP_EQUAL = 336;
	const GLint KEY_LEFT_SHIFT = 340;
	const GLint KEY_LEFT_CONTROL = 341;
	const GLint KEY_LEFT_ALT = 342;
	const GLint KEY_LEFT_SUPER = 343;
	const GLint KEY_RIGHT_SHIFT = 344;
	const GLint KEY_RIGHT_CONTROL = 345;
	const GLint KEY_RIGHT_ALT = 346;
	const GLint KEY_RIGHT_SUPER = 347;

} // namespace bRenderer

/** @brief Maintenance of user inputs
*	@author Benjamin Buergisser
*/
class Input
{
public:

	/* Functions */

	/**	@brief Constructor
	*/
    Input();

	/**	@brief Constructor
	*	@param[in] view
	*/
    Input(ViewPtr view);

	/**	@brief Virtual destructor
	*/
    virtual ~Input();
    
    /**	@brief Set the view to get inputs from
     *	@param[in] view
     */
    void setView(ViewPtr view)                     { _view = view; }

	/**	@brief Gets the current state of a specified key
	*	@param[in] key
	*/
	GLint getKeyState(GLint key);

	/**	@brief Gets the current state of a specified mouse button
	*	@param[in] button Left mouse button = 1, right mouse button = 2, middle mouse button = 3
	*/
	GLint getMouseButtonState(GLint button);

	/**	@brief Returns the current x-position of the cursor
	*/
	double getCursorPositionX();

	/**	@brief Returns the current y-position of the cursor
	*/
	double getCursorPositionY();

	/**	@brief Returns the current position of the cursor
	*	@param[out] x The x position of the cursor
	*	@param[out] y The y position of the cursor
	*	@param[out] deviceHasCursor Returns true if the current device has a cursor
	*/
	void getCursorPosition(double* x, double* y, bool* deviceHasCursor = nullptr);

	/**	@brief Returns the pitch measured by the gyroscope of the mobile device
	*	@param[out] deviceHasCursor Returns true if the current device has a gyroscope
	*/
	double   getGyroscopePitch(bool* deviceHasGyroscope = nullptr);

	/**	@brief Returns the yaw measured by the gyroscope of the mobile device
	*	@param[out] deviceHasCursor Returns true if the current device has a gyroscope
	*/
	double   getGyroscopeYaw(bool* deviceHasGyroscope = nullptr);

	/**	@brief Returns the roll measured by the gyroscope of the mobile device
	*	@param[out] deviceHasCursor Returns true if the current device has a gyroscope
	*/
	double   getGyroscopeRoll(bool* deviceHasGyroscope = nullptr);
    
    /**	@brief Returns a map containing all current touches
     */
    TouchMap getTouches();
    
    /**	@brief Returns true if a single tap was recognized since last calling the function
     */
    bool singleTapRecognized();
    
    /**	@brief Returns true if a single tap was recognized since last calling the function
     */
    bool doubleTapRecognized();
    
    /**	@brief Returns the last single tap location as a touch object
     */
    Touch getLastSingleTapLocation();
    
    /**	@brief Returns the last double tap location as a touch object
     */
    Touch getLastDoubleTapLocation();

	/**	@brief Sets the cursor enabled
	*/
	void setCursorEnabled();

	/**	@brief Sets the cursor enabled
	*	@param[in] enabled Set true to enable, set false to disable
	*/
	void setCursorEnabled(bool enabled);

	/**	@brief Sets the cursor disabled
	*/
	void setCursorDisabled();

	/**	@brief Sets the position of the cursor
	*	@param[in] x The x position of the cursor
	*	@param[in] y The y position of the cursor
	*/
	void setCursorPosition(double x, double y);

	/**	@brief Returns true on mobile devices like the iPad
	*/
	static bool isTouchDevice();

private:

	/* Variables */

	ViewPtr         _view = nullptr;
	CMMotionManager *_motionManager = nullptr;
};

typedef std::shared_ptr< Input >  InputPtr;

#endif /* defined(B_INPUT_H) */
