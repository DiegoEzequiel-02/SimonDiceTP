#include "funcion.h"

void guardarConfiguracion (char dif){
    int secuen;
    int tiempo;
    int vidas;
    FILE *pf = fopen("config.txt","wt");
    if (!pf) {
        printf("Error al abrir el archivo para escritura\n");
        return;
    }
    switch(dif){
    case 'F':
        secuen = 10;
        tiempo = 20;
        vidas = 3;
        break;
    case 'M':
        secuen = 8;
        tiempo = 15;
        vidas = 2;
        break;
    case 'D':
        secuen = 5;
        tiempo = 10;
        vidas = 1;
        break;
    }

    fprintf(pf,"%c | %d | %d | %d\n",dif,secuen,tiempo,vidas);
    fclose(pf);
}

void leerConfiguraciones(int* vidas,int* segs,int* secuen){
    char dif;
    int elementosLeidos;
    FILE *pf = fopen("config.txt","rt");
    if(!pf){
        printf("Error al leer la configuracion\n");
        return;
    }

    elementosLeidos = fscanf(pf,"%c | %d | %d | %d",&dif,secuen,segs,vidas);
    if (elementosLeidos != 4) { // Si no se leyeron los 4 elementos, el formato es incorrecto
        printf("Error: El archivo de configuraci�n tiene un formato incorrecto.\n");
        fclose(pf);
        return;
    }
    if(*vidas > 5){
        *vidas = 5;
    }
    if(*secuen > 20){
        *secuen = 20;
    }
    if(*segs > 20){
        *segs = 20;
    }
    printf("Nivel: %c\n",dif);
    printf("Segundos de secuencia completa: %d\n",*secuen);
    printf("Tiempo para contestar: %d\n",*segs);
    printf("Vidas por participantes: %d\n",*vidas);

    fclose(pf);
}

void vaciarVector(int vec[], int filas){
    int i;
    int *ptr = vec;
    for(i = 0; i< filas;i++){
        *ptr = 0;
        ptr++;
    }
}

void destruirMatriz(void** mat, int filas){
    int i;

    for(i = 0; i < filas;i++){
        free(mat[i]);
    }
    free(mat);
}

void** crearMatriz(int filas, int columnas, size_t tamElem){
    void** mat = malloc(filas* sizeof(void*));
    int i;
    if(!mat){
        return NULL;
    }
    for(i = 0; i< filas;i++){
        mat[i] = malloc(columnas*tamElem);
        if(!mat[i]){
            destruirMatriz(mat,i);
        }
    }
    return mat;
}

void mostrarNombres(char** nombres, int cant){
    int i;
    char** ptr = nombres;
    for(i = 0;i < cant;i++){
        printf("Jugador %d: %s\n", i + 1, *(ptr+i));
    }
}

int pedirNombres(){
    int contJug = 0;
    char nombre[20];
    FILE* pf = fopen("nombres.txt","wt");
    if(!pf){
        return 0;
    }
    printf("Inserte jugadores (TERMINA INGRESANDO '.'): \n");

    do {
    fflush(stdin);
    printf("Ingrese nombre del jugador %d: ", contJug + 1);
    fgets(nombre, 20, stdin);

    // Elimina el salto de l�nea que fgets almacena
    nombre[strcspn(nombre, "\n")] = '\0';

    // Si el nombre no es "x", lo almacena
        if (strcmp(nombre, ".") != 0 && strcmp(nombre,"") != 0){
            fprintf(pf,"%s\n",nombre);
            contJug++;
        }
    } while (strcmp(nombre, ".") != 0);
    fclose(pf);
    return contJug;
}

void guardarNombres(char** nombres) {
    FILE* pf = fopen("nombres.txt", "rt");
    char nombre[20];
    int i = 0;
    if (!pf) {
        return;
    }
    while (fgets(nombre, sizeof(nombre), pf)) {
        // Eliminar el salto de l�nea que fgets almacena
        nombre[strcspn(nombre, "\n")] = '\0';
        strcpy(*(nombres + i), nombre);
        i++;
    }

    fclose(pf);
    remove("nombres.txt");
}


