#include "funcion.h"

void guardarConfiguracion (char dif){
    int secuen;
    int tiempo;
    int vidas;
    FILE *pf = fopen("config.txt","wt");
    if (!pf) {
        printf("Error al crear el archivo de configuracion para escritura.\n");
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
        printf("Error al leer la configuracion.\n");
        return;
    }

    elementosLeidos = fscanf(pf,"%c | %d | %d | %d",&dif,secuen,segs,vidas);
    if (elementosLeidos != 4) { // Si no se leyeron los 4 elementos, el formato es incorrecto
        printf("Error: El archivo de configuracion tiene un formato incorrecto.\n");
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
    char nombre[MAX_NOMBRE];
    FILE* pf = fopen("nombres.txt","wt");
    if(!pf){
        printf("No se puede crear jugadores en este momento.\n");
        return 0;
    }
    printf("Inserte jugadores (MAXIMO 20 CARACTERES | TERMINA INGRESANDO '.'): \n");

    do {
        fflush(stdin);
        printf("Ingrese nombre del jugador %d: ", contJug + 1);
        fgets(nombre, MAX_NOMBRE, stdin);

        // Elimina el salto de línea que fgets guarda
        nombre[strcspn(nombre, "\n")] = '\0';

        // Si el nombre no es ".", lo guarda
        if (strcmp(nombre, ".") != 0 && strcmp(nombre,"") != 0){
            fprintf(pf,"%s\n",nombre);
            contJug++;
        }
    } while (strcmp(nombre, ".") != 0);
    fclose(pf);
    return contJug;
}

int my_strlen(char* cad){
    int cont = 0;
    while(*cad != '\0'){
        cont++;
        cad++;
    }
    return cont;
}

void guardarNombres(char** nombres) {
    FILE* pf = fopen("nombres.txt", "rt");
    char nombre[MAX_NOMBRE];
    int i = 0;
    if (!pf) {
        printf("No se pudo guardar jugadores en este momento.\n");
        return;
    }
    while (fgets(nombre, sizeof(nombre), pf)) {
        nombre[strcspn(nombre, "\n")] = '\0';  // Elimina el salto de línea que fgets guarda
        *(nombres + i) = (char*)malloc((my_strlen(nombre) + 1) * sizeof(char)); //Aseguramos que se guarde el nombre con el tamaño que necesita
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
    int cant;
    char** nombPtr = nombres;
    int pos;

    cant = 0;
    // Inicializa el generador de números aleatorios
    srand(time(NULL));

    while (cant < cantJug) {
        pos = rand() % cantJug;

        aux = *(nombPtr + cant);
        *(nombPtr + cant) = *(nombPtr + pos);
        *(nombPtr + pos) = aux;

        cant++;
    }
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    t_cola *tc = (t_cola *)userp;  // La cola se pasa a través de userp

    char buffer[realsize + 1];
    // Crear un buffer temporal para copiar los datos entrantes
    memcpy(buffer, contents, realsize);
    buffer[realsize] = '\0';  // Asegurarse de que sea una cadena válida

    // Usar strtok para separar los números por saltos de línea
    char *token = strtok(buffer, "\n");
    while (token != NULL) {
        int numero = atoi(token);  // Convertir el número a entero

        // Agregar el número a la cola
        if (!agregarACola(tc, &numero, sizeof(int))) {
            fprintf(stderr, "Error al agregar el numero a la cola\n");
        }

        token = strtok(NULL, "\n");  // Obtener el siguiente número
    }

    return realsize;
}

void menu(){
    char opc;
    int filas = 0;  // Número máximo de jugadores
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
            nombres = (char**)crearMatriz(filas, MAX_NOMBRE, sizeof(char));
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
                fflush(stdin);
                printf("Listo? INGRESE 1: ");
                scanf("%d",&listo);
            }while(listo != 1);

           iniciarJuego(nombres,filas,puntos,segs,secuen,vidas);

           free(nombres);
        }else{
            printf("No se introdujeron jugadores\n");
        }
    }
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

        // Establecer la función de retorno de llamada para manejar la respuesta
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
            *((char*)aux->info) = 'R';  // Reemplaza con el carácter correspondiente
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

