#include "pa2mm.h"
#include "src/hash.h"


void prueboCrearYDestruirUnHash_DevuelveHash(){ 
    hash_t* hash = hash_crear(NULL, 5);
    pa2m_afirmar(hash != NULL, "Se puede crear un hash de tamanio 5");

    hash_destruir(hash);
}


void prueboInsertarElementosEnElHash_Devuelve0SiFueExitoso_SiNoDevuelveMenosUno(){
    int numero_1 = 548, numero_2 = 96, numero_3 = 821 , numero_4 = 335, numero_5 = 88, numero_6 = 55, numero_7 = 584, numero_8 = 20;
    const char* clave1 = "SS";
    const char* clave2 = "FhSS";
    const char* clave3 = "fdss";
    const char* clave4 = "gfFF";
    const char* clave5 = "SsssdeS";
    const char* clave6 = "t4t4t4";

    const char* clave7 = "fdseeac";
    const char* clave8 = "ñl,kob";

    hash_t* hash = hash_crear(NULL, 5);
    pa2m_afirmar(hash_insertar(hash, clave1, &numero_1) == 0, "Pudo insertar un elemento");
    pa2m_afirmar(hash_insertar(hash, clave2, &numero_2) == 0, "Pudo insertar dos elementos");
    pa2m_afirmar(hash_insertar(hash, clave3, &numero_3) == 0, "Pudo insertar tres elementos");
    pa2m_afirmar(hash_insertar(hash, clave4, &numero_4) == 0, "Pudo insertar cuatro elementos");
    pa2m_afirmar(hash_insertar(hash, clave5, &numero_5) == 0, "Pudo insertar cinco elementos");
    
    pa2m_afirmar(hash_insertar(hash, clave1, &numero_4) == 0, "Pudo insertar un elemento con una clave ya existente");
    pa2m_afirmar(hash_insertar(hash, clave2, &numero_5) == 0, "Pudo insertar dos elementos con una clave ya existente");
    pa2m_afirmar(hash_contiene(hash, clave1), "Luego de reemplazar el valor de un elemento puedo encontrarlo");
    pa2m_afirmar(hash_contiene(hash, clave2), "Luego de reemplazar el valor del otro elemento puedo encontrarlo");

    pa2m_afirmar(hash_cantidad(hash) == 5, "La cantidad de elementos es la correcta");
    
    pa2m_afirmar(hash_insertar(hash, clave6, &numero_6) == 0, "Pudo insertar seis elementos y hay un rehasheo");
   
    pa2m_afirmar(hash_contiene(hash, clave1), "Luego del rehash puedo encontrar el primer elemento insertado");
    pa2m_afirmar(hash_contiene(hash, clave2), "Luego del rehash puedo encontrar el segundo elemento insertado");
    pa2m_afirmar(hash_contiene(hash, clave3), "Luego del rehash puedo encontrar el tercer elemento insertado");
    pa2m_afirmar(hash_contiene(hash, clave4), "Luego del rehash puedo encontrar el cuarto elemento insertado");
    pa2m_afirmar(hash_contiene(hash, clave5), "Luego del rehash puedo encontrar el quinto elemento insertado");
    pa2m_afirmar(hash_contiene(hash, clave6), "Luego del rehash puedo encontrar el sexto elemento insertado");

    pa2m_afirmar(hash_insertar(hash, clave7, &numero_7) == 0, "Pudo insertar un elemento despues del rehash");
    pa2m_afirmar(hash_insertar(hash, clave8, &numero_8) == 0, "Pudo insertar dos elementos despues del rehash");

    pa2m_afirmar(hash_insertar(NULL, clave1, &numero_1) == -1, "No se puede insertar en un hash invalido");
    pa2m_afirmar(hash_insertar(hash, NULL, &numero_1) == -1, "No se puede insertar con una clave invalida");


    hash_destruir(hash);
}


