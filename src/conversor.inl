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
inline void Conversor::set(int iScreenWidth, int iScreenHeight, float fWorldWidth, float fWorldHeight)
{
    m_iScreenWidth = iScreenWidth;
    m_iScreenHeight = iScreenHeight;
    m_fWorldWidth = fWorldWidth;
    m_fWorldHeight = fWorldHeight;
}


inline int Conversor::getScreenWidth()
{
    return m_iScreenWidth;
}


inline int Conversor::getScreenHeight()
{
    return m_iScreenHeight;
}


inline float Conversor::getWorldWidth()
{
    return m_fWorldWidth;
}


inline float Conversor::getWorldHeight()
{
    return m_fWorldHeight;
}


inline int Conversor::toScreenX(float fWorldX)
{
    return (int)( (fWorldX * m_iScreenWidth) / m_fWorldWidth );
}


inline int Conversor::toScreenY(float fWorldY)
{
    return (int)( ((m_fWorldHeight - fWorldY) * m_iScreenHeight) / m_fWorldHeight );
}


inline float Conversor::toWorldX(int iScreenX)
{
    return ( (float)(iScreenX * m_fWorldWidth) / (float)m_iScreenWidth );
}


inline float Conversor::toWorldY(int iScreenY)
{
    return ( ((float)(m_iScreenHeight - iScreenY) * m_fWorldHeight) / (float)m_iScreenHeight );
}