void dificultad(char* dif){
    // Bucle para seleccionar la dificultad
    do {
        printf("\n=====Dificultad=====\n");
        printf("F - FACIL\n");
        printf("M - MEDIO\n");
        printf("D - DIFICIL\n");
        printf("Ingrese dificultad: ");
        scanf(" %c", dif);
        *dif = AMAYOR(*dif);
    } while (*dif != 'F' && *dif != 'M' && *dif != 'D');
}

void sortearJugadores(char** nombres, int cantJug) {
    char* aux;
    int cant = 0;
    char** nombPtr = nombres;
    // Inicializa el generador de n�meros aleatorios
    srand(time(NULL));

    while (cant < cantJug) {
        int pos = rand() % cantJug;

        aux = *(nombPtr + cant);
        *(nombPtr + cant) = *(nombPtr + pos);
        *(nombPtr + pos) = aux;

        cant++;
    }
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    t_cola *tc = (t_cola *)userp;  // La cola se pasa a trav�s de userp

    char buffer[realsize + 1];
    // Crear un buffer temporal para copiar los datos entrantes
    memcpy(buffer, contents, realsize);
    buffer[realsize] = '\0';  // Asegurarse de que sea una cadena v�lida

    // Usar strtok para separar los n�meros por saltos de l�nea ('\n')
    char *token = strtok(buffer, "\n");
    while (token != NULL) {
        int numero = atoi(token);  // Convertir el n�mero a entero

        // Agregar el n�mero a la cola
        if (!agregarACola(tc, &numero, sizeof(int))) {
            fprintf(stderr, "Error al agregar el numero a la cola\n");
        }

        token = strtok(NULL, "\n");  // Obtener el siguiente n�mero
    }

    return realsize;
}


void menu(){
    char opc;
    int filas = 0;  // N�mero m�ximo de jugadores
    char dif;
    int vidas;
    int segs;
    int secuen;
    int listo;
    char** nombres;
    int* puntos;
    do{
        fflush(stdin);
        printf("[A] Jugar\n");
        printf("[B] Salir\n");
        printf("Seleccione opcion: ");
        scanf(" %c",&opc);
        opc = toupper(opc);
    }while(opc != 'A' && opc != 'B');
    fflush(stdin);

    if(opc == 'A'){
        filas = pedirNombres();
        if(filas != 0){
            nombres = (char**)crearMatriz(filas, 20, sizeof(char));
            puntos = (int*)malloc(sizeof(int));
            vaciarVector(puntos,filas);

            guardarNombres(nombres);
            dificultad(&dif);

            guardarConfiguracion(dif);
            leerConfiguraciones(&vidas,&segs,&secuen);

            sortearJugadores(nombres,filas);

            printf("\nNuevo orden:\n");
            mostrarNombres(nombres,filas);

            do{
                printf("Listo? INGRESE 1: ");
                scanf("%d",&listo);
            }while(listo != 1);

           iniciarJuego(nombres,filas,puntos,segs,secuen,vidas);

           free(nombres);  // Liberar la memoria de la matriz de punteros
        }else{
            printf("No se introdujeron jugadores\n");
        }
    }

    remove("nombres.txt");
}

void generarSecuencia(t_cola* tc) {
    CURL *curl;
    CURLcode res;

    // Inicializar el manejo de curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        // Establecer la URL de la solicitud GET
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.random.org/integers/?num=10&min=1&max=4&col=1&base=10&format=plain&rnd=new");

        // Establecer los headers
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "X-Secret: FADSFAS");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Establecer la funci�n de retorno de llamada para manejar la respuesta
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, tc);  // Pasar la cola a WriteCallback

        // Realizar la solicitud HTTP GET
        res = curl_easy_perform(curl);

        // Verificar si la solicitud fue exitosa
        if (res != CURLE_OK) {
            fprintf(stderr, "Error en la solicitud: %s\n", curl_easy_strerror(res));
        }

        // Limpiar y cerrar el manejo de curl
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);  // Limpiar los headers
    }
    curl_global_cleanup();
}

