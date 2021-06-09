# :books: Realizado por: 
  - :computer: [Pedro Miguel Carmona Broncano](https://github.com/Pmcb04)
  - :date: Curso 2019/2020
  - :office: [Universidad de Extremadura](https://www.unex.es)

## :computer: PROGRAMAS UTILIZADOS
   - [gcc](https://gcc.gnu.org/)
   - [Qt](https://www.qt.io/)
   - [Qt Creator](https://www.qt.io/product/development-tools)

## :green_book: Práctica: Detección de matrícula

El principal objetivo de esta práctica es completar una aplicación, escrita en C++ sobre Linux, a través de la implementación en ensamblador de las diferentes operaciones que debe proporcionar el programa. La integración de los dos lenguaes se leevará a cabo mediante las facilidades del ensamblado en línea proporcionadas por el compilador _gcc_

A continuación, se detallan diferentes aspectos a tener en cuenta para el desarrollo de esta práctica. 

### Descripción de la aplicación

El código ddque deberá ser completado estará incluido en una aplicación C++, disponible como proyecto de Qt. La siguiente figura muestra una captura de pantalla en un instante de ejecución de la aplicación.

![Figura 1](https://github.com/Pmcb04/Lectura-de-Matriculas/tree/master/images/fig1.png)

#### ¿Qué es Qt?
Qt es un _framework_ de desarollo de aplicaciones que, entre otras cosas aportaciones, proporciona herramientas y librerías de clases para la creación de interfaces de usuario en entornos de escritorio. 

#### Puesta en marcha del proyecto
El fichero que contiene la descripción del proyecto (_pracaoc.pro_) se encuentra disponible en la carpeta principal de la aplicación. Dicho fichero puede ser utilizadoo para importar el proyecto desde diferentes entornos de desarrollo. No obstante, se recomienda trabajar con _QtCreator_ (paquete _qtcreator_). Es necesario instalar además los paquetes _qt5-default y qttools5-dev-tools_.

#### Funcionamiente de la aplicación
La interfaz principal (figura 1) está compuesta por varias de imagen y una serie de botones a través de los cuales es posible ejecutar las diferentes operaciones que proporciona la apliación. Las dos ventanas de imagen situadas en la parte superior se corresponden con la imagen de la matrícula original (ventana de la izquierda) y con la imagen una vez filtrada (ventana de la derecha). 

La primera es usada como punto de partida del proceso de detección, mientras que la segunda muestra el resultado tras la aplicación de distintas fases del proceso. Estas imágenes tienen inicialmente un tamaño de 320x100 pixels y 255 niveles de gris originalmente aunque en fases posteriores se desechan todos los niveles excepto del 0 (negro :black_circle: ) y 255 (blanco :white_circle: ).  

El conjunto de 7 imágenes situadas debajo de la matrícula filtrada tienen un tamaño de 32x55 píxeles en blanco :white_circle: (255) y negro :black_circle: (0) y son usadas para la última fase del proceso de detección. Se describen a continuación las opciones incluidas en la aplicación:

- **Cargar:** carga, en la imagen origen, la imagen del fichero indicado. La imagen debe tener un tamaño mínimo de 320x100. Aunque pueden cargarse imágenes mayores, sólo se visualizarán un recorte de 320x100 píxeles como máximo. Es posible cargar imágenes en color, en tal caso esta opción se encarga de transformar la imagen a escala de grises.  

  '>>>: copia la imagen origen en el contenedor destino. 
  '<<<: copia una imagen procesada en el contenedor origen. 

- **Borrar** borra el contenido de las imágenes destino poniendo a 0 (negro :black_circle: ) cada uno de sus píxeles. Además inicializa ciertos datos, fruto de procesos anteriores.

- **RECONOCER** realiza todos los pasos para la detección de matrículas de forma automática. Empieza con la inversión de la imagen y termina con el proceso de _Matching_.

- **Salir** cierra la aplicación. 

- **Invertir** el nivel de gris de cada píxel dee la imagen origen es invertido (255 - nivel de gris) y almacendao en la imagen destino. 

![Figura 2](https://github.com/Pmcb04/Lectura-de-Matriculas/tree/master/images/fig2.png)

- **Umbralizar** trasforma la imagen en escala de grises en una imagen en blanco y negro para lo cual aplica a cada punto de la imagen origen la comparación con el nivel de gris medio (128). Los píxeles con valores superiores pasan a valer 255  (blanco :white_circle:) mientras que el resto serán 0 (negro :black_circle:).

![Figura 3](https://github.com/Pmcb04/Lectura-de-Matriculas/tree/master/images/fig3.png)

- **Eliminar ruido** elimina el posible ruido en la imagen utilizando dos fases de procesamiento, una vez que la imagen ha sido invertida y umbralizada. En la primera fase, por cada píxel, comprueba si todos los puntos de su entorno (3x3) son blancos. En tal cosa pone a 255 el píxel en el destino y, en caso contrario, lo pone a 0. En la segunda dase, vuelve a analizar el entorno de cada píxel de la iamgen generada por la fase anterior. Si existe algún píxel blanco en su entorno, pone a 255 el píxel en la imagen destino y, en caso contrario, lo pone a 0.

![Figura 4](https://github.com/Pmcb04/Lectura-de-Matriculas/tree/master/images/fig4.png)

- **Margen vertical** partiendo de la imagen derecha, una vez aplicadas las operaciones anteriores, determina los limites en fila superior e inferior a partir de las cuales hay caracteres alfanuméricos. Para realizar esta operación, se aplica la integral proyectica a cada fila de la iamgen (suma de píxeles blancos de la fila). Busca las dos primeras filas desde arriba y desde abajo que cumplan que la suma de sus puntos sea mayor que un valor umbral determinado en la aplicación.  

  Una vez determinadas las filas superior e inferior, la aplicación las marca en la imagen de proceso.

![Figura 5](https://github.com/Pmcb04/Lectura-de-Matriculas/tree/master/images/fig5.png)

- **Margen horizontal** al igual que la operación anterior, realiza la integral proyectiva de cada columna de la imagen procesada de tal forma que determina en que bandas de columna existen dígitos y en cuales no. Lo hace en dos fases, primero calcula que columnas contienen puntos blancos por encima del umbral fijado para posteriormente fijar el punto medio de cada zona. De esta forma se obtiene la columna central aproximada de cada dígito en la imagen.  

  El proceso termina marcando en la imagen filtrada los limites aproximados entre los que se encuentra cada uno de los caracteres alganuméricos (columna_central - 16, columna_central + 16)

![Figura 6](https://github.com/Pmcb04/Lectura-de-Matriculas/tree/master/images/fig6.png)

- **Recortar** con los dos procesos anteriores se puede obtener la ventana de imagen donde aparece cada uno de los caracteres de una matrícula. A partir de las posiciones de estas ventanas, teniendo en cuenta que cada carácter ocupa aproximadamente 32x55 píxeles, este proceso se encarga de recortar de la imagen procesada cada una de las imágenes individuales de caracteres y de copiarlas a las ventanas de imagen correspondientes. 

![Figura 7](https://github.com/Pmcb04/Lectura-de-Matriculas/tree/master/images/fig7.png)

- **Matching** este es el ultimo proceso y el más complejo. Finaliza cuando se obtiene una cadena de caracteres que identifican a los de la imagen.  

  Coteja cada una de las imágenes obtenidas del proceso anterior con las almacenadas en la aplicación (0..9, B..Z). En concreto son 31 imágenes de tamaño 32x55 en blanco :white_circle: y en negro :black_circle: que contienen cada uno de los caracteres necesarios para formar matrículas de coche. Teniendo en cuenta que los 4 primeros dígitos deben ser numéricos y los 3 últimos alfabéticos, es posible minimizar el número de comparaciones y aumentar la posibilidad de acierto.  

  El proceso se realiza para cada una de las imágenes obtenidas del proceso anterior (_recortar_) y consiste en comparar cada imagen con un conjunto posible. La comparación con cada imagen del conjunto da como resultado un valor que representa la similitud entre las dos imagenes. El carácter asociado con la imagen del conjunto para la que el valor de similitud sea mayor, se considera el carácter finalmente identificado. 

![Figura 8](https://github.com/Pmcb04/Lectura-de-Matriculas/tree/master/images/fig8.png)

#### Componentes de la aplicación

El código fuente de la aplicación está formado por 5 ficheros: 

- _main.cpp_
- _pracaoc.h_
- _pracaoc.cpp_
- _imagesprocess.h_
- _imagesprocess.cpp_

Además, existe un formulario de Qt _mainForm.ui_ y otros ficheros necesarios para la gestión del proyecto. El contido de cada fichero fuente es el siguiente: 

- _main.cpp_ contiene el procedimiento principal que permite lanzar la aplicación como crear y mostrar la ventana principal que actúa como interfaz entre el usuario y la aplicación. 

- _pracaoc.h_ fichero que contiene la definición de la clase principal de la aplicación (_pracAOC_). Esta clase contiene los elementos principales de gestión de la aplicación. Estre los atributos, se encuentran las definiciones de las interfaces de usuario incluidas en el programa, así como de las variables que permiten almacenar la información de las imágenes origen y procesada utilizadas en las opciones de porcesamiento de las aplicación.

- _pracaoc.cpp_ incluye la implementación de los métodos de la clase pracAOC. En su mayoría, estos métodos se encargan de responder a los distintos eventos de la interfaz de usuario incluida en el programa y de llamar a las funciones de procesamiento de imagen que correspondan en cada paso (disponibles en _imagesprocess.cpp_ e _imagesprocess.h_)

- _imagesprocess.h_ contiene la definición de las funciones implementadas en el fichero _imagesprocess.cpp_

- _imagesprocess.cpp_ implementación de las funciones de procesamiento de imagen que se ejecutan a través de las distintas opciones de la aplicación. La mayoría de estas funciones contienen una implementacion vacía. el objetivo de esta práctica es completarlas para que el funcionamiento de la aplicación sea el descrito anteriormente. 

#### Extensiones principales en x86-64

- En relación a la representación de datos en memoria, en 64 bits, los punteros y los datos de tipo long ocupan 64 bits. El resto de tipos mantiene el mismo tamaño que en la línea de procesadores de 32 bits (int: 4 bytes, short: 2 bytes, …).  

- Todas las instrucciones de 32 bits pueden utilizarse ahora con operandos de 64 bits. En ensamblador, el sufijo de instrucción para operandos de 64 bits es “q”.

- Los registros de 32 bits se extienden a 64. Para hacer referencia a el los desde un programa en lenguaje ensamblador, hay que sustituir la letra inicial “e” por “r” (%rax, %rbx, …). Los registros de 32 bits de la IA32 se corresponden con los 32 bits de menor peso de estos nuevos registros.

- El byte bajo de los registros %rsi, %rdi, %rsp y %rbp es accesible. Desde el lenguaje ensamblador, el acceso a estos registros se realiza a través del nombre del registro de 16 bits finalizado con la letra “l” (%sil, %dil, …).

- Aparecen 8 nuevos registros de propósito general de 64 bits (%r8, %r9, …, %r15). Es posible acceder a los 4, 2 o al último byte de estos registros incluyendo en su nombre el sufijo d, w o b (%r8d – 4 bytes, %r8w – 2 bytes, %r8b – 1 byte).

### Especificación de los objetivos de la práctica

El principal objetivo de esta práctica es completar el código de la aplicación descrita para que su funcionamiento sea el que se detalla en la sección _“Funcionamiento de la aplicación”_, incluida es esta documentación. Para ello, se deberán implementar, en lenguaje ensamblador, los procedimientos “vacíos” del módulo _“imageprocess.cpp”_. Estos procedimientos son invocados por las distintas opciones del programa. Para cada uno de ellos, se proporciona la estructura inicial del bloque ensamblador, en la que se ha incluido la definición de operandos que afectan a la implementación. La lista de registros utilizados incluye únicamente la palabra _“memory”_, ya que en todos los casos la memoria es modificada. La inclusión de registros dentro de esta lista dependerá de la implementación que se desarrolle en cada caso, por lo que, será necesario completarla para cada uno de los procedimientos.

Se describe a continuación la funcionalidad de cada uno de los  procedimientos a completar.

Para todos ellos, los parámetros _imgO_ e _imgD_ contienen, respectivamente, la dirección de los bloques de memoria que almacenan el contenido de las imágenes origen y destino. Para una descripción más detallada de estas funciones, se recomienda revisar la sección _“Funcionamiento de la aplicación”_

~~~ C++
-  void imageprocess::borrar(uchar * imgD, int w, int h)
~~~

Borra todos los píxeles de _imgD_, asignándoles un valor 0 (negro :black_circle:) a cada uno de ellos. El tamaño de la matriz a procesar viene determinada por _w_ (ancho de imagen) y _h_ (alto de imagen).


~~~ C++
- void imageprocess::invertir(uchar * imgO, uchar * imgD, int w, int h)
~~~

Invierte el nivel de gris de cada píxel de _imgO_ y devuelve el resultado en _imgD_. Para ello asigna a cada píxel de la imagen destino 255 menos el valor del píxel correspondiente de la imagen origen. Al igual que en el procedimiento anterior, _w_ y _h_ determinan las dimensiones de las imágenes.

~~~ C++
- void imageprocess::umbralizar(uchar * imgD, int w, int h)
~~~

Transforma los píxeles de _imgD_ poniendo a 255 aquellos que tengan valor superior a 128 y a 0 los restantes.

~~~ C++
- void imageprocess::eliminarRuido_F1(uchar * imgO, uchar * imgD, int w, int h)
~~~

Realiza la primera fase de la opción _“Eliminar ruido”_. Para ello, por cada píxel de _imgO_ comprueba si todos los píxeles de su entorno de 3x3 tienen valor 255. En tal caso asigna el valor 255 en el píxel correspondiente de _imgD_ y, en caso contrario, el valor 0.

~~~ C++
- void imageprocess::eliminarRuido_F2(uchar * imgO, uchar * imgD, int w, int h)
~~~

Partiendo del resultado del procedimiento anterior, realiza la segunda fase de la opción _“Eliminar ruido”_. Concretamente, por cada píxel de _imgO_ comprueba si algún píxel de su entorno de 3x3 tienen valor 255. En tal caso asigna el valor 255 en el píxel correspondiente de _imgD_ y, en caso contrario, el valor 0.

~~~ C++
- int imageprocess::detectarV_min(uchar *imgD, int U)
~~~

Busca la fila inicial de la imagen a partir de la cual se sitúan los caracteres de la matrícula. Para ello, comenzando por la fila 0, cuenta el número de píxeles de la fila con valor igual a 255. Si el número de píxeles es mayor que U retorna la posición de la fila actual. En caso contrario, repite el proceso para la siguiente fila.

~~~ C++
- int imageprocess::detectarV_max(uchar *imgD, int U)
~~~

Busca la última fila de imagen donde están situados los caracteres de la matrícula. Realiza el mismo proceso que el procedimiento anterior, pero comenzando por la última fila de imagen (99). Cuando el número de píxeles blancos :white_circle: de la fila actual es superior al valor indicado en _U_, retorna dicha posición de fila. En caso contrario, repite el proceso
para la fila anterior.

~~~ C++
- void imageprocess::detectarH_f1(uchar *imgD, uchar *VA, uchar U)
~~~

Este procedimiento realiza la primera fase del procesamiento de la opción _“Margen horizontal”_. En concreto, por cada columna de _imgD_, cuenta el número de píxeles blancos :white_circle: y, si dicho número es superior al indicado en el parámetro _U_, almacena un 1 en la posición correspondiente de _VA_. En caso contrario, pondrá a 0 el elemento que corresponda de _VA_. Hay que considerar que la variable _VA_ representa un array con tantos elementos como columnas tiene _imgD_.

~~~ C++
- void imageprocess::detectarH_f2(uchar *VA, int *Vh)
~~~

Lleva a cabo la segunda fase del procesamiento de la opción _“Margen horizontal”_. Parte del resultado obtenido por el procedimiento anterior, almacenado en _VA_. Se encarga de recorrer dicho array y localizar el primer índice cuyo elemento tiene valor 1 (_i_). A partir de esa posición, se cuenta el número de 1’s consecutivos (_t_) y se calcula la posicion central de la zona detectada como (_i + t/2_). Dicha posición se almacena en _Vh_ y se continúa con el recorrido de VA hasta localizar el siguiente 1, en cuyo caso se repite el proceso anterior, o hasta que se  haya alcanzado el final del vector (_320 elementos_).

~~~ C++
- void imageprocess::recortar(uchar *imgO, uchar *imgD,int x, int y, int w, int h)
~~~

Copia una ventana de imagen de _imgO_ a _imgD_. El tamaño de la ventana está determinado por _w_ (ancho) y _h_ (alto). La esquina superior izquierda de la ventana viene indicada por _x_ (columna) e _y_ (fila).
Hay que considerar que el tamaño de _imgO_ en cualquier caso es _320x100_.

~~~ C++
- int imageprocess::matching(uchar *caracM, uchar *patrones[31], int ini, int tam)
~~~

Este procedimiento recibe por parámetro una imagen (_caracM_) de tamaño _32x55_, que contiene uno de los caracteres de matrícula obtenidos de la fase anterior, y un vector de imágenes (_patrones_) también de tamaño _32x55_. Cada imagen de este vector contiene la representación estándar (_patrón_) de uno de los posibles caracteres de una matrícula (_números y letras_). El procedimiento compara la imagen almacenada en caracM con las imágenes disponibles en el vector patrones y devuelve el índice del vector que contiene la imagen más similar. Para evitar comparar imágenes que contienen números con imágenes de letras y viceversa, el procedimiento recibe por parámetro el índice inicial del vector (_ini_) que contiene la primera imagen con la que se debe comparar y el número de patrones posibles (_tam_).

Para obtener un valor que cuantifique la similitud entre la imagen caracM y el patrón correspondiente, se llevará a cabo una comparación píxel a píxel de la siguiente forma: si los dos píxeles son iguales y tienen valor 255, la similitud se incrementa en 3; si ambos son iguales y su valor es 0, la similitud se incrementa en 1; si los dos píxeles son distintos, la similitud se decrementa en 1. Siguiendo este proceso, el patrón que obtenga el máximo valor de similitud, será el mejor candidato y el procedimiento retornará el índice de dicho patrón en el vector.
