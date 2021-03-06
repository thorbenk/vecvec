#ifndef VECVEC_HXX
#define VECVEC_HXX

#include <stdint.h>

#include <algorithm>
#include <numeric>
#include <vector>
#include <iostream>
#include <iterator>

//these defines are copied from vigra/hdf5impex.hxx
#define H5Gcreate_vers 2
#define H5Gopen_vers 2
#define H5Dopen_vers 2
#define H5Dcreate_vers 2
#define H5Acreate_vers 2

#include "hdf5.h"

#if (H5_VERS_MAJOR == 1 && H5_VERS_MINOR <= 6)
# ifndef H5Gopen
#   define H5Gopen(a, b, c) H5Gopen(a, b)
# endif
# ifndef H5Gcreate
#  define H5Gcreate(a, b, c, d, e) H5Gcreate(a, b, 1)
# endif
# ifndef H5Dopen
#  define H5Dopen(a, b, c) H5Dopen(a, b)
# endif
# ifndef H5Dcreate
#  define H5Dcreate(a, b, c, d, e, f, g) H5Dcreate(a, b, c, d, f)
# endif
# ifndef H5Acreate
#  define H5Acreate(a, b, c, d, e, f) H5Acreate(a, b, c, d, e)
# endif
# ifndef H5Pset_obj_track_times
#  define H5Pset_obj_track_times(a, b) do {} while (0)
# endif
# include <H5LT.h>
#else
# include <hdf5_hl.h>
#endif

#include "hdf5.h"

/*
 * Memory layout:
 * 
 * M: length of outer vector
 * 
 * |M| 2*M | sum length inner vectors|
 * 
 * 1 + 2*M + sum of histogram
 * 
 * 2*M:
 *   for each index of the inner vector:
 *     1.) jump position to start
 *     2.) length
 */
class VecVec {
    public:
        
    friend void writeHDF5(const VecVec&, hid_t group, const char* dataset);
    friend void readHDF5(VecVec&, hid_t group, const char* dataset);

    VecVec();
        
    template<class Iter>
    VecVec(Iter begin, Iter end) {
        uint32_t M = *std::max_element(begin, end)+1;
        std::vector<uint32_t> hist(M), accHist(M+1);
        for(Iter it=begin; it!=end; ++it) {
            ++hist[*it];
        }
        for(size_t h=1; h<accHist.size(); ++h) {
            accHist[h] = accHist[h-1] + hist[h-1];
        }
        uint32_t histSum = accHist[M]; 
        
        length_ = 1 + 2*M + histSum;
        data_ = new uint32_t[length_];
       
        size_t idx = 0;
        data_[idx] = M; ++idx;
        for(size_t h=0; h<hist.size(); ++h) {
            data_[idx] = 1+2*M+accHist[h]; ++idx;
            data_[idx] = hist[h]; ++idx;
        }
        std::vector<size_t> o(M+1); //current offsets
        for(Iter it=begin; it!=end; ++it) {
            data_[offset(*it)+o[*it]] = std::distance(begin, it);
            ++o[*it];
        }
    }
    
    template<class Iter>
    VecVec(Iter fromBegin, Iter fromEnd, Iter toBegin) {
        Iter toEnd = toBegin + std::distance(fromBegin, fromEnd);
        
        uint32_t M = *std::max_element(toBegin, toEnd)+1;
        std::vector<uint32_t> hist(M), accHist(M+1);
        for(Iter it=toBegin; it!=toEnd; ++it) {
            ++hist[*it];
        }
        for(size_t h=1; h<accHist.size(); ++h) {
            accHist[h] = accHist[h-1] + hist[h-1];
        }
        uint32_t histSum = accHist[M]; 
        
        length_ = 1 + 2*M + histSum;
        data_ = new uint32_t[length_];
       
        size_t idx = 0;
        data_[idx] = M; ++idx;
        for(size_t h=0; h<hist.size(); ++h) {
            data_[idx] = 1+2*M+accHist[h]; ++idx;
            data_[idx] = hist[h]; ++idx;
        }
        std::vector<size_t> o(M+1); //current offsets
        Iter fromIt = fromBegin;
        for(Iter toIt=toBegin; toIt!=toEnd; ++toIt, ++fromIt) {
            data_[offset(*toIt)+o[*toIt]] = *fromIt;
            ++o[*toIt];
        }
    }
    
    std::vector<uint32_t> inverse(uint32_t idx) const;
    
    template<class Iter>
    void copyInverse(uint32_t idx, Iter start) const {
        const uint32_t* o = data_+offset(idx);
        std::copy(o, o+size(idx), start);
    }
    
    VecVec& operator=(const VecVec& other);
    
    bool operator==(const VecVec& other) const;
    
    VecVec(const VecVec &other);
    
    ~VecVec();
    
    size_t size() const;
    size_t size(size_t idx) const;
    
    private:
    size_t offset(size_t idx) const;
    
    uint32_t* data_;
    uint32_t length_;
};

std::ostream& operator<<(std::ostream& o, const VecVec& v);

#endif /*VECVEC_HXX*/
