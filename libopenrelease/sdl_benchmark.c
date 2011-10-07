#include <log.h>

#include <SDL.h>

#include <RELEASE.h>

/*-------------------------------------*/
/* Simple SDL benchmark program
 *
 * Matthew J. Gelhaus
 * October 19, 2005
 *
 */


int VIDEOX;
int VIDEOY;


static unsigned char spriteData[] = {
0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,
0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,
0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,
0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,
0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,
0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
0,0,0,1,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,2,0,0,2,0,0,0,0,0,2,0,0,2,0,0,0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,2,0,0,2,0,0,0,0,0,2,0,0,2,0,0,0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,2,0,0,2,0,0,0,0,0,2,0,0,2,0,0,0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,1,0,0,0,0,
0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


static int getInfo(SDL_Surface *surface)
{
	say_debug("SDL_BENCHMARK: Pitch = %d", surface->pitch);

	say_debug("SDL_BENCHMARK: Hardware surfaces avail  = %d", SDL_GetVideoInfo()->hw_available);
	say_debug("SDL_BENCHMARK: Window manager avail     = %d", SDL_GetVideoInfo()->wm_available);
	say_debug("SDL_BENCHMARK: Blitter hardware         = %d", SDL_GetVideoInfo()->blit_hw);
	say_debug("SDL_BENCHMARK: Colorkey blit hardware   = %d", SDL_GetVideoInfo()->blit_hw_CC);
	say_debug("SDL_BENCHMARK: Alpha blit hardware      = %d", SDL_GetVideoInfo()->blit_hw_A);
	say_debug("SDL_BENCHMARK: Software->Hardware accel = %d", SDL_GetVideoInfo()->blit_sw);
	if (SDL_GetVideoInfo()->hw_available == 1)
		say_debug("SDL_BENCHMARK: Video memory             = %d", SDL_GetVideoInfo()->video_mem);
	else
		say_debug("SDL_BENCHMARK: Video memory             = N/A");

	return SDL_GetVideoInfo()->hw_available;
}

static int doSlowPoints(SDL_Surface *surface)
{
	int numFrames = 2;
	int k = 0;
	int x, y;
	__UINT16 *bufp;

	if (SDL_LockSurface(surface) <0) return k;
	for (k=0; k<numFrames; k++)
	{
		for(x=0;x<VIDEOX ;x++){
			for(y=0;y<VIDEOY;y++){
				bufp = (__UINT16 *)surface->pixels + y*(VIDEOX*2)/2 + x;
				*bufp = (__UINT16)(32768-(k+1)*16);
			}
			SDL_UpdateRect(surface, x, 0, 1, VIDEOY);
		}
	}
	SDL_UnlockSurface(surface);
	return k;
}

static int doFastPoints(SDL_Surface *surface)
{
	int numFrames = 32;
	int k = 0;
	int x, y;

__UINT16 *bufp;

	if (SDL_LockSurface(surface) <0) return k;
	for (k=0; k<numFrames; k++)
	{
		for(x=0;x<VIDEOX ;x++){
			for(y=0;y<VIDEOY;y++){
				bufp = (__UINT16 *)surface->pixels + y*(VIDEOX*2)/2 + x;
				*bufp = (__UINT16)(255-k*16-k);
			}
		}
		SDL_UpdateRect(surface, 0, 0, VIDEOX, VIDEOY);
	}
	SDL_UnlockSurface(surface);
	return k;
}

static int doRects(SDL_Surface *surface)
{
	int numRects = 1024;

	int k = 0;

	SDL_Rect rect;

	if (SDL_LockSurface(surface) <0) return k;
	rect.x = 0;
	rect.y = 0;
	for (k=0; k<numRects; k++)
	{
		rect.x++;
		if (rect.x == VIDEOX -6) rect.x = 0;
		rect.y++;
		if (rect.y == VIDEOY -6) rect.y = 0;
		rect.w = (VIDEOX -rect.x)/2;
		rect.h = (VIDEOY -rect.y)/2;

		SDL_FillRect(surface, &rect, k % 16535);
		SDL_UpdateRect(surface, rect.x, rect.y, rect.w, rect.h);
	}
	SDL_UnlockSurface(surface);
	return k;
}


static int doBlitter(SDL_Surface *surface)
{
	int numBlits = 4096;

	int i,j;
	int ofs;
	int minx, miny;
	SDL_Rect rect, orect;
	SDL_Color colors[3];
	colors[0].r = 0;
	colors[0].g = 0;
	colors[0].b = 0;
	colors[1].r = 255;
	colors[1].g = 255;
	colors[1].b = 0;
	colors[2].r = 255;
	colors[2].g = 0;
	colors[2].b = 0;
	SDL_SetColors(surface, colors, 0, 3);

	if (SDL_LockSurface(surface) <0) return 0;
	rect.x = 0;
	rect.y = 0;
	rect.w = VIDEOX;
	rect.h = VIDEOY;
	SDL_FillRect(surface, &rect, 0);
	for (i=0; i<32; i++)
		for (j=0; j<32; j++)
		{
			ofs = j*surface->pitch+i;
			//((unsigned char *)surface->pixels)[ofs] = (unsigned char)(rand()*255/RAND_MAX);
			((__UINT16 *)surface->pixels)[ofs] = spriteData[j*32+i]*20000;
		}
	SDL_UpdateRect(surface, 0, 0, 32, 32);
	SDL_UnlockSurface(surface);

	rect.x = 32;
	rect.y = 32;
	rect.w = 32;
	rect.h = 32;
	orect.x = 0;
	orect.y = 0;
	orect.w = 32;
	orect.h = 32;

	rect.x = 0;
	rect.y = 0;
	for (i=0; i<numBlits; i++)
	{
		orect.x = rect.x;
		orect.y = rect.y;
		rect.x++;
		if (rect.x >= VIDEOX -32) rect.x=0;
		rect.y++;
		if (rect.y >= VIDEOY -32) rect.y=0;
		minx = (orect.x < rect.x) ? orect.x : rect.x;
		miny = (orect.y < rect.y) ? orect.y : rect.y;
		SDL_BlitSurface(surface, &orect, surface, &rect);
		SDL_UpdateRect(surface, minx, miny, 32, 32);
	}
	return i;
}

int benchmark(SDL_Surface *screen)
{
    time_t starttime;
  int i, state=0;
  int hw_avail=0;
  double results[4];

    for (i=0; i<4; i++)
      results[i] = -1.0;

	VIDEOX = 1366;  VIDEOY = 768;

	say_debug("SDL_BENCHMARK: Mode = %dx%d, software", VIDEOX, VIDEOY);


    SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

    starttime = 1;
    state = 0;

    hw_avail = getInfo(screen);
    say_debug("SDL_BENCHMARK: ");

    // Slow points test
    starttime=SDL_GetTicks();
    say_debug("SDL_BENCHMARK: Slow points test");
    state = doSlowPoints(screen);
    results[0] = ((double)state*1000)/(SDL_GetTicks() -starttime);
    say_debug("result: %8g\n", results[0]);

    // Fast points test
    starttime=SDL_GetTicks();
    say_debug("SDL_BENCHMARK: Fast points test");
//    for (i=0; i<2; i++)
       state = doFastPoints(screen);
    results[1] = ((double)state*1000)/(SDL_GetTicks() -starttime);
    say_debug("result: %8g\n", results[1]);

    // Rect fill test
    starttime=SDL_GetTicks();
    say_debug("SDL_BENCHMARK: Rect fill test");
    state = doRects(screen);
    results[2] = ((double)state*1000)/(SDL_GetTicks() -starttime);
    say_debug("result: %8g\n", results[2]);

    // Blitter test
    starttime=SDL_GetTicks();
    say_debug("SDL_BENCHMARK: 32x32 Blitter test");
    state = doBlitter(screen);
    results[3] = ((double)state*1000)/(SDL_GetTicks() -starttime);
    say_debug("result: %8g\n", results[3]);
 

  // Output results
  //
  say_debug("SDL_BENCHMARK: Slow points (frames/sec): ");
  say_debug("SDL_BENCHMARK: %8g ", results[0]);
  say_debug("SDL_BENCHMARK: Fast points (frames/sec): ");
  say_debug("SDL_BENCHMARK: %8g ", results[1]);
  say_debug("SDL_BENCHMARK:    Rect fill (rects/sec): ");
  say_debug("SDL_BENCHMARK: %8g ", results[2]);
  say_debug("SDL_BENCHMARK:  32x32 blits (blits/sec): ");
  say_debug("SDL_BENCHMARK: %8g ", results[3]);
  say_debug("SDL_BENCHMARK: ");


  return 0;
}
