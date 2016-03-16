#include "stego_storage.h"
#include <boost/python.hpp>


void (stego_disk::StegoStorage::*Configure1)() const = &stego_disk::StegoStorage::Configure;
void (stego_disk::StegoStorage::*Configure2)(const std::string&) const = &stego_disk::StegoStorage::Configure;
void (stego_disk::StegoStorage::*Configure3)(const stego_disk::EncoderFactory::EncoderType encoder,
                                             const stego_disk::PermutationFactory::PermutationType global_perm,
                                             const stego_disk::PermutationFactory::PermutationType local_perm) const = &stego_disk::StegoStorage::Configure;

void Write(stego_disk::StegoStorage& self, boost::python::object buffer) {

  PyObject* py_obj = buffer.ptr();
  if (!PyObject_CheckBuffer(py_obj)) {
    throw std::runtime_error("Wrong object type");
  }
  Py_buffer py_buffer;
  if (PyObject_GetBuffer(py_obj, &py_buffer, PyBUF_WRITE)) {
    throw std::runtime_error("Unable to get buffer");
  }
  self.Write(static_cast<void*>(py_buffer.buf), 0, static_cast<size_t>(py_buffer.len));
  PyBuffer_Release(&py_buffer);
}

boost::python::object Read(stego_disk::StegoStorage& self){

  void* buffer_c = (void*)malloc(self.GetSize()*sizeof(char));
  size_t size = 0;
  self.Read(buffer_c, 0, size);

  Py_buffer* py_buf = nullptr;
  if (PyBuffer_ToContiguous(buffer_c, py_buf, static_cast<Py_ssize_t>(size), 'C')) {
    throw std::runtime_error("Unable to copy buffer");
  }
  PyObject *mv = PyMemoryView_FromBuffer(py_buf); //this is not good, not at all
  return boost::python::object(boost::python::handle<>(mv));
  free(buffer_c);
  PyBuffer_Release(py_buf);
}

BOOST_PYTHON_MODULE(stego_py) {


  boost::python::class_<stego_disk::StegoStorage, boost::noncopyable>("StegoStorage")
      .def("open", &stego_disk::StegoStorage::Open)
      .def("load", &stego_disk::StegoStorage::Load)
      .def("save", &stego_disk::StegoStorage::Save)
      .def("read", Read)
      .def("write", Write)
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
