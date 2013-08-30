#ifndef VECVEC_HDF5_HXX
#define VECVEC_HDF5_HXX

#include "vecvec.hxx"

#include "hdf5.h"

void writeHDF5(const VecVec& v, hid_t group, const char* dataset);
void readHDF5(VecVec& v, hid_t group, const char* dataset);

#endif /*VECVEC_HDF5_HXX*/