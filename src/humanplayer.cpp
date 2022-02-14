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



#include "SDL/SDL.h"
#include "humanplayer.h"


HumanPlayer::HumanPlayer()
:
  Player()
{
    m_iType = HUMAN_PLAYER;
}


HumanPlayer::~HumanPlayer()
{
}


void HumanPlayer::doAction(Player::Action &eAction)
{
    bool bNotFired = true;
    bool bQuit = false;
    char cKey = 0;
    SDL_Event tEvent;


    if (m_fLife > 0)
    {
            while (SDL_PollEvent(&tEvent))
            {
                switch(tEvent.type)
                {
                    case SDL_QUIT:
                    break;

                    case SDL_KEYDOWN:
                        switch(tEvent.key.keysym.sym)
                        {
                            case SDLK_UP:
                                if (m_fPower < 1000)
                                    eAction = Player::INC_SPEED;
                            break;

                            case SDLK_DOWN:
                                if (m_fPower >= 1)
                                    eAction = Player::DEC_SPEED;
                            break;

                            case SDLK_LEFT:
                                if (m_fAngle < 180)
                                    eAction = Player::INC_ANGLE;
                            break;

                            case SDLK_RIGHT:
                                if (m_fAngle >= 0)
                                    eAction = Player::DEC_ANGLE;
                            break;

                            case SDLK_F10:
                                eAction = Player::SAVE;
                            break;

                            case SDLK_SPACE:
                                eAction = Player::FIRE;
                            break;

                            case SDLK_ESCAPE:
                                eAction = Player::QUIT;
                            break;
                        }
                    break;

                    case SDL_KEYUP:
                        eAction = Player::DO_NOTHING;
                    break;
               }
            }
    }
}
