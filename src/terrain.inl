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
#include "conversor.h"


inline Image& Terrain::getImage()
{
    return m_kImage;
}


inline unsigned int Terrain::getSeed()
{
    return m_uiSeed;
}


inline float Terrain::getHeight(float x)
{
    return Conversor::toWorldY(m_ipHorizonBuffer[Conversor::toScreenX(x)]);
}


inline int Terrain::getHorizon(int x)
{
    return m_ipHorizonBuffer[x];
}


inline void Terrain::setHorizon(int x, int h)
{
    m_ipHorizonBuffer[x] = h;
}
