# Заголовочный файл

#include \<thread\>

# Запуск потока

std::thread t(callable_object, arg1, arg2, ..) 

Создаёт новый поток выполнения, ассоциируемый с t, который вызывает callable_object(arg1, arg2). Вызываемый объект (т.е. указатель функции, лямбда-выражение, экземпляр класса с вызовом функции operator) немедленно выполняется новым потоком с (выборочно) передаваемыми аргументами. Они копируются по умолчанию. 
Если хотите передать по ссылке, придётся использовать метод warp к аргументу с помощью std::ref(arg). 
Не забывайте: если хотите передать unique_ptr, то должны переместить его (std::move(my_pointer)), так как его нельзя копировать.

# Жизненный цикл потока

t.join() и t.detach()

Если основной поток завершает выполнение, все второстепенные сразу останавливаются без возможности восстановления. Чтобы этого не допустить, у родительского потока имеются два варианта для каждого порождённого:

- Блокирует и ждёт завершения порождённого потока, вызывая на нём метод join.

- Прямо объявляет, что порождённый поток может продолжить выполнение даже после завершения родительского, используя метод detach.


Запомните: объект потока можно перенести, но нельзя копировать.

    ```

    /* BASIC THREAD MANAGEMENT
    * 1. Create threads with three different callable objects (a function newValue, a lambda expression and a class operator)
    * 2. Use of join and detach methods
    * 3. Pass a ref value as argument
    */

    #include <iostream>
    #include <thread>
    #include <vector>
    #include <string>


    void newValue(int& value) {
        value = 2;
        std::string out = "[newValue] t0 new value: " + std::to_string(value) +" \n";
        std::cout << out;
    }

    class workClass {
    public:
        void operator() (){
            std::string out = "[operator] t1 running\n";
            std::cout << out;
        };
    };
    int main() {
        std::cout << "Main thread running" << std::endl;
        std::vector<std::thread> detachPool;

        int value = 5;
        std::string value_out = "[main] \told value: " + std::to_string(value);
        std::thread t0(newValue, std::ref(value));
        //t0 must terminate before inspecting the new value
        t0.join();
        value_out += "\tnew value: " + std::to_string(value) + "\n";
        std::cout << value_out;

        std::thread t1{ workClass() };
        detachPool.push_back(std::move(t1));

        std::thread t2( []() {
                std::string out = "[lambda] t2 running\n";
                std::cout << out; });
        detachPool.push_back(std::move(t2));

        for( std::thread& t : detachPool ) {
            t.detach();
        }
        std::string out = "Main thread exits\n";
        std::cout << out;
        return 0;
    }

    ```

Теперь вы сами можете проверить это недетерминированное поведение многопоточности.

    ```
    /* SHOWING THE NON-DETERMINISTIC BEHAVIOUR OF MULTITHREADING
    * Differently from a single-threaded implementation, each execution produces a different
    * and not predictable output (the only certainty is that A lines are sorted in ascending
    * order, as well as B lines).
    */

    #include <thread>
    #include <iostream>
    #include <string>

    void run(std::string threadName) {
    for (int i = 0; i < 10; i++) {
        std::string out = threadName + std::to_string(i) + "\n";
        std::cout << out;
    }
    }

    int main() {
    std::thread tA(run, "A");
    std::thread tB(run, "\tB");
    tA.join();
    tB.join();
    }
    ```
Возможный вывод:

    B0
    A0
    A1
    A2
       B1
    A3
       B2
       B3
    ..

В отличие от однопоточной реализации, каждое выполнение даёт разный и непредсказуемый результат (единственное, что можно сказать определённо: строки А и B упорядочены по возрастанию). Это может вызвать проблемы, когда очерёдность команд имеет значение.

    ```
    /* SYNCHRONIZATION PROBLEM
    * After thread A evaluates "value" as true, thread B changes it. Now we are inside an if-block, even if the constraint has been violated. 
    * If two threads access the same data, one writing and one reading, there is no guarantee on which operation is executed first. 
    * Accesses must be synchronized.
    */

    #include <thread>
    #include <iostream>
    #include <string>

    void runA(bool& value, int i) {
    if(value) {
        //value should always be 1
        std::string out = "[ " + std::to_string(i) + " ] value " + std::to_string(value)  + "\n";
        std::cout << out;
    }
    }

    void runB(bool& value) {
        value = false;
    }

    int main() {
        for(int i = 0; i < 20; i++) {
            bool value = true; //1
            std::thread tA(runA, std::ref(value), i);
            std::thread tB(runB, std::ref(value));
            tA.join();
            tB.join();
        }
    }
    ```