void crearFecha(char* texto){
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    strftime(texto, 31, "%Y-%m-%d-%H-%M", tm); //la m es mes, la M es minutos
    strcat(texto,".txt");
}

int compararPuntos(const void* a, const void* b){
    return (*(int*)a - *(int*)b);
}

void traducirAColores(t_cola *c, unsigned tam){
    t_nodo *aux = c->prim;

    while(aux) {  // Recorre la cola
        switch(*(int*)aux->info){  // Interpreta info como un entero
        case 1:
            aux->tam = tam;
            *((char*)aux->info) = 'R';  // Reemplaza con el car�cter correspondiente
            break;
        case 2:
            aux->tam = tam;
            *((char*)aux->info) = 'V';
            break;
        case 3:
            aux->tam = tam;
            *((char*)aux->info) = 'A';
            break;
        case 4:
            aux->tam = tam;
            *((char*)aux->info) = 'M';
            break;
        default:
            break;
        }
        aux = aux->sig_nodo;  // Avanza al siguiente nodo
    }
}

void temporizadorDeEntrada(int timeout, char* entrada) {
    int timeElapsed = 0;
    int estadoTeclas[256] = {0}; // Array para rastrear el estado de las teclas
    char* ptr = entrada; // Puntero que apunta al final de la cadena

    // Inicializar la cadena como vac�a
    memset(entrada, 0, 50); // Aseg�rate de que la cadena est� vac�a al inicio

    while (timeElapsed < timeout) {
        for (int i = 'A'; i <= 'Z'; i++) { // Solo letras may�sculas
            // Verificar si la tecla est� presionada
            if (GetAsyncKeyState(i) & 0x8000) {
                // Solo procesar si la tecla no ha sido procesada previamente
                if (estadoTeclas[i] == 0) {
                    // Agregar la letra a la cadena de entrada
                    if (ptr < entrada + 49) { // Asegurarse de no exceder el tama�o del buffer
                        *ptr = (char)i; // Guardar la letra presionada
                        printf("%c", *ptr);
                        ptr++; // Mover el puntero al siguiente espacio
                        *ptr = '\0'; // Null-terminar la cadena
                    }
                    estadoTeclas[i] = 1; // Marcar la tecla como procesada
                }
            } else {
                // Restablecer el estado cuando la tecla no est� presionada
                estadoTeclas[i] = 0;
            }
        }

        // Manejo de la tecla de retroceso
        if (GetAsyncKeyState(VK_BACK) & 0x8000) { // Tecla de retroceso
            if (ptr > entrada) { // Verifica si hay caracteres para borrar
                ptr--; // Mover el puntero hacia atr�s
                *ptr = '\0'; // Null-terminar la cadena despu�s de borrar
                printf("\b \b"); // Borrar el car�cter en la consola
            }
        }

        // Manejo de la tecla ENTER
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) { // Tecla ENTER
            break; // Salir del bucle si se presiona ENTER
        }

        Sleep(100); // Esperar 100 ms
        timeElapsed += 100; // Incrementar el tiempo transcurrido
    }

    if (timeElapsed == timeout) {
        printf("\nNO HAY MAS TIEMPO\n");
    }

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); //Limpia el "buffer" de las teclas ingresadas en esta funci�n
    fflush(stdin);
}

