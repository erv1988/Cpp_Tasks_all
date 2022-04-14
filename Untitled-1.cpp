
    class Person
    {
        protected:
        
        //абстактный базовый класс
        // …
    
        static Person *head; 
        public:
        Person *next;
        //…
        virtual ~Person(){}
        //…
        virtual char* getName() const = 0; 
        virtual void setName(constchar *name_) = 0; 
        virtual int getAge() const = 0; 
        virtual void setAge(constint age_) = 0;
    };
    //наследование
    class Employee : public Person
    {
        protected:
        //… 
        public:
        //…
        virtual char* getName() const; 
        virtual void setName(constchar *name_); 
        virtual int getAge() const; 
        virtual void setAge(constint age_);
        //…
        friend ostream&operator<<(ostream& out, Employee& e);
        //… 
        static void show();
        //…	
        //статический метод просмотра
        void Employee::show()
        {
            Person *p = head; 
            while(p)
            {
                //…
                p = p -> next;
            }
        }
        //…	
    }
    //инициализация статической компоненты
    Person * Person::head = NULL;

    int _tmain(int argc, _TCHAR* argv[])
    {
    }



Пример:

// метод‐итератор ‐	найти все узлы заданного номера

void Uzel::get(int data)
{
    Mech* h=head;//  список
    while (h!=NULL) //цикл
    {
        if (h->getid()==typeid(Uzel).name())
        //найти только узлы
        {
            if (h->nomber == data) //сравнить числа
            {
                h->print();
                // вывести как найденнный
            }
        }
        h=h->next;	// на след
    };
}
