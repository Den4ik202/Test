
#include <OLED_I2C.h>                                              //  подключаем библиотеку OLED
OLED  myOLED(SDA, SCL, 8);                                         // вызываем OLED как myOLED
extern uint8_t SmallFont[];                                        // выдедяем память для отрисовки на дисплей
int visota = 56 - 1;                                               // высота дисплея
int shirina = 123 - 1;                                             // ширина дисплея

class Player {                                                     // создаем класс Игрок
  public:
    int _pin_but0;                                                 // первая кнопка
    int _pin_but1;                                                 // вторая кнопка
    int _pin_but2;                                                 // третья кнопка
    int _pin_but3;                                                 // четвертая кнопка
    Player (int pin_but0,                                          // конструктор для подключения кнопок
            int pin_but1,
            int pin_but2,
            int pin_but3) {
      _pin_but0 = pin_but0;
      _pin_but1 = pin_but1;
      _pin_but2 = pin_but2;
      _pin_but3 = pin_but3;
    }
    int azimut = 2;                                                // направление змейки
    void set_butt() {                                              // функция для получение состояниях функции
      pinMode(_pin_but0, INPUT_PULLUP);
      pinMode(_pin_but1, INPUT_PULLUP);
      pinMode(_pin_but2, INPUT_PULLUP);
      pinMode(_pin_but3, INPUT_PULLUP);
      if (!digitalRead(_pin_but0) && azimut != 2) azimut = 0;      // если первая кнопка нажата то изменяем направление головы
      if (!digitalRead(_pin_but1) && azimut != 3) azimut = 1;      // если первая кнопка нажата то изменяем направление головы
      if (!digitalRead(_pin_but2) && azimut != 0) azimut = 2;      // если первая кнопка нажата то изменяем направление головы
      if (!digitalRead(_pin_but3) && azimut != 1) azimut = 3;      // если первая кнопка нажата то изменяем направление головы
    }
}; Player player(8, 9, 10, 11);                                    // вызываем Игрок как игрок(пин первой кнопки, пин второй кнопки, пин третьей кнопки, пин четвертой кнопки)

struct Hvostic {                                                   // создаем переменную Хвостик
  short x;                                                         // координаты по x
  short y;                                                         // координаты по y
  short id;                                                        // номер змейки
};

class Hvost {                                                      // создаем класс Хвост
  public:
    Hvost () {                                                     // конструктор для Хвоста (нужен для корректной работы)

    }
    const int dlinna = 100;                                        // длинна хвоста
    Hvostic hvostic[100];                                          // массив для хранения кординат хвоста
    int score = 0;                                                 // количество хвостов в игре
    bool key = false;                                              // съели яблоко?

    void rost() {                                                  // если мы съели яблоко
      key = true;                                                  // показываем что мы съели яблоко
    }

    void update_coord(int coord_x1, int coord_y1) {                // функция для движение хвоста
      if (key) {                                                   // если съели яблоко
        score ++;                                                  // увеличиваем переменную которая отвечает за размер змей в игре
        key = false;                                               // мы не съели яблоко
      }
      for (int i = score; i >= 0; i--) {                           // передача кординат
        if (i == 0) {                                              // если мы на первом хвосте
          hvostic[i].x = coord_x1;                                 // получаем кординату головы по x
          hvostic[i].y = coord_y1;                                 // получаем кординату головы по y
          hvostic[i].id = i;                                       // номер хвоста
        }
        else {
          hvostic[i].x = hvostic[i - 1].x;                         // передаем кординату по x
          hvostic[i].y = hvostic[i - 1].y;                         // передаем кординату по y
          hvostic[i].id = i;                                       // номер хвоста
        }
      }
    }

    void draw() {                                                  // функция для отрисовки хвоста
      for (int g = 0; g < dlinna; g++) {                           // пробигаемся по хвосту
        myOLED.drawCircle(hvostic[g].x, hvostic[g].y, 1);          // рисуем круг с диаметром 1 пиксель
      }
      myOLED.update();                                             // обновляем дисплей
    }

}; Hvost hvost;                                                    // вызываем Хвост как хвост


