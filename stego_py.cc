#include "stego_storage.h"
#include <boost/python.hpp>


void (stego_disk::StegoStorage::*Configure1)() const = &stego_disk::StegoStorage::Configure;
void (stego_disk::StegoStorage::*Configure2)(const std::string&) const = &stego_disk::StegoStorage::Configure;
void (stego_disk::StegoStorage::*Configure3)(const stego_disk::EncoderFactory::EncoderType encoder,
                                             const stego_disk::PermutationFactory::PermutationType global_perm,
                                             const stego_disk::PermutationFactory::PermutationType local_perm) const = &stego_disk::StegoStorage::Configure;

BOOST_PYTHON_MODULE(stego_py)
{


  boost::python::class_<stego_disk::StegoStorage, boost::noncopyable>("StegoStorage")
      .def("open", &stego_disk::StegoStorage::Open)
      .def("load", &stego_disk::StegoStorage::Load)
      .def("save", &stego_disk::StegoStorage::Save)
      .def("read", &stego_disk::StegoStorage::Read)
      .def("write", &stego_disk::StegoStorage::Write)
      .def("configure", Configure1)
      .def("configure", Configure2)
      .def("configure", Configure3)
      .def("get_size", &stego_disk::StegoStorage::GetSize)
      ;

  boost::python::enum_<stego_disk::EncoderFactory::EncoderType>("encoder")
      .value("lsb", stego_disk::EncoderFactory::EncoderType::LSB)
      .value("hamming", stego_disk::EncoderFactory::EncoderType::HAMMING)
      ;

  boost::python::enum_<stego_disk::PermutationFactory::PermutationType>("permutation")
      .value("identity", stego_disk::PermutationFactory::PermutationType::IDENTITY)
      .value("affine", stego_disk::PermutationFactory::PermutationType::AFFINE)
      .value("affine64", stego_disk::PermutationFactory::PermutationType::AFFINE64)
      .value("feistel_num", stego_disk::PermutationFactory::PermutationType::FEISTEL_NUM)
      .value("feistel_mix", stego_disk::PermutationFactory::PermutationType::FEISTEL_MIX)
      ;
}

