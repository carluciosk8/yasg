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
#ifndef __IMAGE_H__
#define __IMAGE_H__


#include "SDL/SDL.h"

class Image
{
    public:
        Image(const char* cpFilename);
        Image(int iWidth, int iHeight, int iBpp);
        ~Image();

        int getWidth();
        int getHeight();
        int getBpp();
        SDL_Surface* getSurface();

        void clear();
        void getPixel(int x, int y, SDL_Color& color);
        void putPixel(int x, int y, const SDL_Color& color);
        void line(int xa, int ya, int xb, int yb, const SDL_Color& color);
        void box(int xa, int ya, int xb, int yb, const SDL_Color& color);
        void circle(int x, int y, int rad, const SDL_Color& color);
        void ellipse(int x, int y, int radx, int rady, const SDL_Color& color);
        void fillBox(int xa, int ya, int xb, int yb, const SDL_Color& color);
        void fillCircle(int x, int y, int rad, const SDL_Color& color);
        void fillEllipse(int x, int y, int radx, int rady, const SDL_Color& color);


    protected:
        SDL_Surface* m_tpSurf;


    private:
        void circlePutPixel(int cx, int cy, int x, int y, const SDL_Color& color);
        void ellipsePutPixel(int cx, int cy, int x, int y, const SDL_Color& color);
        void fillellipsePutPixel(int cx, int cy, int x, int y, const SDL_Color& color);
};


#endif //__IMAGE_H__