void juegosXTurno(t_cola* orig, t_cola* aux, int cant, int secuen, int segsParaCompletar, FILE* informe) {
    char ingresa[50] = "";  // Inicializa la cadena como vac�a
    char* ptr;
    printf("Secuencia actual (mostrar hasta el digito %d):\n", cant + 1);
    mostrarDeAUno(orig, cant, secuen);
    system("cls");

    guardarColaEnArchivo(orig, informe, cant);
    fflush(stdin);

    printf("Ingrese secuencia sin espacios (tiene %d segundos): ", segsParaCompletar);
    temporizadorDeEntrada(segsParaCompletar * 1000, ingresa); // Pasar el buffer de entrada
    fflush(stdin);

    fprintf(informe, "Ingreso: ");

    printf("\nHas ingresado: %s\n", ingresa);
    Sleep(500);

    // Verifica si se ha ingresado alguna secuencia
    if (strcmp(ingresa, "") == 0) {
        vaciarCola(aux); // Vac�a la cola si no se ingres� nada
    } else {
        ptr = ingresa;  // Usa la cadena ingresada
        while (*ptr != '\0') {
            *ptr = AMAYOR(*ptr);
            agregarACola(aux, ptr, sizeof(char));
            ptr++;
        }
    }

    fflush(stdin);
    guardarColaEnArchivo(aux, informe, cant);
}



int verificarSecuencia(t_cola* tc, t_cola* tc_aux, int *cant, int* vidas, int* contParaRestar, FILE *informe, int gastoPrevio){
    int gastaVidas;
    int vidasASacar;
    char aux;

    fflush(stdin);
    if(colaVacia(tc_aux) && gastoPrevio == 0){
        printf("No puso nada de la secuencia, pierde 1 vida\n");
        *vidas -= 1;
        Sleep(1000);
        return -1;
    }

    if (!compararColas(tc, tc_aux, *cant)) {
        if (*vidas > 0) {
                do {
                    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); //Limpia el "buffer" de las teclas ingresadas en la funci�n anterior
                    printf("Secuencia incorrecta\n");
                    printf("Tiene %d vidas, quiere gastar? (1 - SI | 0 - NO):", *vidas);
                    if (scanf("%d", &gastaVidas) != 1) { // Verificar si la entrada fue exitosa
                        printf("Entrada invalida. Por favor ingresa 1 o 0.\n");
                        gastaVidas = -1; // Para asegurarte de que la condici�n del bucle siga siendo verdadera
                    }
                } while (gastaVidas != 1 && gastaVidas != 0);

            if (gastaVidas == 1) {
                do {
                    printf("Ingrese cuantas vidas quiere gastar (maximo %d): ", *vidas);
                    scanf("%d", &vidasASacar);
                } while ((vidasASacar > *vidas) && (vidasASacar > *cant));

                // Retroceder la secuencia seg�n las vidas gastadas
                while ((*contParaRestar < vidasASacar) && (*cant - *contParaRestar) >= 0) {
                    sacarDeCola(tc_aux, &aux, sizeof(char));
                    *contParaRestar += 1;
                }
                *vidas -= vidasASacar;  // Descontar las vidas

                printf("Ahora tiene %d vidas\n", *vidas);

                fprintf(informe,"Gast� %d vidas, vidas actuales: %d\n",vidasASacar,*vidas);

                printf("RECUERDE SOLO PONER LAS LETRAS PENDIENTES!!!\n");
                Sleep(5000);
                system("cls");
                return -1;
            } else {
                printf("No ha querido gastar vidas, se termina el juego para este jugador.\n");
                Sleep(2000);
                return -2;
            }

        } else {
                printf("Ya no tiene vidas.\n");
                return -2;
        }
    }else {  // Si la secuencia es correcta, vaciar la cola auxiliar y continuar
        printf("SECUENCIA CORRECTA! Vamos a la siguiente...\n");
        vaciarCola(tc_aux);  // Limpiar la cola auxiliar
        if(gastoPrevio == 1){
            return 1; //gast� vidas para completarlo
        }else{
            return 0; //no gast� vidas
        }
    }
}

