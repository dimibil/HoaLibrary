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

#include "AmbisonicsRestitution.h"

AmbisonicsRestitution::AmbisonicsRestitution(long anOrder, double aConfiguration, long aResitutionMode, long aVectorSize) : Ambisonics(anOrder, aVectorSize)
{
    m_number_of_virtual_loudspeakers = m_number_of_harmonics+1;
    m_decoder = new AmbisonicsDecoder(anOrder, m_number_of_virtual_loudspeakers, m_vector_size);
    
    Cicm_Vector_Float_Malloc(m_vector_float_input, m_number_of_virtual_loudspeakers);
    Cicm_Vector_Double_Malloc(m_vector_double_input, m_number_of_virtual_loudspeakers);
    Cicm_Vector_Float_Malloc(m_loudspeakers_vector_float, m_number_of_virtual_loudspeakers);
    Cicm_Vector_Double_Malloc(m_loudspeakers_vector_double, m_number_of_virtual_loudspeakers);
    
    m_loudspeakers_gains_vector_float   = NULL;
    m_loudspeakers_gains_vector_double  = NULL;
    m_angles_of_loudspeakers = NULL;
    
    m_restitution_mode = Tools::clip(aResitutionMode, (long)Hoa_Amplitude_Panning, (long)Hoa_Microphone_Simulation);
    setConfiguration(aConfiguration);
}

void AmbisonicsRestitution::setRestitutionMode(long aResitutionMode)
{
    m_restitution_mode = Tools::clip(aResitutionMode, (long)Hoa_Amplitude_Panning, (long)Hoa_Microphone_Simulation);
    if(m_restitution_mode == Hoa_Amplitude_Panning)
        computeAmplitudePanning();
    else
        computeMicrophoneSimulation();
}

long AmbisonicsRestitution::getRestitutionMode()
{
    return m_restitution_mode;
}

