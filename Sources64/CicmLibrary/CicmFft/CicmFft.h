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

#ifndef DEF_FFT
#define DEF_FFT

#include "../CicmDefine.h"
#include "../CicmTools.h"

class Cicm_Fft
{
private:
	long				m_window_size;
	long				m_array_size;
	long				m_order;
	double				m_scale;

#ifdef CICM_VDSP
	Cicm_Fft_Handle		m_fft_handle;
#endif
#ifdef CICM_IPPS
	int					m_spec_size;
	int					m_init_size;
	int					m_buff_size;

	Cicm_Fft_Handle*	m_fft_handle;
	Cicm_Fft_Buffer*	m_fft_buff;
	Cicm_Fft_Buffer*	m_fft_init;
	Cicm_Fft_Buffer*	m_fft_spec;
#endif
#ifdef CICM_FFTW_GSL
	Cicm_Fft_Handle		m_handle_forward;
	Cicm_Fft_Handle		m_handle_inverse;
	Cicm_Signal*		m_real_vector;
	Cicm_Packed*		m_complex_vector;
#endif
	
public:
	Cicm_Fft(long aWindowSize = 1024);
	inline void forward(Cicm_Vector_Float aRealVector, Cicm_Complex_Packed_Float* anPackedVector);
	inline void inverse(Cicm_Complex_Packed_Float* anPackedVector, Cicm_Vector_Float aRealVector);
	long	getWindowSize();
	long	getArraySize();
	long	getOrder();
	double	getScale();	
	~Cicm_Fft();
};

inline void Cicm_Fft::forward(Cicm_Vector_Float aRealVector, Cicm_Complex_Packed_Float *anPackedVector)
{
#ifdef CICM_VDSP
	vDSP_ctoz((Cicm_Complex_Float *)aRealVector, 2, anPackedVector, 1, m_array_size); 
	Cicm_fft_forward(m_fft_handle, anPackedVector, m_order);
#endif
#ifdef CICM_IPPS
	Cicm_fft_forward(aRealVector, anPackedVector, m_fft_handle, m_fft_buff);
#endif
#ifdef CICM_FFTW_GSL
	Cicm_signal_copy(aRealVector, m_real_vector, m_window_size);
	Cicm_fft_forward(m_handle_forward);
	Cicm_signal_copy(m_complex_vector, anPackedVector, m_window_size);
#endif
}

inline void Cicm_Fft::inverse(Cicm_Complex_Packed_Float *anPackedVector, Cicm_Vector_Float aRealVector)
{
#ifdef CICM_VDSP
	Cicm_fft_inverse(m_fft_handle, anPackedVector, m_order);
	vDSP_ztoc(anPackedVector, 1, (Cicm_Complex_Float *)aRealVector, 2, m_array_size);
#endif
#ifdef CICM_IPPS
	Cicm_fft_inverse(anPackedVector, aRealVector, m_fft_handle, m_fft_buff);
#endif
#ifdef CICM_FFTW_GSL
	Cicm_signal_copy(anPackedVector, m_complex_vector, m_window_size);
	Cicm_fft_inverse(m_handle_inverse);
	Cicm_signal_copy(m_real_vector, aRealVector, m_window_size);
#endif
}
#endif