void prueboContieneYQuitarUnElementoDelHash(){
    int numero_1 = 548, numero_2 = 96, numero_3 = 821, numero_4 = 335, numero_5 = 88;

    const char* clave1 = "SS";
    const char* clave2 = "FhSS";
    const char* clave3 = "fdss";
    const char* clave4 = "gfFF";
    const char* clave5 = "SsssSDSDSDSDdeS";

    const char* clave_inexistente = "f";

    hash_t* hash = hash_crear(NULL, 5);

    pa2m_afirmar(hash_insertar(hash, clave1, &numero_1) == 0, "Inserto un elemento");
    pa2m_afirmar(hash_insertar(hash, clave1, &numero_1) == 0, "Inserto nuevamente el mismo elemento");
    pa2m_afirmar(hash_quitar(hash, clave1) == 0, "Quito el elemento");
    pa2m_afirmar(hash_quitar(hash, clave1) == -1, "Intento quitar el elemento recien quitado");
    pa2m_afirmar(hash_cantidad(hash) == 0, "El Hash contiene 0 elementos");

    hash_insertar(hash, clave1, &numero_1);
    hash_insertar(hash, clave2, &numero_2);
    hash_insertar(hash, clave3, &numero_3);
    hash_insertar(hash, clave4, &numero_4);
    hash_insertar(hash, clave5, &numero_5);

    size_t elementos_antes_de_quitar = hash_cantidad(hash);

    pa2m_afirmar(hash_contiene(hash, clave4),"Existe un elemento con cierta clave esta en el hash");
    pa2m_afirmar(hash_quitar(hash, clave4) == 0,"Quito el elemento del hash");
    pa2m_afirmar(!hash_contiene(hash, clave4),"El elemento no esta mas en el hash");
    pa2m_afirmar(hash_cantidad(hash) != elementos_antes_de_quitar,"La cantidad de elementos del hash disminuyo");

    pa2m_afirmar(hash_contiene(hash, clave5),"Puedo encontrar el elemento siguiente al que elimine");

    pa2m_afirmar(hash_quitar(NULL, clave3) == -1,"No se puede pasar un hash invalido al hash_quitar");
    pa2m_afirmar(!hash_contiene(NULL, clave3),"No se puede pasar un hash invalido al hash_contiene");
    pa2m_afirmar(hash_quitar(hash, NULL) == -1,"No se puede pasar una clave invalida al hash_quitar");
    pa2m_afirmar(!hash_contiene(hash, NULL),"No se puede pasar una clave invalida al hash_contiene");

    pa2m_afirmar(hash_quitar(hash, clave_inexistente) == -1,"No se puede eliminar una clave que no existe");
    hash_insertar(hash, clave1, &numero_1);
    pa2m_afirmar(hash_quitar(hash, clave1) == 0,"Quito el elemento del hash");
    hash_destruir(hash);
}


void prueboObtenerYQuitarUnElementoDelHash(){
    int numero_1 = 548, numero_2 = 96, numero_3 = 821, numero_4 = 335, numero_5 = 88;

    const char* clave1 = "SS";
    const char* clave2 = "FhSS";
    const char* clave3 = "fdss";
    const char* clave4 = "gfFF";
    const char* clave5 = "SsssdeS";

    hash_t* hash = hash_crear(NULL, 5);

    pa2m_afirmar(hash_insertar(hash, clave1, &numero_1) == 0, "Inserto un elemento");
    pa2m_afirmar(hash_insertar(hash, clave1, &numero_3) == 0, "Inserto otro elemento con la misma clave pero diferente valor");
    pa2m_afirmar((*(int*)hash_obtener(hash, clave1)) == numero_3, "El valor del elemento es el ultimo insertado");
    pa2m_afirmar(hash_quitar(hash, clave1) == 0, "Quito el elemento");
    pa2m_afirmar(hash_insertar(hash, clave1, &numero_3) == 0, "Inserto el mismo elemento quitado");
    pa2m_afirmar((*(int*)hash_obtener(hash, clave1)) == numero_3, "El valor del elemento insertado es correcto");
    pa2m_afirmar(hash_quitar(hash, clave1) == 0,"Quito el elemento del hash");
    pa2m_afirmar(hash_cantidad(hash) == 0, "El hash esta vacio al quitar su unico elemento");

    hash_insertar(hash, clave1, &numero_1);
    hash_insertar(hash, clave2, &numero_2);
    hash_insertar(hash, clave3, &numero_3);
    hash_insertar(hash, clave4, &numero_4);
    hash_insertar(hash, clave5, &numero_5);


    void* elemento_obtenido = hash_obtener(hash, clave3);
    pa2m_afirmar(*(int*)elemento_obtenido == numero_3,"Puedo obtener un elemento del hash");
    pa2m_afirmar(hash_quitar(hash, clave3) == 0,"Quito el elemento del hash");
    pa2m_afirmar(!hash_contiene(hash, clave3),"El elemento no esta mas en el hash");
    pa2m_afirmar(hash_obtener(hash, clave3) == NULL,"No puedo obtener el elemento del hash al quitarlo");

    pa2m_afirmar(hash_contiene(hash, clave5),"Puedo encontrar el elemento siguiente al que elimine");

    pa2m_afirmar(hash_obtener(NULL, clave3) == NULL,"No se puede pasar un hash invalido al hash_obtener");
    pa2m_afirmar(hash_obtener(hash, NULL) == NULL,"No se puede pasar una clave invalida al hash_obtener");


    hash_destruir(hash);
}


