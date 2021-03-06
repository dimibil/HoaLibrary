/*
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

extern "C"
{
	#include "ext.h"
	#include "ext_obex.h"
	#include "z_dsp.h"
}

#include "ambisonicWeight.h"


typedef struct _HoaWeight 
{
	t_pxobject					f_ob;			
	ambisonicWeight				*f_ambisonicWeight;
	
	int						f_ninput;
	int						f_noutput;
} t_HoaWeight;

void *HoaWeight_new(t_symbol *s, long argc, t_atom *argv);
void HoaWeight_free(t_HoaWeight *x);
void HoaWeight_assist(t_HoaWeight *x, void *b, long m, long a, char *s);

void HoaWeight_optim(t_HoaWeight *x, t_symbol *s, long argc, t_atom *argv);
void HoaWeight_dsp(t_HoaWeight *x, t_signal **sp, short *count);
t_int *HoaWeight_perform(t_int *w);

void HoaWeight_dsp64(t_HoaWeight *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void HoaWeight_perform64(t_HoaWeight *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

void *HoaWeight_class;

int main(void)
{	

	t_class *c;
	
	c = class_new("hoa.weight~", (method)HoaWeight_new, (method)dsp_free, (long)sizeof(t_HoaWeight), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)HoaWeight_dsp,			"dsp",		A_CANT, 0);
	class_addmethod(c, (method)HoaWeight_dsp64,			"dsp64",	A_CANT, 0);
	class_addmethod(c, (method)HoaWeight_assist,		"assist",	A_CANT, 0);
	class_addmethod(c, (method)HoaWeight_optim,			"optim",	A_GIMME, 0);
	
	class_dspinit(c);				
	class_register(CLASS_BOX, c);	
	HoaWeight_class = c;
	
	return 0;
}

void *HoaWeight_new(t_symbol *s, long argc, t_atom *argv)
{
	t_HoaWeight *x = NULL;
	int order = 4;
	if (x = (t_HoaWeight *)object_alloc((t_class*)HoaWeight_class)) 
	{
		if(atom_gettype(argv) == A_LONG)
			order = atom_getlong(argv);
		
		x->f_ambisonicWeight	= new ambisonicWeight(order, sys_getblksize());
		
		dsp_setup((t_pxobject *)x, x->f_ambisonicWeight->getParameters("numberOfInputs"));
		for (int i = 0; i < x->f_ambisonicWeight->getParameters("numberOfOutputs"); i++) 
			outlet_new(x, "signal");	
	}
	return (x);
}

void HoaWeight_dsp64(t_HoaWeight *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	object_method(dsp64, gensym("dsp_add64"), x, HoaWeight_perform64, 0, NULL);
}

void HoaWeight_perform64(t_HoaWeight *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	x->f_ambisonicWeight->process(ins, outs);
}

void HoaWeight_dsp(t_HoaWeight *x, t_signal **sp, short *count)
{
	int i;
	int pointer_count;
	t_int **sigvec;
	
	x->f_ninput = x->f_ambisonicWeight->getParameters("numberOfInputs");
	x->f_noutput = x->f_ambisonicWeight->getParameters("numberOfOutputs");
	pointer_count = x->f_ambisonicWeight->getParameters("numberOfInputs") + x->f_ambisonicWeight->getParameters("numberOfOutputs") + 2;
	
	sigvec  = (t_int **)calloc(pointer_count, sizeof(t_int *));
	for(i = 0; i < pointer_count; i++)
		sigvec[i] = (t_int *)calloc(1, sizeof(t_int));
	
	sigvec[0] = (t_int *)x;
	sigvec[1] = (t_int *)sp[0]->s_n;
	for(i = 2; i < pointer_count; i++)
		sigvec[i] = (t_int *)sp[i - 2]->s_vec;
	
	dsp_addv(HoaWeight_perform, pointer_count, (void **)sigvec);
	
	free(sigvec);
}

t_int *HoaWeight_perform(t_int *w)
{
	t_HoaWeight *x			= (t_HoaWeight *)(w[1]);	
	t_float		**ins		= (t_float **)w+3;
	t_float		**outs		= (t_float **)w+3+x->f_ninput;
	
	x->f_ambisonicWeight->process(ins, outs);
	return (w + x->f_ninput + x->f_noutput + 3);
}

void HoaWeight_assist(t_HoaWeight *x, void *b, long m, long a, char *s)
{
	long harmonicIndex = 0;
	if (a == 0)
		harmonicIndex = 0;
	else
		harmonicIndex = floor((a - 1) / 2) + 1;
		if (a % 2 == 1)
			harmonicIndex = - harmonicIndex;
	
	if (m == ASSIST_INLET)
		sprintf(s,"(Signal) Harmonic %ld", harmonicIndex);
	else 
		sprintf(s,"(Signal) Harmonic %ld", harmonicIndex);			
}

void HoaWeight_optim(t_HoaWeight *x, t_symbol *s, long argc, t_atom *argv)
{
	if(atom_gettype(argv) == A_SYM)
	{
		std::string decodingId = atom_getsym(argv)->s_name;
		x->f_ambisonicWeight->setOptimMode(decodingId);
	}
}

void HoaWeight_free(t_HoaWeight *x)
{
	dsp_free((t_pxobject *)x);
	free(x->f_ambisonicWeight);
}

