#include "Player.h"
#include "Viewport.h"

#include <stdlib.h>

// NULL means no filter active
static const struct Filter* allFilters[PLAYER_NUM_FILTERS] = {
		NULL, &Filter_RED,
		&Filter_GREEN, &Filter_BLUE,
		&Filter_ORANGE, &Filter_VIOLET
};

void Player_Init(struct Player* player, struct Texture* spritesheet, int x, int y, int w, int h)
{
	player->direction = &Direction_STAND_RIGHT;
	player->texture = spritesheet;

	player->health = PLAYER_HEALTH;
	player->currFilter = 0;

	player->vx = 0.0f;
	player->vy = 0.0f;
	player->x = (float)x;
	player->y = (float)y;
	player->w = w;
	player->h = h;
}

struct Bounds Player_GetBounds(struct Player* player)
{
	struct Bounds bounds = {
		.x = (player->x) + 10,
		.w = (player->w) - 20,
		.y = player->y,
		.h = player->h
	};
	return bounds;
}

void Player_UpdateDirection(struct Player* player)
{
	// Right
	if (player->vx > 0.0f) 
	{
		if (player->vy != 0.0f) 
			player->direction = &Direction_JUMP_RIGHT;
		else
			player->direction = &Direction_WALK_RIGHT;
	}
	// Left
	else if (player->vx < 0.0f)
	{
		if (player->vy != 0.0f) 
			player->direction = &Direction_JUMP_LEFT;
		else
			player->direction = &Direction_WALK_LEFT;
	}
	// Neither 
	else
	{
		if (player->vy != 0.0f)
		{
			if (player->direction == &Direction_STAND_LEFT ||
				player->direction == &Direction_WALK_LEFT)
					player->direction = &Direction_JUMP_LEFT;
			else
				player->direction = &Direction_JUMP_RIGHT;
		}
		else 
		{
			if (player->direction == &Direction_WALK_LEFT ||
				player->direction == &Direction_JUMP_LEFT)
					player->direction = &Direction_STAND_LEFT;
			else
				player->direction = &Direction_STAND_RIGHT;
		}
	}
}

void Player_Animate(struct Player* player)
{
	static int clipX = 0;
	static int ticks = 0;

	int frames = (player->direction)->frames;
	int waitTime = (player->direction)->sleep;
	int clipY = (player->direction)->value;

	if (ticks >= waitTime)
	{
		clipX ++;
		if (clipX >= frames)
			clipX = 0;

		ticks = 0;
	}
	else ticks ++;

	Texture_MoveClip(player->texture, clipX, clipY);
}

void Player_Render(struct Player* player, SDL_Renderer* renderer)
{
	// For debugging purposes:
	// 
	// Bounds player_bounds;
	// player_bounds = Player_getBounds(player);
	// SDL_SetRenderDrawColor(renderer, 255, 238, 0, 255);
	// SDL_Rect draw_bounds = {.x=player_bounds.x, .y=player_bounds.y, .w=player_bounds.w, .h=player_bounds.h};
	// SDL_RenderDrawRect(renderer, &draw_bounds);

	const struct Filter* drawFilter = allFilters[player->currFilter];

	if (drawFilter != NULL)
	{
		SDL_SetRenderDrawColor(renderer, drawFilter->r, drawFilter->g, drawFilter->b, FILTER_ALPHA);
		SDL_Rect screen_bounds = {
			.x = 0, 
			.y = 0, 
			.w = VIEWPORT_WIDTH, 
			.h = VIEWPORT_HEIGHT
		};
		SDL_RenderFillRect(renderer, &screen_bounds);
	}
	float actualX = player->x - Viewport_X;
	float actualY = VIEWPORT_HEIGHT - player->y - Viewport_Y + 1.0f;

	Texture_Render(player->texture, renderer, (int)actualX, (int)actualY, player->w, player->h);
}
