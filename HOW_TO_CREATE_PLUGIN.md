```
Посмотреть пример создания плагина можно в cFind
```

## Очень удобно работать с файлом
```
Вы принимаете аргументы (cmd.argc) и зачастую первый - это имя файла, то есть: 

const std::string& filename = cmd.argc[0];

Также вы принимаете аргумент filesys
В этом аргументе есть метод getDataFromFile
С ним вы можете делать что вам заблагорассудится

std::string data& = filesys.getDataFromFile() - Получили ссылку на то, что записано в файле

filesys.getDataFromFile() = "Text" - Записали что захотели в файл

filesys.getDataFromFile() += "Text" - Приписали что захотели

Ну и так далее
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

В программе:
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
- Прописали DEFINE AREA 
- Добавили реализацию самой команды
- Прописали в СМаке имя библиотеки