class Apple {                                                      // создаем класс Яблоко
  public:
    Apple () {                                                     // конструктор для Яблока (нужен для корректной работы)

    }
    int cordinata_x = random(0, shirina);                          // кординате Яблока по x присваиваем рандомное значение от 0 до ширины
    int cordinata_y = random(0, visota);                           // кординате Яблока по y присваиваем рандомное значение от 0 до высоты

    void genetate() {                                              // функция для обновление кординат Яблока
      cordinata_x = random(0, shirina);                            // кординате Яблока по x присваиваем рандомное значение от 0 до ширины
      cordinata_y = random(0, visota);                             // кординате Яблока по y присваиваем рандомное значение от 0 до высоты
    }

    void draw() {                                                  // функция для отрисовки Яблока
      myOLED.drawCircle(cordinata_x, cordinata_y, 2);              // рисуем Яблоко с диаметром 2
    }

}; Apple apple;                                                    // Вызываме Яблоко как яблоко


class Head {                                                       // создаем класс Голова
  public:
    Head() {                                                       // конструктор для Головы (нужен для корректной работы)

    }
    int coord_x = shirina / 2;                                     // коорднаты для змейки по координте x
    int coord_y = visota / 2;                                      // коорднаты для змейки по координте y
    bool Deatch = true;                                            // если мы проиграли то true
    bool Win = true;                                               // если мы выиграли то true

    void set_coord(int teleport) {                                             // функция для передвижения головы
      switch (player.azimut) {                                     // получаем направление головы
        case 1:                                                    // если направление головы идет вверх                                               // кординату головы по y уменьшаем на 1
          if (teleport) {                                          // если телепорты истина
            if (0 < coord_y) coord_y--;                            // кординату головы по x уменьшвем на 1
            else           coord_x = coord_x + visota;             // переносим голову на противоположную сторону
          }
          else {
            coord_y--;                                             // кординату головы по x уменьшвем на 1
          }
          break;

        case 3:                                                    // если направление головы идет вниз
          if (teleport) {                                          // если телепорты истина
            if (coord_y < visota) coord_y++;                       // кординату головы по x уменьшвем на 1
            else           coord_y = coord_y - visota;             // переносим голову на противоположную сторону
          }
          else {
            coord_y++;                                             // кординату головы по x уменьшвем на 1
          }
          break;

        case 0:                                                    // если напрваление головы идет влево
          if (teleport) {                                          // если телепорты истина
            if (0 < coord_x) coord_x--;                            // кординату головы по x уменьшвем на 1
            else           coord_x = coord_x + shirina;            // переносим голову на противоположную сторону
          }
          else {
            coord_x--;                                             // кординату головы по x уменьшвем на 1
          }
          break;

        case 2:                                                    // если напрваление головы идет вправо                                               // кординату головы по x увеличиваем на 1
          if (teleport) {                                          // если телепорты истина
            if (coord_x < shirina) coord_x++;                      // кординату головы по x уменьшвем на 1
            else           coord_x = coord_x - shirina;            // переносим голову на противоположную сторону
          }
          else {
            coord_x++;                                              // кординату головы по x уменьшвем на 1
          }
          break;
      }
    }

    void chek_deathWin () {                                        // функция для проверки на проигрыш или победу
      for (int f = 1; f < hvost.score; f++) {                      // пробигаемся по масиву до количества хвоста
        if (coord_x == hvost.hvostic[f].x && coord_y == hvost.hvostic[f].y) {     // если мы сталкнулись с хвостом то мы проиграли
          Deatch = true;                                           // изменяем проигрыш на true
        }
      }
    }

    void eat () {                                                  // функция для проверки съели ли мы яблоко
      if (coord_x == apple.cordinata_x && coord_y == apple.cordinata_y) {    // если кординаты Головы и яблока совпадают то
        hvost.rost();                                              // вызываем функцию рост которая находиться в хвосте
        apple.genetate();                                          // вызываем функцию генерейт которая находиться в яблоке
      }
    }

    void draw() {                                                  // функция для отрисовки Головы
      myOLED.drawCircle(coord_x, coord_y, 1);                      // рисуем голову с диаметром 1
    }

}; Head head;                                                      // вызываем Голову как голову

