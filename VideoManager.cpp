/** @file VideoManager.cpp */

#include "VideoManager.h"
#include "Surface.h"
#include "Color.h"
#include "FileTools.h"

/** @brief Needs Debug and a couple function implementations */

VideoManager* VideoManager::instance = NULL;

//Resolutions
//add force mode

//Make all modes available
const VideoManager::VideoMode VideoManager::forced_mode = NO_MODE;

//Size of the screen in each video mode
Rectangle VideoManager::default_mode_sizes[] =
{
	Rectangle(0, 0, KQ_SCREEN_WIDTH * 2, KQ_SCREEN_HEIGHT * 2),		// WINDOWED_STRETCHED
	Rectangle(0, 0, KQ_SCREEN_WIDTH * 2, KQ_SCREEN_HEIGHT * 2),		// WINDOWED_SCALE2X
	Rectangle(0, 0, KQ_SCREEN_WIDTH, KQ_SCREEN_HEIGHT),				// WINDOWED_NORMAL
	Rectangle(0, 0, KQ_SCREEN_WIDTH * 2, KQ_SCREEN_HEIGHT * 2),		// FULLSCREEN_NORMAL
	Rectangle(0, 0, 0, 0),											// FULLSCREEN_WIDE
	Rectangle(0, 0, KQ_SCREEN_WIDTH * 2, KQ_SCREEN_HEIGHT * 2),		// FULLSCREEN_SCALE2X
	Rectangle(0, 0, 0, 0),											// FULLSCREEN_SCALE2X_WIDE
};

//Properties of SDL surfaces
const int VideoManager::surface_flags = SDL_HWSURFACE | SDL_DOUBLEBUF;

/**
* @brief Lua name of each value of the VideoMode enum.
*/
const std::string VideoManager::video_mode_names[] = 
{
  "windowed_stretched",
  "windowed_scale2x",
  "windowed_normal",
  "fullscreen_normal",
  "fullscreen_wide",
  "fullscreen_scale2x",
  "fullscreen_scale2x_wide",
  "" // Sentinel.
};

/**
* @brief Initializes the video system and creates the window.
*
* This method should be called when the application starts.
* If the argument -no-video is provided, no window will be displayed
* but all surfaces will exist internally.
*
* @param argc command-line arguments number
* @param argv command-line arguments
*/

void VideoManager::initialize(int argc, char** argv)
{
	//check the -no-video option
	bool disable = false;
	for(argv++; argc > 1 && !disable; argv++, argc--)
	{
		const std::string arg = *argv;
		disable = (arg.find("-no-video") == 0);
	}

	instance = new VideoManager(disable);
}

/** @brief Closes the video system */

void VideoManager::quit()
{
	delete instance;
}

/** @brief Returns the video manager */

VideoManager* VideoManager::get_instance()
{
	return instance;
}

/** @brief Constructor */

VideoManager::VideoManager(bool disable_window): disable_window(disable_window), screen_surface(NULL)
{
	//initialize the window
	const std::string window_title = std::string("Kirp's Quest ") + KQ_VERSION;
	set_window_title(window_title);
	_putenv((char*) "SDL_VIDEO_CENTERED=center");
	
	//detect what widescreen resolution is supported (16:10 or 15:10)
	for (int i = 0; i < NB_MODES; i++) 
	{
		mode_sizes[i] = default_mode_sizes[i];
	}
	int flags = surface_flags | SDL_FULLSCREEN;
	if (SDL_VideoModeOK(768, 480, 32, flags)) 
	{
		mode_sizes[FULLSCREEN_WIDE].set_size(768, 480);
		mode_sizes[FULLSCREEN_SCALE2X_WIDE].set_size(768, 480);
		dst_position_wide.set_xy((768 - KQ_SCREEN_WIDTH * 2) / 2, 0);
	}
	else if (SDL_VideoModeOK(720, 480, 32, flags)) 
	{
		mode_sizes[FULLSCREEN_WIDE].set_size(720, 480);
		mode_sizes[FULLSCREEN_SCALE2X_WIDE].set_size(720, 480);
		dst_position_wide.set_xy((720 - KQ_SCREEN_WIDTH * 2) / 2, 0);
	}
	
	set_default_video_mode();
}

