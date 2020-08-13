Taller de programación I
1° Cuatrimestre 2020


Integrantes
Agustin Hejeij 102324,
Francisco Jose Nasif 101044,
Gonzalo Fernandez 94667,
Renzo Jacinto 100627


Sistema Operativo
Ubuntu (Linux)


Libreria utilizada
SDL2: https://www.libsdl.org/index.php

Instalacion por linea de comandos:
Abrir una terminal y ejecutar los siguientes comandos:
	
	sudo apt-cache search libsdl2
	sudo apt-get install libsdl2-dev

Luego instalamos sus complementos

	sudo apt-cache search libsdl2-image
	sudo apt-get install libsdl2-image-dev

	sudo apt-cache search libsdl2-ttf
	sudo  apt-get install libsdl2-ttf-dev

	sudo apt-cache search libsdl2-mixer
	sudo apt-get install libsdl2-mixer-dev



Compilación y ejecucion por consola:

Dentro de la carpeta del server, abrir una terminal y ejecutar el comando:
	make servidor
	./servidor 8080 

Hacemos lo mismo dentro de la carpeta client
	make cliente
	./cliente 127.0.0.1 8080





