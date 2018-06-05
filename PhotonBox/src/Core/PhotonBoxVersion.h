#ifndef PHOTON_BOX_VERSION_H
#define PHOTON_BOX_VERSION_H

#include <chrono>

#define PHOTON_BOX_VERSION_MAJOR 0
#define PHOTON_BOX_VERSION_MINOR 1

#define PHOTON_BOX_VERSION_BUILD std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()

#endif // PHOTON_BOX_VERSION_H