/** @brief Destructor */

VideoManager::~VideoManager()
{
	delete screen_surface;
}

/**
* @brief Returns whether a video mode is supported.
* @param mode a video mode
* @return true if this mode is supported
*/
bool VideoManager::is_mode_supported(VideoMode mode) 
{
  if (mode == NO_MODE) 
  {
    return false;
  }

  if (forced_mode != NO_MODE && mode != forced_mode) 
  {
    return false;
  }

  const Rectangle* size = &mode_sizes[mode];

  if (size->get_width() == 0) 
  {
    return false;
  }

  int flags = surface_flags;
  if (is_fullscreen(mode)) 
  {
    flags |= SDL_FULLSCREEN;
  }

  return SDL_VideoModeOK(size->get_width(), size->get_height(), 32, flags) != 0;
}

/**
* @brief Returns whether a video mode is in fullscreen.
* @param mode A video mode.
* @return true if this video mode is in fullscreen.
*/
bool VideoManager::is_fullscreen(VideoMode mode) 
{
  return mode >= FULLSCREEN_NORMAL;
}

/**
* @brief Returns whether the current video mode is in fullscreen.
* @return true if the current video mode is in fullscreen.
*/
bool VideoManager::is_fullscreen() 
{
  return is_fullscreen(get_video_mode());
}

/**
* @brief Sets the video mode to fullscreen or windowed,
* keeping an equivalent resolution.
* @param fullscreen true to make fullscreen.
*/
void VideoManager::set_fullscreen(bool fullscreen) 
{

  if (fullscreen != is_fullscreen()) 
  {
    switch_fullscreen();
  }
}

/**
* @brief Switches from windowed to fullscreen or from fullscreen to windowed,
* keeping an equivalent resolution.
*/
void VideoManager::switch_fullscreen() 
{
  static const VideoMode next_modes[] = 
  {
      FULLSCREEN_NORMAL, // WINDOWED_STRETCHED
      FULLSCREEN_SCALE2X, // WINDOWED_SCALE2X
      FULLSCREEN_NORMAL, // WINDOWED_NORMAL
      WINDOWED_STRETCHED, // FULLSCREEN_NORMAL
      WINDOWED_STRETCHED, // FULLSCREEN_WIDE
      WINDOWED_SCALE2X, // FULLSCREEN_SCALE2X
      WINDOWED_SCALE2X, // FULLSCREEN_SCALE2X_WIDE
  };

  VideoMode mode = next_modes[get_video_mode()];
  if (is_mode_supported(mode)) 
  {
    set_video_mode(mode);
  }
}

/**
* @brief Sets the next video mode.
*/
void VideoManager::switch_video_mode() 
{
  VideoMode mode = video_mode;
  do 
  {
    mode = (VideoMode) ((mode + 1) % NB_MODES);
  } while (!is_mode_supported(mode));
  set_video_mode(mode);
}

/**
* @brief Sets the default video mode.
*/
void VideoManager::set_default_video_mode() 
{
  VideoMode mode;
  if (forced_mode != NO_MODE) 
  {
    mode = forced_mode;
  }
  else 
  {
    mode = WINDOWED_STRETCHED;
  }

  set_video_mode(mode);
}

