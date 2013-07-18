/** @file Common.h */

#ifndef KQ_COMMON_H
#define KQ_COMMON_H

#define KQ_STRINGIFY1(x) #x
#define KQ_STRINGIFY(x) KQ_STRINGIFY1(x)

#define KQ_MAJOR_VERSION 0

#define KQ_MINOR_VERSION 1

#define KQ_PATCH_VERSION 0

#define KQ_VERSION (KQ_STRINGIFY(KQ_MAJOR_VERSION) "." KQ_STRINGIFY(KQ_MINOR_VERSION) "." KQ_STRINGIFY(KQ_PATCH_VERSION))
#ifndef KQ_DEFAULT_QUEST
#define KQ_DEFAULT_QUEST "."
#endif

#ifndef KQ_WRITE_DIR
#define KQ_WRITE_DIR ".kirpsquest"
#endif

#ifndef KQ_SCREEN_WIDTH
#define KQ_SCREEN_WIDTH 320
#endif

#ifndef KQ_SCREEN_HEIGHT
#define KQ_SCREEN_HEIGHT 240
#endif

#define KQ_SCREEN_WIDTH_MIDDLE (SCREEN_WIDTH / 2)
#define KQ_SCREEN_HEIGHT_MIDDLE (SCREEN_HEIGHT / 2)

#ifndef KQ_COLOR_DEPTH
# if defined(CAANOO) || defined(PANDORA)
# define KQ_COLOR_DEPTH 16
# else
# define KQ_COLOR_DEPTH 32
# endif
#endif

//TODO:  adjust play screen for HUD menus
#define KQ_PLAY_SCREEN_WIDTH
#define KQ_PLAY_SCREEN_HEIGHT


#include "Types.h"

#endif //end Common.h
