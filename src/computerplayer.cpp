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


#include "player.h"
#include "computerplayer.h"
#include "util.h"


ComputerPlayer::ComputerPlayer()
:
  Player()
{
    m_iType = COMPUTER_PLAYER;
}


ComputerPlayer::~ComputerPlayer()
{
}


void ComputerPlayer::doAction(Player::Action& eAction)
{
    if (m_fLife > 0)
    {
        m_fAngle = randomBetween(10.0f, 170.0f);
        m_fPower = randomBetween(30.0f, 150.0f);
    }

    eAction = Player::FIRE;
}
