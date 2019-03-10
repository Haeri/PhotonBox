#ifndef RESOURCES_H
#define RESOURCES_H

#define BUILD_MODE false

#include <string>

namespace Resources
{
#if BUILD_MODE
	const std::string ENGINE_RESOURCES = "./res";
#else
	const std::string ENGINE_RESOURCES = "../PhotonBox/res";
#endif
};

#endif // RESOURCES_H
