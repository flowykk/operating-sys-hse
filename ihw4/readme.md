# ИДЗ №4 - UDP протокол

__Работу выполнил__: Рахманов Данила Дмитриевич

__Вариант__: 31

__Условие задачи__: Задача о Пути Кулака. На седых склонах Гималаев стоит древний буддистский монастырь: Гуань-Инь-Янь. Каждый год в день сошествия на землю боддисатвы монахи монастыря собираются на совместное празднество и показывают свое совершенствование на Пути Кулака. Всех соревнующихся монахов первоначально разбивают на пары. Бои продолжаются до выявления победителя. Монах который победил в финальном бою, забирает себе на хранение статую боддисатвы. Реализовать многопроцессное приложение, определяющего победителя. В качестве входных данных используется массив, в котором хранится количество энергии Ци каждого монаха. При победе монах забирает энергию Ци своего противника. Новые пары образуются среди победителей других пар в порядке завершения поединков. То есть, возможна ситуация, когда бойцы, участвующие в поединке могут быстро победить и начать биться с другими, в то время как поединки начавшиеся ранее, могут продолжаться. Причем длительное время. Каждый поединок протекает некоторое случайное время, которое пропорционально отношению энергии Ци побежденного к энергии Ци победителя, умноженному на поправочный коэффициент, позволяющий отслеживать протекание поединка на экране дисплея (например, путем умножения этого отношения на 1000 миллисекунд или другое более удобное значение).

## Что сделано в ИДЗ №4?

### Оценка 4-5:

- Реализовано клиент–серверное приложение, в котором сервер (или серверы) и клиенты независимо друг от друга отображают только ту информацию, которая поступает им во время обмена.
- Отдельный отчёт можно посмотреть [тут](4-5/)

### Оценка 6-7:

- Реализовано клиент–серверное приложение, в котором есть дополнительный клиент, предназначенный для отображение комплексной информации о выполнении приложения в целом.
- Отдельный отчёт можно посмотреть [тут](6-7/)

### Оценка 8:

- Реализовано клиент–серверное приложение, которое отображает всю комплексную информации о выполнении приложения и допускает возможность подключения множекства клиентов.
- Отдельный отчёт можно посмотреть [тут](8/)

