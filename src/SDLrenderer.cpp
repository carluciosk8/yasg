/***************************************************************************
 *   Copyright (C) 2005 by Carlucio Santos Cordeiro                        *
 *   carlucio@gmail.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <stdexcept>

using std::runtime_error;

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

#include "SDLrenderer.h"
#include "image.h"
#include "font.h"


SDLRenderer::SDLRenderer(int iWidth, int iHeight, int iBpp, Uint32 uiFlags)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
        throw runtime_error("SDLRenderer error: cannot initialize SDL");


    m_tpScreen = SDL_SetVideoMode(iWidth, iHeight, iBpp, uiFlags);
    if (m_tpScreen == NULL)
        throw runtime_error("SDLRenderer error: cannot initialize SDL");
        
    SDL_WM_SetCaption("YASG ver 0.05", NULL);
    //SDL_ShowCursor(SDL_DISABLE);
    //SDL_WM_GrabInput(SDL_GRAB_ON);

    TTF_Init();

    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 512);
    Mix_VolumeMusic(SDL_MIX_MAXVOLUME);    
    Mix_Volume(-1,SDL_MIX_MAXVOLUME);
}


SDLRenderer::~SDLRenderer()
{
    Mix_CloseAudio();  
   	TTF_Quit();
	SDL_Quit();
}



void SDLRenderer::drawText(int x, int y, char* cpText, Font& krFont, SDL_Color tColor)
{
	SDL_Surface *tpTextSurf;
	SDL_Rect tDestRect;

    tpTextSurf = TTF_RenderText_Solid(krFont.getFont(), cpText, tColor);

    tDestRect.x = x;
	tDestRect.y = y;	
	tDestRect.h = tpTextSurf->h;
	tDestRect.w = tpTextSurf->w;

    SDL_BlitSurface(tpTextSurf, NULL, m_tpScreen, &tDestRect);
	SDL_FreeSurface(tpTextSurf);
}



void SDLRenderer::drawOutlineText(int x, int y, char* cpText, Font& krFont, 
                                  int iBorderSize, SDL_Color tBorderColor, SDL_Color tCenterColor)
{
	SDL_Surface *tpBorderSurf;
	SDL_Surface *tpCenterSurf;
	SDL_Rect tDestRect;

    tpBorderSurf = TTF_RenderText_Solid(krFont.getFont(), cpText, tBorderColor);
    tpCenterSurf = TTF_RenderText_Solid(krFont.getFont(), cpText, tCenterColor);

    // draw border
    for (int xx = x-iBorderSize ; xx <= x+iBorderSize ; xx++)
    {
        for (int yy = y-iBorderSize ; yy <= y+iBorderSize ; yy++)
        {
            tDestRect.x = xx;
	        tDestRect.y = yy;	
	        tDestRect.h = tpBorderSurf->h;
	        tDestRect.w = tpBorderSurf->w;
            SDL_BlitSurface(tpBorderSurf, NULL, m_tpScreen, &tDestRect);
        }
    }

    // draw center
    tDestRect.x = x;
	tDestRect.y = y;	
	tDestRect.h = tpCenterSurf->h;
	tDestRect.w = tpCenterSurf->w;
    SDL_BlitSurface(tpCenterSurf, NULL, m_tpScreen, &tDestRect);

	SDL_FreeSurface(tpBorderSurf);
	SDL_FreeSurface(tpCenterSurf);
}



void SDLRenderer::fadeScreen(bool fade_in, int time, Uint32 color)
{
	SDL_Surface *screen_copy, *black_surface;
	Uint32 cur_tick, started_fading;
	
	/* Copy the m_tpScreen */
	screen_copy = SDL_DisplayFormatAlpha(m_tpScreen);
	
	/* Make the color surface */
	black_surface = SDL_CreateRGBSurface(
		m_tpScreen->flags,
		m_tpScreen->w,
		m_tpScreen->h,
		m_tpScreen->format->BitsPerPixel,
		m_tpScreen->format->Rmask,
		m_tpScreen->format->Gmask,
		m_tpScreen->format->Bmask,
		m_tpScreen->format->Amask);
		
	if(fade_in)
		SDL_SetAlpha(black_surface, SDL_SRCALPHA | SDL_RLEACCEL, 255);
	else
		SDL_SetAlpha(black_surface, SDL_SRCALPHA | SDL_RLEACCEL, 0);
	
	SDL_FillRect(black_surface, NULL, color);
	
	/* Now fade in / out */
	for(started_fading = cur_tick = SDL_GetTicks(); cur_tick - started_fading <= (Uint32)time; cur_tick = SDL_GetTicks())
	{
		SDL_BlitSurface(screen_copy, NULL, m_tpScreen, NULL);
		SDL_BlitSurface(black_surface, NULL, m_tpScreen, NULL);
		
		SDL_Flip(m_tpScreen);
		
		if(fade_in)
			SDL_SetAlpha(black_surface,	SDL_SRCALPHA | SDL_RLEACCEL, 255 - ((cur_tick - started_fading) * 255) / time);
		else
			SDL_SetAlpha(black_surface,	SDL_SRCALPHA | SDL_RLEACCEL, ((cur_tick - started_fading) * 255) / time);
		
		SDL_Delay(1);
	}
	
	/* Set it to the desired final state */
	if(fade_in)
		SDL_SetAlpha(black_surface, SDL_SRCALPHA | SDL_RLEACCEL, 0);
	else
		SDL_SetAlpha(black_surface, SDL_SRCALPHA | SDL_RLEACCEL, 255);
	
	SDL_BlitSurface(screen_copy, NULL, m_tpScreen, NULL);
	SDL_BlitSurface(black_surface, NULL, m_tpScreen, NULL);
	
	SDL_Flip(m_tpScreen);
	
	SDL_FreeSurface(screen_copy);
	SDL_FreeSurface(black_surface);
}



void SDLRenderer::draw(Image& krImage, int x, int y)
{
    SDL_Rect dst = {x,y,krImage.getSurface()->w,krImage.getSurface()->h};
	SDL_BlitSurface(krImage.getSurface(), NULL, m_tpScreen, &dst);
}