void AmbisonicsRestitution::setConfiguration(double aConfiguration)
{
    /* Initialize the configuration */
    m_number_of_real_loudspeakers    = Tools::clip_min((long)aConfiguration, (long)1);
    m_configuation                   = m_number_of_real_loudspeakers;
    if(aConfiguration - (long)aConfiguration != 0.)
    {
        m_low_frequency_effect = 1;
        m_configuation += 0.1;
    }
    else
        m_low_frequency_effect = 0;
    
    /* Free memories */
    if(m_loudspeakers_gains_vector_float && m_loudspeakers_gains_vector_double)
    {
        for(int i = 0; i < m_number_of_real_loudspeakers; i++)
        {
            //Cicm_Free(m_loudspeakers_gains_vector_double[i]);
            //Cicm_Free(m_loudspeakers_gains_vector_float[i]);
        }
        Cicm_Free(m_loudspeakers_gains_vector_double);
        Cicm_Free(m_loudspeakers_gains_vector_float);
    }
    if(m_angles_of_loudspeakers)
        free(m_angles_of_loudspeakers);
    
    /* Alloc memories */
    m_loudspeakers_gains_vector_float   = new Cicm_Vector_Float[m_number_of_real_loudspeakers];
    m_loudspeakers_gains_vector_double  = new Cicm_Vector_Double[m_number_of_real_loudspeakers];
    m_angles_of_loudspeakers = new double[m_number_of_real_loudspeakers];
    for(int i = 0; i < m_number_of_real_loudspeakers; i++)
    {
        Cicm_Vector_Float_Malloc(m_loudspeakers_gains_vector_float[i], m_number_of_virtual_loudspeakers);
        Cicm_Vector_Double_Malloc(m_loudspeakers_gains_vector_double[i], m_number_of_virtual_loudspeakers);
    }
    
    /* Define standard configuration */
    m_angles_of_loudspeakers = new double[m_number_of_real_loudspeakers];
    if(m_number_of_real_loudspeakers == 1)          // Mono //
    {
        m_angles_of_loudspeakers[0] = 0.;
    }
    else if(m_number_of_real_loudspeakers == 2)     // Stereo //
    {
        m_angles_of_loudspeakers[0] = 30. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[1] = 330. / 360. * CICM_2PI;
    }
    else if(m_number_of_real_loudspeakers == 3)     // Dolby Surround //
    {
        m_angles_of_loudspeakers[0] = 30. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[1] = 180. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[2] = 330. / 360. * CICM_2PI;
    }
    else if(m_number_of_real_loudspeakers == 4)     // Quadriphonic //
    {
        m_angles_of_loudspeakers[0] = 45. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[1] = 135. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[2] = 225. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[3] = 315. / 360. * CICM_2PI;
    }
    else if(m_number_of_real_loudspeakers == 5)     // Surround 5.1 //
    {
        m_angles_of_loudspeakers[0] = 0. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[1] = 30. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[2] = 110. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[3] = 250. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[4] = 330. / 360. * CICM_2PI;
    }
    else if(m_number_of_real_loudspeakers == 6)     // Surround 6.1 //
    {
        m_angles_of_loudspeakers[0] = 0. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[1] = 30. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[2] = 110. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[3] = 180. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[4] = 250. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[5] = 330. / 360. * CICM_2PI;
    }
    else if(m_number_of_real_loudspeakers == 7)     // Surround 7.1 //
    {
        m_angles_of_loudspeakers[0] = 0. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[1] = 30. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[2] = 110. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[3] = 135 / 360. * CICM_2PI;
        m_angles_of_loudspeakers[4] = 225 / 360. * CICM_2PI;
        m_angles_of_loudspeakers[5] = 250. / 360. * CICM_2PI;
        m_angles_of_loudspeakers[6] = 330. / 360. * CICM_2PI;
    }
    else                                           // Ambisonics base //
    {
        for (int i = 0; i < m_number_of_real_loudspeakers; i++)
            m_angles_of_loudspeakers[i] = (double)i / (double)m_number_of_real_loudspeakers * CICM_2PI;
    }
    
    if(m_restitution_mode == Hoa_Amplitude_Panning)
        computeAmplitudePanning();
    else
        computeMicrophoneSimulation();
    m_number_of_outputs = m_number_of_real_loudspeakers + m_low_frequency_effect;
}

double AmbisonicsRestitution::getConfiguration()
{
    return m_configuation;
}

void AmbisonicsRestitution::setLoudspeakerAngle(long anIndex, double anAngle)
{
    if(anIndex >= 0 && anIndex < m_number_of_real_loudspeakers)
    {
        anAngle = Tools::radianWrap(anAngle / 360. * CICM_2PI);
        m_angles_of_loudspeakers[anIndex] = anAngle;
    }
    Tools::sortVector(m_angles_of_loudspeakers, m_number_of_real_loudspeakers);
    if(m_restitution_mode == Hoa_Amplitude_Panning)
        computeAmplitudePanning();
    else
        computeMicrophoneSimulation();
}

double AmbisonicsRestitution::getLoudspeakerAngle(long anIndex)
{
    if(anIndex >= 0 && anIndex < m_number_of_real_loudspeakers)
        return m_angles_of_loudspeakers[anIndex] / CICM_2PI * 360.;
    else
        return 0.;
}

