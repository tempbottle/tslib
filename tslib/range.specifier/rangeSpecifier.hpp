///////////////////////////////////////////////////////////////////////////
// Copyright (C) 2008  Whit Armstrong                                    //
//                                                                       //
// This program is free software: you can redistribute it and/or modify  //
// it under the terms of the GNU General Public License as published by  //
// the Free Software Foundation, either version 3 of the License, or     //
// (at your option) any later version.                                   //
//                                                                       //
// This program is distributed in the hope that it will be useful,       //
// but WITHOUT ANY WARRANTY; without even the implied warranty of        //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         //
// GNU General Public License for more details.                          //
//                                                                       //
// You should have received a copy of the GNU General Public License     //
// along with this program.  If not, see <http://www.gnu.org/licenses/>. //
///////////////////////////////////////////////////////////////////////////

#ifndef RANGESPECIFIER_HPP
#define RANGESPECIFIER_HPP

#include <vector>

namespace tslib {

  // template friend must be pre-declared
  template<typename T, typename U> class RangeSpecifier;
  template<typename T, typename U> std::ostream& operator<< (std::ostream& os, const RangeSpecifier<T,U>& rs);

  template<typename T, typename U>
  class RangeSpecifier {
  private:
    std::vector<T> dates_;  // intersection dates_
    std::vector<U> index1_;   // index of intersections of index1_
    std::vector<U> index2_;   // index of intersections of index2_
    U size_;    // also the length of all the internal vectors: dates_, index1_, index2_

  public:
    RangeSpecifier(const RangeSpecifier &r); // not allowed!!
    RangeSpecifier(T *dates_1,
		   T *dates_2,
		   const U length_index1_,
		   const U length_index2_);

    const T* getDates() const;
    const U* getArg1() const;
    const U* getArg2() const;
    const U getSize() const;
    friend std::ostream& operator<< <> (std::ostream& os, const RangeSpecifier<T,U>& rs);
  };

  template<typename T, typename U>
  RangeSpecifier<T,U>::RangeSpecifier(T *dates_1,
				      T *dates_2,
				      const U length_index1_,
				      const U length_index2_) {

    // find size of smaller of the two arguments
    const U bufferSize = length_index1_ < length_index2_ ? length_index1_ : length_index2_;
    dates_.resize(bufferSize);

    // find size of date intersection
    T* dates_end =  std::set_intersection(dates_1, dates_1+length_index1_, dates_2, dates_2+length_index2_, &dates_[0]);

    size_ = std::distance(&dates_[0],dates_end);

    // if there is no intersection then specifier has no size, and elements should be set to null
    // must delete buffer of dates_ that we allocated
    if(size_==0) {
      dates_.clear();
    } else {

      // since we have some dates_ in the intersection
      // we can alloc space for the intersection points
      index1_.resize(size_);
      index2_.resize(size_);

      // placeholders to find intersecting dates_
      U date1_index = 0;
      U date2_index = 0;
      U dates_index = 0;

      // go through all the dates_ in the intersection
      while(dates_index < size_) {

        // catch up arg 1 to dates_ intersection
        while(dates_[dates_index] != dates_1[date1_index]) {
          date1_index++;
        }

        // catch up arg 2 to dates_ intersection
        while(dates_[dates_index] != dates_2[date2_index]) {
          date2_index++;
        }

        // when equal, record position of matching elements
        if(dates_[dates_index] == dates_1[date1_index] && dates_[dates_index] == dates_2[date2_index]) {
          index1_[dates_index] = date1_index;
          index2_[dates_index] = date2_index;
          dates_index++;
          date1_index++;
          date2_index++;
        }
      }
    }
  }

  template<typename T, typename U>
  const U RangeSpecifier<T,U>::getSize() const {
    return size_;
  }

  template<typename T, typename U>
  const T* RangeSpecifier<T,U>::getDates() const {
    return &dates_[0];
  }

  template<typename T, typename U>
  const U* RangeSpecifier<T,U>::getArg1() const {
    return &index1_[0];
  }

  template<typename T, typename U>
  const U* RangeSpecifier<T,U>::getArg2() const {
    return &index2_[0];
  }

} // namespace tslib

#endif