Возможный вывод:

    ..
    [ 12 ] value 0
    [ 13 ] value 1
    [ 14 ] value 0
    [ 15 ] value 0
    [ 16 ] value 0
    [ 17 ] value 0
    [ 18 ] value 1
    [ 19 ] value 0
    ..

После того как поток А оценивает «значение» как истинное, поток B меняет его. Теперь мы внутри блока if, даже если нарушены ограничения.

Если два потока имеют доступ к одним и тем же данным (один к записи, другой - к чтению), нельзя сказать наверняка, какая операция будет выполняться первой.

Доступ должен быть синхронизирован.




Проблемный код 1

```
    #include <queue>
    #include <thread>
    #include <iostream>

    /* THE QUEUE PROBLEM #1
    * Two threads (A and B) read the front value of a queue and pop it immediately after.
    * [ QUESTION ] : Do the two threads together always retrieve all the elements of the queue?
    */

    void retrieve_and_delete(std::queue<int>& rawQueue, std::string threadName) {
        if( !rawQueue.empty() ) {
            std::string out = "[ " + threadName + " ] front " + std::to_string(rawQueue.front());
            rawQueue.pop();
            out += " | new front " + std::to_string(rawQueue.front())  + "\n";
            std::cout << out;
        }
    }

    int main() {
        std::queue<int> rawQueue;

        // queue 0 to 19
        for ( int i = 0; i < 20; i++) {
            rawQueue.push(i);
        }

        // two threads reading the front value and popping it immediately after
        for ( int i = 0; i < 10; i++) {
            std::thread tA(retrieve_and_delete, std::ref(rawQueue), "A");
            std::thread tB(retrieve_and_delete, std::ref(rawQueue), "B");
            tA.join();
            tB.join();
        }

    }
```

Проблемный код 2

    ```

        #include <queue>
        #include <deque>
        #include <thread>
        #include <iostream>

        /* THE QUEUE PROBLEM #2
        * Six threads ( A to F ) push values ( 1 to 6 ) in a queue.
        * [ QUESTION ] : Can you say why sometimes there is an element = 0 (value never pushed) ?
        */

        void pushValue(std::queue<int>& rawQueue, int value) {
            std::string out = "Pushing " + std::to_string(value) + " | size before: " + std::to_string(rawQueue.size());
            rawQueue.push(value);
            out += "\tafter: " + std::to_string(rawQueue.size()) + "\n";
            std::cout << out;
        }

        int main() {
            std::queue<int> rawQueue;

            std::thread tA(pushValue, std::ref(rawQueue), 1);
            std::thread tB(pushValue, std::ref(rawQueue), 2);
            std::thread tC(pushValue, std::ref(rawQueue), 3);
            std::thread tD(pushValue, std::ref(rawQueue), 4);
            std::thread tE(pushValue, std::ref(rawQueue), 5);
            std::thread tF(pushValue, std::ref(rawQueue), 6);
            tA.join();
            tB.join();
            tC.join();
            tD.join();
            tE.join();
            tF.join();

            std::cout << std::endl << "Size of the queue: " + std::to_string(rawQueue.size()) << std::endl;
            std::cout << "Elements in the queue: " << std::endl;
            // in the queue there should be 1, 2, 3, 4, 5, 6 in any order
            for ( int i = 0; !rawQueue.empty() ; i++ ) {
                std::cout << std::to_string(rawQueue.front()) << std::endl;
                rawQueue.pop();
            }

        }

    ```

# Заголовочный файл 

#include \<mutex\>

# Объявление 

std::mutex mutex_name;

# Захват мьютекса 

mutex_name.lock();

Поток запрашивает монопольное использование общих данных, защищаемых мьютексом. Дальше два варианта развития событий: происходит захват мьютекса этим потоком (и в этом случае ни один другой поток не сможет получить доступ к этим данным) или поток блокируется (если мьютекс уже захвачен другим потоком).

