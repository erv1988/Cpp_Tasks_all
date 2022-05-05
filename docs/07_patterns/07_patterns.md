# Что такое Паттерн?

Паттерн проектирования — это часто встречающееся решение определённой проблемы при проектировании архитектуры программ.

В отличие от готовых функций или библиотек, паттерн нельзя просто взять и скопировать в программу. Паттерн представляет собой не какой-то конкретный код, а общую концепцию решения той или иной проблемы, которую нужно будет ещё подстроить под нужды вашей программы.

Паттерны часто путают с алгоритмами, ведь оба понятия описывают типовые решения каких-то известных проблем. Но если алгоритм — это чёткий набор действий, то паттерн — это высокоуровневое описание решения, реализация которого может отличаться в двух разных программах.

Если привести аналогии, то алгоритм — это кулинарный рецепт с чёткими шагами, а паттерн — инженерный чертёж, на котором нарисовано решение, но не конкретные шаги его реализации.

## Из чего состоит паттерн?

Описания паттернов обычно очень формальны и чаще всего состоят из таких пунктов:

- проблема, которую решает паттерн;
- мотивации к решению проблемы способом, который предлагает паттерн;
- структуры классов, составляющих решение;
- примера на одном из языков программирования;
- особенностей реализации в различных контекстах;
- связей с другими паттернами.

Такой формализм в описании позволил создать обширный каталог паттернов, проверив каждый из них на состоятельность.


Все примеры на С++ в репозитории: https://github.com/RefactoringGuru/design-patterns-cpp


# Поведенческие
## Снимок

Снимок - поведенческий паттерн проектирования, который позволяет сохранять и восстанавливать прошлые состояния объектов, не раскрывая подробностей их реализации.

