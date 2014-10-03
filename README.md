styles-stuff
============

Este estilo se adapta a las identidades marcarias de la UCI, puede usarse en aplicaciones 
escritas en Qt/C++.

Uso
===
El proyecto es un plugin de estilo, para instalarlo:

1- Elegir la identidad a segir. Para esto se debe descomentar en el fichero Material.pro
   el DEFINES correspondiente a una de las identidades (xilema, xabal, xauce, xedro y xavia).

2- Compilar e instalar.
#qmake-qt5 Material.pro
#make
#make install
* El make install se debe ejecutar como sudo en dependencia de donde esté instalado el Qt

3- Una vez instalado el estilo puede usarse de varias formas:
- Desde la linea de comandos con el parámetro -style
- Desde el código:
qApp->setStyle(QStyleFactory::create("Material");

* Aunque el nombre del estilo sea Material no está relacionado con Google :)
