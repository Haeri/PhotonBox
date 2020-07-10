#ifndef PB_PHOTOB_BOX_CONFIG_H
#define PB_PHOTOB_BOX_CONFIG_H

#if defined(PB_BUILD_SHARED)
	#if defined(_WINDOWS)

		#if defined(PhotonBox_EXPORTS)
			#define PB_PORT __declspec(dllexport)
		#else
			#define PB_PORT __declspec(dllimport)
		#endif

	#else

		#if defined(PhotonBox_EXPORTS)
			#define PB_PORT __attribute__((visibility("default")))
		#else
			#define PB_PORT 
		#endif

	#endif
#else
	#define PB_PORT 
#endif

#endif // PB_PHOTOB_BOX_CONFIG_H