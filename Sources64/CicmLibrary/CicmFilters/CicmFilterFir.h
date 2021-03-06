/*
 *
 * Copyright (C) 2012 Pierre Guillot, Universite Paris 8
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

#ifndef DEFFIR
#define DEFFIR

#include "CicmFilter.h"

class FilterFir : public Filter
{
protected:
	
	Cicm_Vector_Float	m_input_vector;
	Cicm_Vector_Float	m_fir_vector;
	long			m_fir_size;
	long			m_input_size;
	int             m_index;;

public:
	FilterFir(long anImpulseSize = 128);
	void	setImpulseResponse(Cicm_Vector_Float anImpulseResponse);
	
    inline Cicm_Float process(Cicm_Float anInput)
    {
        Cicm_Float result;
        m_input_vector[--m_index] = anInput;
        Cicm_Vector_Float_Dot_Product(m_input_vector+m_index, m_fir_vector, &result, m_fir_size);
        if(m_index <= 0)
        {
            m_index = m_fir_size;
            Cicm_Vector_Float_Copy(m_input_vector, m_input_vector+m_fir_size, m_fir_size);
        }
        return result;
    }
    
	~FilterFir();
};



#endif