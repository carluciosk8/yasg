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


#ifndef __BOMB_H__
#define __BOMB_H__

#include "image.h"

class Player;
class Terrain;


class Bomb
{
    public:
        Bomb();
        ~Bomb();
        void fire(float fStartX, float fStartY, float fStartAngle, float fStartSpeed);
        void update(float fDeltaTime, float fGrav, float fWind);
        bool checkCollision(Terrain& krTerrain);
        void doDamage(Player& krPlayer);

        inline float getPosX() { return m_fPosX; }
        inline float getPosY() { return m_fPosY; }
        inline float getDamageDistance() { return m_fDamageDistance; }
        inline Image& getImage() { return m_kImage; }

    
    protected:
        float m_fPosX;
        float m_fPosY;
        float m_fVelX;
        float m_fVelY;
        float m_fDamageDistance;
        Image m_kImage;
        //Image m_kExplosionImage;
};



#endif
