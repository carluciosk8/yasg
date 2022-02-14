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


#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "image.h"


class Terrain
{
    public:
        Terrain(int iWidth, int iHeight, int iBpp);
        ~Terrain();

        void initRandom(unsigned int uiSeed);
        void update();

        inline Image& getImage();
        inline unsigned int getSeed();
        inline float getHeight(float x);

		inline int getHorizon(int x);
        inline void setHorizon(int x, int h);


    protected:
        unsigned int m_uiSeed;
        int *m_ipHorizonBuffer;
        float m_fWorldWidth;
        float m_fWorldHeight;
        float m_fStartX;
        float m_fStartY;
        float m_fOctaves;
        float m_fAmplitude;
        float m_fFrequency;
        float m_fHeight;
        float m_fOffset;
        Image m_kImage;
};

#include "terrain.inl"

#endif