int temporizadorDeEntrada(int timeout,t_cola* cola) {
    int timeElapsed = 0;
    int estadoTeclas[256] = {0}; // Array para rastrear el estado de las teclas
    int termino;
    while (timeElapsed < timeout) {
        // Revisar solo las teclas especificadas
        if ((GetAsyncKeyState('A') & 0x8000) && estadoTeclas['A'] == 0) {
            char letra = 'A';
            if (!agregarACola(cola, &letra, sizeof(char))) {
                printf("Error al agregar a la cola\n");
                return -1;
            }
            printf("%c ", letra);
            estadoTeclas['A'] = 1;
        } else {
            estadoTeclas['A'] = 0;
        }

        if ((GetAsyncKeyState('R') & 0x8000) && estadoTeclas['R'] == 0) {
            char letra = 'R';
            if (!agregarACola(cola, &letra, sizeof(char))) {
                printf("Error al agregar a la cola\n");
                return -1;
            }
            printf("%c ", letra);
            estadoTeclas['R'] = 1;
        } else {
            estadoTeclas['R'] = 0;
        }

        if ((GetAsyncKeyState('V') & 0x8000) && estadoTeclas['V'] == 0) {
            char letra = 'V';
            if (!agregarACola(cola, &letra, sizeof(char))) {
                printf("Error al agregar a la cola\n");
                return -1;
            }
            printf("%c ", letra);
            estadoTeclas['V'] = 1;
        } else {
            estadoTeclas['V'] = 0;
        }

        if ((GetAsyncKeyState('M') & 0x8000) && estadoTeclas['M'] == 0) {
            char letra = 'M';
            if (!agregarACola(cola, &letra, sizeof(char))) {
                printf("Error al agregar a la cola\n");
                return -1;
            }
            printf("%c ", letra);
            estadoTeclas['M'] = 1;
        } else {
            estadoTeclas['M'] = 0;
        }

        // Ingresar/confirmar
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
            termino = 0;
            break;
        }

        if ((GetAsyncKeyState('H') & 0x8000) && !colaVacia(cola)) { //Letra de ayuda, verifica si el usuario ingresó al menos una letra
            timeElapsed = timeout;
            return 1;
        }

        Sleep(100); // Esperar 100 ms
        timeElapsed += 100; // Incrementar el tiempo transcurrido
    }

    if (timeElapsed == timeout) {
        printf("\nNO HAY MAS TIEMPO\n");
    }

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); //Limpia el "buffer" de las teclas ingresadas en esta función
    fflush(stdin);
    return termino;
}

void recorrerIngresado(t_cola* cola){
    t_nodo* aux = cola->prim;

    while(aux != NULL){
        printf("%c ", *(char*)aux->info);
        aux = aux->sig_nodo;
    }
    printf("\n");
}

int contadorDeNodos(t_cola* cola){
    t_nodo* aux = cola->prim;
    int cont = 0;
    while(aux != NULL){
        cont++;
        aux = aux->sig_nodo;
    }
    return cont;
}

void juegosXTurno(t_cola* orig, t_cola* aux, int cant, int secuen, int segsParaCompletar, FILE* informe,int* vidas) {
    int fintiempo;
    int vidasASacar;
    int cantletras = 0;
    char c_aux;
    printf("Secuencia actual:\n");
    mostrarDeAUno(orig, cant, secuen);
    system("cls");

    guardarColaEnArchivo(orig, informe, cant);
    fflush(stdin);

    printf("Ingrese secuencia (tiene %d segundos): ", segsParaCompletar);
    fintiempo = temporizadorDeEntrada(segsParaCompletar * 1000,aux); // Pasar el buffer de entrada
    while(fintiempo == 1 && *vidas > 0) { //interrumpieron el juego para volver atrás
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); //Limpia el "buffer" de las teclas ingresadas anteriormente
        cantletras = contadorDeNodos(aux);
        system("cls");
        do{
            fflush(stdin);
            printf("Cuantos pasos desea volver? (Recuerde que usted tiene %d vidas): ",(*vidas));
            scanf("%d",&vidasASacar);
        }while(vidasASacar < 0 || vidasASacar > *vidas || vidasASacar > (cantletras + 1));
        while(vidasASacar > 0){
            sacarDeCola(aux,&c_aux,sizeof(char));
            *vidas-=1;
            vidasASacar--;
        }
        if(colaVacia(aux)){
            mostrarDeAUno(orig,cant,secuen);
        }
        printf("Ingrese secuencia (tiene %d segundos): ", segsParaCompletar);
        fintiempo = temporizadorDeEntrada(segsParaCompletar * 1000,aux);
    }

    fflush(stdin);

    fprintf(informe, "Ingreso: ");

    printf("\nHas ingresado: \n");
    recorrerIngresado(aux);

    Sleep(500);

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
                    fflush(stdin);
                    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); //Limpia el "buffer" de las teclas ingresadas en la función anterior
                    printf("Secuencia incorrecta\n");
                    printf("Tiene %d vidas, quiere gastar? (1 - SI | 0 - NO):", *vidas);
                    if (scanf("%d", &gastaVidas) != 1) { // Verificar si la entrada fue exitosa
                        printf("Entrada invalida. Por favor ingresa 1 o 0.\n");
                        gastaVidas = -1; // Para asegurarte de que la condición del bucle siga siendo verdadera
                    }
                } while (gastaVidas != 1 && gastaVidas != 0);

            if (gastaVidas == 1) {
                do {
                    fflush(stdin);
                    printf("Ingrese cuantas vidas quiere gastar (maximo %d): ", *vidas);
                    scanf("%d", &vidasASacar);
                } while ((vidasASacar > *vidas) || (vidasASacar > (*cant)+1) || vidasASacar <= 0);

                // Retroceder la secuencia según las vidas gastadas
                while (vidasASacar > 0) {
                    sacarDeCola(tc_aux, &aux, sizeof(char));
                    *contParaRestar += 1;
                    vidasASacar--;
                    *vidas-=1;
                }

                printf("Ahora tiene %d vidas\n", *vidas);

                fprintf(informe,"Gastó %d vidas, vidas actuales: %d\n",vidasASacar,*vidas);

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
            return 1; //gastó vidas para completarlo
        }else{
            return 0; //no gastó vidas
        }
    }
}

