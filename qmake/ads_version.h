#ifndef ADS_VERSION_H
#define ADS_VERSION_H

#define ADS_VERSION_MAJOR 5
#define ADS_VERSION_MINOR 1
#define ADS_VERSION_PATCH 1
#define ADS_VERSION_CHECK(major, minor, patch) ((major << 16) | (minor << 8) | (patch))
#define ADS_VERSION ADS_VERSION_CHECK(ADS_VERSION_MAJOR, ADS_VERSION_MINOR, ADS_VERSION_PATCH)

#endif