/**
* @brief Needs Debug; cursor disabled?
* @param mode The video mode to set.
* @return true in case of success, false if this mode is not supported.
*/
bool VideoManager::set_video_mode(VideoMode mode)
{
	if(!is_mode_supported(mode))
	{
		return false;
	}
	int flags = surface_flags;
	int show_cursor;
	if(is_fullscreen(mode))
	{
		flags |= SDL_FULLSCREEN;
		show_cursor = SDL_DISABLE;
	}
	else
	{
		show_cursor = SDL_ENABLE;
	}

	const Rectangle& size = mode_sizes[mode];
	if(size.get_width() < KQ_SCREEN_WIDTH * 2)
	{
		//Wide screen resolution with two black side bars
		offset = dst_position_wide.get_x();
	}
	else
	{
		//No side bars
		offset = 0;
	}
	width = size.get_width();
	end_row_increment = 2 * offset + width;

	if(!disable_window)
	{
		SDL_Surface* screen_internal_surface = SDL_SetVideoMode(size.get_width(), size.get_height(), KQ_COLOR_DEPTH, flags);

		//Debug::check_assertion(screen_internal_surface != NULL, StringConcat() << "Cannot create the video surface for mode " << mode);

		SDL_ShowCursor(show_cursor);
		delete this->screen_surface;
		this->screen_surface = new Surface(screen_internal_surface);
	}
	this->video_mode = mode;
	return true;
}

/**
* @brief Returns the current video mode.
* @return the video mode
*/
VideoManager::VideoMode VideoManager::get_video_mode() 
{
  return video_mode;
}

/**
* @brief Returns a list of all supported video modes.
* @return The list of supported video modes.
*/
const std::list<VideoManager::VideoMode> VideoManager::get_video_modes() 
{
  std::list<VideoManager::VideoMode> modes;
  for (int i = 0; i < NB_MODES; i++) 
  {
    VideoMode mode = VideoMode(i);
    if (is_mode_supported(mode)) 
	{
      modes.push_back(mode);
    }
  }
  return modes;
}

/**
* @brief Returns the name of a video mode.
* @param mode A video mode.
* @return The name of this mode, or an empty string if the mode is NO_MODE.
*/
std::string VideoManager::get_video_mode_name(VideoMode mode) 
{
  if (mode == NO_MODE) 
  {
    return "";
  }

  return video_mode_names[mode];
}

/**
* @brief Returns a video mode given its Lua name.
* @param mode_name Lua name of a video mode.
* @return The corresponding video mode, or NO_MODE if the name is invalid.
*/
VideoManager::VideoMode VideoManager::get_video_mode_by_name(const std::string& mode_name) 
{
  for (int i = 0; i < NB_MODES; i++) 
  {
    if (video_mode_names[i] == mode_name) 
	{
      return VideoMode(i);
    }
  }
  return NO_MODE;
}

/**
* @brief Needs Debug
* @param src_surface The source surface to draw on the screen.
*/
void VideoManager::draw(Surface& src_surface) 
{
  if (disable_window) 
  {
    return;
  }
  
  switch (video_mode) 
  {
    case WINDOWED_NORMAL:
      blit(src_surface, *screen_surface);
      break;

    case WINDOWED_STRETCHED:
    case FULLSCREEN_NORMAL:
    case FULLSCREEN_WIDE:
      blit_stretched(src_surface, *screen_surface);
      break;

    case WINDOWED_SCALE2X:
    case FULLSCREEN_SCALE2X:
    case FULLSCREEN_SCALE2X_WIDE:
      blit_scale2x(src_surface, *screen_surface);
      break;

    default:
      //Debug::die(StringConcat() << "Unknown video mode " << video_mode);
      break;
  }
 
  SDL_Flip(screen_surface->get_internal_surface());

}

/**
* @brief Blits a KQ_SCREEN_WIDTH*KQ_SCREEN_HEIGHT surface on a
* KQ_SCREEN_WIDTH*KQ_SCREEN_HEIGHT surface.
* @param src_surface the source surface
* @param dst_surface the destination surface
*/
void VideoManager::blit(Surface& src_surface, Surface& dst_surface) 
{
  src_surface.draw(dst_surface);
}

