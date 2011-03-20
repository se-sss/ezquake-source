#ifndef GL_FLARES_H_INCLUDED
#define GL_FLARES_H_INCLUDED

#define MAX_FLARES 128


extern	cvar_t	gl_flares; // -=MD=-
void init_flare_vars();

void	ClearFlares(void);
dlight_t	*CL_AllocFlare (int key);
void	R_RenderFlare (dlight_t *dlight_t);
void	R_RenderFlares (void);
void	R_Flare (vec3_t start, vec3_t end, int type);

#endif
