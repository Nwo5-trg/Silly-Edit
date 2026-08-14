#pragma once

#ifdef GEODE_IS_WINDOWS
	#ifdef NWO5_SILLYEDIT_EXPORTING
		#define	SILLYEDIT_DLL __declspec(dllexport)
	#else
		#define	SILLYEDIT_DLL __declspec(dllimport)
	#endif
#else
	#define	SILLYEDIT_DLL __attribute__((visibility("default")))
#endif