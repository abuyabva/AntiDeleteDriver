readme_content = """# AntiDeleteDriver

Проект драйвера-минифильтра файловой системы для ОС Windows, предназначенный для защиты файлов от удаления.

## Описание
Драйвер перехватывает операции файловой системы и блокирует попытки удаления файлов, возвращая статус `STATUS_ACCESS_DENIED`.

## Установка на виртуальную машину (для тестирования)

Для установки и тестирования драйвера в тестовой среде выполните следующие шаги:

### 1. Подготовка виртуальной машины
Перед установкой необходимо разрешить системе загружать самоподписанные драйверы:
1. Откройте командную строку (CMD) от имени **Администратора**.
2. Выполните команду:
   ```cmd
   bcdedit /set testsigning on
   ```

1. Обязательно перезагрузите виртуальную машину.

2. Установка драйвера (Ручной метод)
Поскольку для отладочных версий не используется цифровая подпись Microsoft, установка производится через ручную регистрацию службы:

Разместите файлы: Скопируйте файлы AntiDeleteDriver.sys и AntiDeleteDriver.inf в папку на виртуальной машине (например, C:\AntiDeleteDriver).

Создание службы: Откройте командную строку от имени Администратора и выполните:
   ```cmd
   sc create AntiDeleteDriver type= kernel binPath= "C:\AntiDeleteDriver\AntiDeleteDriver.sys"
   ```

Настройка реестра (для Filter Manager):
Чтобы диспетчер фильтров распознал ваш драйвер, выполните следующие команды:

   ```cmd
   reg add "HKLM\SYSTEM\CurrentControlSet\Services\AntiDeleteDriver\Instances" /v DefaultInstance /t REG_SZ /d "AntiDeleteDefaultInstance" /f
   reg add "HKLM\SYSTEM\CurrentControlSet\Services\AntiDeleteDriver\Instances\AntiDeleteDefaultInstance" /v Altitude /t REG_SZ /d "141000" /f
   reg add "HKLM\SYSTEM\CurrentControlSet\Services\AntiDeleteDriver\Instances\AntiDeleteDefaultInstance" /v Flags /t REG_DWORD /d 0 /f
   ```
Загрузка драйвера:

   ```cmd
   fltmc load AntiDeleteDriver
   ```
3. Проверка работоспособности
Чтобы убедиться, что фильтр активен, выполните:

   ```cmd
   fltmc filters
   ```
В списке фильтров должен появиться AntiDeleteDriver. После этого любая попытка удаления файла в системе будет блокироваться.

Как выключить драйвер
Чтобы остановить работу драйвера без перезагрузки:

Отсоедините фильтр от томов (например, C:):

   ```cmd
   fltmc detach AntiDeleteDriver C:
   ```
Выгрузите драйвер:

   ```cmd
   fltmc unload AntiDeleteDriver
   ```
