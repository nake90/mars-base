/*
	mars_base - Design, build and admin your own base on Mars
    Copyright (C) 2009  Alfonso Arbona Gimeno (nake90@terra.es). All rights reserved.

	MIT LICENSE

	Permission is hereby granted, free of charge, to any
	person obtaining a copy of this software and associated
	documentation files (the "Software"), to deal in the
	Software without restriction, including without limitation
	the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the
	Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice
	shall be included in all copies or substantial portions of
	the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
	KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
	WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
	OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
	OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

    If you use any part of this code you must give me (Alfonso Arbona Gimeno) credit.
    If you plan to use any part of this code on a comercial game please email me at:
	   	   nake90@terra.es
*/

/** \file atmosferico.c
 * \brief Controla todos los fenómenos atmosféricos y físicos del exterior
 * \author Alfonso Arbona Gimeno
*/

#include "atmosferico.h"
#include "display.h"
#include "objetos.h"

/* Niebla y atmósfera */
GLfloat fogColor[4]= {0.81f, 0.64f, 0.61f, 1.0f}; /*!< Color de la niebla */
GLfloat fogRange[2]= {25000.0f, 50000.0f}; /*!< Distancia mínima de la niebla y distancia máxima visible en metros */

/* TEXTURAS BÁSICAS */
/*					AMBIENT					DIFFUSE					SPECULAR				POSITION			HORA   MATERIAL*/
t_sun sun= {{0.5f, 0.5f, 0.5f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{10000.0f, 20000.0f, 30000.0f},{12.0f},
    {{1.0f, 1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f, 1.0f},{1.0},{0}}
};
//t_texture sun_texture=	{{1.0f, 1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f, 1.0f},{1.0},{0}};

// Masas atómicas de las moléculas
const double ma_co2		=	12.011 + 15.999 * 2;
const double ma_n2		=	14.007 * 2;
const double ma_ar		=	39.948;
const double ma_o2		=	15.999 * 2;
const double ma_co		=	12.011 + 15.999;
const double ma_h20		=	1.0079 * 2 + 15.999;
//const double ma_h2		=	1.0079 * 2;


// Densidad en *condiciones normales* de los gases [Kg/m3] (25ºC y 101337 Pa)
const double densN_co2		=	1.842;
const double densN_n2		=	1.165;
const double densN_ar		=	1.661;
const double densN_o2		=	1.331;
const double densN_co		=	1.165;
const double densN_h2o		=	0.755;	// ESTE ME LO HE INVENTADO.... He sumado la del H2 y la mitad del O2 ...
//const double densN_h2		=	;

// Viscosidades (En Kg/(m*s))
const double visc_co2	=	0.00001372;
const double visc_n2	=	0.00001657;
const double visc_ar	=	0.00002099;
const double visc_o2	=	0.00001909;
const double visc_co	=	0.00001372;
const double visc_h20	=	0.000013;
const double visc_media	=	0.00001609; // Esta es la realmente usada
//const double visc_h2	=	0.0000865;

/* Datos iniciales de la atmósfera */
t_atmosfera datos_atmosfera =
{
    /* Gases */			{
        /* CO2 */			0.9572 * PRESION_MEDIA / (GAS_R * TEMPERATURA_MEDIA),
        /* N2 */			0.0270 * PRESION_MEDIA / (GAS_R * TEMPERATURA_MEDIA),
        /* Ar */			0.0160 * PRESION_MEDIA / (GAS_R * TEMPERATURA_MEDIA),
        /* O2 */			0.0013 * PRESION_MEDIA / (GAS_R * TEMPERATURA_MEDIA),
        /* CO */			0.0007 * PRESION_MEDIA / (GAS_R * TEMPERATURA_MEDIA),
        /* H2O */			0.0003 * PRESION_MEDIA / (GAS_R * TEMPERATURA_MEDIA),
    },
    /* Presión */		PRESION_MEDIA,
    /* Temperatura */	TEMPERATURA_MEDIA
};

static inline float _get_node_density(t_gas gases)
{
    return (gases.CO2 * densN_co2 + gases.N2 * densN_n2 + gases.Ar * densN_ar
            + gases.O2 * densN_o2 + gases.CO * densN_co + gases.H2O * densN_h2o)
           / (gases.CO2 + gases.N2 + gases.Ar + gases.O2 + gases.CO + gases.H2O);
}

float get_density(float normal_density, float temperatura, float presion)
{
    if(temperatura<=0)return 0;
    return normal_density * presion * 298.0f / (101337.0f * temperatura);
}

float get_presion(t_gas gases, float volumen, float temperatura)
{
    if(volumen <= 0 || temperatura <= 0)return 0;
    float presion;
    // Presión total = Sum(Presiones parciales) y Presión parcial = n * R * T / Vtot
    presion = (gases.CO2 * GAS_R * temperatura) / volumen;
    presion += (gases.N2 * GAS_R * temperatura) / volumen;
    presion += (gases.Ar * GAS_R * temperatura) / volumen;
    presion += (gases.O2 * GAS_R * temperatura) / volumen;
    presion += (gases.CO * GAS_R * temperatura) / volumen;
    presion += (gases.H2O * GAS_R * temperatura) / volumen;

    return presion;
}

#ifdef __ECUACION__ANTIGUA__
void node_flow_gas(t_node_data *node1, t_node_data *node2, float size, float distancia, float step_time) // Size es el tamaño del lado
{
    if(size<=0 || step_time<=0)return;
    if(distancia<1)
    {
        distancia=1;    // Evitamos un dividir por 0, pero debería de estar todo bien hecho en el config del modelo
    }
    float pres1 = get_presion(node1->gases, node1->volumen, node1->temperatura);
    float pres2 = get_presion(node2->gases, node2->volumen, node2->temperatura);
    //float Q = (0.3333 * size - 0.5)*((((pres2 - pres1)/distancia) / (2 * visc_media)) * size * size); // Caudal
    float Q = ((pres1 - pres2)/distancia) * size * size * size * size / (12 * visc_media); // Caudal
    float volumen = Q * step_time; // Si volumen > 0 pasa de node1 a node2, si <0 pasa del 2 al 1
    t_gas delta_gas;
    float prop = ((pres1+pres2)/2) * volumen / (GAS_R * ((node1->temperatura+node2->temperatura)/2));
    prop /= 2000000000.0; // Así evitamos lo de que el caudal sea tan bestialmente grande
    if(volumen>0)
    {
        delta_gas.CO2 = node1->gases.CO2 * prop;
        delta_gas.N2 = node1->gases.N2 * prop;
        delta_gas.Ar = node1->gases.Ar * prop;
        delta_gas.O2 = node1->gases.O2 * prop;
        delta_gas.CO = node1->gases.CO * prop;
        delta_gas.H2O = node1->gases.H2O * prop;
    }
    else
    {
        delta_gas.CO2 = node2->gases.CO2 * prop;
        delta_gas.N2 = node2->gases.N2 * prop;
        delta_gas.Ar = node2->gases.Ar * prop;
        delta_gas.O2 = node2->gases.O2 * prop;
        delta_gas.CO = node2->gases.CO * prop;
        delta_gas.H2O = node2->gases.H2O * prop;
    }
    /*
    debug_printf("\n----------\n");
    debug_printf("Presión 1: %f Pa\n", pres1);
    debug_printf("Presión 2: %f Pa\n", pres2);
    debug_printf("Caudal: %f (m3/s)\n", Q);
    debug_printf("DELTA GASES (prop = %f):\n", prop);
    debug_printf("  CO2: %f (%f)\n", delta_gas.CO2, node1->gases.CO2);
    debug_printf("  N2:  %f (%f)\n", delta_gas.N2, node1->gases.N2);
    debug_printf("  Ar:  %f (%f)\n", delta_gas.Ar, node1->gases.Ar);
    debug_printf("  O2:  %f (%f)\n", delta_gas.O2, node1->gases.O2);
    debug_printf("  CO:  %f (%f)\n", delta_gas.CO, node1->gases.CO);
    debug_printf("  H2O: %f (%f)\n", delta_gas.H2O, node1->gases.H2O);
    //*/

    node1->gases.CO2 -= delta_gas.CO2;
    node1->gases.N2 -= delta_gas.N2;
    node1->gases.Ar -= delta_gas.Ar;
    node1->gases.O2 -= delta_gas.O2;
    node1->gases.CO -= delta_gas.CO;
    node1->gases.H2O -= delta_gas.H2O;

    node2->gases.CO2 += delta_gas.CO2;
    node2->gases.N2 += delta_gas.N2;
    node2->gases.Ar += delta_gas.Ar;
    node2->gases.O2 += delta_gas.O2;
    node2->gases.CO += delta_gas.CO;
    node2->gases.H2O += delta_gas.H2O;
}
#else
void node_flow_gas(t_node_data *node1, t_node_data *node2, float size, float distancia, float step_time) // Size es el tamaño del lado
{
    if(size<=0 || step_time<=0)return;
    if(distancia<1)
    {
        distancia=1;    // Evitamos un dividir por 0, pero debería de estar todo bien hecho en el config del modelo
    }
    float pres1 = get_presion(node1->gases, node1->volumen, node1->temperatura);
    float pres2 = get_presion(node2->gases, node2->volumen, node2->temperatura);

    // Calculamos la densidad normal de cada gas como la media ponderada de las densidades normales
    //float dens1 = get_density(_get_node_density(node1->gases), node1->temperatura, pres1);
    //float dens2 = get_density(_get_node_density(node2->gases), node2->temperatura, pres2);

    //debug_printf("Presiones: %f, %f; Densidades: %f, %f\n",pres1, pres2, dens1, dens2);
    t_gas delta_gas;
    float prop = (pres1 - pres2) * size * size / (distancia*1000000.0f * step_time); // Tomaya! inventada total!

    delta_gas.CO2 = node1->gases.CO2 * prop;
    delta_gas.N2 = node1->gases.N2 * prop;
    delta_gas.Ar = node1->gases.Ar * prop;
    delta_gas.O2 = node1->gases.O2 * prop;
    delta_gas.CO = node1->gases.CO * prop;
    delta_gas.H2O = node1->gases.H2O * prop;

    debug_printf("\nPress 1: %f Pa\n", pres1);
    debug_printf("Press 2: %f Pa\n", pres2);
    debug_printf("DELTA GASES (prop = %f):\n", prop * 2);
    debug_printf("  CO2: %f (%f, %f)\n", delta_gas.CO2, node1->gases.CO2, node2->gases.CO2);
    debug_printf("  N2:  %f (%f, %f)\n", delta_gas.N2, node1->gases.N2, node2->gases.N2);
    debug_printf("  Ar:  %f (%f, %f)\n", delta_gas.Ar, node1->gases.Ar, node2->gases.Ar);
    debug_printf("  O2:  %f (%f, %f)\n", delta_gas.O2, node1->gases.O2, node2->gases.O2);
    debug_printf("  CO:  %f (%f, %f)\n", delta_gas.CO, node1->gases.CO, node2->gases.CO);
    debug_printf("  H2O: %f (%f, %f)\n", delta_gas.H2O, node1->gases.H2O, node2->gases.H2O);
    debug_printf("\n----------\n");

    node1->gases.CO2 -= delta_gas.CO2;
    node1->gases.N2 -= delta_gas.N2;
    node1->gases.Ar -= delta_gas.Ar;
    node1->gases.O2 -= delta_gas.O2;
    node1->gases.CO -= delta_gas.CO;
    node1->gases.H2O -= delta_gas.H2O;

    node2->gases.CO2 += delta_gas.CO2;
    node2->gases.N2 += delta_gas.N2;
    node2->gases.Ar += delta_gas.Ar;
    node2->gases.O2 += delta_gas.O2;
    node2->gases.CO += delta_gas.CO;
    node2->gases.H2O += delta_gas.H2O;
}
#endif

float moles_gas_total(t_gas gas)
{
    return gas.CO2 + gas.N2 + gas.Ar + gas.O2 + gas.CO + gas.H2O;
}

void node_main_control(float step_time)
{
    int p_obj, p_node, o_obj, o_node; // Objeto y nodo propio y del otro
    float distancia; // Distancia entre los centros de presión (Centro del objeto)
    for(p_obj=0; p_obj<lista_objetos_base; p_obj++)
    {
        if(p_obj==0)
        {
            lista_objeto_base[p_obj]->node_data.gases.O2+=0.1;    // Generador de oxígeno
        }

        for(p_node=0; p_node<lista_objeto_base[p_obj]->conx_qty; p_node++) // Para cada nodo propio
        {
            o_obj=lista_objeto_base[p_obj]->conx_id[p_node]; // Obtenemos el ID del otro objeto
            if(o_obj>=0) // Si el objeto existe (-1 si está desconectado)
            {
                o_node = lista_objeto_base[p_obj]->conx_node_id[p_node];
                distancia = vdist(lista_objeto_base[p_obj]->pos, lista_objeto_base[o_obj]->pos); // Distancia entre centros de objeto

                debug_printf("Obj %i -> Obj %i\n", p_obj, o_obj);
                node_flow_gas(	&lista_objeto_base[p_obj]->node_data,
                                &lista_objeto_base[o_obj]->node_data,
                                lista_objeto_base[p_obj]->conx_size[p_node],
                                distancia,
                                step_time/2.0); // Divido por 2 porque realmente se llama a esta función dos veces, uno por parte de cada objeto
            }
        }
    }


    //Hasta que no tenga la ecuación real no hago nada
    /*int obj, p_node, o_obj, o_node;
    float distancia;
    for(obj=0; obj<lista_objetos_base; obj++)
    {
    	if(obj==0){lista_objeto_base[obj]->node_data.gases.O2+=0.1;}
    	for(p_node=0; p_node<lista_objeto_base[obj]->conx_qty; p_node++)
    	{
    		o_obj=lista_objeto_base[obj]->conx_id[p_node];
    		if(o_obj>=0)
    		{
    			o_node = lista_objeto_base[obj]->conx_node_id[p_node];
    			distancia = vdist(	vadd(lista_objeto_base[obj]->pos,lista_objeto_base[obj]->conx_coord[p_node]),
    								vadd(lista_objeto_base[o_obj]->pos,lista_objeto_base[o_obj]->conx_coord[o_node]));

    			node_flow_gas(	&lista_objeto_base[obj]->node_data,
    							&lista_objeto_base[o_obj]->node_data,
    							lista_objeto_base[obj]->conx_size[p_node],
    							distancia,
    							step_time/2.0); // Divido por 2 porque realmente se llama a esta función dos veces, uno por parte de cada objeto
    		}
    	}
    }*/
}