std::string AmbisonicsRestitution::getLoudspeakerName(long anIndex)
{
    if(m_number_of_real_loudspeakers == 1)          // Mono //
    {
        return "Omnidirectionnal Channel";
    }
    else if(m_number_of_real_loudspeakers == 2)     // Stereo //
    {
        if(anIndex == 0)
            return "Left channel";
        else if(anIndex == 1)
            return "Right channel";
        else if(anIndex == 2)
            return "Lfe channel";
    }
    else if(m_number_of_real_loudspeakers == 3)     // Dolby Surround //
    {
        if(anIndex == 0)
            return "Left channel";
        else if(anIndex == 1)
            return "Right channel";
        else if(anIndex == 2)
            return "Back channel";
        else if(anIndex == 3)
            return "Lfe channel";
    }
    else if(m_number_of_real_loudspeakers == 5)     // Surround 5.1 //
    {
        if(anIndex == 0)
            return "Center channel";
        else if(anIndex == 1)
            return "Front left channel";
        else if(anIndex == 2)
            return "Surround left channel";
        else if(anIndex == 3)
            return "Surround right channel";
        else if(anIndex == 4)
            return "Front right channel";
        else if(anIndex == 5)
            return "Lfe channel";
    }
    else if(m_number_of_real_loudspeakers == 6)     // Surround 6.1 //
    {
        if(anIndex == 0)
            return "Front center channel";
        else if(anIndex == 1)
            return "Front left channel";
        else if(anIndex == 2)
            return "Surround left channel";
        else if(anIndex == 3)
            return "Back center channel";
        else if(anIndex == 4)
            return "Surround right channel";
        else if(anIndex == 5)
            return "Front right channel";
        else if(anIndex == 6)
            return "Lfe channel";
    }
    else if(m_number_of_real_loudspeakers == 7)     // Surround 7.1 //
    {
        if(anIndex == 0)
            return "Front center channel";
        else if(anIndex == 1)
            return "Front left channel";
        else if(anIndex == 2)
            return "Surround left channel";
        else if(anIndex == 3)
            return "Back left channel";
        else if(anIndex == 4)
            return "Back right channel";
        else if(anIndex == 5)
            return "Surround right channel";
        else if(anIndex == 6)
            return "Front right channel";
        else if(anIndex == 7)
            return "Lfe channel";
    }
    else                                           // Ambisonics base //
    {
        if(anIndex >= 0 && anIndex < m_number_of_real_loudspeakers)
            return "Channel " + Tools::intToString(anIndex) + " : " + Tools::floatToStringOneDecimal(m_angles_of_loudspeakers[anIndex]/ CICM_2PI * 360.) + "°";
        else if(anIndex == m_number_of_real_loudspeakers && m_low_frequency_effect)
            return "Lfe channel";
        else
            return "No channel";
    }
}

