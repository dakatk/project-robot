#ifndef CLOCKWORKISLAND_PLATFORM_H__
#define CLOCKWORKISLAND_PLATFORM_H__

#include "Texture.h"
#include "Physics.h"

#define PLATFORM_SPRITE_CLIP_SIZE 128
#define NUM_PLATFORM_VISIBLE_OPTIONS 6

struct Platform {

	struct Texture* texture;
	bool visible[NUM_PLATFORM_VISIBLE_OPTIONS];

	double angle;
	int spriteClipX;
	int spriteClipY;

	int x, y;
	int w, h;
};

// Long boi:
void Platform_Init(struct Platform* platform, struct Texture* texture, double angle, int spriteIndex, int x, int y, int w, int h);
bool Platform_IsOffscreen(struct Platform* platform);
void Platform_Render(struct Platform* platform);

#endif /* CLOCKWORKISLAND_PLATFORM_H__ */
