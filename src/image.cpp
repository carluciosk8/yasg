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

#include "image.h"
#include "SDL/SDL_image.h"

#define round(a) ((int)(a+0.5))


Image::Image(int iWidth, int iHeight, int iBpp)
{
    m_tpSurf = SDL_CreateRGBSurface(SDL_SWSURFACE, iWidth, iHeight, iBpp, 0, 0, 0, 0);
    if (m_tpSurf == NULL)
        throw runtime_error("Image error: cannot open file");
    SDL_SetColorKey(m_tpSurf, SDL_SRCCOLORKEY, SDL_MapRGB(m_tpSurf->format, 0, 0, 0 ));
}



Image::Image(const char* cpFilename)
{
    m_tpSurf = IMG_Load(cpFilename);
}



Image::~Image()
{
    SDL_FreeSurface(m_tpSurf);
}



int Image::getWidth()
{
    return m_tpSurf->w;
}



int Image::getHeight()
{
    return m_tpSurf->h;
}



int Image::getBpp()
{
    return m_tpSurf->format->BitsPerPixel;
}



SDL_Surface* Image::getSurface()
{
    return m_tpSurf;
}



void Image::clear()
{
    SDL_FillRect(m_tpSurf,0,0);
}



void Image::getPixel(int x, int y, SDL_Color& c)
{
    int bpp = m_tpSurf->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)m_tpSurf->pixels + y * m_tpSurf->pitch + x * bpp;
    Uint32 color;


    switch(bpp)
    {
        case 1:
            color = *p;
        break;

        case 2:
            color = *(Uint16 *)p;
        break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                color = p[0] << 16 | p[1] << 8 | p[2];
            else
                color = p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            color = *(Uint32 *)p;
        break;

        default:
            color = 0;
        break;
    }
    SDL_GetRGB(color, m_tpSurf->format, &(c.r), &(c.g), &(c.b));
}



void Image::putPixel(int x, int y, const SDL_Color& c)
{
    Uint32 color = SDL_MapRGB(m_tpSurf->format, c.r, c.g, c.b);
    SDL_LockSurface(m_tpSurf);

    if ( x < m_tpSurf->w && x >=0 && y < m_tpSurf->h && y >=0 )
    {
        int bpp = m_tpSurf->format->BytesPerPixel;

        Uint8 *p = (Uint8 *)m_tpSurf->pixels + y * m_tpSurf->pitch + x * bpp;

        switch (bpp)
        {
            case 1:
              *p = color;
            break;

            case 2:
              *(Uint16 *)p = color;
            break;

            case 3:
                if ( SDL_BYTEORDER == SDL_BIG_ENDIAN )
                {
                p[0] = (color >> 16) & 0xff;
                p[1] = (color >> 8) & 0xff;
                p[2] = color & 0xff;
                }
                else
                {
                p[0] = color & 0xff;
                p[1] = (color >> 8) & 0xff;
                p[2] = (color >> 16) & 0xff;
                }
            break;

            case 4:
              *(Uint32 *)p = color;
            break;
        }
    }
    
    SDL_UnlockSurface(m_tpSurf);
}



void Image::line(int xa, int ya, int xb, int yb, const SDL_Color& color)
{
    int x;
    int dx;
    int xinc1;
    int xinc2;
    int y;
    int dy;
    int yinc1;
    int yinc2;
    int d;
    int dinc1;
    int dinc2;
    int i;
    int numpixels;


    if ( xb < xa )
      dx = xa - xb;
     else
      dx = xb - xa;
    if ( yb < ya )
      dy = ya - yb;
     else
      dy = yb - ya;

    if ( dx >= dy )
    {
      numpixels = dx + 1;
      d = 2*dy - dx;
      dinc1 = 2*dy;
      dinc2 = 2*(dy - dx);
      xinc1 = 1;
      xinc2 = 1;
      yinc1 = 0;
      yinc2 = 1;
     }
     else
     {
      numpixels = dy + 1;
      d = 2*dx - dy;
      dinc1 = 2*dx;
      dinc2 = 2*(dx - dy);
      xinc1 = 0;
      xinc2 = 1;
      yinc1 = 1;
      yinc2 = 1;
    }
    if ( xa>xb )
    {
      xinc1 = -xinc1;
      xinc2 = -xinc2;
    }
    if ( ya>yb )
    {
      yinc1 = -yinc1;
      yinc2 = -yinc2;
    }
    x = xa;
    y = ya;


    for ( i=0 ; i<numpixels ; i++ )
    {
      putPixel(x, y, color);
      if ( d < 0 )
      {
        d += dinc1;
        x += xinc1;
        y += yinc1;
       }
       else
       {
        d += dinc2;
        x += xinc2;
        y += yinc2;
      }
    }
}



void Image::box(int xa, int ya, int xb, int yb, const SDL_Color& color)
{
    int x;
    int y;
    int t;

    if ( xb < xa )
    {
      t = xa;
      xa = xb;
      xb = t;
    }

    if ( yb < ya )
    {
      t = ya;
      ya = yb;
      yb = t;
    }

    for ( x=xa ; x<xb ; x++ )
    {
      putPixel(x,ya , color);
      putPixel(x,yb , color);
    }
    for ( y=ya ; y<yb ; y++ )
    {
      putPixel(xa,y , color);
      putPixel(xb,y , color);
    }
                                                                             //
}