void iniciarJuego(char** nombres, int cantJug, int* puntos, int segsParaCompletar, int secuen, int vidas) {
    t_cola tc;
    t_cola tc_aux;

    int i,cant = 0,puntosXJugador = 0;
    int vidasOriginal = vidas;
    int gastaVidas;
    int gastoPrevio = 0;
    int cantParaRestar = 0;

    char txt_informe[31] = "informe_texto_";
    char fechaActual[20];
    crearFecha(fechaActual);
    strcat(txt_informe,fechaActual);

    FILE *informe = fopen(fechaActual,"wt");
    if(!informe){
        printf("Error al crear informe.\n");
        return;
    }

    crearCola(&tc);
    crearCola(&tc_aux);


    for (i = 0; i < cantJug; i++) {
        system("cls");
        Sleep(3000);
        generarSecuencia(&tc);
        traducirAColores(&tc,sizeof(char));
        printf("=========TURNO DEL JUGADOR: %s=========\n", *(nombres + i));
        puntosXJugador = 0;
        fprintf(informe,"Jugador: %s\n",*(nombres + i));
        Sleep(1000);
        while (vidas >= 0 && gastaVidas != -2) {
            system("cls");
            fprintf(informe,"\nSecuencia actual de la ronda %d: ",cant+1);

            if(gastaVidas == -1){
                printf("Lo ultimo colocado:\n");
                mostrarParcial(&tc_aux,cant - (cantParaRestar-1)); //Mostramos como quedó la secuencia despues de perder vidas
            }

            juegosXTurno(&tc,&tc_aux,cant,secuen,segsParaCompletar,informe,&vidas);
            fflush(stdin);
            gastaVidas = verificarSecuencia(&tc,&tc_aux,&cant,&vidas,&cantParaRestar, informe, gastoPrevio);

             // Verificar si usó vidas o no y asignar los puntos
            switch(gastaVidas){
            case -2:
                vidas = 0;
                system("cls");
                break; //termina su turno
            case -1: //vuelve a intentar, pero gastó vidas
                gastoPrevio = 1;
                break;
            case 0:
                puntosXJugador += 3;  // No usó vidas, tres puntos
                fprintf(informe,"Obtuvo 3 puntos por esta ronda\n");
                cant++;
                cantParaRestar = 0;
                segsParaCompletar++;
                break;
            case 1:
                puntosXJugador += 1;  // Usó vidas, solo un punto
                fprintf(informe,"Obtuvo 1 punto por esta ronda\n");
                cant++;
                gastoPrevio = 0; //reiniciamos el marcador de gastos
                segsParaCompletar++;
                cantParaRestar = 0;
                break;
            }

            if(cant % 10 == 0){
                generarSecuencia(&tc); //Si va por mas de 10 letras, se agregan otras 10 a la cola
                traducirAColores(&tc,sizeof(char));
            }
            Sleep(1200);
            fflush(stdin);
        }

        cant = 0;  // Iniciar la secuencia con un dígito
        vidas = vidasOriginal;
        vaciarCola(&tc);
        vaciarCola(&tc_aux);
        *(puntos + i) = puntosXJugador;
        fprintf(informe,"Total de puntos del jugador %s: %d\n", *(nombres+i),*(puntos+i));
        puntosXJugador = 0;
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

void* buscarMayor (void* vec, size_t ce, size_t tam, int cmp(const void* a, const void* b)){
    int i;
    void* mayor = vec;
    void* actual = vec;  // Puntero auxiliar para iterar por el vector

    for(i = 1; i < ce; i++){
        actual += tam;  // Avanza al siguiente elemento
        if(cmp(mayor, actual) < 0){  // Si el actual es mayor que el almacenado en mayor
            mayor = actual;  // Actualiza el puntero mayor
        }
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
    fprintf(informe, "\n");  // Salto de línea al final
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
            return 0;  // Si cola2 tiene más nodos, no son iguales
        }
        return 1;  // Si ambos tienen la misma longitud hasta 'cant', son iguales
    }

    // Si el contador es menor que cant, tienen la misma longitud
    if (contador < cant && (nodo1 || nodo2)) {
        return 0;  // Si una cola es más larga que la otra, no son iguales
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
    float tiempoPorItem = (float)segsSecuencia/(cant+1);
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
