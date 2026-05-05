#ifndef LIST_SEQUENCE_HPP
#define LIST_SEQUENCE_HPP

#include "Sequence.hpp"
#include "LinkedList.hpp"


template <class T> class ListSequence : public Sequence <T>
{
    protected:
    LinkedList <T>* items;
    public:
    ListSequence ()
    {
        items = new LinkedList<T> ();
    }

    ListSequence (T* arr, int count)
    {
        items = new LinkedList<T> (arr, count);
    }

    ListSequence(const LinkedList<T>& list)
    {
        items = new LinkedList<T> (list);
    }

    T GetFirst() override
    {
        if (items->GetLength() == 0)
            throw out_of_range("ListSequence is empty - cannot get first element");
        return items->Get(0);
    }

    
    T GetLast() override
    {
        if (items->GetLength() == 0)
            throw out_of_range("ListSequence is empty - cannot get last element");
        return items->Get(items->GetLength() - 1);
    }

    T Get(int index) override
    {
        if (index < 0 || index >= items->GetLength())
            throw out_of_range("Выход за границы диапазона в ListSequence::Get");
        return items->Get(index);
    }

    ListSequence<T>* GetSubsequence(int start, int end) override
    {
        if (start < 0 || end >= items->GetLength() || start > end)
            throw out_of_range("Неправильные индексы для начала и конца в ListSequence::GetSubsequence");
        
        LinkedList<T>* sublist = new LinkedList<T>();
        for (int i = start; i <= end; i++)
            sublist->Append(items->Get(i));
        
        ListSequence<T>* result = new ListSequence<T>(*sublist);
        delete sublist;
        return result;
    }

    int GetLength() override
    {
        return items->GetLength();
    }

    ListSequence<T>* AppendImpl(const T& item) override
    {
        items->Append(item);
        return this;
    }

    ListSequence<T>* InsertAtImpl(const T& item, int index) override
    {
        if (index < 0 || index > items->GetLength())
            throw out_of_range("Выход за границы диапазона в ListSequence::InsertAtImpl");
        
        items->InsertAt(item, index);
        return this;
    }

    ListSequence<T>* PrependImpl(const T& item) override
    {
        items->Prepend(item);
        return this;
    }

    Sequence<T>* Concat(Sequence<T>* list) override
    {
        if (list == nullptr)
            throw invalid_argument("Не может объединиться с пустым указателем в ListSequence::Concat");
        
        for (int i = 0; i < list->GetLength(); i++)
            items->Append(list->Get(i));
        
        return this;
    }

    Sequence<T>* instance() override
    {
        return new ListSequence<T>(*this);
    }
};

#endif