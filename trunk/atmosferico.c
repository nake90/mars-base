/*
	mars_base - Design, build and maintain your own base on Mars
    Copyright (C) 2009  Alfonso Arbona Gimeno (nake90@terra.es). All rights reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    If you use any part of this code you must give me (Alfonso Arbona Gimeno) credit.
    If you plan to use any part of this code on a comercial game please email me at:
	   	   nake90@terra.es
*/

#include "atmosferico.h"

/* PRESIÓN */

float get_presion (int x, int y)
{
	x=x+(tam_mapa_x/2); /* Descentramos la posición */
	y=y+(tam_mapa_y/2);
	if (x>tam_mapa_x || x<0)return 0;
	if (y>tam_mapa_y || y<0)return 0;
	if (x+y*tam_mapa_x >= tam_mapa_x*2 * tam_mapa_y*2)return 0;
	return presion[x+y*tam_mapa_x];
}

void set_presion (float val, int x, int y)
{
	x=x+(tam_mapa_x/2); /* Descentramos la posición */
	y=y+(tam_mapa_y/2);
	if (x>tam_mapa_x || x<0)return;
	if (y>tam_mapa_y || y<0)return;
	if (x+y*tam_mapa_x >= tam_mapa_x*2 * tam_mapa_y*2)return;
	presion[x+y*tam_mapa_x]=val;
}

float get_presion_cicl (int x, int y)
{
	x=x+(tam_mapa_x/2); /* Descentramos la posición */
	y=y+(tam_mapa_y/2);
	if (x>tam_mapa_x || x<0)return 0;
	if (y>tam_mapa_y || y<0)return 0;
	if (x+y*tam_mapa_x >= tam_mapa_x*2 * tam_mapa_y*2)return 0;
	return presion_cicl[x+y*tam_mapa_x];
}

void set_presion_cicl (float val, int x, int y)
{
	x=x+(tam_mapa_x/2); /* Descentramos la posición */
	y=y+(tam_mapa_y/2);
	if (x>tam_mapa_x || x<0)return;
	if (y>tam_mapa_y || y<0)return;
	if (x+y*tam_mapa_x >= tam_mapa_x*2 * tam_mapa_y*2)return;
	presion_cicl[x+y*tam_mapa_x]=val;
}

void set_presion_from_cicl (float val, int x, int y)
{
	x=x+(tam_mapa_x/2); /* Descentramos la posición */
	y=y+(tam_mapa_y/2);
	if (x>tam_mapa_x || x<0)return;
	if (y>tam_mapa_y || y<0)return;
	if (x+y*tam_mapa_x >= tam_mapa_x*2 * tam_mapa_y*2)return;
	
	float mi_presion=0;
	int i;
	for (i=0; i<CICLONES; i++)
	{
			mi_presion-=get_ciclon_adder (i, x, y);
	}
	
	presion[x+y*tam_mapa_x]=val+mi_presion;
}

void randomize_presion (float media, float rango)
{
	int x,y;
	float valor;
	srand (time (NULL)*time (NULL));
	for (y=-tam_mapa_y/2; y<tam_mapa_y/2; y++)
	for (x=-tam_mapa_x/2; x<tam_mapa_x/2; x++)
	{
		valor=media-(((rand () % (int)(rango*1000))/1000)-(rango/2));
		set_presion(valor,x,y);
	}
}

void calc_presion_cicl (void)
{
	int x,y;
	int i;
	double distancia;
	float mi_presion;
	float adder;
	for (y=-tam_mapa_y/2; y<tam_mapa_y/2; y++)
	for (x=-tam_mapa_x/2; x<tam_mapa_x/2; x++)
	{
		mi_presion=0;
		for (i=0; i<CICLONES; i++)
		{
			mi_presion+=get_ciclon_adder (i, x, y);
		}
		set_presion_cicl(mi_presion+get_presion(x,y),x,y);
	}
}

