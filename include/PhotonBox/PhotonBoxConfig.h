#ifndef PB_PHOTOB_BOX_CONFIG_H
#define PB_PHOTOB_BOX_CONFIG_H

#if defined(PB_PLATFORM_WIN)

	#if defined(PB_EXPORT)
		#define PB_PORT __declspec(dllexport)
	#else
		#define PB_PORT __declspec(dllimport)
	#endif

#else

	#if defined(PB_EXPORT)
		#define PB_PORT __attribute__((visibility("default")))
	#else
		#define PB_PORT 
	#endif

#endif

#endif // PB_PHOTOB_BOX_CONFIG_H