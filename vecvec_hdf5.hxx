#ifndef VECVEC_HDF5_HXX
#define VECVEC_HDF5_HXX

#include "vecvec.hxx"

//fix buggy hdf5 1.8 versions
#include <H5version.h>
#if (H5_VERS_MAJOR == 1 && H5_VERS_MINOR >= 8 && defined(H5_USE_16_API_DEFAULT))
#define H5Gcreate_vers 2
#define H5Gopen_vers 2
#define H5Dopen_vers 2
#define H5Dcreate_vers 2
#define H5Acreate_vers 2
#endif

#include "hdf5.h"

void writeHDF5(const VecVec& v, hid_t group, const char* dataset);
void readHDF5(VecVec& v, hid_t group, const char* dataset);

#endif /*VECVEC_HDF5_HXX*/