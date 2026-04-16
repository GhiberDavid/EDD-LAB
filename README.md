# Sistema de Sucesión de la Mafia

##Descripción: Sistema en C++ que gestiona la línea de sucesión de una familia mafiosa utilizando un "árbol binario". Este programa permite cargar datos desde un archivo CSV, mostrar la línea de sucesión de toda la familia mafiosa, simular eventos (muerte, encarcelamiento, envejecimiento) y actualizar datos de los miembros.

##Estructura del proyecto:

mafia_tree_project/
├── bin/
│ ├── mafia_program # Ejecutable
│ └── data.csv # Datos de la familia
├── src/
│ ├── main.cpp # Menú principal
│ ├── tree.h # Declaración de la clase Tree
│ ├── tree.cpp # Implementación de la clase Tree
│ └── person.h # Estructura Person (nodo)
└── README.md #

## Requisitos

- Compilador C++ (g++ recomendado)
- Sistema operativo Linux/Unix (o WSL en Windows)

## Compilación

-Desde la raíz del proyecto:

En el bash:
g++ src/main.cpp src/tree.cpp -o bin/mafia_program

-Ejecución:

En el bash:

./bin/mafia_program

-Formato del archivo CSV

El archivo bin/data.csv debe tener el siguiente formato:

id,name,last_name,gender,age,id_boss,is_dead,in_jail,was_boss,is_boss
1,Vito,Corleone,H,65,-1,0,0,1,1
2,Michael,Corleone,H,40,1,0,0,0,0
3,Sonny,Corleone,H,38,2,0,0,0,0

Columnas:

    id: Identificador único
    name: Nombre
    last_name: Apellido
    gender: H (Hombre) o M (Mujer)
    age: Edad
    id_boss: ID del jefe directo (-1 para el jefe principal)
    is_dead: 0 (vivo) o 1 (muerto)
    in_jail: 0 (libre) o 1 (encarcelado)
    was_boss: 0 o 1 (si fue jefe anteriormente)
    is_boss: 0 o 1 (si es el jefe actual)

#Menú de opciones:

1: Mostrar línea de sucesión (solo miembros vivos)
2: Simular muerte del jefe actual
3: Simular encarcelamiento del jefe actual
4: Simular envejecimiento del jefe (+70 años)
5: Actualizar datos de un miembro
6; Salir del programa

##Reglas de sucesión implementadas

Muerte del jefe: Se busca el primer sucesor vivo y fuera de la cárcel en su árbol

Encarcelamiento del jefe: Misma regla que la muerte

Edad ≥ 70 años: El jefe es reemplazado automáticamente

Búsqueda en otro sucesor: Si no hay sucesor en el árbol del jefe, se busca en el hermano

Búsqueda ascendente: Se sube niveles de jefes hasta encontrar un sucesor

Jefe con dos sucesores: Se busca el jefe más cercano con al menos dos sucesores disponibles

Encarcelados como último recurso: Si no hay libres, se elige entre encarcelados vivos

##Ejemplo de ejecución

=== Sistema de sucesion de la mafia ===
Datos cargados correctamente desde bin/data.csv

Jefe actual: Vito Corleone (Edad: 65)

=== MENU PRINCIPAL ===
1. Mostrar linea de sucesion
2. Simular muerte del jefe
3. Simular encarcelamiento del jefe
4. Simular envejecimiento del jefe (+70 años)
5. Actualizar datos de un miembro
6. Salir
Opcion: 1

=== LINEA DE SUCESION (Miembros vivos) ===
ID: 3 | Nombre: Sonny Corleone | Edad: 38
ID: 2 | Nombre: Michael Corleone | Edad: 40
ID: 1 | Nombre: Vito Corleone | Edad: 65 | ★ JEFE ACTUAL ★

Ejemplo de Csv para pruebas más complejas: 

id,name,last_name,gender,age,id_boss,is_dead,in_jail,was_boss,is_boss
1,Vito,Corleone,H,65,-1,0,0,1,1
2,Michael,Corleone,H,40,1,0,1,0,0
3,Sonny,Corleone,H,38,2,0,1,0,0
4,Fredo,Corleone,H,45,2,0,0,0,0
5,Tom,Hagen,H,50,4,0,0,0,0
7,Joey,Zasa,H,30,5,1,0,0,0   
8,Anthony,Corleone,H,25,5,0,0,0,0
6,Connie,Corleone,M,35,4,0,0,0,0
9,Mary,Corleone,M,22,6,0,0,0,0
