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


#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "image.h"

#define COMPUTER_PLAYER 0
#define HUMAN_PLAYER    1


class Player
{
    public:
        
        Player();
        virtual ~Player();

        enum Action
        {
            INC_SPEED,
            DEC_SPEED,
            INC_ANGLE,
            DEC_ANGLE,
            FIRE,
			SAVE,
            DO_NOTHING,
            QUIT
        };

        // Do player action. Derived classes must implement this method. Human
        // and computer controlled players are implemented transparently.
        virtual void doAction(Action&)=0;

        void update();

        inline void incAngle() { m_fAngle+=0.2f; }
        inline void decAngle() { m_fAngle-=0.2f; }
        inline void incPower() { m_fPower+=0.2f; }
        inline void decPower() { m_fPower-=0.2f; }

        inline void subLife(float fDamage) { m_fLife -= fDamage; }

        inline float getLife() { return m_fLife; }
        inline float getAngle() { return m_fAngle; }
        inline float getPower() { return m_fPower; }
        inline float getPosX() { return m_fPosX; }
        inline float getPosY() { return m_fPosY; }
        inline Image& getImage() { return m_kImage; }
		inline int getType() { return m_iType; }

        inline void setLife(float fLife) { m_fLife = fLife; }
        inline void setAngle(float fAngle) { m_fAngle = fAngle; }
        inline void setPower(float fPower) { m_fPower = fPower; }
        inline void setPosX(float fPosX) { m_fPosX = fPosX; }
        inline void setPosY(float fPosY) { m_fPosY = fPosY; }
        inline void setPosXY(float fPosX, float fPosY) { m_fPosX = fPosX; m_fPosY = fPosY; }


    protected:
        float m_fLife;
        float m_fAngle;
        float m_fPower;
        float m_fPosX;
        float m_fPosY;
        Image m_kImage;
		int  m_iType;
};



#endif
