#include <iostream>
#include <string>
using namespace std;
using std::cin, std::cout, std::out_of_range, std::invalid_argument
#include "LinkedList.hpp"

template <typename T> 
Node<T> :: Node ()
{
    key = 0;
    next = nullptr;
}

template <typename T> 
Node<T> :: Node (T key)
{
    this->key = key;
    next = nullptr;
}

template <typename T> 
LinkedList<T> :: LinkedList ()
{
    head = nullptr;
    tail = nullptr;
    length = 0;
}

template <typename T> 
LinkedList<T> :: LinkedList (T* items, int count)
{
    if (count >= 0)
    {
        for (int i = 0; i < count; i++)
            Append (items[i]);
    }
    else
    {
        throw invalid_argument("Неверный размер"); 
    }
}

template <typename T> 
LinkedList<T> :: LinkedList (LinkedList <T>& list const)
{
    head = nullptr;
    tail = nullptr;
    length = 0;
    // указатель, который двигается по списку list
    Node <T>* t = list.head;
    while (t != nullptr)
    {
        Append (t->key);
        t = t->next;
    }
}

template <typename T> 
T LinkedList<T> :: GetFirst()
{
    if (length != 0)
        return head->key;
    else
        throw invalid_argument("Последовательность пуста");
}

template <typename T> 
T LinkedList<T> :: GetLast()
{
    if (length != 0)
        return tail->key;
    else
        throw invalid_argument("Последовательность пуста");
}

template <typename T> 
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
        throw out_of_range("Index out of range in LinkedList::Get");
}

template <typename T> 
LinkedList <T>* LinkedList<T> :: GetSubList (int start, int end)
{
    if ((start >= 0) && (end < length))
    {
        LinkedList <T>* result = new LinkedList <T> ();
        for (int i=start; i<=end; i++)
            result->Append(Get(i));
    }
    else
    {
        throw out_of_range("Неправильные индексы для начала и конца в LinkedList::GetSubList");    
    }
}

template <typename T> 
int LinkedList<T> :: GetLength()
{
    return length;
}

// добавление в конец списка
template <typename T> 
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

template <typename T> 
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

template <typename T> 
void LinkedList<T> :: InsertAt (T item, int index)
{
    if (index >=length)
    {
        throw out_of_range("Index out of range in LinjedList::InsertAt");
    }
        
    else
    {
        if (index == 0)
        {
            Prepend (item);
        }
            
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

template <typename T> 
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

template <typename T> 
void LinkedList<T> :: ClearList()
{
    Node <T>* t = head;
    while (t != nullptr)
    {
        Node<T>* next = t->next;
        delete (t);
        t = next;
    }
    head = nullptr;
    tail = nullptr;
    length = 0;
}

template <typename T> 
LinkedList<T> :: ~LinkedList()
{
    ClearList();
}