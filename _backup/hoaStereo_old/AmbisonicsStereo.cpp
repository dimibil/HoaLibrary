/*
 *
 * Copyright (C) 2012 Julien Colafrancesco, Pierre Guillot & Eliott Paris, Universite Paris 8
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

#include "AmbisonicsStereo.h"

AmbisonicsStereo::AmbisonicsStereo(long anOrder, double aDelta, long aVectorSize) : Ambisonics(anOrder, aVectorSize)
{
	m_decoder = new AmbisonicsDecoder(anOrder, m_number_of_harmonics+1, aVectorSize);
}


AmbisonicsStereo::~AmbisonicsStereo()
{
	delete m_decoder;
}

