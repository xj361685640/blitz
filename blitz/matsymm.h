/***************************************************************************
 * blitz/matsymm.h      Declarations for Symmetric matrices
 *
 * $Id$
 *
 * Copyright (C) 1997-1999 Todd Veldhuizen <tveldhui@oonumerics.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Suggestions:          blitz-dev@oonumerics.org
 * Bugs:                 blitz-bugs@oonumerics.org
 *
 * For more information, please see the Blitz++ Home Page:
 *    http://oonumerics.org/blitz/
 *
 ***************************************************************************
 * $Log$
 * Revision 1.1  2000/06/19 12:26:11  tveldhui
 * Initial revision
 *
 * Revision 1.3  1998/03/14 00:04:47  tveldhui
 * 0.2-alpha-05
 *
 * Revision 1.2  1997/01/24 14:42:00  tveldhui
 * Periodic RCS update
 *
 * Revision 1.1  1997/01/13 22:19:58  tveldhui
 * Periodic RCS update
 *
 *
 */

#ifndef BZ_MATSYMM_H
#define BZ_MATSYMM_H

#ifndef BZ_MSTRUCT_H
 #error <blitz/matsymm.h> must be included via <blitz/mstruct.h>
#endif

BZ_NAMESPACE(blitz)

// Symmetric, lower triangular row major ordering
// [ 0 1 3 6 ]
// [ 1 2 4 7 ]
// [ 3 4 5 8 ]
// [ 6 7 8 9 ]

class SymmetricIterator {
public:
    SymmetricIterator(unsigned rows, unsigned cols)
    {
        BZPRECONDITION(rows == cols);
        size_ = rows;
        good_ = true;
        offset_ = 0;
        i_ = 0;
        j_ = 0;
    }
   
    operator _bz_bool() const
    { return good_; }

    void operator++()
    {
        BZPRECONDITION(good_);
        ++offset_;
        ++j_;
        if (j_ > i_)
        {
            j_ = 0;
            ++i_;
            if (i_ == size_)
                good_ = false;
        }
    }

    unsigned row() const
    { return i_; }

    unsigned col() const
    { return j_; }

    unsigned offset() const
    { return offset_; }

protected:
    unsigned size_;
    _bz_bool good_;
    unsigned offset_;
    unsigned i_, j_;
};

class Symmetric : public MatrixStructure {

public:
    typedef SymmetricIterator T_iterator;

    Symmetric()
        : size_(0)
    { }

    Symmetric(unsigned size)
        : size_(size)
    { }

    Symmetric(unsigned rows, unsigned cols)
        : size_(rows)
    {
        BZPRECONDITION(rows == cols);
    }

    unsigned columns() const
    { return size_; }

    unsigned coordToOffset(unsigned i, unsigned j) const
    {
        BZPRECONDITION(inRange(i,j));
        if (i >= j)
            return i*(i+1)/2 + j;
        else
            return j*(j+1)/2 + i;
    }

    unsigned firstInRow(unsigned i) const
    { return 0; }

    template<class T_numtype>
    T_numtype get(const T_numtype * _bz_restrict data,
        unsigned i, unsigned j) const
    {
        BZPRECONDITION(inRange(i,j));
        return data[coordToOffset(i,j)];
    }

    template<class T_numtype>
    T_numtype& get(T_numtype * _bz_restrict data, unsigned i, unsigned j)
    {
        BZPRECONDITION(inRange(i,j));
        return data[coordToOffset(i,j)];
    }

    unsigned lastInRow(unsigned i) const
    { return i; }

    unsigned firstInCol(unsigned j) const
    { return j; }

    unsigned lastInCol(unsigned j) const
    { return size_ - 1; }

    _bz_bool inRange(unsigned i, unsigned j) const
    {
        return (i < size_) && (j < size_);
    }

    unsigned numElements() const
    { return size_ * (size_ + 1) / 2; }

    unsigned rows() const
    { return size_; }

    void resize(unsigned size)
    {
        size_ = size;
    }

    void resize(unsigned rows, unsigned cols)
    {
        BZPRECONDITION(rows == cols);
        size_  = rows;
    }

private:
    unsigned size_;
};

BZ_NAMESPACE_END

#endif // BZ_MATSYMM_H
