#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>
#include <locale>
#include <functional>
#include <array>
#include <string>

using namespace std;

bool predicate_odd(int n) {
    return (n % 2 != 0);
}

struct Sum
{
    Sum() { sum = 0; }
    void operator()(int n) { sum += n; }
    int sum;
};


int strcmp2(string a, const string& b) { return a.compare(b); }

class WrapInt
{
public:
    WrapInt() : val_(0) {}
    WrapInt(int x) : val_(x) {}

    void showval() {
        std::cout << val_ << " ";
    }

    bool is_prime() {
        for (int i = 2; i <= (val_ / 2); i++)
            if ((val_ % i) == 0)
                return false;  return true;
    }
private:  int val_;
};


int main(int argc, char** argv) {
    setlocale(LC_ALL, "ru-RU");
    //find: в векторе целых чисел ищется заданное.
    {
        int n1 = 3;
        int n2 = 5;

        vector<int> v{ 0, 1, 2, 3, 4 };

        auto result1 = find(v.begin(), v.end(), n1);
        auto result2 = find(v.begin(), v.end(), n2);

        if (result1 != v.end())
        {
            cout << "v содержит: " << n1 << '\n';
        }
        else
        {
            cout << "v не содержит: " << n1 << '\n';
        }

        if (result2 != v.end())
        {
            cout << "v содержит: " << n2 << '\n';
        }
        else
        {
            cout << "v не содержит: " << n2 << '\n';
        }
        /*
        Вывод:
        v содержит: 3
        v не содержит: 5
        */
    }



    /// adjacent_find : находит смежную пару равных элементов в массиве целых чисел.
    {
        vector<int> v1{ 0, 1, 2, 3, 40, 40, 5 };

        vector<int>::iterator result;
        result = adjacent_find(v1.begin(), v1.end());
        if (result == v1.end())
            cout << "нет совпадающих соседних элементов" << endl;
        else
            cout << "совпадение в позиции: " << distance(v1.begin(), result) << endl;
        result = adjacent_find(v1.begin(), v1.end(), greater<int>());
        if (result == v1.end())
            cout << "нет спада соседних элементов" << endl;
        else
            cout << "спад в позиции: " << distance(v1.begin(), result) << endl;
        // 
        // совпадение в позиции: 4
        // спад в позиции: 5
    }


    // copy
    {
        vector<int> from_vector;
        for (int i = 0; i < 10; i++)
            from_vector.push_back(i);

        vector<int> to_vector(5);
        copy_if(from_vector.begin(), from_vector.end(), to_vector.begin(), predicate_odd);
        cout << "to_vector содержит: ";
        copy(to_vector.begin(), to_vector.end(),
            ostream_iterator<int>(cout, " "));
        cout << endl;
    }

    {
        int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        vector<int> v1(data, data + 10);

        fill(v1.begin(), v1.end(), -1);

        for (vector<int>::iterator it = v1.begin(); it != v1.end(); ++it)
            cout << *it << " ";
        cout << "\n";
    }


    /* {
 array<int, 10> s{ 5, 7, 4, 2, 8, 6, 1, 9, 0, 3 };
 replace(s.begin(), s.end(), 8, 88);

 for (int a : s)
     cout << a << " ";
 cout << '\n';

 replace_if(s.begin(), s.end(),
     bind(less<int>(), placeholders::_1, 5), 55);
 for (int a : s)
     cout << a << " ";
 cout << '\n';
     }*/

    {
        string str = "Текст с несколькими   пробелами";
        str.erase(remove(str.begin(), str.end(), ' '),
            str.end());
        cout << str << '\n';
    }

    {
        string s("hello");
        transform(s.begin(), s.end(), s.begin(), (int (*)(int))toupper);
        cout << s;
    }

    {
        vector<int> nums{ 3, 4, 2, 9, 15, 267 };

        cout << "до: ";
        for (auto n : nums)
            cout << n << " ";
        cout << '\n';

        for_each(nums.begin(), nums.end(), [](int& n) { n++; });
        Sum s = for_each(nums.begin(), nums.end(), Sum());

        cout << "после:  ";
        for (auto n : nums)
            cout << n << " ";
        cout << '\n';
        cout << "сумма: " << s.sum << '\n';
    }

    {
        auto p1 = bind1st(plus<int>(), 10);
        auto p2 = bind2nd(plus<int>(), 10);
        cout << p1(20) << endl;             // 10 + 20 = 30
        cout << p2(20) << endl;             // 20 + 10 = 30

        auto m1 = bind1st(minus<int>(), 10);
        auto m2 = bind2nd(minus<int>(), 10);
        cout << m1(20) << endl;             // 10 – 20 = –10
        cout << m2(20) << endl;             // 20 – 10 = 10
    }



    {
        srand(time(0));
        std::vector<int> v, v2(10, 20);
        std::generate_n(std::back_inserter(v), 10, rand);
        std::transform(v.begin(), v.end(), v2.begin(), v.begin(), std::modulus<int>());
        std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, "\n"));
        std::cout << std::endl;
        int factor = 2;
        std::transform(v.begin(), v.end(), v.begin(), std::bind2nd(std::multiplies<int>(), factor));
        std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, "\n"));
    }

    {
        std::string s = "spaces in text";
        std::cout << s << std::endl;
        std::string::iterator new_end =
            std::remove_if(
                s.begin(), s.end(),
                std::bind2nd(std::equal_to<char>(), ' '));
        s.erase(new_end, s.end());  std::cout << s << std::endl;
    }


    {
        std::vector<int> v;
        v.push_back(1); v.push_back(4); v.push_back(2);
        v.push_back(8); v.push_back(5); v.push_back(7);
        std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
        int sum = std::count_if(v.begin(), v.end(),
            std::bind2nd(std::greater<int>(), 5));
        std::cout << "\nThere are " << sum << " number(s) greater than 5" << std::endl;
        std::vector<int>::iterator new_end = //"remove all the elements less than 4.";
            std::remove_if(v.begin(), v.end(),
                std::bind2nd(std::less<int>(), 4));
        v.erase(new_end, v.end());
        std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
        std::cout << "\nElements less than 4 removed" << std::endl;
    }

    {
        std::vector<int> v1;
        v1.push_back(1); v1.push_back(2); v1.push_back(3); v1.push_back(4);
        std::vector<int> v2;
        std::remove_copy_if(v1.begin(), v1.end(), std::back_inserter(v2),
            std::bind2nd(std::greater<int>(), 3));
        std::copy(v2.begin(), v2.end(),
            std::ostream_iterator<int>(std::cout, "\n"));

        std::vector<int> v3;
        std::remove_copy_if(v1.begin(), v1.end(), std::back_inserter(v3),
            std::not1(std::bind2nd(std::greater<int>(), 3)));
        std::copy(v3.begin(), v3.end(), std::ostream_iterator<int>(std::cout, "\n"));
    }


    {


        std::vector<WrapInt> v(10);

        for (int i = 0; i < 10; i++)  v[i] = WrapInt(i + 1);

        std::cout << "Sequence contains: ";
        std::for_each(v.begin(), v.end(), std::mem_fun_ref(&WrapInt::showval));
        std::cout << std::endl;

        std::vector<WrapInt>::iterator end_p = std::remove_if(v.begin(), v.end(), std::mem_fun_ref(&WrapInt::is_prime));

        std::cout << "Sequence after removing primes: ";
        for_each(v.begin(), end_p, std::mem_fun_ref(&WrapInt::showval));
        std::cout << std::endl;

    }


    {
std::vector<string> v;
v.push_back("One"); v.push_back("Two"); v.push_back("Three");
v.push_back("Four");

std::cout << "Sequence	contains:";
std::copy(v.begin(), v.end(), std::ostream_iterator<string>(std::cout, " "));
std::cout << std::endl << "Searching for Three.\n";
std::vector<string>::iterator it = std::find_if(v.begin(), v.end(),
    std::not1(std::bind2nd(std::ptr_fun(strcmp2), "Three")));
if (it != v.end())
{
    std::cout << "Found it! Here is the rest of the story:";
    std::copy(it, v.end(), std::ostream_iterator<char*>(std::cout, "\n"));
}

    }
    return 0;

}