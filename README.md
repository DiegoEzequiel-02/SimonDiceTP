# SimonDice - Estructuras de datos
## Tabla de contenidos
- [Reglas del juego](#reglas-del-juego)
- [Instrucciones](#instrucciones)
- [Dificultades](#dificultades)
- [Instalación](#instalación)
- [Requisitos](#requisitos)
### Reglas del juego
- A cada jugador se le mostrará la secuencia durante un tiempo X, una letra a la vez.
- Cada jugador tendrá Y segundos para ingresar su secuencia, letra por letra. 
- Durante el ingreso de la secuencia, cualquier carácter que no sea una letra será ignorado, a menos que sea la tecla utilizado para usar una vida.
- Si el jugador no ingresó ninguna letra y finaliza el tiempo, podrá utilizar una vida para reiniciar su turno.
- El jugador puede utilizar sus vidas para retroceder N letras, según las vidas disponibles, tras ingresar una letra. Para esto, debe pulsar la tecla 'H' en su teclado.
- Si el jugador no tiene vidas suficientes para corregir las letras incorrectas o para reiniciar su turno, finaliza su turno e inicia el del próximo jugador.
- Si el jugador ingresa correctamente la secuencia, el programa le muestra la siguiente secuencia.
- Por cada secuencia bien ingresada, sin usar vida, se recibe tres puntos.
- Por cada secuencia bien ingresada, usando vidas, se recibe un punto.
- El jugador o los jugadores que obtengan la mayor cantidad de puntos serán los ganadores.

### Instrucciones 
1. Ingresar los nombres de los jugadores, para finalizar presione '.'.
2. Elegir la dificultad (fácil, medio, difícil).
3. Se mostrará el orden de los jugadores, decidido por sorteo.
4. Antes de iniciar la partida, se mostrará la cantidad de vidas de cada jugador, el tiempo que tendrán para ver la secuencia y el tiempo que tendrán para ingresarla.

### Dificultades
Se puede elegir entre tres niveles de dificultad diferentes:
- Fácil: 
    - Tiempo para ver la secuencia: 10 segundos
    - Tiempo para ingresar la secuencia: 20 segundos
    - Cantidad de vidas: 3
- Medio:
    - Tiempo para ver la secuencia: 8 segundos
    - Tiempo para ingresar la secuencia: 15 segundos
    - Cantidad de vidas: 2
- Difícil:
    - Tiempo para ver la secuencia: 5 segundos
    - Tiempo para ingresar la secuencia: 10 segundos
    - Cantidad de vidas: 1

Los niveles de dificultad pueden ser configurados de manera manual modificando el archivo "config.txt", respetando el siguiente formato:
>Nivel|Tiempo para ver la secuencia completa|Tiempo para ingresar la secuencia|Vidas

Ejemplo
>F|10|20|3

## Requisitos
Este proyecto funciona con curl, si no lo tiene, seguir estas instrucciones, en caso de ejecutarlo utilizando el IDE Codeblocks:
1) Descargar curl desde: https://curl.se/windows (En caso de no usar Windows, https://curl.se/download.html).
2) Descomprimir el zip y pasar esa carpeta a una ubicación segura.
3) Agregar dentro de PATH (como variable de entorno) la ubicación a la carpeta “bin”.
4) En Codeblocks: Settings > Compiler > Linker Settings: agregar ubicación a “lib\libcurl.dll.a”.
5) En Codeblocks: Settings > Compiler > Search directories > Compiler y Resource compiler: agregar ubicación a “include”.
6) En Codeblocks: Settings > Compiler > Search directories > Linker: agregar ubicación a “lib”.

Todo se realiza mediante teclado, las instrucciones dentro del juego le indicarán que ingresar (Se confirma la elección presionando ENTER).
