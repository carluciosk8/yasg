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
#include <stdexcept>

using std::runtime_error;

#include "sound.h"


Sound::Sound(const char* cpFilename)
{
    m_tpSound = Mix_LoadWAV(cpFilename);
    if (m_tpSound == NULL)
        throw runtime_error("Sound error: cannot open file");
}



Sound::~Sound()
{
    Mix_FreeChunk(m_tpSound);
}



void Sound::play()
{
    Mix_PlayChannel(0, m_tpSound, 0);
}
