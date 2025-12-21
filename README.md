![Graph](screenshots/flower.jpg)

<h1 align="center">Graph Filesystem</h1>

<p align="center">
  <strong>Advanced C++ Visualization & MVC Engine</strong>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C++%2020-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" />
  <img src="https://img.shields.io/badge/Framework-Qt%205-41CD52?style=for-the-badge&logo=qt&logoColor=white" />
  <img src="https://img.shields.io/badge/Build-CMake-064F67?style=for-the-badge&logo=cmake&logoColor=white" />
</p>

<p align="center">
  <img src="https://img.shields.io/badge/IDE-CLion-0C4B33?style=flat-square&logo=jetbrains&logoColor=white" />
  <img src="https://img.shields.io/badge/Testing-GTest-607D8B?style=flat-square&logo=googletest&logoColor=white" />
</p>

---

<h3 align="center"><strong>Core Architecture</strong></h3>
<p align="center">
  <img src="https://img.shields.io/badge/Pattern-MVC-7B1FA2?style=flat-square&logo=architecture&logoColor=white" />
  <img src="https://img.shields.io/badge/Logic-PluginFactory-00796B?style=flat-square&logo=plugin&logoColor=white" />
  <img src="https://img.shields.io/badge/System-Multithreading-455A64?style=flat-square&logo=thread&logoColor=white" />
</p>

<h3 align="center">Algorithms & Data</strong></h3>
<p align="center">
  <img src="https://img.shields.io/badge/Structure-AVL%20Tree-AF601A?style=flat-square&logo=tree&logoColor=white" />
  <img src="https://img.shields.io/badge/Storage-Custom%20STL-1A237E?style=flat-square&logo=c&logoColor=white" />
</p>


## Compilation

```
cmake ..
cmake --build
```

## Executables

```
main
```


<h1 align="center">About program</h1>

## Демонстрация проекта

<div align="center">
<video width="900" height="500" controls poster="screenshots/gui.png">
  <source src="screenshots/FileSystem.mp4" type="video/mp4">
</video>
</div>

### Данный проект собой представляет другой вариант визуализации файловой системы - с помощью древовидного графа

![Graph](screenshots/first.jpg)

### В проекте реализована физика взаимодействия узлов

### В программе реализована "фабрика команд", благодаря чему была реализована динамическая загрузка библиотек

</strong>

 - Model/PluginHandler - реализация динамической загрузки команд
 - cFind - пример написания плагина
 - install {filepath} - командлет внутри системы, которым можно в runtime установить дополнительную команду

<strong>

## Была выбрана Архитектура MVC 

## В команде cFind была продемонстрирована работа с многопоточностью

## В каталоге STL реализован шаблонный класс table, являющийся аналогом std::map
</strong>


### реализован терминал (Нажать "T"), с помощью которого можно управлять файловой системой, полный список команд можно получить, нажав на "H"

### Файлы в каталогах-узлах можно получить, нажав "Enter", они представлены в виде кругового меню выбора, а после выбора файла будет круговой выбор команд над данным файлом
![Graph](screenshots/second.jpg)
