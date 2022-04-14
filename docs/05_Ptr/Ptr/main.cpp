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


static X staticX;   // В памяти имеется ровно один такой объект

thread_local unsigned int rage = 1;
std::mutex cout_mutex;

void increase_rage(const std::string& thread_name) {
    ++rage; // изменение вне блокировки, это нормально; это локальная переменная потока
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Счётчик производительности для " << thread_name << ": " << rage << '\n';
}

void _thread() {
    std::thread a(increase_rage, "a"), b(increase_rage, "b");
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Счётчик производительности для main: " << rage << '\n';
    }
    a.join();
    b.join();
}

void _dynamic() {
    X* ptr = nullptr; //Указатель, не указывающий ни на что
    {
        X* ptr2 = new X(); // Размещение объекта класса X в памяти 
                           // (также известной как динамическая память, 
                           // зачастую реализованной в виде кучи). 
                           // ptr2 - указатель на этот объект
        ptr = ptr2; //ptr указывает на тот же объект
    } //Указатель ptr2 выходит из области видимости и уничтожается, 
      // но объект, на который он указывает, остаётся в памяти
    delete ptr; //Происходит вызов деструктора, а после этого объект, 
                // на который указывает ptr, удаляется из памяти
}

void _auto() {
    X object; //В памяти создается объект класса X
    {
        X object2; //Ещё один объект размещён в памяти
        //В данный момент в памяти размещены и доступны оба объекта
    } //Здесь object2 выходит из области видимости и уничтожается
} //Здесь уничтожается object



int* some_func() { return new int; }

void some_result() {
    int* int_ptr = some_func();
    // ...
    // Что делать с int_ptr?
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
        std::unique_ptr<X> ptr(new X()); //Объект класса X создан в динамической памяти
    } //Здесь указатель ptr покидает свою область видимости и уничтожается, но перед этим удаляет из памяти объект, на который указывает

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
    //funcO(a); //Не скомпилируется, std::unique_ptr нельзя копировать, потому что иначе у объекта было бы несколько владельцев
    funcO(std::move(a)); //Владение объектом передано в func, main больше не владеет объектом, на выходе из func объект будет уничтожен
}



void funcG(X* a) {} // Будьте осторожны
void mainG() {
    auto a = std::make_unique<X>();
    funcG(a.get());
}



void shared()     {
    std::shared_ptr<X> ptr = std::make_shared<X>(); //Создаётся объект
    {
        std::shared_ptr<X> ptr2 = ptr; //Теперь у объекта два владельца, выраженных в виде ptr и ptr2
    } //ptr2 выходит из области видимости, но объект не освобождается, потому что есть ptr, который по-прежнему ссылается на него
} //ptr выходит из области видимости, и объект уничтожается





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
            //Попытка получить оригинальный std::shared_ptr из std::weak_ptr, если возвращён пустой std::shared_ptr, значит, объект уже был удалён
            object->func();
        }
        else {
            //Объект уже удалён
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