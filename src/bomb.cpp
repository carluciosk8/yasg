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


#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265359f
#endif

#define MAX_DAMAGE        100.0f
#define DAMAGE_DISTANCE   33.0f

#include "bomb.h"
#include "terrain.h"
#include "player.h"


Bomb::Bomb()
: m_kImage(4,4,32),
  m_fDamageDistance(DAMAGE_DISTANCE)
{
    int x = 2;
    int y = 2;
    SDL_Color color = {255,255,255};

    m_kImage.fillCircle(x, y, 2, color);
}


Bomb::~Bomb()
{
}


void Bomb::fire(float fStartX, float fStartY, float fStartAngle, float fStartSpeed)
{
    m_fPosX = fStartX;
    m_fPosY = fStartY;
    m_fVelX = fStartSpeed * cosf(fStartAngle * (M_PI/180.0f));
    m_fVelY = fStartSpeed * sinf(fStartAngle * (M_PI/180.0f));
}


void Bomb::update(float fDeltaTime, float fGrav, float fWind)
{
    // do a simple euler integration:
    //
    // dvx = w.dt
    //                 w.dt²
    // dx  = Vx.dt  +  -----
    //                  2
    // dvy = -g.dt
    //                 g.dt²
    // dy  = Vy.dt  -  -----
    //                  2
    m_fVelX += fWind * fDeltaTime;
    m_fVelY += -fGrav * fDeltaTime;

    //m_fPosX += (m_fVelX * fDeltaTime);
    m_fPosX += m_fVelX*fDeltaTime + 0.5f*fWind*fDeltaTime*fDeltaTime;
    m_fPosY += m_fVelY*fDeltaTime - 0.5f*fGrav*fDeltaTime*fDeltaTime;
}


bool Bomb::checkCollision(Terrain& krTerrain)
{
    if (m_fPosY < krTerrain.getHeight(m_fPosX))
        return true;
    else
        return false;
}


void Bomb::doDamage(Player& krPlayer)
{
    float fDamage = 0;
    float fDeltaX = m_fPosX - krPlayer.getPosX();
    float fDeltaY = m_fPosY - krPlayer.getPosY();
    float fDist = sqrtf(fDeltaX*fDeltaX + fDeltaY*fDeltaY);

    if (fDist < DAMAGE_DISTANCE)
    {
        fDamage = ((DAMAGE_DISTANCE-fDist)*MAX_DAMAGE) / DAMAGE_DISTANCE;
        krPlayer.subLife(fDamage);
    }
}
