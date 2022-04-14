# Умные указатели в C++

Поговорим об умных указателях в C++, зачем они нужны, чем различаются указатели owning и non-owning и почему нельзя всегда использовать new/delete для создания и удаления объектов.

# Как C++ управляет памятью?

Прежде чем объяснить, зачем нужны и какие проблемы решают умные указатели, мы кратко опишем то, как C++ управляет памятью.
Когда мы говорим про управление памятью в C++, мы неизменно обращаемся к термину Длительность хранения (длительность хранения). Длительность хранения – это свойство объекта, которое описывает, когда тот попадает в память и когда её освобождает.

В C++ существует четыре вида [1] Длительность хранения:

1. Автоматическая Длительность хранения. 

    Когда управление входит в область видимости объекта (также известную как scope [2]), он размещается в автоматической памяти, зачастую реализованной в виде стека; когда управление покидает эту область, вызывается деструктор и память освобождается.


        //Подопытный клас
        class X {
            int a;
            double b;
        public:
            void func() {};
        };

        void _auto() {
            X object; //В памяти создается объект класса X
            {
                X object2; //Ещё один объект размещён в памяти
                //В данный момент в памяти размещены и доступны оба объекта
            } //Здесь object2 выходит из области видимости и уничтожается
        } //Здесь уничтожается object

        
2. Статическая длительность хранения
    
    Статическая связана с использованием спецификаторов static и extern. Объекты со статической длительностью хранения создаются при запуске программы и удаляются при её завершении.

        static X staticX;   // В памяти имеется ровно один такой объект

3. Длительность хранения потока 
    
    Устанавливается спецификатором thread_local. Имеющие эту Длительность хранения объекты создаются при старте потока и удаляются при его завершении.

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

4. Динамическая длительность хранения 

    Неразрывно связана с использованием ключевых слов new и delete.

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
    
Можно сказать, что в случае с автоматической Длительность хранения память освобождается автоматически, а в случае с динамической – вручную. Почему же тогда не использовать всегда автоматическую память?

Чтобы использовать стек, необходимо заранее на этапе компиляции знать, как много памяти понадобится, а это известно не всегда.
Иногда надо, чтобы объект оставался в памяти и после выхода из области видимости в которой был создан, а в случае размещения объекта на стеке это невозможно.
Чтобы обойти эти ограничения, необходимо использовать динамическую память про использование которой мы и будем сегодня говорить.

# Что такое умные указатели и зачем они нужны?

