#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <iostream> 
#define START_SIZE 1000

namespace aisdi
{

template <typename Type>
class Vector
{
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
private: 
	Type* tab;
	size_type index_empty;
	size_type size;
public: 

  Vector(): index_empty(0),size(START_SIZE)
  {
	tab = new Type[size];
   }

  Vector(std::initializer_list<Type> l): index_empty(0), size((int)(l.size()+l.size()*0.6)) 
  {
	tab = new Type[size];
    auto it = l.begin();
    while(it != l.end())
    {
		append(*it);
		++it;
	}
  }

  Vector(const Vector& other): index_empty(0),size((int)(other.getSize()+other.getSize()*0.6))
  {
	tab = new Type[size];
    auto it = other.begin();
    while(it != other.end())
    {
		append(*it);
		++it;
	}
  }

  Vector(Vector&& other)
  {
    tab = other.tab;
    size = other.size;
    index_empty = other.index_empty;
    
    other.size = 0;
    other.index_empty = 0;
    other.tab = new Type[other.size];
  }

  ~Vector()
	{
	  delete[] tab;
	}

  Vector& operator=(const Vector& other)
  {
    index_empty = 0;
    delete[] tab;
    size = other.getSize() + ((int)(other.getSize()*0.6));
    tab = new Type[size];
    auto it = other.begin();
    while(it != other.end())
    {
		append(*it);
        ++it;
    }
    return *this;
  }

  Vector& operator=(Vector&& other)
  {
    index_empty = 0;
    delete[] tab;
    size = other.getSize() + ((int)(other.getSize()*0.6));;
    tab = new Type[size];
    auto it = other.begin();
    while(it != other.end())
    {
		append(*it);
        ++it;
    }
    other.erase(other.begin(),other.end());
    return *this;
  }

  bool isEmpty() const
  {
    return(index_empty == 0);
  }

  size_type getSize() const
  {
    return index_empty;
  }

  void append(const Type& item)
  {
    if (size == index_empty)
		enlargeTab();
	tab[index_empty] = item;
	index_empty++;
  }
  
  void enlargeTab()
  { 
	Type* temp;
	size+= size ;
	temp = new Type[size];
	unsigned int i;
	for(i=0;i<index_empty;i++)
		 temp[i] = tab[i];
	delete[] tab;
	tab = temp;
  }

  void prepend(const Type& item)
  {
	if(size <=(index_empty+1)) 
		enlargeTab();
	unsigned int i = index_empty;
	while(i>0)
	{
		tab[i] = tab[i-1];
		i--;
	}
	tab[0] = item;
	index_empty++;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    if(insertPosition.index>index_empty)
		throw std::logic_error("Logic: Can't insert element not in vector");
    if (insertPosition == cend()) 
   {
		append(item);	
		return;
	}	
    else if (insertPosition == cbegin())
    {
		prepend(item);
		return;
	}
	else
	{
            int i = index_empty;
            while( i > (int)insertPosition.index)
            {
                tab[i] = tab[i-1];
                i--;
            }
            tab[insertPosition.index] = item;
            index_empty++;
     }
  }

  Type popFirst()
  {
    if(isEmpty())
		throw std::out_of_range("Empty vector");
	Type temp = tab[0];
	for(int i = 0; i < (int)index_empty; i++)
		tab[i] = tab[i+1];
	index_empty--;
	return temp;
  }

  Type popLast()
  {
    if(isEmpty())
		throw std::out_of_range("Empty vector");
	Type temp;
	temp = tab[--index_empty];
	return temp;
  }

  void erase(const const_iterator& possition)
  {
    if(isEmpty() || possition.index>=index_empty)
		throw std::out_of_range("Range: Iterator position is not proper");
 
     int temp = possition.index; 
     for(int i = temp; i<(int)index_empty; i++)
            tab[i]=tab[i+1];
     --index_empty;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    if(firstIncluded==lastExcluded)
		return;
 
            size_type begin_delete_index = firstIncluded.index;
            size_type end_delete_ex_index = lastExcluded.index;
 
            if(begin_delete_index>=end_delete_ex_index)
                throw std::logic_error("Logic: Begin of delete is further than End");
            else
            {
                while (end_delete_ex_index < index_empty) {
                    tab[begin_delete_index] = tab[end_delete_ex_index];
                    begin_delete_index++;
                    end_delete_ex_index++;
                }
 
                while (end_delete_ex_index > begin_delete_index) {
                    popLast();
                    end_delete_ex_index--;
                }
            }
  }

  iterator begin()
  {
	  return iterator(cbegin());
  }

  iterator end()
  {
    return iterator(cend());
  }

  const_iterator cbegin() const
  {
    const_iterator it(this,&(tab[0]),0);
    return it;
  }

  const_iterator cend() const
  {
   const_iterator it(this,&(tab[index_empty]),index_empty);
    return it;
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename Type>
class Vector<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;

private: 
	const Vector<Type> *Vct;
    Type *ptr;

public:
	size_type index;
	
	explicit ConstIterator(): ptr(nullptr)
	{}
  ConstIterator(const Vector<Type> *Vct_in,Type* ptr_in,int index_in):Vct(Vct_in),ptr(ptr_in),index(index_in)
  {}

  reference operator*() const
  {
    if(Vct->getSize()<=index)
		throw std::out_of_range("Can`t reference to empty object");
    return *ptr;
  }

  ConstIterator& operator++()
  {
    if(Vct->getSize() <= index)
		throw std::out_of_range("Can`t go any further ");
	ptr++;
	index++;
	return *this;
  }

  ConstIterator operator++(int)
  {
	if(Vct->getSize() <= index)
		throw std::out_of_range("Can`t go any further");
    ConstIterator temp= *this;
    operator++();
    return temp;
  }

  ConstIterator& operator--()
  {
	if( index == 0) 
		throw std::out_of_range("Can`t go earlier ");
    ptr--;
    index--;
    return *this;
  }

  ConstIterator operator--(int)
  {
	if( index == 0)
		throw std::out_of_range("Can`t go earlier  ");
    ConstIterator temp= *this;
    operator--();
    return temp;
  }

  ConstIterator operator+(difference_type d) const
  {
    if( (static_cast<int>(d)+index) > Vct->getSize() )
		throw std::out_of_range("Operator +(diff) failure ");
    else
        return ConstIterator(Vct,&(Vct->tab[index+static_cast<int>(d)]) , index+static_cast<int>(d));
  }

  ConstIterator operator-(difference_type d) const
  {
	 int i = index-static_cast<int>(d);
    if(i < 0)
		throw std::out_of_range("Operator -(diff) failure ");
    else
		return ConstIterator(Vct,&(Vct->tab[index-static_cast<int>(d)]) , index-static_cast<int>(d));
  }

  bool operator==(const ConstIterator& other) const
  {
	if(ptr == other.ptr) return true;
    else return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
    if(ptr!=other.ptr) return true;
    else return false;
  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

  explicit Iterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};
}

#endif // AISDI_LINEAR_VECTOR_H
