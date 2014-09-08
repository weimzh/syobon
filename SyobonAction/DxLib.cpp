#include "DxLib.h"
#include <set>

SDL_Joystick *joystick;

std::set<SDL_Keycode> keysHeld;
bool sound = true;
bool fullscreen = false;

void deinit();

#define SET_KEY(keycode) do { \
	std::set<SDL_Keycode>::iterator it = keysHeld.find(keycode); \
	if (it == keysHeld.end()) \
	{ \
		keysHeld.insert(keycode); \
	} \
} while (0)

#define UNSET_KEY(keycode) do { \
	std::set<SDL_Keycode>::iterator it = keysHeld.find(keycode); \
	if (it != keysHeld.end()) \
	{ \
		keysHeld.erase(it); \
	} \
} while (0)

#define CHECK_KEY(keycode) \
	(keysHeld.find(keycode) != keysHeld.end())

int
DxLib_Init ()
{
    atexit (deinit);

    if (SDL_Init (SDL_INIT_EVERYTHING) < 0)
    {
        fprintf (stderr, "Unable to init SDL: %s\n", SDL_GetError ());
        return -1;
    }

#if defined (_WIN32) && !defined (__SDL2__)
    putenv("SDL_VIDEODRIVER=directx");
#endif

#ifdef __SDL2__
	if (!(window = SDL_CreateWindow ("Syobon Action", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		480, 420, (fullscreen ? SDL_WINDOW_FULLSCREEN : 0))))
	{
		SDL_Quit ();
		return -1;
	}

	if (!(renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED)))
	{
		SDL_DestroyWindow (window);
		window = NULL;
		SDL_Quit ();
		return -1;
	}

	if (!(screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 480, 420,
		32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000)))
	{
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
		SDL_DestroyWindow(window);
		window = NULL;
		SDL_Quit();
		return -1;
	}

	if (!(texture = SDL_CreateTextureFromSurface(renderer, screen)))
	{
		SDL_FreeSurface(screen);
		screen = NULL;
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
		SDL_DestroyWindow(window);
		window = NULL;
		SDL_Quit();
		return -1;
	}
#else
    if (!(screen = SDL_SetVideoMode (480 /*(int)fmax/100 */ ,
                                     420 /*(int)fymax/100 */ , 32,
                                     SDL_HWSURFACE | SDL_DOUBLEBUF | (fullscreen ? SDL_FULLSCREEN : 0))))
    {
        if (!(screen = SDL_SetVideoMode (480 /*(int)fmax/100 */ ,
                                         420 /*(int)fymax/100 */ , 32,
                                         SDL_SWSURFACE | SDL_DOUBLEBUF | (fullscreen ? SDL_FULLSCREEN : 0))))
        {
            SDL_Quit ();
            return -1;
        }
    }

	SDL_WM_SetCaption("Syobon Action", NULL);
#endif
	if (fullscreen) SDL_ShowCursor(SDL_DISABLE);

    //Audio Rate, Audio Format, Audio Channels, Audio Buffers
#define AUDIO_CHANNELS 2
    if (sound && Mix_OpenAudio (22050, AUDIO_S16SYS, AUDIO_CHANNELS, 1024))
    {
        fprintf (stderr, "Unable to init SDL_mixer: %s\n", Mix_GetError ());
        sound = false;
    }
    //Try to get a joystick
	if (SDL_NumJoysticks() > 0)
	{
		joystick = SDL_JoystickOpen(0);
	}
	else
	{
		joystick = NULL;
	}

    keysHeld.clear();

    srand ((unsigned int) time (NULL));

    return 0;
}

#ifdef __SDL2__
SDL_Renderer *renderer;
SDL_Window *window;
SDL_Texture *texture;
#endif

//Main screen
SDL_Surface *screen;

byte fontType = DX_FONTTYPE_NORMAL;

void
ChangeFontType (byte type)
{
    fontType = type;
}

#include "font.h"

static void
DrawChar (const unsigned char *ch, int a, int b, Uint32 c)
{
    if (!screen) return;

    int i, j;

	if (c == 0) {
		c = SDL_MapRGB(screen->format, 0, 0, 0);
	}

    if (*ch <= 0x7f) {
        // ASCII character
        unsigned short cvtchar = ascii2sjis(*ch);

        if (cvtchar) {
            unsigned char buf[2];

            buf[0] = cvtchar >> 8;
            buf[1] = cvtchar & 0xff;

            unsigned short *font = (unsigned short *)kanjiaddr(buf);

            if (font) {
                for (i = 0; i < 15; i++) {
                    for (j = 0; j < 16; j++) {
                        if (*font & (1 << (16 - j))) {
                            if (b + i >= screen->h || b + i < 0) continue;
                            if (a + j >= screen->w || a + j < 0) continue;

                            int offset = (b + i) * screen->pitch + (a + j) * 4;
                            *(Uint32 *)&((Uint8 *)screen->pixels)[offset] = c;
                        }
                    }
                    font++;
                }
            }
        }
    } else {
        // full-width char
        unsigned short *font = (unsigned short *)kanjiaddr(ch);

        if (font) {
            for (i = 0; i < 15; i++) {
                for (j = 0; j < 16; j++) {
                    if (*font & (1 << (16 - j))) {
                        if (b + i >= screen->h || b + i < 0) continue;
                        if (a + j >= screen->w || a + j < 0) continue;

                        int offset = (b + i) * screen->pitch + (a + j) * 4;
                        *(Uint32 *)&((Uint8 *)screen->pixels)[offset] = c;
                    }
                }
                font++;
            }
        }
    }
}

