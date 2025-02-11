# BrickGame Тетрис
Резюме: в данном проекте  реализована игра «Тетрис» на языке программирования С с использованием структурного подхода.

## Особенности

- Для формализации логики данной игры можно представить следующий вариант конечного автомата:

![Конечный автомат](images/tetris.png)

- Данный КА состоит из следующих состояний:

  - Старт — состояние, в котором игра ждет, пока игрок нажмет кнопку готовности к игре.
  - Спавн — состояние, в которое переходит игра при создании очередного блока и выбора следующего блока для спавна.
  - Перемещение — основное игровое состояние с обработкой ввода от пользователя — поворот блоков/перемещение блоков по горизонтали.
  - Сдвиг — состояние, в которое переходит игра после истечения таймера. В нем текущий блок перемещается вниз на один уровень.
  - Соединение — состояние, в которое преходит игра после «соприкосновения» текущего блока с уже упавшими или с землей. Если образуются заполненные линии, то она уничтожается и остальные блоки смещаются вниз. Если блок остановился в самом верхнем ряду, то игра переходит в состояние «игра окончена».
  - Игра окончена — игра окончена.

- Игра написана на языке С стандарта C11 с использованием компилятора gcc.

- Программа должна состоять из двух частей: библиотеки, реализующей логику игры тетрис, и терминального интерфейса с использованием библиотеки ncurses

## Управление
  - Enter - Начало игры 
  - P - Пауз
  - Стрелка влево — движение фигуры влево
  - Стрелка вправо — движение фигуры вправо
  - Стрелка вниз — падение фигуры
  - Space - Поворот фигуры