void Image::circle(int cx, int cy, int rad, const SDL_Color& color)
{
    int x = 0;
    int y = rad;
    int d = 1 - rad;
    int dE = 3;
    int dSE = -2*rad + 5;

    circlePutPixel(cx, cy, x, y, color );
    while ( y>x )
    {
      if ( d<0 )
      {
        d += dE;
        dE += 2;
        dSE += 2;
        }
        else
        {
        d += dSE;
        dE += 2;
        dSE += 4;
        y--;
      }
      x++;
      circlePutPixel(cx,cy, x,y, color );
    }
}



void Image::ellipse(int cx, int cy, int radx, int rady, const SDL_Color& color)
{
    int rx2 = radx*radx;
    int ry2 = rady*rady;
    int tworx2 = 2*rx2;
    int twory2 = 2*ry2;
    int p;
    int x = 0;
    int y = rady;
    int px = 0;
    int py = tworx2*y;

    ellipsePutPixel(cx,cy, x,y, color );
    p = round( ry2 - (rx2*rady) + (0.25*rx2) );
    while ( px < py )
    {
      x++;
      px += twory2;
      if ( p<0 )
        p += ry2 + px;
      else
      {
        y--;
        py -= tworx2;
        p += ry2 + px - py;
      }
      ellipsePutPixel(cx,cy, x,y, color );
    }

    p = round( (float)ry2*((float)x+0.5)*((float)x+0.5) + (float)rx2*(y-1)*(y-1) - (float)rx2*ry2 );
    while ( y > 0 )
    {
      y--;
      py -= tworx2;
      if ( p>0 )
        p += rx2 - py;
      else
      {
        x++;
        px += twory2;
        p += rx2 - py + px;
      }
      ellipsePutPixel(cx,cy, x,y, color );
    }
}



void Image::fillBox(int xa, int ya, int xb, int yb, const SDL_Color& color)
{
    int x;
    int y;
    int t;

    if ( xb < xa )
    {
      t = xa;
      xa = xb;
      xb = t;
    }

    if ( yb < ya )
    {
      t = ya;
      ya = yb;
      yb = t;
    }

    for ( x=xa ; x<xb ; x++ )
      for ( y=ya ; y<yb ; y++ )
        putPixel(x, y, color);
}



void Image::fillCircle(int cx, int cy, int rad, const SDL_Color& color)
{
    int r2 = rad*rad;
    int twor2 = 2*r2;
    int p;
    int x = 0;
    int y = rad;
    int px = 0;
    int py = twor2*y;

    fillellipsePutPixel(cx,cy, x,y, color );
    p = round( r2 - (r2*rad) + (0.25*r2) );
    while ( px < py )
    {
      x++;
      px += twor2;
      if ( p<0 )
        p += r2 + px;
       else
       {
        y--;
        py -= twor2;
        p += r2 + px - py;
      }
      fillellipsePutPixel(cx,cy, x,y, color );
    }

    p = round( r2*(x + 0.5)*(x + 0.5) + r2*(y - 1)*(y - 1) - r2*r2 );
    while ( y > 0 )
    {
      y--;
      py -= twor2;
      if ( p>0 )
        p += r2 - py;
       else
       {
        x++;
        px += twor2;
        p += r2 - py + px;
      }
      fillellipsePutPixel(cx,cy, x,y, color );
    }

}



void Image::fillEllipse(int cx, int cy, int radx, int rady, const SDL_Color& color)
{
    int rx2 = radx*radx;
    int ry2 = rady*rady;
    int tworx2 = 2*rx2;
    int twory2 = 2*ry2;
    int p;
    int x = 0;
    int y = rady;
    int px = 0;
    int py = tworx2*y;

    fillellipsePutPixel(cx,cy, x,y, color );
    p = round( ry2 - (rx2*rady) + (0.25*rx2) );
    while ( px < py )
    {
      x++;
      px += twory2;
      if ( p<0 )
        p += ry2 + px;
       else
       {
        y--;
        py -= tworx2;
        p += ry2 + px - py;
      }
      fillellipsePutPixel(cx,cy, x,y, color );
    }

    p = round( ry2*(x + 0.5)*(x + 0.5) + rx2*(y - 1)*(y - 1) - rx2*ry2 );
    while ( y > 0 )
    {
      y--;
      py -= tworx2;
      if ( p>0 )
        p += rx2 - py;
       else
       {
        x++;
        px += twory2;
        p += rx2 - py + px;
      }
      fillellipsePutPixel(cx,cy, x,y, color );
    }
}



void Image::circlePutPixel(int cx, int cy, int x, int y, const SDL_Color& color)
{
    putPixel(cx+x, cy+y, color);
    putPixel(cx+y, cy+x, color);
    putPixel(cx+y, cy-x, color);
    putPixel(cx+x, cy-y, color);
    putPixel(cx-x, cy-y, color);
    putPixel(cx-y, cy-x, color);
    putPixel(cx-y, cy+x, color);
    putPixel(cx-x, cy+y, color);
} 



void Image::ellipsePutPixel(int cx, int cy, int x, int y, const SDL_Color& color)
{
    putPixel(cx+x, cy+y, color);
    putPixel(cx+x, cy-y, color);
    putPixel(cx-x, cy-y, color);
    putPixel(cx-x, cy+y, color);
}



void Image::fillellipsePutPixel(int cx, int cy, int x, int y, const SDL_Color& color)
{
    for (int i=0 ; i<=y ; i++)
    {
        putPixel(cx+x, cy+i, color);
        putPixel(cx+x, cy-i, color);
        putPixel(cx-x, cy-i, color);
        putPixel(cx-x, cy+i, color);
    }
}
