#include "vecvec_hdf5.hxx"

void writeHDF5(const VecVec& v, hid_t group, const char* name) {
    hid_t dataset, datatype, dataspace;
    hsize_t size = v.length_;
    dataspace = H5Screate_simple(1, &size, NULL); 
    datatype = H5Tcopy(H5T_STD_U32LE);
    dataset = H5Dcreate(group, name, datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    H5Dwrite(dataset, H5T_STD_U32LE, H5S_ALL, H5S_ALL, H5P_DEFAULT, v.data_);
    H5Tclose(datatype);
    H5Dclose(dataset);
    H5Sclose(dataspace); 
}

void readHDF5(VecVec& v, hid_t group, const char* dataset) {
    hid_t d = H5Dopen(group, dataset, H5P_DEFAULT);
    hid_t filespace = H5Dget_space(d);
    hsize_t shape;
    H5Sget_simple_extent_dims(filespace, &shape, NULL);
    int dimension = H5Sget_simple_extent_ndims(filespace);
    delete[] v.data_;
    v.data_ = new uint32_t[shape];
    v.length_ = shape;
    H5Dread(d, H5T_STD_U32LE, H5S_ALL, H5S_ALL, H5P_DEFAULT, v.data_);
    H5Dclose(d);
    H5Sclose(filespace);
}