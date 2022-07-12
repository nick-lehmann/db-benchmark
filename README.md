# Datenbanken Benchmark

## Cmake 

### Build

Zum Initailisieren von CMake folgenden Befehl in diesem Ordner ausführen. An dieser stelle muss bereits angegeben werden, ob CMake im Release- (mit `-03`) oder Debug-Mode builden soll.
```
cmake -DCMAKE_BUILD_TYPE=Release .
```
Um im Debug-Mode zu compilieren muss `Release` im obenstehenden Befehl durch `Debug` ersetzt werden.

Builden aller Executables:
```
cmake --build .
```

Builden einer Executable:
```
cmake --build . --taget <executable-name>
```

Es stehen folgende `executable-name`s zur Verfügung: 
    * row_store
    * column_store
    * pax_store
    
### Source-Files hinzufügen

Um Source-Files für eine der drei Speicherlayouts hinzuzufügen muss in der CMakeLists.txt 
im Befehl zum Hinzufügen der Executable des Speicherlayouts der relative Pfade zu der Source-File hinzugefügt werden.

```
add_executable(<executable-name> main.cpp <files>)
```
