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

    Node()
    {
        key = 0;
        next = nullptr;
    }

    Node(T key)
    {
        this->key = key;
        next = nullptr;
    }
};

template <class T> class LinkedList
{
    private:
    Node <T>* head;
    Node <T>* tail;
    int length;

    public:
    LinkedList()
    {
        head = nullptr;
        tail = nullptr;
        length = 0;
    }

    LinkedList(T* items, int count)
    {
        head = nullptr;
        tail = nullptr;
        length = 0;
        if (count >= 0)
        {
            for (int i = 0; i < count; i++)
                Append(items[i]);
        }
        else
            throw std::invalid_argument("Count cannot be negative");
    }

    LinkedList(const LinkedList<T>& list)
    {
        head = nullptr;
        tail = nullptr;
        length = 0;
        Node<T>* t = list.head;
        while (t != nullptr)
        {
            Append(t->key);
            t = t->next;
        }
    }

    T GetFirst()
    {
        if (length != 0)
            return head->key;
        else
            throw std::out_of_range("List is empty");
    }

    T GetLast()
    {
        if (length != 0)
            return tail->key;
        else
            throw std::out_of_range("List is empty");
    }

    T Get(int index)
    {
        if (index < length && index >= 0)
        {
            Node<T>* t = head;
            int i = 0;
            while (i != index)
            {
                i++;
                t = t->next;
            }
            return t->key;
        }
        else
            throw std::out_of_range("Index out of range");
    }

    LinkedList<T>* GetSubList(int start, int end)
    {
        if ((start >= 0) && (end < length) && (start <= end))
        {
            LinkedList<T>* result = new LinkedList<T>();
            for (int i = start; i <= end; i++)
                result->Append(Get(i));
            return result;
        }
        else
            throw std::out_of_range("Invalid start or end indices");
    }

    int GetLength()
    {
        return length;
    }

    void Append(T item)
    {
        if ((head == nullptr) && (tail == nullptr))
        {
            head = new Node<T>(item);
            tail = head;
        }
        else
        {
            Node<T>* p = new Node<T>(item);
            tail->next = p;
            tail = p;
        }   
        length++;
    }

    void Prepend(T item)
    {
        if ((head == nullptr) && (tail == nullptr))
        {
            head = new Node<T>(item);
            tail = head;
        }
        else
        {
            Node<T>* p = new Node<T>(item);
            p->next = head;
            head = p;
        }
        length++;
    }

    void InsertAt(T item, int index)
    {
        if (index < 0 || index > length)
            throw std::out_of_range("Index out of range");
        else
        {
            if (index == 0)
                Prepend(item);
            else if (index == length)
                Append(item);
            else
            {
                Node<T>* p = new Node<T>(item);
                int i = 0;
                Node<T>* t = head;
                while (i < index - 1)
                {
                    t = t->next;
                    i++;
                }
                p->next = t->next;
                t->next = p;
                length++;
            }
        }
    }

    LinkedList<T>* Concat(LinkedList<T>* list)
    {
        LinkedList<T>* result = new LinkedList<T>(*this);
        Node<T>* t = list->head;
        while (t != nullptr)
        {
            result->Append(t->key);
            t = t->next;
        }
        return result;
    }

    void ClearList()
    {
        Node<T>* t = head;
        while (t != nullptr)
        {
            Node<T>* next = t->next;
            delete t;
            t = next;
        }
        head = nullptr;
        tail = nullptr;
        length = 0;
    }

    ~LinkedList()
    {
        ClearList();
    }
};

#endif