# Освобождение мьютекса | mutex_name.unlock();

Когда ресурс больше не нужен, текущий владелец должен вызвать функцию разблокирования unlock, чтобы и другие потоки могли получить доступ к этому ресурсу. 

Когда мьютекс освобождается, доступ предоставляется одному из ожидающих потоков.

# Unique lock, дающий свободу

Как только владение мьютексом получено (благодаря std::lock_guard), он может быть освобождён. std::unique_lock действует в схожей манере плюс делает возможным многократный захват и освобождение (всегда в таком порядке) мьютекса, используя те же преимущества безопасности парадигмы RAII.

# Как создать потокобезопасную очередь

Разберёмся, как реализовать простейшие потокобезопасные очереди, то есть очереди с безопасным доступом для потоков.

В библиотеке стандартных шаблонов уже есть готовая очередь (rawQueue). Наша реализация будет предполагать: а) извлечение и удаление целочисленного значения из начала очереди и б) добавление нового в конец очереди. И всё это при обеспечении потокобезопасности.

Сначала выясним, почему и как эти две операции могут создавать проблемы для многопоточности.

# Извлечение и удаление

Для извлечения и удаления значения из начала очереди необходимо выполнить три операции:
1. Проверить, не пуста ли очередь.
2. Если нет, получается ссылка на начало очереди (rawQueue.front()). 
3. Удаляется начало очереди (rawQueue.pop()). 

В промежутках между этими этапами к очереди могут получать доступ и другие потоки с целью внесения изменений или чтения.

# Добавление

Рассмотрим теперь добавление нового значения с помощью rawQueue.push(): новый элемент добавляется в конец контейнера и становится следующим за последним на данный момент элементом. Дальше на единицу увеличивается размер. Заметили здесь проблему? А что, если два потока одновременно добавят новое значение, увидев этот последний элемент? И что может произойти в интервале между добавлением нового элемента и увеличением размера? Кто-нибудь возьмёт да и прочитает неправильный размер. 

Получается, мы должны быть уверены, что никто не будет трогать очередь, пока мы выполняем наши задачи. Используем мьютекс для защиты этих многоступенчатых операций и сделаем так, чтобы все вместе они смотрелись как одна атомарная операция.

    ```

    #include <mutex>
    #include <queue>

    /* The simplest thread-safe queue, using just raw mutex ( no lock_guard ); Check "3 | Thread Safe Queue.h" for a better version.
    * To implement: isEmpty(), getSize()
    * How to use it: #include "2 | Thread Safe Queue (dummy).h" and then use an instance of threadSafe_queue as a normal queue
    */

    class threadSafe_queue {

        std::queue<int> rawQueue; // shared structure between all threads
        std::mutex m; // rawQueue's red door

    public:

        int& retrieve_and_delete() {
            int front_value = 0; // if empty return 0
            m.lock();
            // From now on, the current thread is the only one that can access rawQueue
            if( !rawQueue.empty() ) {
                front_value = rawQueue.front();
                rawQueue.pop();
            }
            m.unlock();
            // other threads can lock the mutex now
            return front_value;
        };

        void push(int val) {
            m.lock();
            rawQueue.push(val);
            m.unlock();
        };

    };

    ```

# Lock guard и парадигма RAII

У нас две большие проблемы с этим простым мьютексом:

Что произойдёт, если мы забудем вызвать unlock()? Ресурс будет недоступен в течение всего времени существования мьютекса, и уничтожение последнего в неразблокированном состоянии приведёт к неопределённому поведению.

Что произойдёт, если до вызова unlock() будет выброшено исключение? unlock() так и не будет исполнен, а у нас будут все перечисленные выше проблемы.