void iniciarJuego(char** nombres, int cantJug, int* puntos, int segsParaCompletar, int secuen, int vidas) {
    t_cola tc;
    t_cola tc_aux;
    crearCola(&tc);
    crearCola(&tc_aux);
    int i,cant = 0,puntosXJugador = 0;
    int vidasOriginal = vidas;
    int gastaVidas;
    int gastoPrevio = 0;
    int cantParaRestar = 0;
    int segsOrig = segsParaCompletar;

    char txt_informe[31] = "informe_texto_";
    char fechaActual[20];
    crearFecha(fechaActual);
    strcat(txt_informe,fechaActual);

    FILE *informe = fopen(fechaActual,"wt");

    for (i = 0; i < cantJug; i++) {
        system("cls");
         Sleep(3000);
        // Generar secuencia y agregar a la cola
        generarSecuencia(&tc);
        traducirAColores(&tc,sizeof(char));
        printf("=========TURNO DEL JUGADOR: %s=========\n", *(nombres + i));
        puntosXJugador = 0;  // Inicializar los puntos del jugador
        fprintf(informe,"Jugador: %s\n",*(nombres + i));
        Sleep(1000);
        while (vidas >= 0 && gastaVidas != -2) {
            system("cls");
            fprintf(informe,"\nSecuencia actual de la ronda %d: ",cant+1);

            if(gastaVidas == -1){
                printf("Lo ultimo colocado:\n");
                mostrarParcial(&tc_aux,cant - cantParaRestar); //Mostramos como qued� la secuencia despues de perder vidas
            }

            juegosXTurno(&tc,&tc_aux,cant,secuen,segsParaCompletar,informe);
            fflush(stdin);
            gastaVidas = verificarSecuencia(&tc,&tc_aux,&cant,&vidas,&cantParaRestar, informe, gastoPrevio);

             // Verificar si us� vidas o no y asignar los puntos
            switch(gastaVidas){
            case -2:
                vidas = 0;
                system("cls");
                break; //termina su turno
            case -1: //vuelve a intentar, pero gast� vidas
                gastoPrevio = 1;
                break;
            case 0:
                puntosXJugador += 3;  // No us� vidas, tres puntos
                fprintf(informe,"Obtuvo 3 puntos por esta ronda\n");
                cant++;
                cantParaRestar = 0;
                //segsParaCompletar++; //El tiempo aumenta a medida que avanzan las rondas
                break;
            case 1:
                puntosXJugador += 1;  // Us� vidas, solo un punto
                fprintf(informe,"Obtuvo 1 punto por esta ronda\n");
                cant++;
               // segsParaCompletar++; //El tiempo aumenta a medida que avanzan las rondas
                gastoPrevio = 0; //reiniciamos el marcador de gastos
                break;
            }

            if(cant % 10 == 0){
                generarSecuencia(&tc); //Si va por mas de 10 letras, se agregan otras 10 a la cola
                traducirAColores(&tc,sizeof(char));
            }
            Sleep(1200);
            fflush(stdin);
        }

        cant = 0;  // Iniciar la secuencia con un d�gito
        vidas = vidasOriginal;
        vaciarCola(&tc);
        vaciarCola(&tc_aux);
        *(puntos + i) = puntosXJugador;
        fprintf(informe,"Total de puntos del jugador %s: %d\n", *(nombres+i),*(puntos+i));
        puntosXJugador = 0;
        segsParaCompletar = segsOrig;
        gastaVidas = 0;
    }

    mostrarYGuardarGanadores(nombres,puntos,cantJug,informe);

    fclose(informe);
}

void mostrarYGuardarGanadores(char** nombres, int puntos[],int cantJug, FILE* archivo){
    int i;
    int* ptr = puntos;
    int* ganador = (int*)buscarMayor(puntos,cantJug,sizeof(int),compararPuntos);

    fprintf(archivo,"GANADORES:\n");
    printf("El/los ganador/es son:\n");
    for(i = 0; i< cantJug;i++){
        if(*(ptr + i) == *ganador){
            printf("- %s | %d pts\n", *(nombres + i),*ganador);
            fprintf(archivo,"- %s | %d pts\n",*(nombres + i),*ganador);
        }
    }
}

