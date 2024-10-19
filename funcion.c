#include "funcion.h"

void guardarConfiguracion (char dif){
    FILE *pf = fopen("config.txt","wt");
    if (!pf) {
        printf("Error al abrir el archivo para escritura\n");
        return;
    }
    int secuen;
    int tiempo;
    int vidas;
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
    FILE *pf = fopen("config.txt","rt");
    if(!pf){
        printf("Error al leer la configuracion\n");
        return;
    }

    char dif;
    fscanf(pf,"%c | %d | %d | %d",&dif,secuen,segs,vidas);
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
    void** mat = malloc(filas* sizeof(filas));
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
    printf("Inserte jugadores (TERMINA INGRESANDO 'x'): \n");

    do {
    printf("Ingrese nombre del jugador %d: ", contJug + 1);
    fgets(nombre, 20, stdin);

    // Elimina el salto de línea que fgets almacena
    nombre[strcspn(nombre, "\n")] = '\0';

    // Si el nombre no es "x", lo almacena
        if (strcmp(nombre, "x") != 0 && strcmp(nombre, "X") != 0 && strcmp(nombre,"") != 0) {
            fprintf(pf,"%s\n",nombre);
            contJug++;
        }
    } while (strcmp(nombre, "x") != 0 && strcmp(nombre, "X") != 0);
    fclose(pf);
    return contJug;
}

void guardarNombres(char** nombres){
    FILE* pf = fopen("nombres.txt","rt");
    char nombre[20];
    int i= 0;
    if(!pf){
        return;
    }
    while(fscanf(pf,"%s\n",nombre) == 1){
        strcpy(*(nombres + i),nombre);
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
        *dif = toupper(*dif);
    } while (*dif != 'F' && *dif != 'M' && *dif != 'D');
}

