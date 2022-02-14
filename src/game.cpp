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
#include <ctime>
#include <cassert>
#include <fstream>
#include <iostream>

using std::ifstream;
using std::ofstream;
using std::ios;
using std::endl;
using std::cout;

#include <stdexcept>

using std::runtime_error;

#include "game.h"
#include "conversor.h"
#include "computerplayer.h"
#include "humanplayer.h"
#include "util.h"


Game::Game()
:
    m_kRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SCREEN_FLAGS),
    m_kTerrain(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP),
    m_kBackgroundImage(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP),
    m_kMenuFont("resources/menu.ttf", 20),
    m_kPlacarFont("resources/placar.ttf", 12),
    m_kBigFont("resources/win.ttf", 60),
    m_kIntroMusic("resources/title.mid"),
    m_kGameMusic("resources/game.mid"),
    m_kFireSound("resources/fire.wav"),
    m_kExplosionSound("resources/explosion.wav"),
    m_kDeathSound("resources/death.wav"),
    m_bIsRunning(false),
    m_bNotQuit(false),
    m_iCurrentPlayer(0),
    m_kPlayers(0),
    m_fGravity(9.81f)
{
    Conversor::set(SCREEN_WIDTH, SCREEN_HEIGHT, WORLD_WIDTH, WORLD_HEIGHT);
}



Game::~Game()
{}



void Game::run(bool bLoadGame)
{
    Player::Action eAction = Player::DO_NOTHING;
    int iWinner = -1;

    m_kGameMusic.play();
    initWorld(bLoadGame);

    while (m_bIsRunning)
    {
        m_kPlayers[m_iCurrentPlayer]->doAction(eAction);
        switch (eAction)
        {
            case Player::INC_SPEED:
                m_kPlayers[m_iCurrentPlayer]->incPower();
            break;

            case Player::DEC_SPEED:
                m_kPlayers[m_iCurrentPlayer]->decPower();
            break;

            case Player::INC_ANGLE:
                m_kPlayers[m_iCurrentPlayer]->incAngle();
            break;

            case Player::DEC_ANGLE:
                m_kPlayers[m_iCurrentPlayer]->decAngle();
            break;

            case Player::QUIT:
                m_bIsRunning = false;
            break;

            case Player::SAVE:
                save();
            break;

            case Player::FIRE:
                m_kFireSound.play();
                fire(m_iCurrentPlayer);

                // check if has a winner
                for (int i=0; i<getNumPlayers(); i++)
                    if (checkWinner(i))
                    {
                        iWinner = i;
                        m_bIsRunning = false;
                    }

                // next player
                m_iCurrentPlayer = (m_iCurrentPlayer + 1) % getNumPlayers();
                while (m_kPlayers[m_iCurrentPlayer]->getLife() <= 0)
                    m_iCurrentPlayer = (m_iCurrentPlayer + 1) % getNumPlayers();

                eAction = Player::DO_NOTHING;
                m_fWind = ((float)rand()/RAND_MAX)*1.0f - 0.5f;

            break;
        }
        updateWorld();
        drawWorld();
        m_kRenderer.swapBuffers();
    }

    if (iWinner >= 0)
    {
        char cpText[50];
        SDL_Color c1 = {200,200,200};
        SDL_Color c2 = {55,55,55};
        sprintf(cpText, "Player %d", iWinner+1);
        m_kRenderer.drawOutlineText(70, 190, cpText, m_kBigFont, 6, c2 ,c1);
        sprintf(cpText, "Wins!!!");
        m_kRenderer.drawOutlineText(160, 290, cpText, m_kBigFont, 6, c2 ,c1);
        m_kRenderer.swapBuffers();
        SDL_Delay(4000);
    }
}



void Game::showIntro()
{
    Image kIntroImage("resources/intro.png");

    m_kRenderer.draw(kIntroImage,0,0);
    m_kRenderer.fadeScreen(true, 2500, 0);
    SDL_Delay(2000);
    m_kRenderer.fadeScreen(false, 2500, 0);
}