class Wall {                                                       // создаем класс Стена
  public:
    Wall () {                                                      // конструктор для Стены (нужен для корректной работы)

    }
    void perimetr() {                                              // функция для проверки столкнулись ли мы со стеной
      if (head.coord_y < 0 || head.coord_y > visota || head.coord_x < 0 || head.coord_x > shirina) {        // если мы врезолись в стену то мы проирали
        head.Deatch = true;                                        // говорим что мы проиграли
      }
    }

    void draw() {                                                  // функция для отрисовки стены
      myOLED.clrScr();                                             // очищаем дисплей
      myOLED.drawRect(shirina + 1, 0, 0, visota + 1);              // рисуем прямаугольник по краю дисплея
    }
}; Wall wall;                                                      // вызываем Стену как стену

class Window {
  public:
    Window() {                                                     // конструктор для Окна (нужен для корректной работы)

    }

    bool teleport = false;                                         // если мы хотим чтоб при врезании в стену мы проигрывали то flase

    void Menu() {                                                  // функция которая ввыводит начальное меню
      myOLED.clrScr();
      myOLED.print("Snake Game", 38, 5);
      myOLED.print("press any button", 0, 30);
      myOLED.print("by: Den4ik202", 50, 55);
      myOLED.update();
    }

    void Win() {                                                   // функция которая ввыводит экран победы
      myOLED.clrScr();
      myOLED.print("You Win", 38, 5);
      myOLED.print("press any button", 0, 30);
      myOLED.update();
    }

    void Death() {                                                 // функция которая ввыводит экран поражения
      myOLED.clrScr();
      myOLED.print("You Lose", 38, 5);
      myOLED.print("press any button", 0, 30);
      myOLED.update();
    }
}; Window window;                                                 // вызываем Окно как окно

void setup() {
  myOLED.begin();                                                  // подключаем дисплей
  myOLED.setFont(SmallFont);
}

void loop() {
  if (head.Deatch == true && head.Win == true) {
    window.Menu();
    if (!digitalRead(player._pin_but0)) {
      window.teleport = true;
      head.Deatch = false;
      head.Win = false;
    }
    else if (!digitalRead(player._pin_but1)) {
      window.teleport = false;
      head.Deatch = false;
      head.Win = false;
    }
  }

  else if (!head.Deatch && !head.Win) {                            // если мы не проиграли или не выиграли то играем дальше
    wall.draw();                                                   // рисуем стену
    apple.draw();                                                  // рисуем яблоко
    head.draw();                                                   // рисуем голову
    head.eat();                                                    // съели ли мы яблоко?
    hvost.update_coord(head.coord_x, head.coord_y);                // передаем кординаты хвоста
    player.set_butt();                                             // считаем кнопки
    head.set_coord(window.teleport);                                              // даем кординаты голове
    hvost.draw();                                                  // рисуем хвост
    head.chek_deathWin();                                          // столкнулись ли мы с хвостом?
    wall.perimetr();                                               // столкнулись ли мы со стеной?
    delay(100);                                                    // ждем 100 милисикунд
  }

  else if (head.Deatch && !head.Win) {
    window.Death();
    if (!digitalRead(player._pin_but0) || !digitalRead(player._pin_but1) || !digitalRead(player._pin_but2) || !digitalRead(player._pin_but3)) {
      head.Deatch = true;
      head.Win = true;
      for (int s = 0; s < hvost.score; s++) {
        hvost.hvostic[s] = {0, 0, s};
      }
      hvost.score = 0;
      head.coord_x = shirina / 2;
      head.coord_y = visota / 2;
    }
  }

  else if (!head.Deatch && head.Win) {
    window.Win();
    if (!digitalRead(player._pin_but0) || !digitalRead(player._pin_but1) || !digitalRead(player._pin_but2) || !digitalRead(player._pin_but3)) {
      head.Deatch = true;
      head.Win = true;
      hvost.score = 0;
      head.coord_x = shirina / 2;
      head.coord_y = visota / 2;
    }
  }
}