void sortearJugadores(char** nombres, int cantJug) {
    char* aux;
    int cant = 0;

    // Inicializa el generador de números aleatorios
    srand(time(NULL));

    while (cant < cantJug) {
        int pos = rand() % cantJug;

        aux = nombres[cant];
        nombres[cant] = nombres[pos];
        nombres[pos] = aux;

        cant++;
    }
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
    size_t realsize = size * nmemb;
    t_cola *tc = (t_cola *)userp;  // La cola se pasa a través de userp

    // Usar strtok para separar los números por saltos de línea ('\n')
    char *token = strtok((char *)contents, "\n");
    while (token != NULL) {
        int numero = atoi(token);  // Convertir el número a entero

        // Agregar el número a la cola
        if (!agregarACola(tc, &numero, sizeof(int))) {
            fprintf(stderr, "Error al agregar el número a la cola\n");
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
            char** nombres = (char**)crearMatriz(filas, 20, sizeof(char));
            int* puntos = (int*)malloc(sizeof(int));
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

void juegosXTurno (t_cola* orig, t_cola* aux, int cant, int secuen, int segsParaCompletar, FILE *informe){
    time_t inicio, actual;
    char ingresa[50];
    char* ptr;

    printf("Secuencia actual (mostrar hasta el digito %d):\n", cant+1);
    mostrarDeAUno(orig, cant);
    Sleep(secuen * 1000);  // Mostrar secuencia por un tiempo determinado
    system("cls");

    guardarColaEnArchivo(orig,informe,cant);
    // Obtener el tiempo de inicio
    inicio = time(NULL);
    fflush(stdin);

    printf("Ingrese secuencia sin espacios (tiene %d segundos): ", segsParaCompletar);

     // Bucle para esperar la entrada del usuario y verificar el tiempo
    while (1) {
        actual = time(NULL);
        fprintf(informe,"Ingreso: ");
        // Verificar si se supera el tiempo límite
        if ((actual - inicio) >= segsParaCompletar) {
            printf("\nNO HAY MAS TIEMPO\n");
            break;
        }
        if (fgets(ingresa, 50, stdin) != NULL) {  // Verificar que fgets haya leído algo
            // Eliminar el salto de línea que se incluye con fgets
            ingresa[strcspn(ingresa, "\n")] = '\0';  // Reemplazar el salto de línea con '\0'

            printf("Has ingresado: %s\n", ingresa);
            break;
        }
    }
    ptr = ingresa;
    while (*ptr != '\0') {
        *ptr = AMAYOR(*ptr);
        agregarACola(aux, ptr, sizeof(char));
        ptr++;
    }

    guardarColaEnArchivo(aux,informe,cant);
}

int verificarSecuencia(t_cola* tc, t_cola* tc_aux, int *cant, int* vidas, int* contParaRestar, FILE *informe, int gastoPrevio){
    int gastaVidas;
    int vidasASacar;
    char aux;
    if (!compararColas(tc, tc_aux, *cant)) {
        if (*vidas > 0) {
            do{
                fflush(stdin);
                printf("Secuencia incorrecta\n");
                printf("Tiene %d vidas, quiere gastar? (1 - SI | 0 - NO):", *vidas);
                scanf("%d", &gastaVidas);
            } while(gastaVidas != 1 && gastaVidas != 0);

            if (gastaVidas == 1) {
                do {
                    printf("Ingrese cuantas vidas quiere gastar (máximo %d): ", *vidas);
                    scanf("%d", &vidasASacar);
                } while (vidasASacar > *vidas && vidasASacar > cant);

                // Retroceder la secuencia según las vidas gastadas
                while (*contParaRestar < vidasASacar && (*cant - *contParaRestar) >= 0) {
                    sacarDeCola(tc_aux,&aux,sizeof(char));  // Eliminar los dígitos incorrectos ingresados
                    *contParaRestar = *contParaRestar + 1;
                }
                *vidas -= vidasASacar;  // Descontar las vidas

                printf("Ahora tiene %d vidas\n", *vidas);

                fprintf(informe,"Gastó %d vidas, vidas actuales: %d\n",vidasASacar,*vidas);

                printf("RECUERDE SOLO PONER LAS LETRAS PENDIENTES!!!\n");
                return -1;
            } else {
                printf("No ha querido gastar vidas, se termina el juego para este jugador.\n");
                return -2;
            }

        } else {
                printf("Ya no tiene vidas.\n");
                return -2;
        }
    }else {  // Si la secuencia es correcta, vaciar la cola auxiliar y continuar
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
        // Generar secuencia y agregar a la cola
        generarSecuencia(&tc);
        traducirAColores(&tc,sizeof(char));
        printf("=========TURNO DEL JUGADOR: %s=========\n", *(nombres + i));
        puntosXJugador = 0;  // Inicializar los puntos del jugador
        fprintf(informe,"Jugador: %s\n",*(nombres + i));

        while (vidas >= 0 && gastaVidas != -2) {
            fprintf(informe,"\nSecuencia actual de la ronda %d: ",cant);

            if(gastaVidas == -1){
                printf("Lo ultimo colocado:\n");
                mostrarDeAUno(&tc_aux,cant - cantParaRestar); //Mostramos como quedó la secuencia despues de perder vidas
            }
            fflush(stdin);
            juegosXTurno(&tc,&tc_aux,cant,secuen,segsParaCompletar,informe);
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
                gastoPrevio = 0; //reiniciamos el marcador de gastos
                cantParaRestar = 0;
                segsParaCompletar++; //El tiempo aumenta a medida que avanzan las rondas
                break;
            case 1:
                puntosXJugador += 1;  // Usó vidas, solo un punto
                fprintf(informe,"Obtuvo 1 punto por esta ronda\n");
                cant++;
                segsParaCompletar++; //El tiempo aumenta a medida que avanzan las rondas
                break;
            }

            if(cant % 10 == 0){
                printf("%d\n", cant);
                generarSecuencia(&tc); //Si va por mas de 10 letras, se agregan otras 10 a la cola
            }
        }

        cant = 0;  // Iniciar la secuencia con un dígito
        vidas = vidasOriginal;
        vaciarCola(&tc);
        vaciarCola(&tc_aux);
        *(puntos + i) = puntosXJugador;
        fprintf(informe,"Total de puntos del jugador %s: %d\n", *(nombres+i),*(puntos+i));
        puntosXJugador = 0;
        segsParaCompletar = segsOrig;
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
            printf("- %s\n", *(nombres + i));
            fprintf(archivo,"- %s\n",*(nombres + i));
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

void mostrar(const void* a){
    printf("%c ",*(char*)a);
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

    // Si contador es menor que cant, ambas colas deben tener la misma longitud
    if (contador < cant && (nodo1 || nodo2)) {
        return 0;  // Si una cola es más larga que la otra, no son iguales
    }

    // Si todo fue igual y se recorrieron 'cant' elementos, las colas son iguales
    return 1;
}

void mostrarDeAUno(t_cola* c, int cant){
    int cont = 0;
    t_nodo* actual = c->prim;  // Usar un puntero auxiliar en lugar de modificar 'prim'

    while(actual != NULL && cont <= cant){
        mostrar(actual->info);
        actual = actual->sig_nodo;  // Mover el puntero auxiliar
        cont++;
    }
    printf("\n");
}
