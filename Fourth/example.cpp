#include <iostream>
#include <string>
using namespace std;

int* prefix_func(const string& s)
{
    int* mas = new int[s.length()]{};
    string help = "";
    for (int i = 0; i < s.length() - 1; i++)
    {
        help += s[i];
        for (int j = 0; j < i - 1; j++)
        {
            if (help.substr(0, j+1) == help.substr(help.length() - 1 - j))
                mas[i] = j+1;
        }
    }
    return mas;
}

int* prefix_func1(const string& s)
{
    int* mas = new int[s.length()]{};
    mas[0] = 0;
    for (int i = 1; i < s.length() - 1; i++) // i - индекс элементов в суффиксе
    {
        int k = mas[i - 1]; // k - длина подстроки и индекс элементов в префиксе
        while (k > 0 && s[i] != s[k])
            k = mas[k - 1]; // зануляем k
        if (s[i] == s[k])
            k++;
        mas[i] = k;
    }
    return mas;
}

int* KMP(const string& P, const string& T)
{
    int p_len = P.length();
    int t_len = T.length();
    int* mas = new int[p_len + t_len + 1] {};
    int* p = prefix_func1(P + "#" + T);
    int count = 0;
    for (int i = 0; i < t_len - 1; i++)
    {
        if (p[p_len + i+1] == p_len)
            mas[count++] = i - p_len;
    }
    return mas;
}

void print(int* mas, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (i < count - 1)
            cout << mas[i] << ", ";
        else   
            cout << mas[i] << endl;
    }
}

int main()
{
    string s = "abcabcd";
    string p = "aba";
    string t = "babac";
    int* array = prefix_func(s);
    int* arr = prefix_func1(s);
    int* a = KMP(p, t);
    print(array, s.length());
    print(arr, s.length());
    print(a, p.length() + t.length() + 1);
    delete[] array;

    return 0;
}

// abcabcd
// a - 0
// ab - 0
// abc - 0
// abca - 1
// abcab - 2
// abcabc - 3
// abcabcd - 0