# Esercizi Grafica al Calcolatore
###### Andrea Dall'Alba

#### Compilazione in linux:
Linux: indicare path per l'esecuzione dentro la cartella librerie
```
export LD_LIBRARY_PATH=lib/lin
```

Per gli esercizi sono presenti i MakeFile
```
make < _nome esercizio senza estensione_ >
```

### Compilazione in Visual Studio

#### Librerie in file interni al progetto
Le librerie si possono includere con path diretti:
```c++
#include "../../librerie\include\GL/glew.h"
#include "../../librerie\include\GLFW/glfw3.h"
```

oppure tramite:
```c++
#include <GL/glew.h>
#include <GLFW/glfw3.h>
```

specificando nelle proprietà del progetto -> c/c++ -> general -> Additional Include Direcories il path della cartella include



####Librerie nella cartella di visual studio
 
Nella cartella di installazione di visual studio es: `C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include\` inseriamo i file e le cartelle dentro la cartella `librerie\include\` del progetto, mentre in `C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib\` i file `.lib/.LIB` e i file `.obj` della cartella `librerie\lib\include\`
 
 
Il file glew32.dll può essere inserito nella cartella di windows in `C:\Windows\System32`
(In caso non funzioni si può copiare in `C:\Windows\SysWOW64`)
 
