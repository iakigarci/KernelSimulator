# Proyecto de Sistemas Operativos

## Prerequisitos

Es necesario tener instalado el compilador de C y el paquete _make_.

## Instalación

En primer lugar, se compila el programa. Para ello, se utiliza el comando `make`. Si la ejecución ha sido la correcta, sepuede ejecutar el script.
Para ello:

```
./kernel frecuenciaClock frecuenciaTimer frecuenciaProcessGen
```

Un ejemplo de ejecución sería el siguiente. A recalcar que la frecuencia del clock tiene que ser un múltiplo de 10. 

```
./kernel 10 10 10
```