К счастью, проблемы можно решить с помощью класса std::lock_guard. Он всегда гарантированно освобождает мьютекс, используя парадигму RAII (Resource Acquisition Is Initialization, что означает «получение ресурса есть инициализация»). Вот как это происходит: мьютекс инкапсулируется внутри lock_guard, который вызывает lock() в его конструкторе и unlock() в деструкторе при выходе из области видимости. Это безопасно даже при исключениях: раскрутка стека уничтожит lock_guard, вызывая деструктор и таким образом освобождая мьютекс.

    ```

    #include <mutex>
    #include <queue>

    /* The simplest thread-safe queue, using lock_guard
    * To implement: isEmpty(), getSize()
    * How to use it: #include "3 | Thread Safe Queue.h" and then use an instance of threadSafe_queue as a normal queue
    */

    class threadSafe_queue {

        std::queue<int> rawQueue; // shared structure between all threads
        std::mutex m; // rawQueue's red door

    public:

        int& retrieve_and_delete() {
            int front_value = 0; // if empty return 0
            std::lock_guard<std::mutex> lg(m);
            // From now on, the current thread is the only one that can access rawQueue
            if( !rawQueue.empty() ) {
                front_value = rawQueue.front();
                rawQueue.pop();
            }
            return front_value;
        };  // other threads can lock the mutex now

        void push(int val) {
            std::lock_guard<std::mutex> lg(m);
            // from now on, the current thread is the only one that can access rawQueue
            rawQueue.push(val);
        }; // other threads can lock the mutex now
    };


    ```

# Когда использовать?

Когда вам не всегда нужен захват ресурса.

Вместе с std::condition_variable.

При захвате std::shared_mutex в эксклюзивном режиме (см. далее). 

# Общий мьютекс + общий захват дают больше читателей

std::mutex  — это мьютекс, которым одномоментно может владеть только один поток. Однако это ограничение не всегда обязательно. Например, потоки могут одновременно и безопасно читать одни и те же общие данные. Просто читать, не производя с ними никаких изменений. Но в случае с доступом к записи только записывающий поток может иметь доступ к данным.

Начиная с C++17, std::shared_mutex формирует доступ двух типов:

Общий доступ: потоки вместе могут владеть мьютексом и иметь доступ к одному и тому же ресурсу. Доступ такого типа можно запросить с помощью std::shared_lock (lock guard для общего мьютекса). При таком доступе любой эксклюзивный доступ блокируется.

Эксклюзивный доступ: доступ к ресурсу есть только у одного потока. Запрос этого типа осуществляется с помощью класса unique lock.

Синтаксис

# Заголовочный файл 

 #include \<shared_mutex\>;

# Объявление 

std::shared_mutex raw_sharedMutex;

# Для захвата в общем режиме 

std::shared_lock<std::shared_mutex> sharedLock_name(raw_sharedMutex);

# Для захвата в эксклюзивном режиме 

std::unique_lock<std::shared_mutex> uniqueLock_name(raw_sharedMutex);

    #include <shared_mutex>
    #include <vector>
    std::shared_mutex door; //объявление мьютекса
    std::vector<int> v;
    int readVectorSize() {
        /* потоки могут вызывать эту функцию одновременно
        * доступ на запись запрещена, когда получен sl */    
        
        std::shared_lock<std::shared_mutex> sl(door);
        return v.size();
    }
    void pushElement(int new_element) {
        /* гарантирован эксклюзивный доступ к вектору */
        
        std::unique_lock<std::shared_mutex> ul(door);
        v.push_back(new_element);
    }

# Scoped lock, дающий больше мьютексов (и без клинча)

Впервые появившийся в C++17 и действующий в схожей манере, что и std::lock_guard, он даёт возможность получения нескольких мьютексов. Без std::scoped_lock такая операция очень опасна, так как может привести к взаимной блокировке.

# Краткая история взаимоблокировки:

Поток A хочет увести 200$ с банковского счёта Жеки на счёт Бяки в виде одной атомарной операции. Он начинает с того, что захватывает мьютекс, защищающий счёт Жеки, чтобы изъять деньги, а затем пытается захватить счёт Бяки.
В то же время поток B хочет увести 100$ со счёта Бяки на счёт Жеки. Он получает захват счёта Бяки, чтобы изъять деньги и попытаться захватить счёт Жеки. Оба потока блокируются, уснув в ожидании друг друга.

std::scoped_lock одновременно захватывают (а затем освобождают) все мьютексы, передаваемые в качестве аргумента, по принципу «всё или ничего»: если хотя бы один захват выбрасывает исключение, освобождаются все уже захваченные мьютексы.

std::scoped_lock<std::mutex> scoped_lock_name(raw_mutex1, raw_mutex2, ..);    