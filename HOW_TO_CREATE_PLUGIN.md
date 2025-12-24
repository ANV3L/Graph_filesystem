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

#include "include/filesystem_for_plugin.hpp" [ В этом файле вся совместимость с файловой системой, больше ничего подключать не нужно. ]

#include <string>

class {Name of your command} final : public RootCommand {
public:
    std::string execute(const Command& cmd, FileSystem& filesys) override;
};

extern "C" {
    bool get_plugin_data(PluginCommandInfo* out);
    void* create_plugin_command();
    void destroy_plugin_command(void* cmd);
}

```
 - Пропишите путь к filesystem_for_plugin.hpp и выберите имя класса команды, больше ничего делать не нужно

 ### plugin.cpp
 ```
#include "plugin.hpp"

std::string { Имя вашей команды }::execute(const Command& cmd, FileSystem& filesys) {

 [ Реализуете здесь логику работы команды ]

}



extern "C" bool get_plugin_data(PluginCommandInfo* out) {
    if (!out) return false;
    out->command_name = "{ Желаемое имя команды }";
    out->alias_name   = "{ Имя команды в карусели команд }";
    return true;
}

extern "C" void* create_plugin_command() {
    return static_cast<void*>(new { Имя класса вашего }());
}

extern "C" void destroy_plugin_command(void* cmd) {
    delete static_cast<{ Имя класса вашего }*>(cmd);
}
 ```

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
        OUTPUT_NAME "{ Имя вашего плагина }"
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
install {Путь до вашего плагина относительно папки build}
```

Готово! Теперь вы можете спокойно ей пользоваться в терминале или найти в карусели команд (Если в директории есть хотя бы один файл, то вы можете с помощью Enter открыть список файлов в директории и при повторном нажатии Enter вы выберите файл и откроется карусель команд)