void add_noise_presion (float rango)
{
	int x,y;
	float mi_presion;
	srand (time (NULL));
	for (y=-tam_mapa_y/2; y<tam_mapa_y/2; y++)
	for (x=-tam_mapa_x/2; x<tam_mapa_x/2; x++)
	{
		mi_presion=get_presion(x,y)-(((rand () % (int)(rango*1000.0))/1000.0)-(rango/2));
		if (mi_presion<0){mi_presion=0;}
		if (mi_presion<PRESION_MIN){mi_presion+(PRESION_MIN-mi_presion)*(PRESION_MIN-mi_presion)/50.0;}
		if (mi_presion>PRESION_MAX){mi_presion-(PRESION_MIN-mi_presion)*(PRESION_MIN-mi_presion)/50.0;}
		set_presion(mi_presion,x,y);
	}
}

void flow_presion (float factor)
{
	calc_presion_cicl();
	srand (time (NULL));
	int x,y;
	float mi_presion;
	float su_presion;
	float flow;
	float random;
	
	for (y=-tam_mapa_y/2+1; y<tam_mapa_y/2-1; y++)
	for (x=-tam_mapa_x/2+1; x<tam_mapa_x/2-1; x++)
	{
		mi_presion=get_presion_cicl(x,y);
		
		random = (((rand () % (int)(factor*1000.0))/1000.0)-(factor/2))/2;
		su_presion=get_presion_cicl(x-1,y-1);
		flow = (su_presion-mi_presion)/((su_presion+mi_presion)/2);
		mi_presion += flow*factor + random;
		su_presion -= flow*factor - random;
		set_presion_from_cicl (su_presion, x-1,y-1);
		
		random = (((rand () % (int)(factor*1000.0))/1000.0)-(factor/2))/2;
		su_presion=get_presion_cicl(x,y-1);
		flow = (su_presion-mi_presion)/((su_presion+mi_presion)/2);
		mi_presion += flow*factor + random;
		su_presion -= flow*factor - random;
		set_presion_from_cicl (su_presion, x,y-1);
		
		random = (((rand () % (int)(factor*1000.0))/1000.0)-(factor/2))/2;
		su_presion=get_presion_cicl(x+1,y-1);
		flow = (su_presion-mi_presion)/((su_presion+mi_presion)/2);
		mi_presion += flow*factor + random;
		su_presion -= flow*factor - random;
		set_presion_from_cicl (su_presion, x+1,y-1);
		
		random = (((rand () % (int)(factor*1000.0))/1000.0)-(factor/2))/2;
		su_presion=get_presion_cicl(x-1,y);
		flow = (su_presion-mi_presion)/((su_presion+mi_presion)/2);
		mi_presion += flow*factor + random;
		su_presion -= flow*factor - random;
		set_presion_from_cicl (su_presion, x-1,y);
		
		random = (((rand () % (int)(factor*1000.0))/1000.0)-(factor/2))/2;
		su_presion=get_presion_cicl(x+1,y);
		flow = (su_presion-mi_presion)/((su_presion+mi_presion)/2);
		mi_presion += flow*factor + random;
		su_presion -= flow*factor - random;
		set_presion_from_cicl (su_presion, x+1,y);
		
		random = (((rand () % (int)(factor*1000.0))/1000.0)-(factor/2))/2;
		su_presion=get_presion_cicl(x-1,y+1);
		flow = (su_presion-mi_presion)/((su_presion+mi_presion)/2);
		mi_presion += flow*factor + random;
		su_presion -= flow*factor - random;
		set_presion_from_cicl (su_presion, x-1,y+1);
		
		random = (((rand () % (int)(factor*1000.0))/1000.0)-(factor/2))/2;
		su_presion=get_presion_cicl(x,y+1);
		flow = (su_presion-mi_presion)/((su_presion+mi_presion)/2);
		mi_presion += flow*factor + random;
		su_presion -= flow*factor - random;
		set_presion_from_cicl (su_presion, x,y+1);
		
		random = (((rand () % (int)(factor*1000.0))/1000.0)-(factor/2))/2;
		su_presion=get_presion_cicl(x+1,y+1);
		flow = (su_presion-mi_presion)/((su_presion+mi_presion)/2);
		mi_presion += flow*factor + random;
		su_presion -= flow*factor - random;
		set_presion_from_cicl (su_presion, x+1,y+1);
		
		set_presion_from_cicl (mi_presion, x,y);
	}
}

