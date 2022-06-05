#pragma once
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace RIL{

namespace py = pybind11;
template<typename T>
class AbstractImage {
    public:
        using dtype =T;
        AbstractImage(size_t o_nrow, size_t o_ncol, size_t o_nchan)
            : _nrow(o_nrow), _ncol(o_ncol), _nchan(o_nchan)
        {
            // _data_type = dtype;
            reset_buffer(_nrow, _ncol, _nchan);
        }
        AbstractImage(const AbstractImage<T> & other)
        {
            reset_buffer(other._nrow, other._ncol, other._nchan);
            for (size_t i = 0; i < _nrow; ++i)
            {
                for (size_t j = 0; j < _ncol; ++j)
                {
                    for (size_t k = 0; j < _nchan; ++k)
                    {
                        (*this)(i, j, k) = other(i, j, k);
                    }
                }
            }
        }

        T const &operator()(size_t row, size_t col, size_t chan) const
        {
            return _ptr[_nrow * _ncol * _nchan + col * _nchan + _nchan];
        };
        T &operator()(size_t row, size_t col, size_t chan)
        {
            return _ptr[row * _ncol * _nchan + col * _nchan + _nchan];
        }

        AbstractImage & operator=(const AbstractImage &other)
        {
            std::cerr << "FAILURE: operator not yet part of implementation!!" << std::endl;
            if (this == &other)
            {
                return *this;
            }
            if (_nrow != other._nrow || _ncol != other._ncol || _nchan != other._nchan)
            {
                reset_buffer(other.nrow, other.ncol, other.nchan);
            }
            for (size_t i = 0; i < _nrow; ++i)
            {
                for (size_t j = 0; j < _ncol; ++j)
                {
                    for (size_t k = 0; k < _nchan; ++k)
                    {
                        (*this)(i, j, k) = other(i, j, k);
                    }
                }
            }
            return *this;
        }
        AbstractImage(AbstractImage<T> && other)

        {
            //TODO implement this better
            reset_buffer(0, 0, 0);
            std::swap(_nrow, other._nrow);
            std::swap(_ncol, other._ncol);
            std::swap(_nchan, other._nchan);
            std::swap(_ptr, other._ptr);
        }

        AbstractImage operator=(AbstractImage<T> && other)
        {
            if (this == &other) { return *this; }
            //TODO implement this 
            reset_buffer(0, 0, 0);
            std::swap(_nrow, other._nrow);
            std::swap(_nrow, other._ncol);
            std::swap(_nchan,other._nchan);
            std::swap(_ptr, other._ptr);
            return *this;
        }
        py::array_t<T> get_ndarray(){
            return py::array_t<T>(
                    {_nrow,_ncol,_nchan},// Shape
                    {_ncol*_nchan*sizeof(T),_nchan*sizeof(T)},// Strides
                    _ptr,// Data Pointer
                    // py::cast(_ptr)//TODO: Tell numpy to take care of this, or somehow share the responsibility
                    py::none() //Lets assume that we wont use the np version when this c++ instance doesnt exist. If not use above
                    );
        }

        size_t index(size_t row, size_t col, size_t chan) const
        {
            //Ew no, row-major better
            //return row + col * m_nrow;
            return row*_ncol*_nchan + col*this->_nchan + chan;
        }
        ~AbstractImage()
        {
            reset_buffer(0, 0, 0);
        }

        size_t size() const { return _ncol*_nrow*_nchan; }

        void zero_out(){
            memset(_ptr,0,_nrow*_ncol*_nchan*sizeof(double));
        }
        T * data() { return _ptr; };
        T * data() const { return _ptr; }

        size_t get_nrow() const {return _nrow;}
        size_t get_ncol() const {return _ncol;}
        size_t get_nchan() const {return _nchan;}


    private:

    protected:
        void reset_buffer(size_t o_nrow, size_t o_ncol, size_t o_nchannels)
        {

            if(_ptr != nullptr){
                delete [] _ptr;
            }
            const size_t nelement = o_nrow * o_ncol * o_nchannels;
            if (nelement)
            {
                _ptr = new T[nelement];
            }
            else
            {
                _ptr = nullptr;
            }
            _nrow = o_nrow;
            _ncol = o_ncol;
            _nchan = o_nchannels;
        }

        /********************
         * Actual members
         * we take care of
        ********************/


        size_t _nrow = 0;
        size_t _ncol = 0;
        size_t _nchan = 0;
        T * _ptr = nullptr;
};
}