void AmbisonicsRestitution::computeAmplitudePanning()
{
    for(int i = 0; i < m_number_of_virtual_loudspeakers; i++)
    {
        double virtualAngle = ((double)i / (double)m_number_of_virtual_loudspeakers) * CICM_2PI;
       
        double ratio1;
        double ratio2;
        
        if(virtualAngle <= m_angles_of_loudspeakers[0])
        {
             // Le virtualHp est < à Hp(0) -> gain entre Hp(0) et Hp(last) //
            int ilast = m_number_of_real_loudspeakers - 1;
            double distance = m_angles_of_loudspeakers[0] + (CICM_2PI - m_angles_of_loudspeakers[ilast]);
            
            ratio1 = (m_angles_of_loudspeakers[0] - virtualAngle) / distance;
            ratio2 = 1. - ratio1;
            m_loudspeakers_gains_vector_double[0][i] = cos(ratio1 * CICM_PI2);
            m_loudspeakers_gains_vector_float[0][i] =  m_loudspeakers_gains_vector_double[0][i];
            
            m_loudspeakers_gains_vector_double[ilast][i] = cos(ratio2 * CICM_PI2);
            m_loudspeakers_gains_vector_float[ilast][i] = m_loudspeakers_gains_vector_double[ilast][i];
            
            // Le gain des autres Hp est nul //
            for(int j = 1; j < m_number_of_real_loudspeakers - 1; j++)
            {
                m_loudspeakers_gains_vector_double[j][i] = 0.;
                m_loudspeakers_gains_vector_float[j][i] = 0.;
            }
        }
        else if(virtualAngle > m_angles_of_loudspeakers[m_number_of_real_loudspeakers - 1])
        {
            // Le virtualHp est  à 0 -> gain entre le 0 et le dernier //
            int ilast = m_number_of_real_loudspeakers - 1;
            double distance = m_angles_of_loudspeakers[0] + (CICM_2PI - m_angles_of_loudspeakers[ilast]);
            
            ratio1 = (m_angles_of_loudspeakers[0] - (virtualAngle - CICM_2PI)) / distance ;
            ratio2 = (virtualAngle - m_angles_of_loudspeakers[ilast]) / distance;
            
            m_loudspeakers_gains_vector_double[0][i] = cos(ratio1 * CICM_PI2);
            m_loudspeakers_gains_vector_float[0][i] =  m_loudspeakers_gains_vector_double[0][i];
            
            m_loudspeakers_gains_vector_double[ilast][i] = cos(ratio2  * CICM_PI2);
            m_loudspeakers_gains_vector_float[ilast][i] = m_loudspeakers_gains_vector_double[ilast][i];
            
            // Le gain des autres Hp est nul //
            for(int j = 1; j < m_number_of_real_loudspeakers - 1; j++)
            {
                m_loudspeakers_gains_vector_double[j][i] = 0.;
                m_loudspeakers_gains_vector_float[j][i] = 0.;
            }
        }
        else
        {
            for(int j = 0; j < m_number_of_real_loudspeakers - 1; j++)
            {
                if(virtualAngle > m_angles_of_loudspeakers[j] && virtualAngle <= m_angles_of_loudspeakers[j+1])
                {
                    double distance = m_angles_of_loudspeakers[j+1] - m_angles_of_loudspeakers[j];
                    
                    ratio1 = (virtualAngle - m_angles_of_loudspeakers[j]) / distance;
                    ratio2 = (m_angles_of_loudspeakers[j+1] - virtualAngle) / distance;
                    
                    m_loudspeakers_gains_vector_double[j][i] = cos(ratio1 * CICM_PI2);
                    m_loudspeakers_gains_vector_float[j][i] =  m_loudspeakers_gains_vector_double[j][i];
                    
                    m_loudspeakers_gains_vector_double[j+1][i] = cos(ratio2 * CICM_PI2);
                    m_loudspeakers_gains_vector_float[j+1][i] =  m_loudspeakers_gains_vector_double[j+1][i];
                    
                    for(int k = 0; k < m_number_of_real_loudspeakers; k++)
                    {
                        if(k != j && k != j+1)
                        {
                            m_loudspeakers_gains_vector_double[k][i] = 0.;
                            m_loudspeakers_gains_vector_float[k][i] = 0.;
                        }
                    }
                }
            }
        }
        
    }

}


void AmbisonicsRestitution::computeMicrophoneSimulation()
{
    for(int i = 0; i < m_number_of_real_loudspeakers - 1; i++)
    {
        for(int j = 0; j < m_number_of_virtual_loudspeakers; j++)
        {
            double distance;
            double virtualAngle = ((double)i / (double)m_number_of_virtual_loudspeakers) * CICM_2PI;
            distance = fabs(m_angles_of_loudspeakers[i] - virtualAngle) / 2.;
            m_loudspeakers_gains_vector_double[i][j] = cos(distance);
            m_loudspeakers_gains_vector_float[j][i] = m_loudspeakers_gains_vector_double[i][j];
        }
    }    
}


AmbisonicsRestitution::~AmbisonicsRestitution()
{
    Cicm_Free(m_vector_float_input);
    Cicm_Free(m_vector_double_input);
    Cicm_Free(m_loudspeakers_vector_float);
    Cicm_Free(m_loudspeakers_vector_double);
    for(int i = 0; i < m_number_of_real_loudspeakers; i++)
    {
        Cicm_Free(m_loudspeakers_gains_vector_double[i]);
        Cicm_Free(m_loudspeakers_gains_vector_float[i]);
    }
    Cicm_Free(m_loudspeakers_gains_vector_double);
    Cicm_Free(m_loudspeakers_gains_vector_float);
    free(m_angles_of_loudspeakers);
	delete m_decoder;
}

