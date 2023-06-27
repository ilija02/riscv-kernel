#ifndef LIST_HPP
#define LIST_HPP

#include "MemoryAllocator.hpp"
#include "../lib/mem.h"

using size_t = decltype(sizeof(0));

template<typename T>

class Dequeue {
public:
  struct Node {
    Node *next = nullptr;
    T *data;

    Node(T *data, Node *next = nullptr) {
      this->data = data;
      this->next = next;
    }

    void *operator new(size_t size);

    void *operator new[](size_t size);

    void operator delete(void *chunk);

    void operator delete[](void *chunk);
  };

  void *operator new(size_t size);
  void operator delete(void *chunk);
  Node *head = nullptr;
  Node *tail = nullptr;

  Dequeue() = default;
  Dequeue(const Dequeue &) = delete; // delete copy constructor
  Dequeue &operator=(const Dequeue &) = delete; // delete copy assignment operator
  void push_front(T *element);

  void push_back(T *element);

  T *pop_front();

  T *pop_back();

  T *front() const;

  T *back() const;
};

// override operator new for Node
template<typename T>
void *Dequeue<T>::Node::operator new(size_t size) {
  return MemoryAllocator::get().mem_alloc(size);
}

//override operator new for Node
template<typename T>
void *Dequeue<T>::Node::operator new[](size_t size) {
  return MemoryAllocator::get().mem_alloc(size);
}

//override operator delete for Node
template<typename T>
void Dequeue<T>::Node::operator delete(void *chunk) {
  MemoryAllocator::get().mem_free(chunk);
}

//override operator delete for Node
template<typename T>
void Dequeue<T>::Node::operator delete[](void *chunk) {
  MemoryAllocator::get().mem_free(chunk);
}

template<typename T>
void Dequeue<T>::push_front(T *element) {
  Node *node = new Node(element, this->head);
  this->head = node;
  if (this->tail == nullptr)
    this->tail = this->head;
}

template<typename T>
void Dequeue<T>::push_back(T *element) {
  Node *node = new Node(element);
  if (this->tail != nullptr)
  {
    this->tail->next = node;
    this->tail = node;
  }
  else
    this->tail = this->head = node;
}

template<typename T>
T *Dequeue<T>::pop_front() {
  if (this->head == nullptr)
    return nullptr;
  Node *node = this->head;
  this->head = this->head->next;
  if (this->head == nullptr)
    this->tail = nullptr;
  T *data = node->data;
  delete node;
  return data;
}

template<typename T>
T *Dequeue<T>::pop_back() {
  if (this->head == nullptr)
    return nullptr;
  Node *prev = nullptr;
  for (Node *curr = this->head; curr && curr != this->tail; curr = curr->next)
    prev = curr;
  Node *node = this->tail;
  if (prev != nullptr)
    prev->next = nullptr;
  else
    this->head = nullptr;
  this->tail = prev;
  T *data = node->data;
  delete node;
  return data;
}

template<typename T>
T *Dequeue<T>::front() const {
  if (this->head == nullptr)
    return nullptr;
  else
    return this->head->data;
}

template<typename T>
T *Dequeue<T>::back() const {
  if (this->tail == nullptr)
    return nullptr;
  else
    return this->tail->data;
}

template<typename T>
void *Dequeue<T>::operator new(size_t size) {
  return MemoryAllocator::get().mem_alloc(size);
}

template<typename T>
void Dequeue<T>::operator delete(void *chunk) {
  MemoryAllocator::get().mem_free(chunk);
}

#endif //LIST_HPP
