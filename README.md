# Datenbanken Benchmark

## Cmake 

### Build

Zum Initailisieren von CMake folgenden Befehl in diesme Ordner ausführen.
```
cmake .
```

Builden aller Executables:
```
cmake --build .
```

Builden einer Executable:
```
cmake --build . --config Debug --taget <executable-name>
```

Soll mit `-03` compiliert werden muss `Debug` entsprechend durch Release ersetzt werden.

Es stehen folgende Executable-names zur Verfügung: 
    * row_store
    * column_store
    * pax_store
    
### Source-Files hinzufügen

Um Source-Files für eine der drei Speicherlayouts hinzuzufügen muss in der CMakeLists.txt 
im Befehl zum Hinzufügen der Executable des Speicherlayouts der relative Pfade zu der Source-File hinzugefügt werden.

```
add_executable(<executable-name> main.cpp <files>)
```