/**
* @brief Blits a KQ_SCREEN_WIDTH*KQ_SCREEN_HEIGHT surface on a
* double-size surface, stretching the image.
*
* Two black side bars are added if the destination surface is wider than KQ_SCREEN_WIDTH * 2.
*
* @param src_surface the source surface
* @param dst_surface the destination surface
*/
void VideoManager::blit_stretched(Surface& src_surface, Surface& dst_surface) {

  SDL_Surface* src_internal_surface = src_surface.get_internal_surface();
  SDL_Surface* dst_internal_surface = dst_surface.get_internal_surface();

  SDL_LockSurface(src_internal_surface);
  SDL_LockSurface(dst_internal_surface);

  uint32_t* dst = (uint32_t*) dst_internal_surface->pixels;

  int p = offset;
  for (int i = 0; i < KQ_SCREEN_HEIGHT; i++) {
    for (int j = 0; j < KQ_SCREEN_WIDTH; j++) {
      dst[p] = dst[p + 1] = dst[p + width] = dst[p + width + 1] = src_surface.get_mapped_pixel(i * KQ_SCREEN_WIDTH + j, dst_internal_surface->format);
      p += 2;
    }

    p += end_row_increment;
  }

  SDL_UnlockSurface(dst_internal_surface);
  SDL_UnlockSurface(src_internal_surface);
}

/**
* @brief Blits a KQ_SCREEN_WIDTH*KQ_SCREEN_HEIGHT surface on a
* double-size surface.
*
* The image is scaled with an implementation of the Scale2x algorithm.
* Two black side bars if the destination surface is wider than
* KQ_SCREEN_WIDTH * 2.
*
* @param src_surface the source surface
* @param dst_surface the destination surface
*/
void VideoManager::blit_scale2x(Surface& src_surface, Surface& dst_surface) {

  SDL_Surface* src_internal_surface = src_surface.get_internal_surface();
  SDL_Surface* dst_internal_surface = dst_surface.get_internal_surface();

  SDL_LockSurface(src_internal_surface);
  SDL_LockSurface(dst_internal_surface);

  uint32_t* src = (uint32_t*) src_internal_surface->pixels;
  uint32_t* dst = (uint32_t*) dst_internal_surface->pixels;

  int b, d, e = 0, f, h;
  int e1 = offset, e2, e3, e4;
  for (int row = 0; row < KQ_SCREEN_HEIGHT; row++) {
    for (int col = 0; col < KQ_SCREEN_WIDTH; col++) {

      // compute a to i

      b = e - KQ_SCREEN_WIDTH;
      d = e - 1;
      f = e + 1;
      h = e + KQ_SCREEN_WIDTH;

      if (row == 0) { b = e; }
      if (row == KQ_SCREEN_HEIGHT - 1) { h = e; }
      if (col == 0) { d = e; }
      if (col == KQ_SCREEN_WIDTH - 1) { f = e; }

      // compute e1 to e4
      e2 = e1 + 1;
      e3 = e1 + width;
      e4 = e3 + 1;

      // compute the color

      if (src[b] != src[h] && src[d] != src[f]) {
        dst[e1] = src_surface.get_mapped_pixel((src[d] == src[b]) ? d : e, dst_internal_surface->format);
        dst[e2] = src_surface.get_mapped_pixel((src[b] == src[f]) ? f : e, dst_internal_surface->format);
        dst[e3] = src_surface.get_mapped_pixel((src[d] == src[h]) ? d : e, dst_internal_surface->format);
        dst[e4] = src_surface.get_mapped_pixel((src[h] == src[f]) ? f : e, dst_internal_surface->format);
      }
      else {
        dst[e1] = dst[e2] = dst[e3] = dst[e4] = src_surface.get_mapped_pixel(e, dst_internal_surface->format);
      }
      e1 += 2;
      e++;
    }
    e1 += end_row_increment;
  }

  SDL_UnlockSurface(dst_internal_surface);
  SDL_UnlockSurface(src_internal_surface);
}

/**
* @brief Returns the current text of the window title bar.
* @return The window title.
*/
const std::string VideoManager::get_window_title() 
{
  char* window_title;
  char* icon;
  SDL_WM_GetCaption(&window_title, &icon);
  return window_title;
}

/**
* @brief Sets the text of the window title bar.
* @param window_title The window title to set.
*/
void VideoManager::set_window_title(const std::string& window_title) 
{
  SDL_WM_SetCaption(window_title.c_str(), NULL);
}