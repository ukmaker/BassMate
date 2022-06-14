#ifndef SIMPLEGUI_LINKED_LIST_H
#define SIMPLEGUI_LINKED_LIST_H

template <class T>
class LinkedListNode {
 public:
  LinkedListNode(T* nodeData) : data(nodeData), next(nullptr) {}
  ~LinkedListNode() {}

  T* data;
  LinkedListNode<T>* next;
};

template <class T>
class LinkedListIterator {
 public:
  LinkedListIterator(LinkedListNode<T>* first) { _current = first; }

  ~LinkedListIterator() {}

  T* next() {
    LinkedListNode<T>* p = _current;
    if (p != nullptr) {
      _current = _current->next;
      return p->data;
    }

    return nullptr;
  }

 private:
  LinkedListNode<T>* _current;
};

template <class T>
class LinkedList {
 public:
  LinkedList() : _first(nullptr), _next(nullptr), _current(nullptr), _size(0) {}
  ~LinkedList() {}

  LinkedListIterator<T> getIterator() {
    LinkedListIterator<T> iterator(_first);
    return iterator;
  }

  void append(T* data) {
    if (_first == nullptr) {
      _first = new LinkedListNode<T>(data);

    } else {
      LinkedListNode<T>* p = _first;
      LinkedListNode<T>* d = new LinkedListNode<T>(data);

      while (p->next != nullptr) {
        p = p->next;
      }

      p->next = d;
    }
    _size++;
  }

  void appendOnce(T* data) {
    if (_first == nullptr) {
      _first = new LinkedListNode<T>(data);
      _size++;

    } else {
      LinkedListNode<T>* p = _first;
      LinkedListNode<T>* d = new LinkedListNode<T>(data);

      while (p->next != nullptr) {
        if (p->data == data) {
          return;
        }

        p = p->next;
      }

      p->next = d;
      _size++;
    }
  }
  void remove(T* data) {
    if (_first == nullptr) {
      return;
    }

    LinkedListNode<T>* next = _first;
    LinkedListNode<T>* prev = _first;

    if (_first->data == data) {
      _first = _first->next;
      delete next;
      _size--;
    } else {
      next = _first->next;
      while (next) {
        if (next->data == data) {
          prev->next = next->next;
          delete next;
          _size--;
          return;
        }
        prev = next;
        next = next->next;
      }
    }
  }

  void clear() {
    LinkedListNode<T>* here = _first;
    LinkedListNode<T>* next;
    while (here != nullptr) {
      next = here->next;
      delete here;
      here = next;
    }
    _first = nullptr;
    _size = 0;
  }

  void reset() { _current = _first; }

  T* next() {
    LinkedListNode<T>* p = _current;
    if (p != nullptr) {
      _current = _current->next;
      return p->data;
    }

    return nullptr;
  }

  bool contains(T* thing) {
    LinkedListNode<T>* here = _first;
    while (here != nullptr) {
      if (here->data == thing) {
        return true;
      }
      here = here->next;
    }
    return false;
  }

  int size() { return _size; }

  T* getByIndex(int idx)
  {
    if ((idx + 1) > _size) return nullptr;
    LinkedListNode<T>* here = _first;
    while (here != nullptr) {
      if (idx == 0) {
        return here->data;
      }
      here = here->next;
      idx--;
    }
    return nullptr;
  }

  int indexOf(T* thing) {
    int idx = 0;
    LinkedListNode<T>* here = _first;
    while (here != nullptr) {
      if (here->data == thing) {
        return idx;
      }
      here = here->next;
      idx++;
    }
    return -1;
  }

 protected:
  LinkedListNode<T>* _first;
  LinkedListNode<T>* _next;
  LinkedListNode<T>* _current;
  int _size;
};

#endif