
# В конце файла есть краткая инструкция

```
Посмотреть пример создания плагина можно в cFind
```

## Как работать с командой
### Принимаемые аргументы

- cmd - структура команды, имя команды вам не сильно пригодится, так что можно смотреть только на cmd.args
- cmd.args - вектор строк, где перечислены все переданные вашей команде аргументы (Пример "filename first second" = {"filename", "first", "second"})
- filesys - файловая система, через которую можно работать, есть две команды для упрощенного взаимодействия с ней

### filesys.getDataFromFile
```
const std::string& filename = Имя файла;

std::string data& = filesys.getDataFromFile(filename) - Получили ссылку на то, что записано в файле

filesys.getDataFromFile(filename) = "Text" - Записали что захотели в файл

filesys.getDataFromFile(filename) += "Text" - Приписали что захотели

И так далее


```
### filesys.createFile
```
filesys.createFile(filename) - с его помощью вы можете создать файл в текущей директории


```

# Файлы с вашим плагином

### plugin.hpp
``` 
#pragma once

#include "{Your path to}/include/filesystem_for_plugin.hpp"

#include <string>




// DEFINE AREA START ---------------------------

#define c_class_command_name _ // Напишите вместо "_" то, как вы бы хотели, чтобы назывался класс вашей команды

#define c_command_name "Your command_name" // Напишите вместо "Your command_name" как бы вы хотели, чтобы команду звали

#define c_alias_name "Your alias" // Напишите вместо "Your alias" как бы вы хотели, чтобы звали команду в карусели команд

// DEFINE AREA END -----------------------------




// Корреляция с файловой системой, тут ничего трогать не нужно

class class_command_name final : public RootCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

extern "C" {
    bool get_plugin_data(PluginCommandInfo* out);
    void* create_plugin_command();
    void destroy_plugin_command(void* cmd);
}

extern "C" bool get_plugin_data(PluginCommandInfo* out) {
    if (!out) return false;
    out->command_name = c_command_name;
    out->alias_name   = c_alias_name;
    return true;
}

extern "C" void* create_plugin_command() {
    return static_cast<void*>(new class_command_name());
}

extern "C" void destroy_plugin_command(void* cmd) {
    delete static_cast<class_command_name*>(cmd);
}

```
 - Пропишите путь к filesystem_for_plugin.hpp и заполните DEFINE_AREA

 ### plugin.cpp
 ```
#include "plugin.hpp"

std::string class_command_name::execute(const Command& cmd, FileSystem& filesys) {

    // Your command

}
 ```

 - Просто Напишите реализацию вашей команды
 - filesys - вся работа с системой
 - cmd.args - вектор строк, где перечислены все переданные вашей команде аргументы (Пример "filename first second" = {"filename", "first", "second"})
 

 ### CMakeLists.txt
 ```

cmake_minimum_required(VERSION 3.28)
project(PLUGIN)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PLUGIN_SRC
    plugin.cpp
)

add_library(plugin SHARED ${PLUGIN_SRC})

set_target_properties(plugin PROPERTIES
        OUTPUT_NAME "{ Имя вашего плагина }" # Здесь напишите желаемое имя для плагина, по итогу там соберется lib{имя}.so
        POSITION_INDEPENDENT_CODE ON
)

 ```

Теперь вы можете собрать свою динамическую библиотеку

```
mkdir build 
cd build
cmake ..
cmake --build . -j
```

теперь в build запустите
```
./main
```

### В программе: 
(Буквы английские)
 - "H" - чтобы посмотреть допустимые команды
 - "Enter" - открыть перечень файлов в директории (Узле/вершине графа)
 - "T" - открыть терминал

После открытия терминала выполните команду
```
install {Путь до вашего плагина относительно каталога, в котором запущена программа}
```

Готово! Теперь вы можете спокойно ей пользоваться в терминале или найти в карусели команд (Если в директории есть хотя бы один файл, то вы можете с помощью Enter открыть список файлов в директории и при повторном нажатии Enter вы выберите файл и откроется карусель команд)

# Чеклист, что вы всё сделали

- Прописали путь к include/filesystem_for_plugin.hpp
- Прописали в plugin.hpp DEFINE AREA 
- Добавили реализацию самой команды
- Прописали в СМаке имя библиотеки

# Шаги по разработке плагина, кратко

- Сделать всё в чеклисте (Команду делаете, чтобы получить ссылку на строку данных файла getDataFromFile(filename), чтобы создать новый createFile(filename), то что вы возвращаете через return - это то что выведется на экран, при пустой строке просто ничего не выведется)
- Создать в каталоге плагина build и там собрать динамическую библиотеку
- Создать build в корневом каталоге и в нём собрать сам проект
```
cd build
cmake ..
cmake --build . -j
```
- запустить
```
./main
``` 
- делаем через терминал (Нажать на кнопку T английскую) команду install {путь к вашей динамической библиотеки от текущего каталога (Скорее всего вы не уходили из build, поэтому оттуда)}. Должно быть сообщение installed {Имя вашего плагина}
- дальше создать файл через терминал (touch filename) или кнопку E (Английская)
- нажимаем на Enter, выбираем файл и ещё раз нажимаем на Enter - в карусели команд появится ваша команда (Имя у неё такое, какое вы алиасу дали)

```
Посмотреть пример создания плагина можно в pluginExample
```