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


#ifndef __SDLRENDERER_H__
#define __SDLRENDERER_H__

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

class Image;
class Font;

class SDLRenderer
{
    public:
        SDLRenderer(int iWidth, int iHeight, int iBpp, Uint32 uiFlags);
        ~SDLRenderer();

        void draw(Image& krImage, int x, int y);
        void drawText(int x, int y, char* cpText, Font& krFont, SDL_Color tColor);
        void drawOutlineText(int x, int y, char* cpText, Font& krFont, int iBorderSize, SDL_Color tBorderColor, SDL_Color tCenterColor);
        void fadeScreen(bool fade_in, int time, Uint32 color);
        
        inline void swapBuffers() { SDL_Flip(m_tpScreen); }
        inline SDL_Surface* getSurface() { return m_tpScreen; }

    protected:
        SDL_Surface* m_tpScreen;
};


#endif
