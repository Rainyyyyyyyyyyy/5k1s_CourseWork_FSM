# FSM  

---  
### Сборка и запуск 
В папке FSM (на уровне ```main.cpp```)  
- ``` cmake --build build --config Release ```  
- ``` build\Release\FSM.exe ```  
  
  
# tree.dot  
Метод ``` printTreeFileDot(Node<T> *root, std::ofstream &out) ``` создаёт файл ```tree.dot``` в папку вместе с ```main.cpp```.  
  
Для отрисовки файла tree.dot необходимо выполнить:   
- ``` dot -Tpng tree.dot -o tree.png ```   