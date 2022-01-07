#include <stdbool.h>
#include <stdio.h>

#include <SDL_image.h>

int RenderScaleFlip(
	SDL_Renderer* renderer, SDL_Texture* t,
	int x, int y, int w, int h, double angle, double sx, double sy, SDL_RendererFlip flip)
{
	SDL_Rect dst;
	dst.x = x - (w * sx) / 2;
	dst.y = y - (h * sy) / 2;
	dst.w = (int)(w * sx);
	dst.h = (int)(h * sy);
	return SDL_RenderCopyEx(renderer, t, NULL, &dst, angle, NULL, flip);
}

SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* filename, int* w, int* h)
{
	SDL_Texture* t = NULL;

	// Load image from file
	SDL_Surface* s = IMG_Load(filename);
	if (s == NULL)
	{
		printf("Failed to load image from file %s\n", filename);
		goto bail;
	}

	// Convert to texture
	t = SDL_CreateTextureFromSurface(renderer, s);
	if (t == NULL)
	{
		printf("Failed to create texture from surface: %s\n", SDL_GetError());
		goto bail;
	}

	*w = s->w;
	*h = s->h;

bail:
	return t;
}

int main(int argc, char* argv[])
{
	SDL_Texture* tShura = NULL;
	int wShura, hShura;
	SDL_Texture* tDoor = NULL;
	int wDoor, hDoor;
	SDL_Texture* tFrame = NULL;
	int wFrame, hFrame;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	int width = 520;
	int height = 700;

	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL_Init error: %s\n", SDL_GetError());
		goto bail;
	}

	// Create display window
	window = SDL_CreateWindow(
		"SDL2_RenderCopyEx", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, 0);
	if (window == NULL)
	{
		printf("Failed to create window: %s\n", SDL_GetError());
		goto bail;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		goto bail;
	}
	SDL_RenderSetLogicalSize(renderer, width, height);

	// Load textures from file
	tShura = LoadTexture(renderer, "shura.png", &wShura, &hShura);
	if (tShura == NULL)
	{
		goto bail;
	}
	tDoor = LoadTexture(renderer, "door.png", &wDoor, &hDoor);
	if (tDoor == NULL)
	{
		goto bail;
	}
	tFrame = LoadTexture(renderer, "frame.png", &wFrame, &hFrame);
	if (tFrame == NULL)
	{
		goto bail;
	}

	// Initialise render color (for dots)
	if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) != 0)
	{
		printf("Failed to set render draw color: %s\n", SDL_GetError());
		goto bail;
	}

	// Render the textures
	for (int i = 0; i < 5; i++)
	{
		int x = 60 + 100 * i;
		double angle = 90 / 4 * i;

		// rotate
		if (RenderScaleFlip(renderer, tShura, x, 50, wShura, hShura, angle, 1.0, 1.0, SDL_FLIP_NONE) != 0)
		{
			printf("Failed to render texture: %s\n", SDL_GetError());
			goto bail;
		}
		// rotate and scale
		if (RenderScaleFlip(renderer, tShura, x, 150, wShura, hShura, angle, 0.5, 1.5, SDL_FLIP_NONE) != 0)
		{
			printf("Failed to render texture: %s\n", SDL_GetError());
			goto bail;
		}
		// rotate and flip
		if (RenderScaleFlip(renderer, tShura, x, 250, wShura, hShura, angle, 1.0, 1.0, SDL_FLIP_HORIZONTAL) != 0)
		{
			printf("Failed to render texture: %s\n", SDL_GetError());
			goto bail;
		}

		SDL_Rect src, dst;

		// wipe right
		if (RenderScaleFlip(renderer, tFrame, x, 350, wFrame, hFrame, 0.0, 1.0, 1.0, SDL_FLIP_NONE) != 0)
		{
			printf("Failed to render texture: %s\n", SDL_GetError());
			goto bail;
		}
		src.x = wDoor * i / 5;
		src.y = 0;
		src.w = dst.w = wDoor - src.x;
		src.h = dst.h = hDoor;
		dst.x = x + src.x - wDoor / 2;
		dst.y = 354 - hDoor / 2;
		if (SDL_RenderCopyEx(renderer, tDoor, &src, &dst, 0.0, NULL, SDL_FLIP_NONE) != 0)
		{
			printf("Failed to render texture: %s\n", SDL_GetError());
			goto bail;
		}

		// wipe up
		if (RenderScaleFlip(renderer, tFrame, x, 450, wFrame, hFrame, 0.0, 1.0, 1.0, SDL_FLIP_NONE) != 0)
		{
			printf("Failed to render texture: %s\n", SDL_GetError());
			goto bail;
		}
		src.x = 0;
		src.y = 0;
		src.w = dst.w = wDoor;
		src.h = dst.h = hDoor - hDoor * i / 5;
		dst.x = x - wDoor / 2;
		dst.y = 454 - hDoor / 2;
		if (SDL_RenderCopyEx(renderer, tDoor, &src, &dst, 0.0, NULL, SDL_FLIP_NONE) != 0)
		{
			printf("Failed to render texture: %s\n", SDL_GetError());
			goto bail;
		}

		// slide right
		if (RenderScaleFlip(renderer, tFrame, x, 550, wFrame, hFrame, 0.0, 1.0, 1.0, SDL_FLIP_NONE) != 0)
		{
			printf("Failed to render texture: %s\n", SDL_GetError());
			goto bail;
		}
		src.x = 0;
		src.y = 0;
		src.w = dst.w = wDoor - wDoor * i / 5;
		src.h = dst.h = hDoor;
		dst.x = x + wDoor * i / 5 - wDoor / 2;
		dst.y = 554 - hDoor / 2;
		if (SDL_RenderCopyEx(renderer, tDoor, &src, &dst, 0.0, NULL, SDL_FLIP_NONE) != 0)
		{
			printf("Failed to render texture: %s\n", SDL_GetError());
			goto bail;
		}

		// slide up
		if (RenderScaleFlip(renderer, tFrame, x, 650, wFrame, hFrame, 0.0, 1.0, 1.0, SDL_FLIP_NONE) != 0)
		{
			printf("Failed to render texture: %s\n", SDL_GetError());
			goto bail;
		}
		src.x = 0;
		src.y = hDoor * i / 5;
		src.w = dst.w = wDoor;
		src.h = dst.h = hDoor - src.y;
		dst.x = x - wDoor / 2;
		dst.y = 654 - hDoor / 2;
		if (SDL_RenderCopyEx(renderer, tDoor, &src, &dst, 0.0, NULL, SDL_FLIP_NONE) != 0)
		{
			printf("Failed to render texture: %s\n", SDL_GetError());
			goto bail;
		}
	}

	// Display
	SDL_RenderPresent(renderer);

	// Wait for keypress to exit
	bool quit = false;
	while (!quit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN || e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		SDL_Delay(100);
	}


bail:
	SDL_DestroyTexture(tShura);
	SDL_DestroyTexture(tDoor);
	SDL_DestroyTexture(tFrame);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