void Game::showStartScreen(Image& krTitleImage, int iNumPlayers, bool bFirstTime)
{
    SDL_Color c1 = {215,215,215};
    SDL_Color c2 = {128,128,0};

    m_kRenderer.draw(krTitleImage,0,0);

    if (bFirstTime)
    {
        m_kIntroMusic.play();
        m_kRenderer.fadeScreen(true, 2500, 0);
    }

    m_kRenderer.drawOutlineText(280, 400, "[F1]  - ADDS A HUMAN PLAYER", m_kMenuFont, 1, c1, c2);
    m_kRenderer.drawOutlineText(280, 425, "[F2]  - ADDS A COMPUTER PLAYER", m_kMenuFont, 1, c1, c2);
    m_kRenderer.drawOutlineText(280, 450, "[F10] - LOAD GAME", m_kMenuFont, 1, c1, c2);
    m_kRenderer.drawOutlineText(280, 475, "SPACE - START GAME", m_kMenuFont, 1, c1, c2);
    m_kRenderer.drawOutlineText(280, 500, "ESC   - QUIT GAME", m_kMenuFont, 1, c1, c2);

    char cpText[50];
    sprintf(cpText, "TOTAL PLAYERS: %d", iNumPlayers);
    m_kRenderer.drawOutlineText(299, 540, cpText, m_kMenuFont, 1, c2, c1);

    m_kRenderer.swapBuffers();
}



void Game::initWorld(bool bGameLoad)
{
    float fMarginX = (float)WORLD_WIDTH / 6.0f;
    float fPlayersDistanceX = (float)(WORLD_WIDTH - 2*fMarginX) / (float)(getNumPlayers() - 1);
    float fPlayerPosX;
    float fPlayerPosY;

    unsigned int uiSeed;
    int iNumPlayers;
    ifstream kFile;

    if (bGameLoad)
    {
        kFile.open("gamesav.dat", ios::in);
        if (!kFile)
            bGameLoad = false;
    }

    if (bGameLoad)
    {
        float fAngle, fPower, fLife;
        int iType;
        m_bIsRunning = true;

        kFile >> m_iCurrentPlayer
              >> m_fGravity
              >> m_fWind
              >> uiSeed
              >> iNumPlayers;

        m_kTerrain.initRandom(uiSeed);
        createBackgroundImage(m_kBackgroundImage);

        clearPlayers();
        fPlayerPosX = fMarginX;
        fPlayersDistanceX = (float)(WORLD_WIDTH - 2*fMarginX) / (float)(iNumPlayers - 1);
        for (int i=0; i<iNumPlayers; i++)
        {
            kFile >> iType >> fAngle >> fPower >> fLife;

            if (iType == COMPUTER_PLAYER)
            {
                addPlayer(new ComputerPlayer);
            }
            else
            {
                addPlayer(new HumanPlayer);
            }

            fPlayerPosY = m_kTerrain.getHeight(fPlayerPosX)+2.0f;
            m_kPlayers[i]->setPosXY(fPlayerPosX, fPlayerPosY);
            m_kPlayers[i]->setAngle(fAngle);
            m_kPlayers[i]->setPower(fPower);
            m_kPlayers[i]->setLife(fLife);
            fPlayerPosX += fPlayersDistanceX;
        }
    }
    else
    {
        // set common
        m_iCurrentPlayer = 0;
        m_bIsRunning = true;
        m_fWind = ((float)rand()/RAND_MAX)*1.0f - 0.5f;
        m_fGravity = 9.81f;

        // create a random new terrain
        m_kTerrain.initRandom((unsigned int)time(0));
        m_kTerrain.update();
        createBackgroundImage(m_kBackgroundImage);

        // initialize players positions
        fPlayerPosX = fMarginX;
        for (int i=0; i<getNumPlayers(); i++)
        {
            fPlayerPosY = m_kTerrain.getHeight(fPlayerPosX)+2.0f;
            m_kPlayers[i] -> setPosXY(fPlayerPosX, fPlayerPosY);
            fPlayerPosX += fPlayersDistanceX;
        }
    }
}



void Game::updateWorld()
{
}



