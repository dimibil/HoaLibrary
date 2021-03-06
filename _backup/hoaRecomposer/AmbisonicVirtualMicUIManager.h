/*
 *
 * Copyright (C) 2012 Julien Colafrancesco, Pierre Guillot, Eliott Paris Universite Paris 8
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation; either version 2 of the License.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#ifndef __hoa_recomposer__AmbisonicVirtualMicUIManager__
#define __hoa_recomposer__AmbisonicVirtualMicUIManager__

#include "CicmProjectHeader.h"
#include "AmbisonicVirtualMicUI.h"

class AmbisonicVirtualMicUIManager
{
private:
    int m_numberOfMics;
    AmbisonicVirtualMicUI m_mic[MAX_MICROPHONES];
    
public:
    AmbisonicVirtualMicUIManager();
    ~AmbisonicVirtualMicUIManager();
    
    void setAnglesInRadian(double* _radians, long _len);
    void setAnglesInDegree(double* _radians, long _len);
    void setAngleInRadian(const int _index, double _radian);
    void setAngleInDegree(const int _index, double _degree);
    void setDistance(const int _index, double _distance);
    void setSelected(const int _index, bool _selectedState);
    
    inline bool isSelected(const int _index) {return m_mic[Tools::clip(_index, 0, MAX_MICROPHONES-1)].isSelected();};
    inline double getDistance(const int _index) {return m_mic[Tools::clip(_index, 0, MAX_MICROPHONES-1)].getDistance();}
    inline double getAngleInRadian(const int _index) {return m_mic[Tools::clip(_index, 0, MAX_MICROPHONES-1)].getAngleInRadian();}
    inline double getAngleInDegree(const int _index) {return m_mic[Tools::clip(_index, 0, MAX_MICROPHONES-1)].getAngleInDegree();}
};

#endif /* defined(__hoa_recomposer__AmbisonicVirtualMicManager__) */
