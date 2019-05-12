# Универсальный таймер на микроконтроллере STM8

Разработан для использования совместно с [УФ лампой](https://github.com/vadzimyatskevich/uv_lamp)

Прошивка может работать с оригинальной платой, а также с термостатом W1209 доступном на Aliexpress и ebay. Тип используемой платы задаётся в файле [uv_timer.h](/software/uv_timer.h) см. строки 6-8.

Перечень элементов и gerber-файлы ПП находятся в каталоге /pcb. Для заказа ПП используйте файл [UVTimer_V1.02.zip](/pcb/UVTimer_V1.02.zip)

## Характеристики:

```
Uпит:..............7-30V
Uвых:..............Uпит
Iмакс:.............1A
Выдержка:..........0.1c-90мин
```

## Управление

Упраление таймером произвонится при помощи энкодера. Врещением ручки энкоера осуществляется выбор интервала времени, короткое нажатие - запуск/пауза, днинное нажатие - остановка/сброс на начальное значение.

## Схема

Элементы С1, С2, ZQ1 могут не устанавливаться при соответствущих изменениях в функции `CLK_Config()` в файле `main.c`. В качестве DD1 допустимо использовать STM8S003F3xx либо STM8S103F3xx. Перемычка J1 предназначена для отладки и должна быть замкнута. Самовосстанавливающийся предохранитель R3 предназначен для защиты выходого транзистора от перегрузки и короткого замыкания, для увеличения нагрузочной способности может быть удален.
![Scheme](/doc/img/sch1.jpg)
![Board](/doc/img/IMG_6590.JPG)

## W1209

На рынке присутствуют термостаты с различнами микрокотроллерами, прошивку можно использовать только с термостатом на основе STM8