void Game::drawWorld()
{
    m_kRenderer.draw(m_kBackgroundImage,0,0);
    m_kRenderer.draw(m_kTerrain.getImage(),0,0);

    // draw players
    for (int i=0; i<getNumPlayers(); i++)
    {
        m_kPlayers[i]->update();
        m_kRenderer.draw(m_kPlayers[i]->getImage(),
                         Conversor::toScreenX(m_kPlayers[i]->getPosX())-10,
                         Conversor::toScreenY(m_kPlayers[i]->getPosY())-10);
    }

    // draw placar
    char cpText[150];
    SDL_Color color = {255,255,255};
    for (int i=0 ; i<m_kPlayers.size() ; i++)
    {
        sprintf(cpText, "Player: %d", i+1);
        m_kRenderer.drawText(Conversor::toScreenX(m_kPlayers[i]->getPosX())-50, 2, cpText, m_kPlacarFont, color);

        sprintf(cpText, "Life: %.2f", m_kPlayers[i]->getLife());
        m_kRenderer.drawText(Conversor::toScreenX(m_kPlayers[i]->getPosX())-50, 14, cpText, m_kPlacarFont, color);

        sprintf(cpText, "Angle: %.2f", m_kPlayers[i]->getAngle());
        m_kRenderer.drawText(Conversor::toScreenX(m_kPlayers[i]->getPosX())-50, 26, cpText, m_kPlacarFont, color);

        sprintf(cpText, "Power: %.2f", m_kPlayers[i]->getPower());
        m_kRenderer.drawText(Conversor::toScreenX(m_kPlayers[i]->getPosX())-50, 38, cpText, m_kPlacarFont, color);
    }
    sprintf(cpText, "Wind: %.2f", m_fWind*10.0f);
    m_kRenderer.drawText(SCREEN_WIDTH/2 - 20, 55, cpText, m_kPlacarFont, color);
}



void Game::fire(int iPlayer)
{
    bool bCollided = false;
    bool bTerrainCollided = false;
    float fOldTime = getTime();
    float fCurrentTime;
    float fDeltaTime;
    SDL_Event tEvent;

    m_kBomb.fire(m_kPlayers[m_iCurrentPlayer]->getPosX(),
                 m_kPlayers[m_iCurrentPlayer]->getPosY(),
                 m_kPlayers[m_iCurrentPlayer]->getAngle(),
                 m_kPlayers[m_iCurrentPlayer]->getPower());

    while (!bCollided)
    {
        fCurrentTime = getTime();
        fDeltaTime = fCurrentTime - fOldTime;
        fOldTime = fCurrentTime;

        m_kBomb.update(fDeltaTime, m_fGravity, m_fWind);

        drawWorld();
        m_kRenderer.draw(m_kBomb.getImage(), Conversor::toScreenX(m_kBomb.getPosX())-2,
                                             Conversor::toScreenY(m_kBomb.getPosY())-2);
        m_kRenderer.swapBuffers();

        if (m_kBomb.getPosX() < 0 || m_kBomb.getPosX() > WORLD_WIDTH)
            bCollided = true;
        else
            bTerrainCollided = bCollided = m_kBomb.checkCollision(m_kTerrain);

        if (SDL_PollEvent(&tEvent))
            if (tEvent.type == SDL_KEYDOWN)
                if (tEvent.key.keysym.sym == SDLK_ESCAPE)
                {
                    bCollided = true;
                    m_bIsRunning = false;
                }
    }

    if (m_bIsRunning)
    {
        if (bTerrainCollided)
        {
            int iRad = Conversor::toScreenX(m_kBomb.getDamageDistance());
            for (int px=-iRad ; px<=iRad ; px++)
            {
                int x = Conversor::toScreenX(m_kBomb.getPosX()) + px;
                int h = m_kTerrain.getHorizon(x) + sqrtf( pow(iRad,2) - pow(px,2) );
                if (x>=0 && x<SCREEN_WIDTH && h>=0 && h<SCREEN_HEIGHT)
                    m_kTerrain.setHorizon(x, h);
            }
            m_kTerrain.update();
            m_kExplosionSound.play();
            playExplosionAnim();

            for (int i=0; i<getNumPlayers(); i++)
            {
                if (m_kPlayers[i]->getLife() > 0)
                {
                    m_kBomb.doDamage(*m_kPlayers[i]);
                    if (m_kPlayers[i]->getLife() <= 0)
                    {
                        m_kDeathSound.play();
                        m_kPlayers[i]->setLife(0.0f);
                        char cpText[50];
                        SDL_Color c1 = {200,200,200};
                        SDL_Color c2 = {55,55,55};
                        sprintf(cpText, "Player %d", i+1);
                        m_kRenderer.drawOutlineText(70, 190, cpText, m_kBigFont, 6, c2 ,c1);
                        sprintf(cpText, "Dies!!!");
                        m_kRenderer.drawOutlineText(160, 290, cpText, m_kBigFont, 6, c2 ,c1);
                        m_kRenderer.swapBuffers();
                        SDL_Delay(3000);
                    }
                }
            }
            for (int i=0; i<getNumPlayers(); i++)
            {
                m_kPlayers[i] -> setPosY(m_kTerrain.getHeight(m_kPlayers[i]->getPosX())+2.0f);
            }
        }
    }
}



