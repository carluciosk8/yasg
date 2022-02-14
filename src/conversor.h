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
#ifndef __CONVERSOR_H__
#define __CONVERSOR_H__


class Conversor
{
public:
    static inline void set(int iScreenWidth, int iScreenHeight, float fWorldWidth, float fWorldHeight);

    static inline int getScreenWidth();
    static inline int getScreenHeight();
    static inline float getWorldWidth();
    static inline float getWorldHeight();

    static inline int toScreenX(float fWorldX);
    static inline int toScreenY(float fWorldY);
    static inline float toWorldX(int iScreenX);
    static inline float toWorldY(int iScreenY);


protected:
    static int m_iScreenWidth;
    static int m_iScreenHeight;
    static float m_fWorldWidth;
    static float m_fWorldHeight;
};

#include "conversor.inl"

#endif