void init_ciclon (void)
{
	int x, y;
	int i;
	float valor;
	srand (time (NULL)*time (NULL));
	for (i=0; i<CICLONES; i++)
	{
		x=(rand () % tam_mapa_x)-(tam_mapa_x/2);
		y=(rand () % tam_mapa_y)-(tam_mapa_y/2);
		ciclon[i].pos_x=x;
		ciclon[i].pos_y=y;
		ciclon[i].valor=((rand() % (int)(CICLONES_VAL_MAX*100))/100)*((rand()%2)-1);
	}
}

void update_ciclon (void)
{
	int x, y;
	int i;
	float valor;
	srand (time (NULL)*time (NULL));
	for (i=0; i<CICLONES; i++)
	{
		x=(rand() % CICLONES_DELTA_ACEL*2)-(CICLONES_DELTA_ACEL);
		y=(rand() % CICLONES_DELTA_ACEL*2)-(CICLONES_DELTA_ACEL);
		ciclon[i].acel_x+=x;
		ciclon[i].acel_y+=y;
		ciclon[i].vel_x+=ciclon[i].acel_x;
		ciclon[i].vel_y+=ciclon[i].acel_y;
		ciclon[i].pos_x+=ciclon[i].vel_x/100;
		ciclon[i].pos_y+=ciclon[i].vel_y/100;
		if (ciclon[i].pos_x<=-(tam_mapa_x/2)){ciclon[i].pos_x=-tam_mapa_x/2+2;ciclon[i].acel_x=0;ciclon[i].vel_x*=-1;}
		if (ciclon[i].pos_y<=-(tam_mapa_y/2)){ciclon[i].pos_y=-tam_mapa_y/2+2;ciclon[i].acel_y=0;ciclon[i].vel_y*=-1;}
		if (ciclon[i].pos_x>=+(tam_mapa_x/2)){ciclon[i].pos_x=+tam_mapa_x/2-2;ciclon[i].acel_x=0;ciclon[i].vel_x*=-1;}
		if (ciclon[i].pos_y>=+(tam_mapa_y/2)){ciclon[i].pos_y=+tam_mapa_y/2-2;ciclon[i].acel_y=0;ciclon[i].vel_y*=-1;}
		if (ciclon[i].vel_x>CICLONES_MAX_VEL || ciclon[i].vel_x<-CICLONES_MAX_VEL){ciclon[i].acel_x=0;ciclon[i].vel_x*=0.8;}
		if (ciclon[i].vel_y>CICLONES_MAX_VEL || ciclon[i].vel_y<-CICLONES_MAX_VEL){ciclon[i].acel_y=0;ciclon[i].vel_y*=0.8;}
	}
}


double get_ciclon_adder (int i, int x, int y)
{
	double distancia;
	double adder;
	if(ciclon[i].pos_x!=x || ciclon[i].pos_y!=y)
	{
		distancia=((ciclon[i].pos_x-x)*(ciclon[i].pos_x-x)+(ciclon[i].pos_y-y)*(ciclon[i].pos_y-y));
		if(distancia<CICLONES_MIN_DIST)
		{
			//adder=(ciclon[i].valor)/(distancia);
			adder=((ciclon[i].valor)-abs(distancia*0.5));
			if(abs(adder)<abs(ciclon[i].valor)){return adder;}
			else{return ciclon[i].valor;}
		}
	}else{return ciclon[i].valor;}
}

void update_sun_now (t_sun sol)
{
	sol.position[0]++;
	
}
