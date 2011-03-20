#include "quakedef.h"
#include "gl_model.h"
#include "gl_local.h"

#include "gl_flare.h"


void R_RenderDlight (dlight_t *light);

cvar_t	gl_flares = {"gl_flares", "0"}; // -=MD=-

cvar_t	gl_flares_rockettrail = {"gl_flares_rockettrail", "1"};

//	{ 0.9, 0.7, 0.3, 1.0 },		// rocket trail 1 (gl_flares)
//	{ 0.3, 0.2, 0.1, 0.2 }		// rocket trail 2 (gl_flares)


cvar_t	gl_flares_rockettrail1_red =
		 {"gl_flares_rockettrail1_red", "0.9"};
cvar_t	gl_flares_rockettrail1_green =
		 {"gl_flares_rockettrail1_green", "0.7"};
cvar_t	gl_flares_rockettrail1_blue =
		 {"gl_flares_rockettrail1_blue", "0.3"};
cvar_t	gl_flares_rockettrail1_alpha =
		 {"gl_flares_rockettrail1_alpha", "1"};

cvar_t	gl_flares_rockettrail2_red =
		 {"gl_flares_rockettrail2_red", "0.3"};
cvar_t	gl_flares_rockettrail2_green =
		 {"gl_flares_rockettrail2_green", "0.2"};
cvar_t	gl_flares_rockettrail2_blue =
		 {"gl_flares_rockettrail2_blue", "0.1"};
cvar_t	gl_flares_rockettrail2_alpha =
		 {"gl_flares_rockettrail2_alpha", "0.2"};


cvar_t	gl_flares_rockettrail1_radius = {"gl_flares_rockettrail1_radius", "20"};
cvar_t	gl_flares_rockettrail2_radius = {"gl_flares_rockettrail2_radius", "50"};


dlight_t	cl_flares[MAX_FLARES];

void init_flare_vars()
{
    Cvar_Register (&gl_flares);
    Cvar_Register (&gl_flares_rockettrail);

    Cvar_Register (&gl_flares_rockettrail1_red);
    Cvar_Register (&gl_flares_rockettrail1_green);
    Cvar_Register (&gl_flares_rockettrail1_blue);
    Cvar_Register (&gl_flares_rockettrail1_alpha);
	Cvar_Register (&gl_flares_rockettrail1_radius);

    Cvar_Register (&gl_flares_rockettrail2_red);
    Cvar_Register (&gl_flares_rockettrail2_green);
    Cvar_Register (&gl_flares_rockettrail2_blue);
    Cvar_Register (&gl_flares_rockettrail2_alpha);
	Cvar_Register (&gl_flares_rockettrail2_radius);
}


void ClearFlares(void)
{
	int	i;
	for (i=0 ; i<MAX_FLARES ; i++) {
		cl_flares[i].die = 0;
		cl_flares[i].key = 0;
	}
}

dlight_t	*CL_AllocFlare (int key)
{
	int		i;
	dlight_t	*fl;

	if (key)
	{
		fl = cl_flares;
		for (i=0 ; i<MAX_FLARES ; i++, fl++)
		{
			if (fl->key == key)
			{
				memset (fl, 0, sizeof(*fl));
				fl->key = key;
				return fl;
			}
		}
	}

// then look for anything else
	fl = cl_flares;
	for (i=0 ; i<MAX_FLARES ; i++, fl++)
	{
		if (fl->die < cl.time)
		{
			memset (fl, 0, sizeof(*fl));
			fl->key = key;
			return fl;
		}
	}

	fl = &cl_flares[0];
	memset (fl, 0, sizeof(*fl));
	fl->key = key;
	return fl;
}

void R_RenderFlares (void)
{
	int		i;
	dlight_t	*l;

	glDepthMask (GL_FALSE);
	glDisable (GL_TEXTURE_2D);
	glShadeModel (GL_SMOOTH);
	glEnable (GL_BLEND);
	glBlendFunc (GL_ONE, GL_ONE);

	l = cl_flares;
	for (i=0 ; i<MAX_FLARES ; i++, l++)
	{
		if (l->die < cl.time || !l->radius)
			continue;
		l->radius += l->decay;
//		color change is now made in R_RenderDlight
//		l->color[3] = l->color[3] * 0.5;
		R_RenderDlight ((dlight_t *)l);
	}

	glColor3ubv (color_white);
	glDisable (GL_BLEND);
	glEnable (GL_TEXTURE_2D);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask (GL_TRUE);
}

void R_Flare (vec3_t start, vec3_t end, int type)
{
	vec3_t		vec, v;
	float		len;
	dlight_t		*fl;

	VectorSubtract (end, start, vec);
	len = VectorNormalize (vec);

	v[0] = (start[0] + end[0]) / 2;
	v[1] = (start[1] + end[1]) / 2;
	v[2] = (start[2] + end[2]) / 2;

	VectorNormalize (v);
	
	if (len > 0)
	{
		switch (type)
		{
		case 0:
			if(gl_flares_rockettrail.value)
			{
			fl = CL_AllocFlare (0);
			VectorCopy (end, fl->origin);
			fl->radius = gl_flares_rockettrail1_radius.value;
			fl->die = cl.time + 0.5;
			fl->decay = -1;
			fl->color[0] = gl_flares_rockettrail1_red.value * 255;
			fl->color[1] = gl_flares_rockettrail1_green.value * 255;
			fl->color[2] = gl_flares_rockettrail1_blue.value * 255;
			fl->color[3] = gl_flares_rockettrail1_alpha.value * 255;
			fl->type = lt_custom;//!!! lt_rocket_trail_1;
			fl = CL_AllocFlare (0);
			VectorCopy (end, fl->origin);
			fl->radius = gl_flares_rockettrail2_radius.value;
			fl->die = cl.time + 0.5;
			fl->decay = -2;
			fl->color[0] = gl_flares_rockettrail2_red.value * 255;
			fl->color[1] = gl_flares_rockettrail2_green.value * 255;
			fl->color[2] = gl_flares_rockettrail2_blue.value * 255;
			fl->color[3] = gl_flares_rockettrail2_alpha.value * 255;
			fl->type = lt_custom; //!!!lt_rocket_trail_2;
			}
			break;

// FIXME This part is never used?
/*		case 1:
			fl = CL_AllocFlare (0);
			VectorCopy (end, fl->origin);
			fl->radius = 50;
			fl->die = cl.time + 0.5;
			fl->decay = -2;
			fl->color[0] = 0.1;
			fl->color[1] = 0.1;
			fl->color[2] = 0.1;
			fl->color[3] = 0.2;
			fl = CL_AllocFlare (0);
			VectorCopy (v, fl->origin);
			fl->radius = 50;
			fl->die = cl.time + 0.5;
			fl->decay = -2;
			fl->color[0] = 0.1;
			fl->color[1] = 0.1;
			fl->color[2] = 0.1;
			fl->color[3] = 0.2;
			break;
		case 2:
			fl = CL_AllocFlare (0);
			VectorCopy (end, fl->origin);
			fl->radius = 20;
			fl->die = cl.time + 0.5;
			fl->decay = -2;
			fl->color[0] = 0.3;
			fl->color[1] = 0.0;
			fl->color[2] = 0.0;
			fl->color[3] = 0.1;
			break; */
		}
	}
}