bool contar_claves_menos_la_ultima(hash_t* hash, const char* clave, void* aux){
  if(!clave)
    return true;

  aux=aux;
  hash=hash;

  return false;
}


void prueboAplicarUnaFuncionACadaElementoDeLaTablaHash(){
    int numero_1 = 548, numero_2 = 96, numero_3 = 821, numero_4 = 335, numero_5 = 88;

    const char* clave1 = "SS";
    const char* clave2 = "FhSS";
    const char* clave3 = "fdss";
    const char* clave4 = "gfFF";
    const char* clave5 = "SsssdeS";

    hash_t* hash = hash_crear(NULL, 5);

    hash_insertar(hash, clave1, &numero_1);
    hash_insertar(hash, clave2, &numero_2);
    hash_insertar(hash, clave3, &numero_3);
    hash_insertar(hash, clave4, &numero_4);
    hash_insertar(hash, clave5, &numero_5);

    pa2m_afirmar(hash_con_cada_clave(hash, contar_claves_menos_la_ultima, NULL) == 5, "Aplico la funcion hasta el elemento especificado");
    hash_quitar(hash, clave3);
    pa2m_afirmar(hash_con_cada_clave(hash, contar_claves_menos_la_ultima, NULL) == 4,"Al eliminar un elemento y aplicar la funcion me devuelve la cantidad correcta");

    pa2m_afirmar(hash_con_cada_clave(NULL, contar_claves_menos_la_ultima, NULL) == 0,"No se puede pasar un hash invalido a hash_con_cada_clave");
    pa2m_afirmar(hash_con_cada_clave(hash, NULL, NULL) == 0,"No se puede pasar una funcion invalida a hash_con_cada_clave");


    hash_destruir(hash);
}


int main(){
    pa2m_nuevo_grupo("Pruebas Con Crear Hash");
    prueboCrearYDestruirUnHash_DevuelveHash();

    pa2m_nuevo_grupo("Pruebas Con Insertar Elementos En El Hash");
    prueboInsertarElementosEnElHash_Devuelve0SiFueExitoso_SiNoDevuelveMenosUno();

    pa2m_nuevo_grupo("Pruebas Con Quitar y Ver Si Contiene Elementos Del Hash");
    prueboContieneYQuitarUnElementoDelHash();

    pa2m_nuevo_grupo("Pruebas Con Quitar y Obtener Elementos Del Hash");
    prueboObtenerYQuitarUnElementoDelHash();

    pa2m_nuevo_grupo("Pruebas Con Aplicar Una Funcion A Cada Elemento De La Tabla Hash");
    prueboAplicarUnaFuncionACadaElementoDeLaTablaHash();

    return pa2m_mostrar_reporte();
}