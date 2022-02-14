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

#include "player.h"



Player::Player()
: m_fLife(100.0f),
  m_fAngle(60.0f),
  m_fPower(100.0f),
  m_kImage(20,20,32)
{}


Player::~Player()
{}


void Player::update()
{
    int x = 10;
    int y = 10;
    float fCannonSize = 10.0f;
    SDL_Color color = {0,0,255};

    SDL_FillRect(m_kImage.getSurface(), 0, 0);
    if (getLife() > 0)
    {
        m_kImage.fillCircle(x, y, 5, color);
        m_kImage.line(x, y,
             x + fCannonSize*cosf(m_fAngle*(M_PI/180.0f)), y - fCannonSize*sinf(m_fAngle*(M_PI/180.0f)),
             color);
        m_kImage.line(x - 1.0f, y,
             x - 1.0f + fCannonSize*cosf(m_fAngle*(M_PI/180.0f)), y - fCannonSize*sinf(m_fAngle*(M_PI/180.0f)),
             color);
        m_kImage.line(x, y + 1.0f,
             x + fCannonSize*cosf(m_fAngle*(M_PI/180.0f)), y + 1.0f - fCannonSize*sinf(m_fAngle*(M_PI/180.0f)),
             color);
    }
}