#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>


namespace aisdi
{

template <typename Type>
class LinkedList
{
	struct Node
	{
		Node* prev;
		Node* next;
		Type* data;
		Node()
		{
			data = nullptr;
			prev = nullptr;
			next = nullptr;
		}
		Node(const Type val)
		{
			data = new Type(val);
			prev = nullptr;
			next = nullptr;
		}
		~Node()
		{
			prev = nullptr;
			next = nullptr;
			delete data;
		}
	};
		
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
	Node *head;
	Node *tail;
	size_type length;
	
	
public:	
  LinkedList(): head(nullptr), tail(nullptr), length(0)
  {
	  head = new Node;
	  tail = new Node;
	  head->next = tail;
	  tail->prev = head;
  }

  LinkedList(std::initializer_list<Type> l):LinkedList()
  {
	for (auto it = l.begin(); it != l.end(); ++it)
      append(*it);
  }

  LinkedList(const LinkedList& other):LinkedList()
  {
    for (auto it = other.begin(); it != other.end(); it++)
      append(*it);
  } 

  LinkedList(LinkedList&& other): LinkedList()
  {
    while(!other.isEmpty())
    {
		append(*(other.begin()));
		other.erase(other.begin());
	}
  }

  ~LinkedList()
  {
	 while(!isEmpty()) 
		erase(begin());
	delete head;
	delete tail;
  }

  LinkedList& operator=(const LinkedList& other)
  {
    while (!isEmpty()) 
      erase(begin());
	for (auto it = other.begin(); it != other.end(); it++)
      append(*it);
 
    return *this;
  }

  LinkedList& operator=(LinkedList&& other)
  {
    while (!isEmpty()) 
      erase(begin());
    while (!other.isEmpty()) 
    {
      append(*(other.begin()));
      other.erase(other.begin());
    }
    return *this;
  }

  bool isEmpty() const
  {
    if(length != 0) return false;
    return true;
  }

  size_type getSize() const
  {
    return length;
  }

  void append(const Type& item)
  {
    Node * temp = new Node(item);
    temp->prev = tail->prev;
    temp->next = tail;
    tail->prev->next = temp;
    tail->prev = temp;    
    length++;
  }

  void prepend(const Type& item)
  {
    Node* temp = new Node(item);
    temp->prev = head;
    temp->next = head->next;
    head->next->prev = temp;
	head->next = temp;
	length++;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    if (insertPosition == begin() )
      prepend(item);
    else if (insertPosition == end())
      append(item);
    else 
    {
      Node* temp = new Node(item);
 
      temp->next = insertPosition.pointee;
      temp->prev = insertPosition.pointee->prev;
 
      insertPosition.pointee->prev->next = temp;
      insertPosition.pointee->prev = temp;
      ++length;
     }
  }

  Type popFirst()
  {
    if(isEmpty()) throw std::out_of_range("Can't erase element from empty list");
    auto temp = *begin();
    erase(begin());
    return temp;
  }

  Type popLast()
  {
    if(this->isEmpty()) throw std::out_of_range("Can't erase element from empty list");
    auto temp=*(end()-1);
    erase(end()-1);
    return temp;
  }

  void erase(const const_iterator& possition)
  {
   if (possition == end())
	throw std::out_of_range("Can`t erase the end");
   possition.pointee->next->prev = possition.pointee->prev;
   possition.pointee->prev->next = possition.pointee->next;
   delete possition.pointee;
   --length;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    auto it = firstIncluded;
    while (it != lastExcluded) 
    {
      erase(it++);
    }
  }

  iterator begin()
  {
    return iterator(head->next);
  }

  iterator end()
  {
    return iterator(tail);
  }

  const_iterator cbegin() const
  {
    return const_iterator(head->next);
  }

  const_iterator cend() const
  {
    return const_iterator(tail);
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
class LinkedList<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;
private:
	Node *pointee;
	friend void LinkedList <Type>::insert(const const_iterator&, const Type&);
	friend void LinkedList <Type>::erase(const const_iterator&);
public: 
  explicit ConstIterator(Node *ptr = nullptr):pointee(ptr)
  {}

  reference operator*() const
  {
    if(pointee->data == nullptr)
        throw std::out_of_range("Argument without value");
    return *(pointee->data);
  }

  ConstIterator& operator++()
  {
    if (pointee->next == nullptr)
        throw std::out_of_range("Can`t append iterator at the end");
    pointee = pointee->next;
    return *this;
  }

  ConstIterator operator++(int)
  {
    auto result = *this;
    operator++();
    return result;
  }

  ConstIterator& operator--()
  {
    if (pointee->prev->prev == nullptr)
        throw std::out_of_range("Can`t decrease iterator at the beginning");
    pointee = pointee->prev;
    return *this;
  }

  ConstIterator operator--(int)
  {
    auto result = *this;
    operator--();
    return result;
  }

  ConstIterator operator+(difference_type d) const
  {
    auto result = *this;
    for (difference_type i = 0; i < d; ++i)
		++result;
    return result;
  }

  ConstIterator operator-(difference_type d) const
  {
    auto result = *this;
    for (difference_type i = 0; i < d; ++i)
		--result;
    return result;
  }

  bool operator==(const ConstIterator& other) const
  {
    return (this->pointee == other.pointee);
  }

  bool operator!=(const ConstIterator& other) const
  {
    return (this->pointee != other.pointee);
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  explicit Iterator(Node *pnt = nullptr): const_iterator(pnt)
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

#endif // AISDI_LINEAR_LINKEDLIST_H