void
DrawString (int a, int b, const char *x, Uint32 c)
{
    int len = (int)strlen(x);
    unsigned char *p = (unsigned char *)x;

    if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);

    while (len > 0) {
        if (*p == 'I' || *p == 'l') a -= 2;

        if (fontType == DX_FONTTYPE_EDGE) {
            DrawChar(p, a - 1, b - 1, 0);
            DrawChar(p, a, b - 1, 0);
            DrawChar(p, a + 1, b - 1, 0);
            DrawChar(p, a - 1, b, 0);
            DrawChar(p, a + 1, b, 0);
            DrawChar(p, a - 1, b + 1, 0);
            DrawChar(p, a, b + 1, 0);
            DrawChar(p, a + 1, b + 1, 0);
        }

        DrawChar(p, a, b, c);

        if (*p <= 0x7f) {
            a += 9;

            if (*p == '-') a += 3;
            else if (*p >= '0' && *p <= '9') a += 3;
            else if (*p == '?') a += 8;
            else if (*p >= 'A' && *p <= 'Z' && *p != 'I') a += 4;

            p++;
            len--;
        } else {
            a += 17;
            p += 2;
            len -= 2;
        }
    }

    if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
}

void
DrawFormatString (int a, int b, Uint32 color, const char *str, ...)
{
    va_list args;
    char *newstr = new char[strlen (str) + 16];
    va_start (args, str);
    vsprintf (newstr, str, args);
    va_end (args);
    DrawString (a, b, newstr, color);
    delete[]newstr;
}

//Key Aliases
#define KEY_INPUT_ESCAPE SDLK_ESCAPE

bool ex = false;

void
UpdateKeys ()
{
    SDL_Event event;
    while (SDL_PollEvent (&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            SET_KEY(event.key.keysym.sym);
            break;
		case SDL_KEYUP:
			UNSET_KEY(event.key.keysym.sym);
            break;
        case SDL_JOYAXISMOTION:
            if (event.jaxis.which == 0)
            {
                if (event.jaxis.axis == JOYSTICK_XAXIS)
                {
                    if (event.jaxis.value < -16383)
                        SET_KEY(SDLK_LEFT);
                    else if (event.jaxis.value > 16383)
                        SET_KEY(SDLK_RIGHT);
                    else
                    {
                        UNSET_KEY(SDLK_LEFT);
                        UNSET_KEY(SDLK_RIGHT);
                    }
                }
                else if (event.jaxis.axis == JOYSTICK_YAXIS)
                {
                    if (event.jaxis.value < -16383)
                        SET_KEY(SDLK_UP);
                    else if (event.jaxis.value > 16383)
                        SET_KEY(SDLK_DOWN);
                    else
                    {
                        UNSET_KEY(SDLK_UP);
                        UNSET_KEY(SDLK_DOWN);
                    }
                }
            }
            break;
        case SDL_QUIT:
            ex = true;
            break;
        }
    }
}

byte
ProcessMessage ()
{
    return ex;
}

byte
CheckHitKey (int key)
{
    if (key == SDLK_z && CHECK_KEY(SDLK_SEMICOLON))
        return true;
    return CHECK_KEY(key);
}

void
WaitKey ()
{
    while (true)
    {
		SDL_Delay (100);
		UpdateKeys ();

		if (!keysHeld.empty())
			return;

		if (joystick != NULL && SDL_JoystickGetButton (joystick, JOYSTICK_JUMP))
			return;

		if (ex)
			exit (0);
    }
}

void
DrawGraphZ (int a, int b, SDL_Surface * mx)
{
    if (mx)
    {
        SDL_Rect offset;
        offset.x = a;
        offset.y = b;
        SDL_BlitSurface (mx, NULL, screen, &offset);
    }
}