![Снимок](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/01_memento.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/memento

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/memento.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/memento/cpp/example
Пример на С++: https://github.com/RefactoringGuru/design-patterns-cpp/tree/main/src/Memento/Conceptual


## Цепочка обязанностей

Цепочка обязанностей — это поведенческий паттерн проектирования, который позволяет передавать запросы последовательно по цепочке обработчиков. Каждый последующий обработчик решает, может ли он обработать запрос сам и стоит ли передавать запрос дальше по цепи.

![Цепочка обязанностей](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/02_chain.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/chain-of-responsibility

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/chain-of-responsibility.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/chain-of-responsibility/cpp/example
Пример на С++: https://github.com/RefactoringGuru/design-patterns-cpp/tree/main/src/chainofresponsibility/Conceptual

## Наблюдатель

Наблюдатель — это поведенческий паттерн проектирования, который создаёт механизм подписки, позволяющий одним объектам следить и реагировать на события, происходящие в других объектах.

![Наблюдатель](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/03_observer.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/observer

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/observer.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/observer/cpp/example
Пример на С++: https://github.com/RefactoringGuru/design-patterns-cpp/tree/main/src/observer/Conceptual


## Команда

Команда — это поведенческий паттерн проектирования, который превращает запросы в объекты, позволяя передавать их как аргументы при вызове методов, ставить запросы в очередь, логировать их, а также поддерживать отмену операций.

![Команда](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/04_command.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/command

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/command.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/command/cpp/example
Пример на С++: https://github.com/RefactoringGuru/design-patterns-cpp/tree/main/src/command/Conceptual


## Состояние

Состояние — это поведенческий паттерн проектирования, который позволяет объектам менять поведение в зависимости от своего состояния. Извне создаётся впечатление, что изменился класс объекта.

![Состояние](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/05_state.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/state

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/state.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/state/cpp/example
Пример на С++: https://github.com/RefactoringGuru/design-patterns-cpp/tree/main/src/state/Conceptual

## Стратегия

Стратегия — это поведенческий паттерн проектирования, который определяет семейство схожих алгоритмов и помещает каждый из них в собственный класс, после чего алгоритмы можно взаимозаменять прямо во время исполнения программы.

![Стратегия](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/07_strategy.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/strategy

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/strategy.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/strategy/cpp/example
Пример на С++: https://github.com/RefactoringGuru/design-patterns-cpp/tree/main/src/strategy/Conceptual


## Итератор

Итератор — это поведенческий паттерн проектирования, который даёт возможность последовательно обходить элементы составных объектов, не раскрывая их внутреннего представления.

![Итератор](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/08_iterator.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/iterator

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/iterator.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/iterator/cpp/example
Пример на С++: https://github.com/RefactoringGuru/design-patterns-cpp/tree/main/src/iterator/Conceptual


## Шаблонный метод

Шаблонный метод — это поведенческий паттерн проектирования, который определяет скелет алгоритма, перекладывая ответственность за некоторые его шаги на подклассы. Паттерн позволяет подклассам переопределять шаги алгоритма, не меняя его общей структуры.

![Шаблонный метод](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/09_templatemethod.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/template-method

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/template-method.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/template-method/cpp/example
Пример на С++: https://github.com/RefactoringGuru/design-patterns-cpp/tree/main/src/templatemethod/Conceptual


## Посетитель

Посетитель — это поведенческий паттерн проектирования, который позволяет добавлять в программу новые операции, не изменяя классы объектов, над которыми эти операции могут выполняться.

![Посетитель](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/10_visitor.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/visitor

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/visitor.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/visitor/cpp/example
Пример на С++: https://github.com/RefactoringGuru/design-patterns-cpp/tree/main/src/visitor/Conceptual


## Посредник

Посредник — это поведенческий паттерн проектирования, который позволяет уменьшить связанность множества классов между собой, благодаря перемещению этих связей в один класс-посредник.

![Посредник](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/11_mediator.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/mediator

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/mediator.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/mediator/cpp/example
Пример на С++: https://github.com/RefactoringGuru/design-patterns-cpp/tree/main/src/mediator/Conceptual


# Структурные

## Адаптер

Адаптер — это структурный паттерн проектирования, который позволяет объектам с несовместимыми интерфейсами работать вместе.

![Адаптер](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/12_adapter.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/adapter

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/adapter.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/adapter/cpp/example


## Заместитель

Заместитель — это структурный паттерн проектирования, который позволяет подставлять вместо реальных объектов специальные объекты-заменители. Эти объекты перехватывают вызовы к оригинальному объекту, позволяя сделать что-то до или после передачи вызова оригиналу.

![Заместитель](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/13_proxy.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/proxy

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/proxy.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/proxy/cpp/example

## Мост

Мост — это структурный паттерн проектирования, который разделяет один или несколько классов на две отдельные иерархии — абстракцию и реализацию, позволяя изменять их независимо друг от друга.

![Мост](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/15_bridge.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/bridge

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/bridge.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/bridge/cpp/example


## Компоновщик

Компоновщик — это структурный паттерн проектирования, который позволяет сгруппировать множество объектов в древовидную структуру, а затем работать с ней так, как будто это единичный объект.

![Компоновщик](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/16_composite.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/composite

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/composite.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/composite/cpp/example

## Декоратор

Декоратор — это структурный паттерн проектирования, который позволяет динамически добавлять объектам новую функциональность, оборачивая их в полезные «обёртки».

![Декоратор](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/18_decorator.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/decorator

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/decorator.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/decorator/cpp/example


## Фасад

Фасад — это структурный паттерн проектирования, который предоставляет простой интерфейс к сложной системе классов, библиотеке или фреймворку.

![Фасад](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/21_facade.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/facade

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/facade.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/facade/cpp/example

## Легковес

Легковес — это структурный паттерн проектирования, который позволяет вместить бóльшее количество объектов в отведённую оперативную память. Легковес экономит память, разделяя общее состояние объектов между собой, вместо хранения одинаковых данных в каждом объекте.

![Легковес](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/22_flyweight.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/flyweight

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/flyweight.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/flyweight/cpp/example

# Порождающие

## Абстрактная фабрика

Абстрактная фабрика — это порождающий паттерн проектирования, который позволяет создавать семейства связанных объектов, не привязываясь к конкретным классам создаваемых объектов.

![Абстрактная фабрика](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/14_abstractfactory.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/abstract-factory

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/abstract-factory.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/abstract-factory/cpp/example

## Строитель

Строитель — это порождающий паттерн проектирования, который позволяет создавать сложные объекты пошагово. Строитель даёт возможность использовать один и тот же код строительства для получения разных представлений объектов.

![Строитель](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/17_builder.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/builder

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/builder.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/builder/cpp/example

## Фабричный метод 

Фабричный метод — это порождающий паттерн проектирования, который определяет общий интерфейс для создания объектов в суперклассе, позволяя подклассам изменять тип создаваемых объектов.

![Шаблон](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/19_factorymethod.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/factory-method

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/factory-method.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/factory-method/cpp/example

## Прототип

Прототип — это порождающий паттерн проектирования, который позволяет копировать объекты, не вдаваясь в подробности их реализации.

![Прототип](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/20_prototype.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/prototype

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/prototype.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/prototype/cpp/example

## Одиночка

Одиночка — это порождающий паттерн проектирования, который гарантирует, что у класса есть только один экземпляр, и предоставляет к нему глобальную точку доступа.

![Одиночка](https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/images/23_singleton.png?raw=true)

Описание: https://refactoring.guru/ru/design-patterns/singleton

Описание pdf: https://github.com/erv1988/Cpp_Tasks_all/blob/master/docs/07_patterns/pdf/singleton.pdf

Пример на С++: https://refactoring.guru/ru/design-patterns/singleton/cpp/example

