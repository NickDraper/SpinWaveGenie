//
//  UniquePositions.h
//  positions
//
//  Created by Hahn, Steven E. on 1/2/14.
//
//

#ifndef __positions__UniqueThreeVectors__
#define __positions__UniqueThreeVectors__

#include <iostream>
#include <boost/foreach.hpp>
#include <boost/range/iterator_range.hpp>
#include "ThreeVectors.h"

template<class T>
class UniqueThreeVectors: public ThreeVectors<T>
{
public:
    bool operator==(UniqueThreeVectors& other);
    void insert(T x, T y, T z);
};

template<class T>
struct isEqual
{
    double epsilon;
    boost::tuple<T,T,T> FirstVector;
    isEqual(boost::tuple<T,T,T> First)
    {
        epsilon = 1.0e-8;
        FirstVector = First;
        
    };
    bool operator()(boost::tuple<T,T,T> SecondVector)
    {
        T x = FirstVector.template get<0>() - SecondVector.template get<0>();
        T y = FirstVector.template get<1>() - SecondVector.template get<1>();
        T z = FirstVector.template get<2>() - SecondVector.template get<2>();
        if (std::abs(x) < epsilon && std::abs(y) < epsilon && std::abs(z) < epsilon)
            return true;
        else
            return false;
    };
};

template<class T>
void UniqueThreeVectors<T>::insert(T x, T y, T z)
{
    boost::tuple<T,T,T> MyThreeVector(x,y,z);
    auto equalVector = isEqual<T>(MyThreeVector);
    if (std::find_if(this->begin(), this->end(),equalVector)==this->end())
        ThreeVectors<T>::insert(x,y,z);
}

template<class T>
bool UniqueThreeVectors<T>::operator==(UniqueThreeVectors<T>& other)
{
    boost::tuple<T,T,T> MyThreeVector,OtherThreeVector;
    BOOST_FOREACH(MyThreeVector,boost::make_iterator_range(this->begin(),this->end()))
    {
        auto equalVector = isEqual<T>(MyThreeVector);
        if (std::find_if(other.begin(), other.end(),equalVector)==other.end() )
            return false;
    }
    return true;
}


#endif /* defined(__positions__UniquePositions__) */
