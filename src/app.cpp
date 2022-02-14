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


#define MIN_PLAYERS 2
#define MAX_PLAYERS 4

#include "SDL/SDL.h"

#include "app.h"
#include "humanplayer.h"
#include "computerplayer.h"
#include "image.h"



void App::init()
{
    m_kGame.showIntro();
}



void App::shutdown()
{
}



void App::run()
{
    bool bGameRun=true;
    bool bGameNotStart=true;
    bool bFirstTime=true;
    bool bLoadGame=false;
    SDL_Event tEvent;
    Image kTitleImage("resources/title.png");


    while (bGameRun)
    {
        bFirstTime = true;
        bLoadGame=false;
        while (bGameNotStart)
        {
            m_kGame.showStartScreen(kTitleImage, m_kGame.getNumPlayers(), bFirstTime);
            bFirstTime = false;

            while (SDL_PollEvent(&tEvent))
            {
                switch(tEvent.type)
                {
                    case SDL_QUIT:
                    break;

                    case SDL_KEYDOWN:
                        switch(tEvent.key.keysym.sym)
                        {
                            case SDLK_F1:
                                if (m_kGame.getNumPlayers() < MAX_PLAYERS)
                                    m_kGame.addPlayer(new HumanPlayer);
                            break;

                            case SDLK_F2:
                                if (m_kGame.getNumPlayers() < MAX_PLAYERS)
                                    m_kGame.addPlayer(new ComputerPlayer);
                            break;

                            case SDLK_F10:
                                bGameNotStart = false;
                                bLoadGame=true;
                            break;

                            case SDLK_SPACE:
                                bGameNotStart = false;
                            break;

                            case SDLK_ESCAPE:
                                bGameNotStart = false;
                                bGameRun = false;
                            break;
                        }
                    break;
                }
            }
        }


        if (bGameRun)
        {
            if ( (m_kGame.getNumPlayers() >= MIN_PLAYERS && m_kGame.getNumPlayers() <= MAX_PLAYERS) || bLoadGame )
            {
                m_kGame.run(bLoadGame);
                m_kGame.clearPlayers();
            }
            bGameNotStart = true;
        }
    }
}
