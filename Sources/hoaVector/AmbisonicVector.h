/*
 *
 * Copyright (C) 2012 Julien Colafrancesco & Pierre Guillot, Universite Paris 8
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

#ifndef DEF_AMBISONICVECTOR
#define DEF_AMBISONICVECTOR

#include "cicmTools.h"

class Ambisonicvector
{	
private:
	long		m_number_of_loudspeakers;
	long		m_number_of_inputs;
	long		m_number_of_outputs;
	long		m_vector_size;
    double*     m_abscissa_of_loudspeakers;
    double*     m_ordinate_of_loudspeakers;
    int         m_mode;
public:
	Ambisonicvector(long aNumberOfLoudspeakers, std::string aMode = "polar", long aVectorSize = 0);
	
	long getNumberOfLoudspeakers();
	long getNumberOfInputs();
	long getNumberOfOutputs();
	long getVectorSize();
    std::string getMode();

    void setMode(std::string aMode);
	void setVectorSize(long aVectorSize);
    
	~Ambisonicvector();
	
	/* Perform sample by sample */
	void process(double* anInput, double *anOutput)
	{
        double sum = 0., weihtedSumAbscissa = 0, weihtedSumOrdinate = 0,  sumAbs = 0.,weihtedSumAbsAbscissa = 0, weihtedSumAbsOrdinate = 0;
        for (int i = 0; i < m_number_of_loudspeakers; i++)
        {
            sum += anInput[i];
            sumAbs += fabs(anInput[i]) * fabs(anInput[i]);
            weihtedSumAbscissa += anInput[i] * m_abscissa_of_loudspeakers[i];
            weihtedSumOrdinate += anInput[i] * m_ordinate_of_loudspeakers[i];
            weihtedSumAbsAbscissa += fabs(anInput[i]) * fabs(anInput[i]) * m_abscissa_of_loudspeakers[i];
            weihtedSumAbsOrdinate += fabs(anInput[i]) * fabs(anInput[i]) * m_ordinate_of_loudspeakers[i];
        }
        if(sum != 0.)
        {
            weihtedSumAbscissa /= sum;
            weihtedSumOrdinate /= sum;
            if(!m_mode)
            {
                anOutput[0] = Tools::radius(weihtedSumAbscissa, weihtedSumOrdinate);
                anOutput[1] = Tools::angle(weihtedSumAbscissa, weihtedSumOrdinate);
            }
            else
            {
                anOutput[0] = weihtedSumAbscissa;
                anOutput[1] = weihtedSumOrdinate;
            }
        }
        else
        {
            anOutput[0] = 0.;
            anOutput[1] = 0.;
        }
        if(sumAbs != 0.)
        {
            weihtedSumAbsAbscissa /= sumAbs;
            weihtedSumAbsOrdinate /= sumAbs;
            if(!m_mode)
            {
                anOutput[2] = Tools::radius(weihtedSumAbsAbscissa, weihtedSumAbsOrdinate);
                anOutput[3]= Tools::angle(weihtedSumAbsAbscissa, weihtedSumAbsOrdinate);
            }
            else
            {
                anOutput[2] = weihtedSumAbsAbscissa;
                anOutput[3] = weihtedSumAbsOrdinate;
            }
        }
        else
        {
            anOutput[2] = 0.;
            anOutput[3] = 0.;
        }
	}
    
    void process(float* anInput, float *anOutput)
	{
        double sum = 0., weihtedSumAbscissa = 0, weihtedSumOrdinate = 0,  sumAbs = 0.,weihtedSumAbsAbscissa = 0, weihtedSumAbsOrdinate = 0;
        for (int i = 0; i < m_number_of_loudspeakers; i++)
        {
            sum += anInput[i];
            sumAbs += fabs(anInput[i]) * fabs(anInput[i]);
            weihtedSumAbscissa += anInput[i] * m_abscissa_of_loudspeakers[i];
            weihtedSumOrdinate += anInput[i] * m_ordinate_of_loudspeakers[i];
            weihtedSumAbsAbscissa += fabs(anInput[i]) * fabs(anInput[i]) * m_abscissa_of_loudspeakers[i];
            weihtedSumAbsOrdinate += fabs(anInput[i]) * fabs(anInput[i]) * m_ordinate_of_loudspeakers[i];
        }
        if(sum != 0.)
        {
            weihtedSumAbscissa /= sum;
            weihtedSumOrdinate /= sum;
            if(!m_mode)
            {
                anOutput[0] = Tools::radius(weihtedSumAbscissa, weihtedSumOrdinate);
                anOutput[1] = Tools::angle(weihtedSumAbscissa, weihtedSumOrdinate);
            }
            else
            {
                anOutput[0] = weihtedSumAbscissa;
                anOutput[1] = weihtedSumOrdinate;
            }
        }
        else
        {
            anOutput[0] = 0.;
            anOutput[1] = 0.;
        }
        if(sumAbs != 0.)
        {
            weihtedSumAbsAbscissa /= sumAbs;
            weihtedSumAbsOrdinate /= sumAbs;
            if(!m_mode)
            {
                anOutput[2] = Tools::radius(weihtedSumAbsAbscissa, weihtedSumAbsOrdinate);
                anOutput[3]= Tools::angle(weihtedSumAbsAbscissa, weihtedSumAbsOrdinate);
            }
            else
            {
                anOutput[2] = weihtedSumAbsAbscissa;
                anOutput[3] = weihtedSumAbsOrdinate;
            }
        }
        else
        {
            anOutput[2] = 0.;
            anOutput[3] = 0.;
        }

	}
	
	/* Perform sample block */
	void process(double** anInputVector, double** anOutputVector)
	{
        for(int j = 0; j < m_vector_size; j++)
		{
            double sum = 0., weihtedSumAbscissa = 0, weihtedSumOrdinate = 0,  sumAbs = 0.,weihtedSumAbsAbscissa = 0, weihtedSumAbsOrdinate = 0;
            for (int i = 0; i < m_number_of_loudspeakers; i++)
            {
                sum += anInputVector[i][j];
                sumAbs += fabs(anInputVector[i][j]) * fabs(anInputVector[i][j]);
                weihtedSumAbscissa += anInputVector[i][j] * m_abscissa_of_loudspeakers[i];
                weihtedSumOrdinate += anInputVector[i][j] * m_ordinate_of_loudspeakers[i];
                weihtedSumAbsAbscissa += fabs(anInputVector[i][j]) * fabs(anInputVector[i][j]) * m_abscissa_of_loudspeakers[i];
                weihtedSumAbsOrdinate += fabs(anInputVector[i][j]) * fabs(anInputVector[i][j]) * m_ordinate_of_loudspeakers[i];
            }
            if(sum != 0.)
            {
                weihtedSumAbscissa /= sum;
                weihtedSumOrdinate /= sum;
                if(!m_mode)
                {
                    anOutputVector[0][j] = Tools::radius(weihtedSumAbscissa, weihtedSumOrdinate);
                    anOutputVector[1][j] = Tools::angle(weihtedSumAbscissa, weihtedSumOrdinate);
                }
                else
                {
                    anOutputVector[0][j] = weihtedSumAbscissa;
                    anOutputVector[1][j] = weihtedSumOrdinate;
                }
            }
            else
            {
                anOutputVector[0][j] = 0.;
                anOutputVector[1][j] = 0.;
            }
            if(sumAbs != 0.)
            {
                weihtedSumAbsAbscissa /= sumAbs;
                weihtedSumAbsOrdinate /= sumAbs;
                if(!m_mode)
                {
                    anOutputVector[2][j] = Tools::radius(weihtedSumAbsAbscissa, weihtedSumAbsOrdinate);
                    anOutputVector[3][j] = Tools::angle(weihtedSumAbsAbscissa, weihtedSumAbsOrdinate);
                }
                else
                {
                    anOutputVector[2][j] = weihtedSumAbsAbscissa;
                    anOutputVector[3][j] = weihtedSumAbsOrdinate;
                }
            }
            else
            {
                anOutputVector[2][j] = 0.;
                anOutputVector[3][j] = 0.;
            }            
		}
	}
    
    void process(float** anInputVector, float** anOutputVector)
	{
        for(int j = 0; j < m_vector_size; j++)
		{
            double sum = 0., weihtedSumAbscissa = 0, weihtedSumOrdinate = 0,  sumAbs = 0.,weihtedSumAbsAbscissa = 0, weihtedSumAbsOrdinate = 0;
            for (int i = 0; i < m_number_of_loudspeakers; i++)
            {
                sum += anInputVector[i][j];
                sumAbs += fabs(anInputVector[i][j]) * fabs(anInputVector[i][j]);
                weihtedSumAbscissa += anInputVector[i][j] * m_abscissa_of_loudspeakers[i];
                weihtedSumOrdinate += anInputVector[i][j] * m_ordinate_of_loudspeakers[i];
                weihtedSumAbsAbscissa += fabs(anInputVector[i][j]) * fabs(anInputVector[i][j]) * m_abscissa_of_loudspeakers[i];
                weihtedSumAbsOrdinate += fabs(anInputVector[i][j]) * fabs(anInputVector[i][j]) * m_ordinate_of_loudspeakers[i];
            }
            if(sum != 0.)
            {
                weihtedSumAbscissa /= sum;
                weihtedSumOrdinate /= sum;
                if(!m_mode)
                {
                    anOutputVector[0][j] = Tools::radius(weihtedSumAbscissa, weihtedSumOrdinate);
                    anOutputVector[1][j] = Tools::angle(weihtedSumAbscissa, weihtedSumOrdinate);
                }
                else
                {
                    anOutputVector[0][j] = weihtedSumAbscissa;
                    anOutputVector[1][j] = weihtedSumOrdinate;
                }
            }
            else
            {
                anOutputVector[0][j] = 0.;
                anOutputVector[1][j] = 0.;
            }
            if(sumAbs != 0.)
            {
                weihtedSumAbsAbscissa /= sumAbs;
                weihtedSumAbsOrdinate /= sumAbs;
                if(!m_mode)
                {
                    anOutputVector[2][j] = Tools::radius(weihtedSumAbsAbscissa, weihtedSumAbsOrdinate);
                    anOutputVector[3][j] = Tools::angle(weihtedSumAbsAbscissa, weihtedSumAbsOrdinate);
                }
                else
                {
                    anOutputVector[2][j] = weihtedSumAbsAbscissa;
                    anOutputVector[3][j] = weihtedSumAbsOrdinate;
                }
            }
            else
            {
                anOutputVector[2][j] = 0.;
                anOutputVector[3][j] = 0.;
            }
		}
	}
	
};

#endif