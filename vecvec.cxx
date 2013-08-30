#include "vecvec.hxx"

#define DEBUG_VEC(name, vec) std::cout << name << " = ("; std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " ")); std::cout << ")" << std::endl;

VecVec::VecVec()
    : data_(0)
    , length_(0)
{}

std::vector<uint32_t> VecVec::inverse(uint32_t idx) const {
    const uint32_t* o = data_+offset(idx);
    return std::vector<uint32_t>(o, o+size(idx));
}

VecVec& VecVec::operator=(const VecVec& other) {
    if (this != &other) {
        delete[] data_;
        data_ = new uint32_t[other.length_];
        std::copy(other.data_, other.data_+other.length_, data_);
        length_ = other.length_;
    }
    return *this;
}

bool VecVec::operator==(const VecVec& other) const {
    if(other.length_ != length_) return false;
    return std::equal(data_, data_+length_, other.data_);
}

VecVec::VecVec(const VecVec &other)
    : data_(0), length_(other.length_)
{
    data_ = new uint32_t[length_];
    std::copy(other.data_, other.data_+other.length_, data_);
}

VecVec::~VecVec() {
    delete[] data_;
}

size_t VecVec::size() const {
    return data_[0];
}

size_t VecVec::offset(size_t idx) const {
    return data_[1+2*idx];
}

size_t VecVec::size(size_t idx) const {
    return data_[1+2*idx+1];
}

std::ostream& operator<<(std::ostream& o, const VecVec& v) {
    o << "VecVec(size=" << v.size() << ")";
    return o; 
}
