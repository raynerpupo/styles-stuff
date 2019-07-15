## styles-stuff

Set of Qt/C++ styles.

### Usage

This is a style plugin, to install it:

1- Choose the color schema by editing the Material.pro file and setting the DEFINES to one of the following
   (xilema, xabal, xauce, xedro or xavia).

2- Compile and install.

```
#qmake-qt5 Material.pro
#make
#make install
```

3- You can use the style either by:
- From the command line by using the `-style` paramenter 
- From code: `qApp->setStyle(QStyleFactory::create("Material");`

* I called this **Material** given my lack of creativity, it's not related by any means with Google's :)
