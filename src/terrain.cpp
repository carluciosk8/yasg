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

#include "terrain.h"
#include "multifractal.h"
#include "util.h"


Terrain::Terrain(int iWidth, int iHeight, int iBpp)
:
    m_kImage(iWidth, iHeight, iBpp),
        m_ipHorizonBuffer(new int[iWidth])
{}


Terrain::~Terrain()
{
    delete[] m_ipHorizonBuffer;
}


void Terrain::initRandom(unsigned int uiSeed)
{
    srand(uiSeed);
    m_uiSeed = uiSeed;
    m_fWorldWidth = Conversor::getWorldWidth();
    m_fWorldHeight = Conversor::getWorldHeight();
    m_fStartX = (float) (rand() % 256);
    m_fStartY = (float) (rand() % 256);
    m_fOctaves = 2.0f;
    m_fAmplitude = randomBetween(m_fWorldHeight/4.0f, m_fWorldHeight/2.0f);
    m_fFrequency = randomBetween(0.005f, 0.06f);
    m_fHeight = 1.0f;
    m_fOffset = 1.0f;

    for (int x=0 ; x<m_kImage.getWidth() ; x++)
        m_ipHorizonBuffer[x] = Conversor::toScreenY((m_fWorldHeight / 3.0f) +
           multifractal(m_fStartX + x, m_fStartY, m_fOctaves, m_fAmplitude, m_fFrequency, m_fHeight, m_fOffset));
}



void Terrain::update()
{
    SDL_Color c = {255,255,255};
    int y1, y2;

    m_kImage.clear();
    for (int x=0 ; x<m_kImage.getWidth() ; x++)
    {
        y1 = m_ipHorizonBuffer[x];
        y2 = y1 + 50 + (rand()%50);
        if (y2 > m_kImage.getHeight())
            y2 = m_kImage.getHeight();
        for (int y=y1 ; y<y2 ; y++)
        {
            c.r = 220 + rand()%36;
            c.g = 220 + rand()%36;
            c.b = 220 + rand()%36;
            m_kImage.putPixel(x, y, c);
        }
        for (int y=y2 ; y<m_kImage.getHeight() ; y++)
        {
            c.r = rand()%100;
            c.g = 255;
            c.b = rand()%100;
            m_kImage.putPixel(x, y, c);
        }
    }
}
