/*
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


#include "AmbisonicPolyEase.h"

AmbisonicPolyEase::AmbisonicPolyEase(long anOrder, long aNumberOfSources, long aVectorSize)
{
    m_order					= Tools::clip_min(anOrder, (long)1);
	m_number_of_harmonics	= m_order * 2 + 1;
	m_number_of_outputs		= m_number_of_harmonics;
    m_number_of_sources     = Tools::clip(aNumberOfSources, (long)1, (long)64);;
    m_number_of_inputs		= m_number_of_sources;
    
    for(int i = 0; i < m_number_of_sources; i++)
    {
        m_eases.push_back(new AmbisonicEase(m_order));
        m_mute.push_back(0);
    }
    setVectorSize(aVectorSize);    
    for(int i = 0; i < m_number_of_sources; i++)
       setPolarCoordinates(i, 1., 0.);
     
}

long AmbisonicPolyEase::getOrder()
{
	return m_order;
}

long AmbisonicPolyEase::getNumberOfHarmonics()
{
	return m_number_of_harmonics;
}

long AmbisonicPolyEase::getNumberOfInputs()
{
	return m_number_of_inputs;
}

long AmbisonicPolyEase::getNumberOfOutputs()
{
	return m_number_of_outputs;
}

long AmbisonicPolyEase::getVectorSize()
{
	return m_vector_size;
}

long AmbisonicPolyEase::getNumberOfSources()
{
	return m_number_of_sources;
}

float AmbisonicPolyEase::getRadius(long aSourceIndex)
{
    return m_radius[aSourceIndex];
}

float AmbisonicPolyEase::getAzimuth(long aSourceIndex)
{
    return m_azimuth[aSourceIndex];
}

long AmbisonicPolyEase::getMuted(long aSourceIndex)
{
    if(aSourceIndex >= 0 && aSourceIndex < m_number_of_sources)
    {
        return m_mute[aSourceIndex];
    }
    return 1;
}

void AmbisonicPolyEase::setVectorSize(long aVectorSize)
{
	m_vector_size = Tools::clip_power_of_two(aVectorSize);
    for(int i = 0; i < m_number_of_sources; i++)
        m_eases[i]->setVectorSize(m_vector_size);
}

void AmbisonicPolyEase::setPolarCoordinates(long aSourceIndex, double aRadius, double anAzimuth)
{
    if(aSourceIndex >= 0 && aSourceIndex < m_number_of_sources)
    {
        double abs = Tools::abscisse(aRadius, anAzimuth + CICM_PI2);
        double ord = Tools::ordinate(aRadius, anAzimuth + CICM_PI2);
        m_eases[aSourceIndex]->setCartesianCoordinatesLine(abs, ord);
    }
}

void AmbisonicPolyEase::setCartesianCoordinates(long aSourceIndex, double anAbscissa, double anOrdinate)
{
    if(aSourceIndex >= 0 && aSourceIndex < m_number_of_sources)
    {
        m_eases[aSourceIndex]->setCartesianCoordinatesLine(anAbscissa, anOrdinate);
    }
}

void AmbisonicPolyEase::setMuted(long aSourceIndex, long aValue)
{
    if(aSourceIndex >= 0 && aSourceIndex < m_number_of_sources)
    {
        m_mute[aSourceIndex] = Tools::clip(aValue, (long)0, (long)1);
    }
}

void AmbisonicPolyEase::setNumberOfSources(long aNumberOfSources)
{
    Tools::clip(aNumberOfSources, (long)1, (long)64);
    if(aNumberOfSources > m_number_of_sources)
    {
        for(int i = m_number_of_sources; i < aNumberOfSources; i++)
        {
            m_eases.push_back(new AmbisonicEase(m_order));
            setPolarCoordinates(i, 1, 0);
            m_mute.push_back(0);
        }
        m_number_of_sources = aNumberOfSources;
    }
    else if(aNumberOfSources < m_number_of_sources)
    {
        for(int i = m_number_of_sources; i < aNumberOfSources; i++)
        {
            m_eases.pop_back();
            m_mute.pop_back();
        }
        m_number_of_sources = aNumberOfSources;
    }
    m_number_of_inputs = m_number_of_sources;
}

AmbisonicPolyEase::~AmbisonicPolyEase()
{
	for(int i = 0; i < m_number_of_sources; i++)
	{
        delete m_eases[i];
        m_eases[i] = 0;
        m_mute.pop_back();
	}
}

