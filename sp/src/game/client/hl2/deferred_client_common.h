#ifndef DEFERRED_COMMON_H
#define DEFERRED_COMMON_H

#ifndef DEFERRED_SHARED_COMMON
#error must include deferred_shared_common.h instead
#endif

#define DEFLIGHT_SPOT_ZNEAR 5.0f

#define DEFLIGHT_MAX_LEAVES 64

extern ConVar deferred_rt_shadowspot_res;
extern ConVar deferred_rt_shadowpoint_res;

extern ConVar deferred_lightmanager_debug;

extern ConVar deferred_override_globalLight_enable;
extern ConVar deferred_override_globalLight_shadow_enable;
extern ConVar deferred_override_globalLight_diffuse;
extern ConVar deferred_override_globalLight_ambient_high;
extern ConVar deferred_override_globalLight_ambient_low;

extern ConVar deferred_radiosity_enable;
extern ConVar deferred_radiosity_propagate_count;
extern ConVar deferred_radiosity_propagate_count_far;
extern ConVar deferred_radiosity_blur_count;
extern ConVar deferred_radiosity_blur_count_far;
extern ConVar deferred_radiosity_debug;


#define PROFILER_DECLARE CFastTimer __pft; __pft.Start()
#define PROFILER_RESTART __pft.Start();
#define PROFILER_OUTPUT( line, format ) __pft.End();\
	engine->Con_NPrintf( line, format, __pft.GetDuration().GetCycles() )

#define PATHLOCATION_PROJECTABLE_SCRIPTS "scripts/vguiprojected"

#include "../../materialsystem/stdshaders/IDeferredExt.h"

//#include "deferred/deferred_rt.h"
//#include "deferred/IDefCookie.h"
//#include "deferred/DefCookieTexture.h"
//#include "deferred/DefCookieProjectable.h"
//#include "deferred/def_light_t.h"
//#include "deferred/cascade_t.h"

//#include "deferred/vgui/vgui_deferred.h"

//#include "deferred/cdeferred_manager_client.h"
//#include "deferred/clight_manager.h"
//#include "deferred/clight_editor.h"

//#include "deferred/viewrender_deferred.h"

//#include "deferred/flashlighteffect_deferred.h"
#include "materialsystem_passthru.h"

#endif