void* buscarMayor (void* vec, size_t ce, size_t tam,int cmp(const void* a, const void* b)){
    int i;
    void* mayor = vec;
    for(i = 1; i< ce;i++){
        if(cmp(mayor,vec) > 0){
            mayor= vec;
        }
        vec+= tam;
    }

    return mayor;
}

void guardarColaEnArchivo(t_cola* cola, FILE* informe, int cant) {
    t_nodo* actual = cola->prim;  // Puntero al nodo actual
    int cont = 0;
    // Recorrer la cola nodo por nodo
    while (actual != NULL && cont <= cant) {
        char valor = *(char*)actual->info;  // Obtener el valor del nodo actual

        // Guardar el valor en el archivo
        fprintf(informe, "%c", AMAYOR(valor));

        // Avanzar al siguiente nodo
        actual = actual->sig_nodo;
        cont++;
    }

    fprintf(informe, "\n");  // Salto de l�nea al final
}

int compararColas(t_cola *cola1, t_cola *cola2, int cant) {
    t_nodo *nodo1 = cola1->prim;
    t_nodo *nodo2 = cola2->prim;
    int contador = 0;

    // Recorre las colas hasta que uno de los nodos sea NULL o se alcance 'cant'
    while (nodo1 && nodo2 && contador <= cant) {
        // Comparar los valores de los nodos como caracteres
        if (*(char*)(nodo1->info) != *(char*)(nodo2->info)) {
            return 0;  // Si los caracteres no son iguales, retorna 0 (colas no iguales)
        }
        nodo1 = nodo1->sig_nodo;
        nodo2 = nodo2->sig_nodo;
        contador++;
    }

    if (contador > cant) {
        // Verificar si cola2 tiene nodos adicionales
        if (nodo2 != NULL) {
            return 0;  // Si cola2 tiene m�s nodos, no son iguales
        }
        return 1;  // Si ambos tienen la misma longitud hasta 'cant', son iguales
    }

    // Si el contador es menor que cant, tienen la misma longitud
    if (contador < cant && (nodo1 || nodo2)) {
        return 0;  // Si una cola es m�s larga que la otra, no son iguales
    }

    // Si todo fue igual y se recorrieron todos los elementos, las colas son iguales
    return 1;
}

void mostrar(const void* a){
    printf("%c ",*(char*)a);
}

void mostrarDeAUno(t_cola* c, int cant, int segsSecuencia){
    int cont = 0;
    t_nodo* actual = c->prim;  // Usar un puntero auxiliar en lugar de modificar 'prim'
    float tiempoPorItem = segsSecuencia/(cant+1);
    while(actual != NULL && cont <= cant){
        printf("(%d) ", cont+1);
        mostrar(actual->info);
        barraDeProgreso(cont,cant);
        Sleep(tiempoPorItem * 1000); //tiempo correspondiente a cada item (ej, secuencia de 10 segundos con 2 elementos, 5 segs para cada uno)
        system("cls");
        actual = actual->sig_nodo;  // Mover el puntero auxiliar
        cont++;
    }
    printf("\n");
}

void mostrarParcial(t_cola* c, int cant){
    int cont = 0;
    t_nodo* actual = c->prim;  // Usar un puntero auxiliar en lugar de modificar 'prim'

    while(actual != NULL && cont <= cant){
        mostrar(actual->info);
        actual = actual->sig_nodo;  // Mover el puntero auxiliar
        cont++;
    }
    printf("\n");
}

void barraDeProgreso(int cant, int total){
    int i;
    int max = 30;
    int progreso = ((cant+1) * max) / (total+1); // Calcula la cantidad de caracteres llenos

    printf("\nCARGANDO...");
    printf("["); // Empieza la barra de progreso
    for (i = 0; i < max; i++) {
        if (i < progreso) {
            printf("#"); // Parte completada
        } else {
            printf(" "); // Parte pendiente
        }
    }
    printf("]");
}
