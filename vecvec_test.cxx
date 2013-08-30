#include "vecvec.hxx"
#include "vecvec_hdf5.hxx"

#include <vigra/unittest.hxx>
#include <vigra/random.hxx>

struct VecVecTest {
void test1() {
    int relabel[7] = {4,3,2,3,4,1,2};
    VecVec v = VecVec(relabel, relabel+7);
  
    std::vector<uint32_t> v0 = v.inverse(0);
    shouldEqual(v0.size(), 0);
    
    uint32_t s1[1] = { 5 };
    std::vector<uint32_t> v1 = v.inverse(1);
    shouldEqual(v1.size(), 1);
    shouldEqualSequence(v1.begin(), v1.end(), s1);
    
    uint32_t s2[2] = { 2, 6 };
    std::vector<uint32_t> v2 = v.inverse(2);
    shouldEqual(v2.size(), 2);
    shouldEqualSequence(v2.begin(), v2.end(), s2);
    
    uint32_t s3[2] = { 1, 3 };
    std::vector<uint32_t> v3 = v.inverse(3);
    shouldEqual(v3.size(), 2);
    shouldEqualSequence(v3.begin(), v3.end(), s3);
    
    uint32_t s4[2] = { 0, 4 };
    std::vector<uint32_t> v4 = v.inverse(4);
    shouldEqual(v4.size(), 2);
    shouldEqualSequence(v4.begin(), v4.end(), s4);
    
    shouldEqual(v.size(), 5);
}
void test2() {
    size_t sz1 = 1000;
    size_t sz2 = 200;
    std::vector<uint32_t> relabel(sz1);
    typename vigra::RandomNumberGenerator<vigra::RandomTT800> rng(0);
    for(size_t i=0; i<sz1; ++i) {
       relabel[i] = rng.uniformInt(sz2); 
    }
    std::vector<std::vector<uint32_t> > vv(sz1);
    for(size_t i=0; i<sz1; ++i) {
        vv[ relabel[i] ].push_back(i);
    }
    
    VecVec v(relabel.begin(), relabel.end());
    shouldEqual(v.size(), sz2);
    
    for(size_t j=0; j<sz2; ++j) {
        std::vector<uint32_t> x = v.inverse(j);
        shouldEqual(x.size(), vv[j].size());
        shouldEqualSequence(x.begin(), x.end(), vv[j].begin());
    }
}

void test3() {
    int relabel[7] = {4,3,2,3,4,1,2};
    int idx[7]     = {0,1,2,3,4,5,6};
    
    VecVec v = VecVec(idx, idx+7, relabel);
  
    std::vector<uint32_t> v0 = v.inverse(0);
    shouldEqual(v0.size(), 0);
    
    uint32_t s1[1] = { 5 };
    std::vector<uint32_t> v1 = v.inverse(1);
    shouldEqual(v1.size(), 1);
    shouldEqualSequence(v1.begin(), v1.end(), s1);
    
    uint32_t s2[2] = { 2, 6 };
    std::vector<uint32_t> v2 = v.inverse(2);
    shouldEqual(v2.size(), 2);
    shouldEqualSequence(v2.begin(), v2.end(), s2);
    
    uint32_t s3[2] = { 1, 3 };
    std::vector<uint32_t> v3 = v.inverse(3);
    shouldEqual(v3.size(), 2);
    shouldEqualSequence(v3.begin(), v3.end(), s3);
    
    uint32_t s4[2] = { 0, 4 };
    std::vector<uint32_t> v4 = v.inverse(4);
    shouldEqual(v4.size(), 2);
    shouldEqualSequence(v4.begin(), v4.end(), s4);
    
    shouldEqual(v.size(), 5);
}
void test_hdf5() {
    int relabel[7] = {4,3,2,3,4,1,2};
    int idx[7]     = {0,1,2,3,4,5,6};
    VecVec v = VecVec(idx, idx+7, relabel);
    
    hid_t file = H5Fcreate("test_vecvec.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    writeHDF5(v, file, "vecvec");
    H5Fclose(file);
   
    VecVec v2;
    file = H5Fopen("test_vecvec.h5", H5F_ACC_RDONLY, H5P_DEFAULT);
    readHDF5(v2, file, "vecvec");
    
    shouldEqual(v, v2);
}

}; /* struct VecVecTest */

struct VecVecTestSuite : public vigra::test_suite {
    VecVecTestSuite()
        : vigra::test_suite("VecVecTestSuite")
    {
        add( testCase(&VecVecTest::test1));
        add( testCase(&VecVecTest::test2));
        add( testCase(&VecVecTest::test3));
        add( testCase(&VecVecTest::test_hdf5));
    }
};

int main(int argc, char ** argv) {
    VecVecTestSuite test;
    int failed = test.run(vigra::testsToBeExecuted(argc, argv));
    std::cout << test.report() << std::endl;
    return (failed != 0);
}
