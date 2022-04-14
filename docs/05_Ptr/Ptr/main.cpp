#include <string>
#include <thread>
#include <mutex>
#include <iostream>
class X
{
    int a;
    double b;
public:
    void func() {};
};


static X staticX;   // � ������ ������� ����� ���� ����� ������

thread_local unsigned int rage = 1;
std::mutex cout_mutex;

void increase_rage(const std::string& thread_name) {
    ++rage; // ��������� ��� ����������, ��� ���������; ��� ��������� ���������� ������
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "������� ������������������ ��� " << thread_name << ": " << rage << '\n';
}

void _thread() {
    std::thread a(increase_rage, "a"), b(increase_rage, "b");
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "������� ������������������ ��� main: " << rage << '\n';
    }
    a.join();
    b.join();
}

void _dynamic() {
    X* ptr = nullptr; //���������, �� ����������� �� �� ���
    {
        X* ptr2 = new X(); // ���������� ������� ������ X � ������ 
                           // (����� ��������� ��� ������������ ������, 
                           // �������� ������������� � ���� ����). 
                           // ptr2 - ��������� �� ���� ������
        ptr = ptr2; //ptr ��������� �� ��� �� ������
    } //��������� ptr2 ������� �� ������� ��������� � ������������, 
      // �� ������, �� ������� �� ���������, ������� � ������
    delete ptr; //���������� ����� �����������, � ����� ����� ������, 
                // �� ������� ��������� ptr, ��������� �� ������
}

void _auto() {
    X object; //� ������ ��������� ������ ������ X
    {
        X object2; //��� ���� ������ �������� � ������
        //� ������ ������ � ������ ��������� � �������� ��� �������
    } //����� object2 ������� �� ������� ��������� � ������������
} //����� ������������ object



int* some_func() { return new int; }

void some_result() {
    int* int_ptr = some_func();
    // ...
    // ��� ������ � int_ptr?
}

bool func() { return true; }
void func2(){}
void weak_fun() {
    X* ptr = new X();
    if (func())
    {
        func2();
        return;
    }
    delete ptr;
}




void unique() {
    

    {
        std::unique_ptr<X> ptr(new X()); //������ ������ X ������ � ������������ ������
    } //����� ��������� ptr �������� ���� ������� ��������� � ������������, �� ����� ���� ������� �� ������ ������, �� ������� ���������

}


void not_weak_fun_1() {
    std::unique_ptr<X> ptr(new X());
    if (func())
    {
        func2();
        return;
    }
}

void not_weak_fun_2() {
    auto ptr = std::make_unique<X>();
    if (func())
    {
        func2();
        return;
    }
}


class A{};
class B{};

void funcAB(std::unique_ptr<A> a, std::unique_ptr<B> b) {}
void callAB() {
    funcAB(std::unique_ptr<A>(new A()), std::unique_ptr<B>(new B()));
}
void callAB2() {
    funcAB(std::make_unique<A>(), std::make_unique<B>());
}



void funcO(std::unique_ptr<X> a) {}
void mainO() {
    auto a = std::make_unique<X>();
    //funcO(a); //�� ��������������, std::unique_ptr ������ ����������, ������ ��� ����� � ������� ���� �� ��������� ����������
    funcO(std::move(a)); //�������� �������� �������� � func, main ������ �� ������� ��������, �� ������ �� func ������ ����� ���������
}



void funcG(X* a) {} // ������ ���������
void mainG() {
    auto a = std::make_unique<X>();
    funcG(a.get());
}



void shared()     {
    std::shared_ptr<X> ptr = std::make_shared<X>(); //�������� ������
    {
        std::shared_ptr<X> ptr2 = ptr; //������ � ������� ��� ���������, ���������� � ���� ptr � ptr2
    } //ptr2 ������� �� ������� ���������, �� ������ �� �������������, ������ ��� ���� ptr, ������� ��-�������� ��������� �� ����
} //ptr ������� �� ������� ���������, � ������ ������������





class Owner
{
public:
    std::shared_ptr<X> owningPtr;

    Owner() {
        owningPtr = std::make_shared<X>();
    }
};

class User
{
    std::weak_ptr<X> usingPtr;
public:
    User(std::weak_ptr<X> object) {
        usingPtr = object;
    }

    void use() {
        if (std::shared_ptr<X> object = usingPtr.lock()) { 
            //������� �������� ������������ std::shared_ptr �� std::weak_ptr, ���� ��������� ������ std::shared_ptr, ������, ������ ��� ��� �����
            object->func();
        }
        else {
            //������ ��� �����
        }
    }
};

void mainUO() {
    Owner owner;
    User user(owner.owningPtr);
    user.use();
}





int main() {
    staticX.func();
    _auto();
    _dynamic();
    _thread();
}