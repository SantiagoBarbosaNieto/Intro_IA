Introducci�n a la Inteligencia Artificial - Proyecto 2
Motor de inferencia

Integrantes: (Grupo 4)
Fabio Alejandro Camrgo Diaz
Santiago Andres Diaz Diaz
Ivan Rene Ramirez Castro
Santiago Barbosa Nieto

El archivo main.cpp contiene todo el c�digo fuente que permite correr la soluci�n planteada.

La soluci�n planteada tiene un men� principal con 4 diferentes opciones:
	0. Salir -> termina la ejecuci�n de la soluci�n
	1. Agregar regla -> empieza el proceso para agregar una nueva expresi�n compuesta de reglas
	2. Listar reglas -> crea una lista de todas las expresiones con todas las reglas cargadas
	3. Validar condicion -> empieza el proceso para validar una regla con el sistema cargado

El archivo entrada.txt contiene el proceso de inputs necesarios para 2 cosas:
	1. Cargar al sistema el ejemplo de prueba dado 
	2. Validar la regla �Criminal(West) con este sistema cargado
Para esto se debe correr el programa y copiar todo el contenido del archivo entrada.txt como input.

Tambi�n es posible cargar un sistema manualmente.


Al validar una regla el sistema no la niega autom�ticamente para realizar el proceso de inferencia por refutaci�n. (Toca ingresar 's' cuando el sistema pregunte si estar� negada)
	Ej: Si se quiere validar la regla Criminal(West) se debe ingresar como �Criminal(West) 

