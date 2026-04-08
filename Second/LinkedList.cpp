#include <iostream>
#include <string>
using namespace std;
#include "LinkedList.h"


Node<T> :: Node ()
{
    key = 0;
    next = nullptr;
}
Node<T> :: Node (T key)
{
    this->key = key;
    next = nullptr;
}


LinkedList<T> :: LinkedList ()
{
    head = nullptr;
    tail = nullptr;
    length = 0;
}
LinkedList<T> :: LinkedList (T* items, int count)
{
    if (count >=0)
    {
        for (int i=0; i<count; i++)
            Append (items[i]);
    }
    else
    // кидаем исключение через throw
}
LinkedList<T> :: LinkedList (LinkedList <T>& list const)
{
    head = nullptr;
    tail = nullptr;
    length = 0;
    // указатель, который двигается по списку list
    Node <T>* t = list.head;
    while (t!=nullptr)
    {
        Append (t->key);
        t = t->next;
    }
}
T LinkedList<T> :: GetFirst()
{
    if (length !=0)
        return head->key;
    else
        // кидаем исключение
}
T LinkedList<T> :: GetLast()
{
    if (length !=0)
        return tail->key;
    else
        // кидаем исключение
}
T LinkedList<T> :: Get (int index)
{
    if (index < length)
    {
        // указатель, который двигается по списку list
        Node <T>* t = list.head;
        int i = 0;
        while (i != index)
        {
            i++;
            t = t->next;
        }
        return t->key;
    }
    else
        // исключение, выход за границы списка
}
LinkedList <T>* LinkedList<T> :: GetSubList (int start, int end)
{
    if ((start>=0) && (end <length))
    {
        LinkedList <T>* result = new LinkedList <T> ();
        for (int i=start; i<=end; i++)
            result->Append(Get(i));
    }
    else
    // исключение
}

int LinkedList<T> :: GetLength()
{
    return length;
}
// добавление в конец списка
void LinkedList<T> :: Append (T item)
{
    if ((head == nullptr) && (tail == nullptr))
    {
        head = new Node <T> (item);
        tail = head;
    }
    else
    {
        Node <T>* p = new Node <T> (item);
        tail->next = p;
        tail = p;
    }   
    length++;
}
void LinkedList<T> :: Prepend (T item)
{
    if ((head == nullptr) && (tail == nullptr))
    {
        head = new Node <T> (item);
        tail = head;
    }
    else
    {
        Node <T>* p = new Node <T> (item);
        p->next = head;
        head = p;
    }
    length++;
}
void LinkedList<T> :: InsertAt (T item, int index)
{
    if (index >=length)
        // исключение, выход за границы
    else
    {
        if (index == 0)
            Prepend (item);
        else
        {
            Node <T>* p = new Node <T> (item);
            int i = 0;
            Node <T>* t = head;
            while (i < index - 1)
            {
                t = t -> next;
                i++;
            }
            p -> next = t->next;
            t-next = p;
        }
    }
}
LinkedList <T>* LinkedList<T> :: Concat (LinkedList <T>* list)
{
    LinkedList<T> result = new LinkedList<T>(*this)
    // указатель, который двигается по списку list
    Node <T>* t = list.head;
    while (t!=nullptr)
    {
        result.Append (t->key);
        t = t->next;
    }
    return result;
}
void LinkedList<T> :: ClearList()
{
    Node <T>* t = head;
    while (t!=nullptr)
    {
        Node<T>* next = t->next;
        delete (t);
        t = next;
    }
    head = nullptr;
    tail = nullptr;
    length = 0;
}

LinkedList<T> :: ~LinkedList()
{
    ClearList();
}