Используем динамическую память, отлично. Теперь объекты могут покидать область видимости, где были созданы, и иметь определяемый во время выполнения размер – жизнь стала налаживаться и жаловаться как будто не на что.

    * Примечание
    Все умные указатели доступны через включение соответствующего хедера (#include <memory>).


Предлагаем взглянуть на следующий фрагмент кода:


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

    
На первый взгляд, здесь всё хорошо, но есть нюансы:

- Если func() выбросит исключение, то управление не дойдёт до delete и память не освободится.
- Если func() вернёт true, то после выполнения func2() управление покинет функцию, но память не освободится, т.к. автор кода забыл добавить delete внутрь условия.
- Если бы автор забыл delete также в 6-й строке, память тоже не освободилась бы.

Тут C++ программисты решили, что с них хватит, и придумали правило, заключающееся в том, чтобы никогда не использовать new/delete. Как – увидим ниже.
Помимо проблем непосредственно с new/delete, существует проблема и с простыми указателями. Она заключается в сложности разделения указателей, которые владеют объектом (owning pointer), а значит, и ответственны за вызов new/delete, и указателей, которые используют объект (non owning pointer).

При использование простых указателей (также известных как raw pointers) невозможно без дополнительных комментариев или дополнительного изучения кода определить, какой указатель объектом владеет, а какой – только использует. Взгляните на следующую декларацию:
        

    int* func();

    
Главная проблема здесь, что тому, кто будет вызывать функцию, совершенно неясно, должен он вызвать delete для возвращаемого указателя или за это ответственен код где-то в другой части программы. Иначе говоря, здесь не видно, является указатель владеющим или использующим.

Все вышеназванные проблемы изящно решаются умными указателями. Умные указатели в C++ – это не что-то магическое, встроенное в синтаксис языка, а не более чем набор классов из стандартной библиотеки. Разберёмся с ними один за одним.

# std::unique_ptr

Первым умным указателем, с которым мы познакомимся, будет std::unique_ptr [3]. Он ссылается на объект в динамической памяти и при выходе из области видимости уничтожает хранимый объект. Взглянем на пример кода ниже:

       
    {
        std::unique_ptr<X> ptr(new X()); //Объект класса X создан в динамической памяти
    } //Здесь указатель ptr покидает свою область видимости и уничтожается, но перед этим удаляет из памяти объект, на который указывает

    
Когда std::unique_ptr выходит из области видимости, утечки памяти не происходит, потому что в своем деструкторе умный указатель вызывает delete для объекта на который ссылается, высвобождая тем самым память.

Важно понять, что внутри умные указатели всё равно используют new/delete, они лишь позволяют программисту не делать этого и, как следствие, защищают его от ошибок.
От проблем с внезапными исключениями использующих умные указатели (в частности std::unique_ptr) программистов защищает развёртывание стека (stack-unwinding [4]).

Подробное рассмотрение этого механизма С++ выходит за рамки, но главное, что нужно знать о нём – если на стеке был создан объект, а после этого было выброшено исключение, C++ гарантированно вызовет деструктор для этого объекта. Это значит, что если мы обновим код в листинге 3 так, чтобы он использовал умные указатели, то избавимся от всех трёх вышеназванных проблем:

        
    std::unique_ptr<X> ptr(new X());
    if (func()) {
        func2();
        return;
    }

    
Теперь программисту не надо ставить delete, а в случае, если одна из функций выбросит исключение, развёртывание стека защитит нас от утечки памяти.

И всё бы хорошо, но мы по-прежнему используем new. Чтобы правило никогда не использовать new/delete соблюдалось, была придумана функция std::make_unique [5], которая позволяет создавать std::unique_ptr, но с несколькими дополнительными фичами:

Теперь правило никогда не использовать new/delete может быть полностью соблюдено.
std::make_unique позволяет не писать имя класса дважды:
        
    auto ptr = std::make_unique<X>();

    
std::make_unique решает проблему неопределённого порядка вычисления аргументов (unspecified evaluation order). Рассмотрим следующий фрагмент кода:
        
    void func(std::unique_ptr<A> a, std::unique_ptr<B> b) {}
    int main() {
        func(std::unique_ptr<A>(new A()), std::unique_ptr<B>(new B()));
    }

    
Здесь возможен следующий порядок вычисления аргументов:

    new A()
    new B()
    std::unique_ptr<A>(...)
    std::unique_ptr<B>(...)

Если при вызове new B() произойдет исключение, занятая при вызове new A() память не освободится, потому что умный указатель для этого объекта ещё не был создан, а delete никто вызывать и не собирался. Использование std::make_unique решает подобные проблемы.

std::unique_ptr используется тогда, когда объект должен иметь только одного владельца, однако мы можем передать право на владение кому-то другому. Чтобы это сделать, необходимо использовать std::move[6]. Рассмотрим код:

        
    void func(std::unique_ptr<X> a) {}
    int main() {
        auto a = std::make_unique<X>();
        func(a); //Не скомпилируется, std::unique_ptr нельзя копировать, потому что иначе у объекта было бы несколько владельцев
        func(std::move(a)); //Владение объектом передано в func, main больше не владеет объектом, на выходе из func объект будет уничтожен
    }

    
Раз std::unique_ptr нельзя копировать, становится непонятно, как разрешить кому-то использовать указываемый объект, не передавая ему право на владение. Очень просто: нужно всего лишь использовать простой указатель на созданный объект, который можно получить с помощью метода get(), как в листинге ниже. Однако будьте внимательны: не допускайте ситуации, когда std::unique_ptr вместе с хранимым объектом были уничтожены, но где-то по-прежнему находится простой указатель указывающий на невалидную (уже) область памяти.

Делайте так, чтобы простые non owning указатели переставали существовать до того, как перестанет существовать объект, на который они указывают.
        
    void func(X* a) {}
    int main() {
        auto a = std::make_unique<X>();
        func(a.get());
    }

    
Когда в коде используются простые указатели и умные, сразу становится понятно, где указатель владеет объектом, а где – только использует.

# std::experimental::observer_ptr

На самом деле в коде с умными указателями не все простые указатели только используют объекты – некоторые из них могут ими владеть. Чтобы полностью отделить non owning, полученные через get() простые указатели от любых других простых указателей, был придуман std::experimental::observer_ptr[7], но на данный момент он ещё не вошёл в стандарт.

std::unique_ptr – это умный указатель, о котором вы должны подумать в первую очередь, когда решите разместить что-нибудь в динамической памяти. Это ваш умный указатель по умолчанию.

# std::shared_ptr
std::unique_ptr и правда хорош, но он не поможет в ситуации, когда мы хотим, чтобы несколько объектов работали с одним общим ресурсом и чтобы в момент, когда все эти объекты были выгружены из памяти, за ненадобностью автоматически выгрузился бы и ресурс.

В такой ситуации необходимо использовать std::shared_ptr [8]. Этот умный указатель разрешает объекту иметь несколько владельцев, а когда все владельцы уничтожаются, уничтожается и объект. Такое поведение достигается за счёт наличия специального счётчика ссылок внутри std::shared_ptr. Каждый раз, когда такой указатель копируется, счётчик инкрементируется, а когда один из указателей уничтожается – декрементируется. В момент, когда счётчик достигает нуля, объект уничтожается. Посмотрим на код:

        
{
        std::shared_ptr<X> ptr = std::make_shared<X>(); //Создаётся объект
        {
            std::shared_ptr<X> ptr2 = ptr; //Теперь у объекта два владельца, выраженных в виде ptr и ptr2
        } //ptr2 выходит из области видимости, но объект не освобождается, потому что есть ptr, который по-прежнему ссылается на него
    } //ptr выходит из области видимости, и объект уничтожается

    
std::make_shared является аналогом std::make_unique для std::shared_ptr.

Существуют ситуации, когда объект A должен ссылаться на B, а B – на A. Это называется циклической ссылкой (cyclic reference/circular dependency). В таком случае оба объекта никогда не будут выгружены из памяти.

# std::weak_ptr

Чтобы разорвать цикличность, необходимо использовать std::weak_ptr[9]. Это фактически умный указатель non owning, предназначенный для использования именно с std::shared_ptr. Копирование std::weak_ptr не увеличивает счётчик в std::shared_ptr, а значит и не защищает объект от уничтожения. При этом всегда имеется возможность проверить, существует ли ещё объект, на который ссылается std::weak_ptr, или нет. Внимание на код:

        
    class Owner {
    public:
        std::shared_ptr<X> owningPtr;

        Owner() {
            owningPtr = std::make_shared<X>();
        }
    };

    class User {
        std::weak_ptr<X> usingPtr;
    public:
        User(std::weak_ptr<X> object) {
            usingPtr = object;
        }

        void use() {
            if (std::shared_ptr<X> object = usingPtr.lock()) { //Попытка получить оригинальный std::shared_ptr из std::weak_ptr, если возвращён пустой std::shared_ptr, значит, объект уже был удалён
            object->func();
            } else {
                //Объект уже удалён
            }
        }
    };

    int main() {
        Owner owner;
        User user(owner.owningPtr);
        user.use();
    }

    
Вообще говоря, std::weak_ptr необходимо использовать всегда, когда надо ссылаться на управляемый std::shared_ptr объект, но не защищать его от уничтожения.

# std::auto_ptr
Существует одно самое главное правило использования std::auto_ptr[10]: никогда не использовать std::auto_ptr. Этот умный указатель был помечен как устаревший в C++ 11, а в C++ 17 был полностью удалён из стандарта языка.

# Выводы

Каждый программист на C++ должен уметь использовать умные указатели. Умные указатели – это ваш способ управления динамической памятью по умолчанию. std::unique_ptr – это ваш умный указатель по умолчанию. Использование умных указателей не противоречит использованию простых указателей, в случае, если последние используют объекты, а не владеют ими. std::auto_ptr – зло.

Источники
https://en.cppreference.com/w/cpp/language/storage_duration
https://en.cppreference.com/w/cpp/language/scope
https://en.cppreference.com/w/cpp/memory/unique_ptr
https://en.cppreference.com/w/cpp/language/throw
https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
https://en.cppreference.com/w/cpp/utility/move
https://en.cppreference.com/w/cpp/experimental/observer_ptr
https://en.cppreference.com/w/cpp/memory/shared_ptr
https://en.cppreference.com/w/cpp/memory/auto_ptr
https://en.cppreference.com/w/cpp/memory/weak_ptr