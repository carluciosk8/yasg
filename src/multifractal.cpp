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
#include<cstdio>
#include "multifractal.h"

/*
From Game Programming Gems 2 - Programming Fractals - Jesse Laeuchli
*/

double randomnumber(int x,int y)
{
    short int n=x+y*57;
    n=(n<<13)^n;
    double ret;
    ret= (1 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 2147483647) / 1073741824.0);
    return ret;
}



double cosineinterpolation(double number1,double number2,double x)
{
    double ft;
    double f;
    double ret;
    ft = x * 3.1415927;
    f = (1.0 - cos(ft)) * 0.5;
    ret=number1*(1.0-f) + number2*f;
    return ret;
}



double smoothrandom(int x,int y)
{
    double corners=(randomnumber(x-1,y-1)+randomnumber(x+1,y-1)+randomnumber(x-1,y+1)+randomnumber(x+1,y+1))/16.0;
    double sides   = (randomnumber(x-1, y)+randomnumber(x+1, y)+randomnumber(x, y-1)+randomnumber(x, y+1) ) /8.0;
    double center  =  randomnumber(x, y) / 4.0;
    double ret=corners+sides+center;
    return ret;
}



float noise(float x, float y)
{
    printf("x: %f\n", x);
    int xinterger=static_cast<int>(x);
    double fractionx = x - static_cast<float>(xinterger);
    int yinteger=static_cast<int>(y);
    double fractiony = y - static_cast<float>(yinteger);
    double v1,v2,v3,v4,i1,i2;
    double ret;
    printf("fractionx: %f\n", fractionx);
    v1=smoothrandom(xinterger, yinteger);
    v2=smoothrandom(xinterger + 1, yinteger);
    v3=smoothrandom(xinterger, yinteger + 1);
    v4=smoothrandom(xinterger + 1, yinteger + 1);
    i1= cosineinterpolation (v1,v2,fractionx);
    i2= cosineinterpolation (v3,v4,fractionx);
    ret = cosineinterpolation (i1,i2,fractiony);
    return ret;
}



float multifractal(float x, float y, float octaves, float amplitude, float frequency, float h,float offset)
{
    double ret=1;
    for (int i=0; i<(octaves-1); i++)
    {
        printf("x: %f\n", x);
        printf("frequency: %f\n", frequency);
        ret *=(offset)*( noise(x* frequency, y* frequency)* amplitude);
        amplitude*=h;
    }
    return ret;
}

