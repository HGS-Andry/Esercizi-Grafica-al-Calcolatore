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
Le librerie si possono includere con path diretti:
```
#include "../../librerie\include\GL/glew.h"
#include "../../librerie\include\GLFW/glfw3.h"
```

oppure tramite:
```
#include <GL/glew.h>
#include <GLFW/glfw3.h>
```

specificando nelle proprietà del progetto -> c/c++ -> general -> Additional Include Direcories il path della cartella include
