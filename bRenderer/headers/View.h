#ifndef B_VIEW_H
#define B_VIEW_H

#include "OSdetect.h"
#include <memory>
#include "Renderer_GL.h"
#include "Logger.h"
#include "Configuration.h"

#ifdef __OBJC__
#import "../os_specific/ios/BView.h"
#endif

/** @brief An abstraction of the view the scene is drawn into
*
*	The view class represents a window on desktop systems or a UIView on iOS. 
*	The view possesses an OpenGL context and allows for a scene to be displayed to the screen.
*
*	@author Benjamin Buergisser
*/
class View
{
public:

#ifdef B_OS_IOS
	/* Typedefs */
	typedef int GLFWwindow;
#endif
#ifndef __OBJC__
    typedef int UIView;
    typedef int BView;
#endif

	/* Functions */

	/**	@brief Constructor
	*/
	View();
	
	/**	@brief Virtual destructor
	*/
	virtual ~View();

	/**	@brief Do all necessary initializations for the view to be ready to run (returns true in the case of success)
	*	@param[in] windowTitle The title for the window on desktop systems (optional)
	*/
	bool initView(std::string windowTitle = bRenderer::DEFAULT_WINDOW_TITLE());

	/**	@brief Do all necessary initializations for the view to be ready to run (returns true in the case of success)
	*	@param[in] fullscreen Decides whether or not the application runs in full screen mode
	*	@param[in] windowTitle The title for the window on desktop systems (optional)
	*/
	bool initView(bool fullscreen, std::string windowTitle = bRenderer::DEFAULT_WINDOW_TITLE());

	/**	@brief Do all necessary initializations for the view to be ready to run (returns true in the case of success)
	*	@param[in] width The width of the view in pixels
	*	@param[in] height The height of the view in pixels
	*	@param[in] fullscreen Decides whether or not the application runs in full screen mode (optional)
	*	@param[in] windowTitle The title for the window on desktop systems (optional)
	*/
	bool initView(GLint width, GLint height, bool fullscreen = false, std::string windowTitle = bRenderer::DEFAULT_WINDOW_TITLE());

	/**	@brief Terminate the view
	*/
	void terminateView();

	/**	@brief Returns true if the view has already been initialized
	*/
	bool isInitialized();

	/**	@brief Returns true if the view is running
	*/
	bool isRunning();

	/**	@brief Returns true if the view runs in full screen mode
	*/
	bool isFullscreen();

	/**	@brief Returns the width of the view in pixels
	*/
	GLint getWidth();

	/**	@brief Returns the height of the view in pixels
	*/
	GLint getHeight();

	/**	@brief Gets the size of the view in pixels
	*	@param[out] width The width of the view
	*	@param[out] height The height of the view
	*/
	void getSize(GLint* width, GLint* height);

	/**	@brief Returns the width of the screen in pixels
	*/
	static GLint getScreenWidth();

	/**	@brief Returns the height of the screen in pixels
	*/
	static GLint getScreenHeight();

	/**	@brief Gets the size of the screen in pixels
	*	@param[out] width The width of the screen
	*	@param[out] height The height of the screen
	*/
	static void getScreenSize(GLint* width, GLint* height);

	/**	@brief Returns the width of the OpenGL viewport in pixels
	*/
	GLint getViewportWidth();

	/**	@brief Returns the height of the OpenGL viewport in pixels
	*/
	GLint getViewportHeight();

	/**	@brief Gets the size of the OpenGL viewport in pixels
	*	@param[out] width The width in pixels
	*	@param[out] height The height in pixels
	*/
	void getViewportSize(GLint* width, GLint* height);

	/**	@brief Returns the aspect ratio of the view
	*/
	GLfloat getAspectRatio();

	/**	@brief Returns the x position of the view in pixels
	*/
	GLint getPositionX();

	/**	@brief Returns the y position of the view in pixels
	*/
	GLint getPositionY();

	/**	@brief Gets the position of the upper left corner of the view
	*	@param[out] x The x position of the upper left corner of the view
	*	@param[out] y The y position of the upper left corner of the view
	*/
	void getPosition(GLint* x, GLint* y);

	/**	@brief Returns the GLFW window
	*
	*	This function returns the window created using GLFW.
	*	This is useful if GLFW functionality is used within the application.
    *   On iOS this function returns an integer set to 0.
	*/
	GLFWwindow* getWindow();
    
    /**	@brief Returns the UIView on iOS (only usable in Objective C)
     */
    BView* getUIView();
    
    /**	@brief Attaches (adds as a subview) the underlying UIView to a UIView of your choosing
     *	@param[in] view The UIView to attach the view to
     */
    void attachToUIView(UIView* view);

	/**	@brief Set fullscreen mode
	*	@param[in] fullscreen Set true if full screen is desired
	*/
	void setFullscreen(bool fullscreen);

	/**	@brief Sets the width of the view in pixels
	*	@param[in] width The width in pixels
	*/
	void setWidth(GLint width);

	/**	@brief Sets the height of the view in pixels
	*	@param[in] height The height in pixels
	*/
	void setHeight(GLint height);

	/**	@brief Sets the size of the view in pixels
	*	@param[in] width The width in pixels
	*	@param[in] height The height in pixels
	*/
	void setSize(GLint width, GLint height);

	/**	@brief Sets the size of the OpenGL viewport in pixels
	*	@param[in] width The width in pixels
	*	@param[in] height The height in pixels
	*/
	void setViewportSize(GLint width, GLint height);

	/**	@brief Sets the position of the upper left corner of the view
	*	@param[in] x The x position of the upper left corner of the view
	*	@param[in] y The y position of the upper left corner of the view
	*/
	void setPosition(GLint x, GLint y);

	/**	@brief Sets the context of this view to be current
	*/
	void setContextCurrent();
    
    /**	@brief Bind the framebuffer (on iOS only)
     */
    void bindFramebuffer();

	/**	@brief Clears the screen
	*/
	static void clearScreen();

	/**	@brief Displays the buffer to the screen
	*/
	void presentBuffer();

private:
	/* Functions */
    
	/**	@brief Gets called whenever the window size is changed (desktop only)
	*	@param[in] window The window that got changed
	*	@param[in] width The width in pixels
	*	@param[in] height The height in pixels
	*/
	static void windowSizeChanged(GLFWwindow* window, int width, int height);
    
	/* Variables */

	GLFWwindow  *_window = nullptr;
	BView       *_view = nullptr;

	bool _initialized = false;
	bool _fullscreen = false;

};

typedef std::shared_ptr< View >  ViewPtr;


#endif /* defined(B_VIEW_H) */
