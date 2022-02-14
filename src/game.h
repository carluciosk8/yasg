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


#ifndef __GAME_H__
#define __GAME_H__

#include <vector>

using std::vector;

#include "image.h"
#include "font.h"
#include "music.h"
#include "sound.h"
#include "player.h"
#include "terrain.h"
#include "bomb.h"
#include "SDLrenderer.h"

#define WORLD_WIDTH         1280
#define WORLD_HEIGHT        1024
#define SCREEN_WIDTH        800
#define SCREEN_HEIGHT       600
#define SCREEN_BPP          32
#define SCREEN_FLAGS        SDL_ANYFORMAT //| SDL_FULLSCREEN

#define TIME_SCALE          3.0f


class Game
{
    public:

        Game();
        ~Game();
        
        void showIntro();
        void showStartScreen(Image& krTitleImage, int iNumPlayers, bool bFirstTime);

        void addPlayer(Player*);
        void clearPlayers();
        int getNumPlayers();

        void run(bool bLoadGame);
        void load();
        void save();


    protected:

        // Management
        bool  m_bIsRunning;
        bool  m_bNotQuit;
        int  m_iCurrentPlayer;

        // game
        float m_fGravity;
        float m_fWind;
        Terrain  m_kTerrain;
        Bomb  m_kBomb;
        vector<Player*>  m_kPlayers;

        // gfx & sfx
        SDLRenderer  m_kRenderer;
        Image m_kBackgroundImage;
        Font m_kMenuFont;
        Font m_kPlacarFont;
        Font m_kBigFont;
        Music m_kIntroMusic;
        Music m_kGameMusic;
        Sound m_kFireSound;
        Sound m_kExplosionSound;
        Sound m_kDeathSound;


    private:

        void initWorld(bool bGameLoad);
        void drawWorld();
        void updateWorld();
        float getTime();

        bool checkWinner(int iPlayer);
        void fire(int iPlayer);

        void createBackgroundImage(Image& krImage);
        void playExplosionAnim();
};

#include "game.inl"


#endif