float Game::getTime()
{
    //return (float) TIME_SCALE * clock() / CLK_TCK;
    return (float) TIME_SCALE * SDL_GetTicks() / 1000.0f;
}



bool Game::checkWinner(int iPlayer)
{
    bool bWinner = false;
    int iCount = 0;

    for (int i=0; i<getNumPlayers(); i++)
        if (i != iPlayer)
            if (m_kPlayers[i]->getLife() > 0)
                iCount++;

    if (iCount == 0)
        bWinner = true;

    return bWinner;
}



void Game::createBackgroundImage(Image& krImage)
{
    SDL_Color tColorA={rand()%50, rand()%50, rand()%50};
    SDL_Color tColorB={rand()%256, rand()%256, rand()%56+200};
    float r, g, b;
    float dr, dg, db;

    r = tColorA.r;
    g = tColorA.g;
    b = tColorA.b;

    dr = ((float)tColorB.r - tColorA.r)/(float)krImage.getHeight();
    dg = ((float)tColorB.g - tColorA.g)/(float)krImage.getHeight();
    db = ((float)tColorB.b - tColorA.b)/(float)krImage.getHeight();

    for (int y=0 ; y<krImage.getHeight() ; y++)
    {
        SDL_Color color = {r,g,b};
        krImage.line(0, y, krImage.getWidth(), y, color);
        r += dr;
        g += dg;
        b += db;
    }
}



void Game::playExplosionAnim()
{
    int rad = Conversor::toScreenX(m_kBomb.getDamageDistance());
    float db = 256.0f/rad;
    //float b = 255;
    Image kImage(2*rad, 2*rad, 32);
    SDL_Color c = {255, 255, 0};


    SDL_SetColorKey(kImage.getSurface(), SDL_SRCCOLORKEY, SDL_MapRGB(SDL_GetVideoSurface()->format, 0, 0, 0 ));
    for (int r=0 ; r<rad ; r++)
    {
        //b = 255;
        kImage.clear();
        for (int r1=0 ; r1<r ; r1++)
        {
            c.b = r1*db;
            kImage.circle(rad, rad, r1, c);
        }
        drawWorld();
        m_kRenderer.draw(kImage, Conversor::toScreenX(m_kBomb.getPosX()) - rad,
                                 Conversor::toScreenY(m_kBomb.getPosY()) - rad);
        m_kRenderer.swapBuffers();
        SDL_Delay(10);
    }
    for (int r=0 ; r<rad ; r++)
    {
        //b = 255;
        kImage.clear();
        for (int r1=r ; r1<rad ; r1++)
        {
            c.b = r1*db;
            kImage.circle(rad, rad, r1, c);
            //b -= db;
        }
        drawWorld();
        m_kRenderer.draw(kImage, Conversor::toScreenX(m_kBomb.getPosX()) - rad,
                                 Conversor::toScreenY(m_kBomb.getPosY()) - rad);
        m_kRenderer.swapBuffers();
        SDL_Delay(10);
    }
}



void Game::save()
{
    ofstream kFile("gamesav.dat", ios::out);
    unsigned int uiSeed = m_kTerrain.getSeed();
    int iNumPlayers = m_kPlayers.size();

    SDL_Color c1 = {215,215,215};
    SDL_Color c2 = {128,128,0};

    if (!kFile)
        throw runtime_error("Game Error: cannot open file for save");

    m_kRenderer.drawOutlineText(70, 190, "Saving", m_kBigFont, 6, c2 ,c1);
    m_kRenderer.drawOutlineText(160, 290, "Game...", m_kBigFont, 6, c2 ,c1);
    m_kRenderer.swapBuffers();

    kFile << m_iCurrentPlayer << endl
          << m_fGravity << endl
          << m_fWind << endl
          << uiSeed << endl
          << iNumPlayers << endl;

    for (int i=0; i<iNumPlayers; i++)
    {
        kFile << m_kPlayers[i]->getType() << endl
              << m_kPlayers[i]->getAngle() << endl
              << m_kPlayers[i]->getPower() << endl
              << m_kPlayers[i]->getLife() << endl;
    }
    SDL_Delay(1000);
}