void
DrawTurnGraphZ (int a, int b, SDL_Surface * mx)
{
    if (mx && mx->format->BitsPerPixel == 32)
    {
        if (SDL_MUSTLOCK (screen)) SDL_LockSurface (screen);
        if (SDL_MUSTLOCK (mx)) SDL_LockSurface (mx);

        Uint32 *src = (Uint32 *) mx->pixels;
        Uint32 *dst = (Uint32 *) screen->pixels;

        int i, j;
        Uint8 rv = 0, gv = 0, bv = 0;
        Uint32 key = SDL_MapRGB (mx->format, 9 * 16 + 9, 255, 255);

        for (i = 0; i < mx->h; i++)
        {
            for (j = 0; j < mx->w; j++)
            {
                int x = a + j, y = b + i;
                if (x < 0 || y < 0 || x >= screen->w || y >= screen->h)
                    continue;

                Uint32 pixel = src[(i + 1) * mx->pitch / 4 - j - 1];
                if (pixel == key)
                    continue;

                SDL_GetRGB (pixel, mx->format, &rv, &gv, &bv);
                dst[y * screen->pitch / 4 + x] = SDL_MapRGB (screen->format, rv, gv, bv);
            }
        }

        if (SDL_MUSTLOCK (screen)) SDL_UnlockSurface (screen);
        if (SDL_MUSTLOCK (mx)) SDL_UnlockSurface (mx);
    }
}

void
DrawVertTurnGraph (int a, int b, SDL_Surface * mx)
{
    if (mx && mx->format->BitsPerPixel == 32)
    {
        if (SDL_MUSTLOCK (screen)) SDL_LockSurface (screen);
        if (SDL_MUSTLOCK (mx)) SDL_LockSurface (mx);

        Uint32 *src = (Uint32 *) mx->pixels;
        Uint32 *dst = (Uint32 *) screen->pixels;

        int i, j;
        Uint8 rv = 0, gv = 0, bv = 0;
        Uint32 key = SDL_MapRGB (mx->format, 9 * 16 + 9, 255, 255);

        for (i = 0; i < mx->h; i++)
        {
            for (j = 0; j < mx->w; j++)
            {
                int x = a + j, y = b + i;
                if (x < 0 || y < 0 || x >= screen->w || y >= screen->h)
                    continue;

                Uint32 pixel = src[(mx->h - i - 1) * mx->pitch / 4 + j];
                if (pixel == key)
                    continue;

                SDL_GetRGB (pixel, mx->format, &rv, &gv, &bv);
                dst[y * screen->pitch / 4 + x] = SDL_MapRGB (screen->format, rv, gv, bv);
            }
        }

        if (SDL_MUSTLOCK (screen)) SDL_UnlockSurface (screen);
        if (SDL_MUSTLOCK (mx)) SDL_UnlockSurface (mx);
    }
}

SDL_Surface *
DerivationGraph (int srcx, int srcy, int width, int height, SDL_Surface * src)
{
    SDL_Surface *img = SDL_CreateRGBSurface (SDL_SWSURFACE, width, height,
                       screen->format->BitsPerPixel,
					   screen->format->Rmask,
					   screen->format->Gmask,
					   screen->format->Bmask,
					   screen->format->Amask);

    SDL_Rect offset;
    offset.x = srcx;
    offset.y = srcy;
    offset.w = width;
    offset.h = height;

    SDL_BlitSurface (src, &offset, img, NULL);
    SDL_SetColorKey (img, 1,
                     SDL_MapRGB (img->format, 9 * 16 + 9, 255, 255));
    return img;
}

//Noticably different than the original
SDL_Surface *
LoadGraph (const char *filename)
{
    SDL_Surface *image = SDL_LoadBMP (filename);

    if (image)
        return image;
    fprintf (stderr, "Error: Unable to load %s: %s\n", filename,
             SDL_GetError ());
    exit (1);
}

void
PlaySoundMem (Mix_Chunk * s, int l)
{
    if (sound)
        Mix_PlayChannel (-1, s, l);
}

Mix_Chunk *
LoadSoundMem (const char *f)
{
    if (!sound)
        return NULL;

    Mix_Chunk *s = Mix_LoadWAV (f);
    if (s)
        return s;
    fprintf (stderr, "Error: Unable to load sound %s: %s\n", f,
             Mix_GetError ());
    return NULL;
}

Mix_Music *
LoadMusicMem (const char *f)
{
    if (!sound)
        return NULL;

    Mix_Music *m = Mix_LoadMUS (f);
    if (m)
        return m;
    fprintf (stderr, "Error: Unable to load music %s: %s\n", f,
             Mix_GetError ());
    return NULL;
}

#ifdef __SDL2__

void
ScreenFlip()
{
	SDL_RenderClear(renderer);
	SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);

	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	SDL_Rect dstrect;
	dstrect.x = (w - 480 * h / 420) / 2;
	dstrect.y = 0;
	dstrect.w = 480 * h / 420;
	dstrect.h = h;

	SDL_RenderCopy(renderer, texture, NULL, &dstrect);

	SDL_RenderPresent(renderer);
}

#endif
