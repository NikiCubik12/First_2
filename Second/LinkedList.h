#ifndef LINKED_LIST 
#define LINKED_LIST

#include <iostream>
#include <string>
using namespace std;

template <class T> class Node
{
    public:
    T key;
    Node* next;

    Node ();
    Node (T key);
}

template <class T> class LinkedList
{
    private:
    Node <T>* head;
    Node <T>* tail;
    int length;

    public:
    LinkedList ();
    LinkedList (T* items, int count);
    LinkedList (LinkedList <T>& list const);
    T GetFirst();
    T GetLast();
    T Get (int index);
    LinkedList <T>* GetSubList (int start, int end);
    int GetLength();
    void Append (T item);
    void Prepend (T item);
    void InsertAt (T item, int index);
    LinkedList <T>* Concat (LinkedList <T>* list);
    void ClearList();
    ~LinkedList();
}

#endif