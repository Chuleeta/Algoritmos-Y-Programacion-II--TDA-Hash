#include <stdlib.h>
#include "hash.h"
#include "lista.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define ERROR -1
#define EXITO 0

typedef struct valor_nodo{
	char* clave;
	void* elemento;
}valor_nodo_t;


struct hash{
	size_t cantidad_elementos_totales;
	
	lista_t** lista_de_hash;
	size_t cantidad_de_listas;
	size_t cantidad_de_casilleros;

	hash_destruir_dato_t destruir_elemento;
};


hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad_inicial){
	hash_t* hash = calloc(1, sizeof(hash_t));
	if(!hash)
		return NULL;

	hash->destruir_elemento = destruir_elemento;	

	
	size_t capacidad_inicial_final = 0;
	if(capacidad_inicial <= 3){
		capacidad_inicial_final = 3;
	}else{
		capacidad_inicial_final = capacidad_inicial;
	}
	
	lista_t** lista = calloc(capacidad_inicial_final, sizeof(lista_t*));

	if(!lista){
		free(lista);
		return NULL;
	}

	hash->lista_de_hash = lista;
	hash->cantidad_de_casilleros = capacidad_inicial_final;

	return hash;
}


/*
* Pre Condiciones: Recibe una clave y un hash
* Post Condiciones: Devuelve la posicion del hash que debe ir la calve
*/
size_t clave_hash(const char* clave, hash_t* hash){
	size_t resultado = 0;
	while(*clave){
		resultado += *(clave++);
	}
	return (resultado % hash->cantidad_de_casilleros);
}


/*
* Pre Condiciones: Recibe un hash, el numero de hash donde esta la lista, una clave y un elemento a insertar
* Post Condiciones:	Recorre la lista indicada, busca si hay un nodo con la misma clave, lo actualiza y 
* devuelve true. De no encontrar ninguno igual devuelve false. 
*/
bool buscar_y_actualizar_elemento_en_lista(hash_t* hash, char* clave, void* elemento, size_t numero_hash){
	lista_iterador_t* iterador;
	void* elemento_a_comparar;

	if(lista_tamanio(hash->lista_de_hash[numero_hash]) > 1){
		for(iterador = lista_iterador_crear(hash->lista_de_hash[numero_hash]);
			lista_iterador_tiene_siguiente(iterador);
			lista_iterador_avanzar(iterador)){

			elemento_a_comparar = lista_iterador_elemento_actual(iterador);

			if(strcmp(((valor_nodo_t*)elemento_a_comparar)->clave, clave) == 0){
				if(hash->destruir_elemento)
					hash->destruir_elemento(((valor_nodo_t*)elemento_a_comparar)->elemento);
				((valor_nodo_t*)elemento_a_comparar)->elemento = elemento;
				lista_iterador_destruir(iterador);
				free(clave);
				return true;
			}
		}

		lista_iterador_destruir(iterador);
	}else if(lista_tamanio(hash->lista_de_hash[numero_hash]) == 1){
		elemento_a_comparar = lista_primero(hash->lista_de_hash[numero_hash]);
		if(strcmp(((valor_nodo_t*)elemento_a_comparar)->clave, clave) == 0){
			if(hash->destruir_elemento)
				hash->destruir_elemento(((valor_nodo_t*)elemento_a_comparar)->elemento);
			((valor_nodo_t*)elemento_a_comparar)->elemento = elemento;
			free(clave);
			return true;
		}
	}

	return false;
}


/*
* Pre Condiciones: Recibe un hash, un numero hash que nos da la ubicacion de la lista, una clave y un elemento
* a insertar.
* Post Condiciones: Recorre la lista y si encuentra una clave igual actualiza su elemento. De no ser asi inserta
* la clave y el elemento al final de la lista en un nuevo nodo.
*/
void insertar_en_lista_hash(size_t numero_hash, hash_t* hash, char* clave, void* elemento){
	if(!hash->lista_de_hash[numero_hash]){
		lista_t* lista_nueva = lista_crear();
		if(!lista_nueva) return;

		hash->lista_de_hash[numero_hash] = lista_nueva;
	}
	if((lista_tamanio(hash->lista_de_hash[numero_hash]) > 0) &&
	 (buscar_y_actualizar_elemento_en_lista(hash, clave, elemento, numero_hash))){
		return;
	}

	valor_nodo_t* nodo_a_insertar = malloc(sizeof(valor_nodo_t));
	if(!nodo_a_insertar)
		return;

	nodo_a_insertar->clave = clave;
	nodo_a_insertar->elemento = elemento;
	void* elemento_a_insertar = nodo_a_insertar;
	if(lista_tamanio(hash->lista_de_hash[numero_hash]) < 1)
		hash->cantidad_de_listas++;
	hash->cantidad_elementos_totales++;

	lista_insertar(hash->lista_de_hash[numero_hash], elemento_a_insertar);
}


/*
* Pre Condiciones: Recibe un nodo de la lista
* Post Condiciones: Libera cada nodo de la lista
*/
bool destruir_cada_elemento(void* elemento_a_destruir, void* contexto){
	if(!elemento_a_destruir || contexto)
		return false;
	free(elemento_a_destruir);
	return true;
}


/*
* Pre Condiciones: Recibe una lista de lista_t y la cantidad de casilleros que hay en total
* Post Condiciones: Recorre cada una eliminando cada nodo
*/
void eliminar_lista(lista_t** lista, size_t cantidad_de_casilleros){
	if(!lista)
		return;

	int j = 0;
	for(int z = 0; z < cantidad_de_casilleros; z++){
		if(lista[j]){
			lista_con_cada_elemento(lista[j], destruir_cada_elemento, NULL);
			lista_destruir(lista[j]);
		}
		j++;
	}
	free(lista);
}


/*
* Pre Condiciones: Recibe un hash
* Post Condiciones: Crea una lista de listas_t nueva y reinserta cada nodo en la nueva lista. Al 
* final asigna la nueva lista de lista_t al hash y elimina la anterior
*/
void rehasheo_lista(hash_t* hash){
	if(!hash)
		return;
	lista_t** lista_nueva = calloc((hash->cantidad_de_casilleros)*2, sizeof(lista_t*));
	if(!lista_nueva)
		return;
	
	lista_iterador_t* iterador;
	void* elemento_a_comparar;
	
	size_t cantidad_de_casilleros_lista_vieja = hash->cantidad_de_casilleros;
	lista_t** lista_de_hash_vieja = hash->lista_de_hash;
	
	hash->lista_de_hash = lista_nueva;
	hash->cantidad_de_casilleros = ((hash->cantidad_de_casilleros)*2);
	hash->cantidad_elementos_totales = 0;

	for (int i = 0; i < cantidad_de_casilleros_lista_vieja; ++i){
		if(lista_tamanio(lista_de_hash_vieja[i]) > 1){
			for(iterador = lista_iterador_crear(lista_de_hash_vieja[i]);
				lista_iterador_tiene_siguiente(iterador);
				lista_iterador_avanzar(iterador)){

				elemento_a_comparar = lista_iterador_elemento_actual(iterador);

				size_t nuevo_index_hash = clave_hash(((valor_nodo_t*)elemento_a_comparar)->clave, hash);

				insertar_en_lista_hash(nuevo_index_hash, hash, ((valor_nodo_t*)elemento_a_comparar)->clave, ((valor_nodo_t*)elemento_a_comparar)->elemento);
			}
			lista_iterador_destruir(iterador);
		}
		else if(lista_tamanio(lista_de_hash_vieja[i]) == 1){
			elemento_a_comparar = lista_primero(lista_de_hash_vieja[i]);
			size_t nuevo_index_hash = clave_hash(((valor_nodo_t*)elemento_a_comparar)->clave, hash);
			insertar_en_lista_hash(nuevo_index_hash, hash, ((valor_nodo_t*)elemento_a_comparar)->clave, ((valor_nodo_t*)elemento_a_comparar)->elemento);
		}
	}

	
	eliminar_lista(lista_de_hash_vieja, cantidad_de_casilleros_lista_vieja);
}


/*
* Pre Condiciones: Recibe un hash
* Post Condiciones: Verifica si el hash supero el umbral para hacer el rehasheo
*/
bool debo_rehashear(hash_t* hash){
	if(!hash)
		return false;

	if(((hash->cantidad_de_listas*100)/hash->cantidad_de_casilleros) >= 75)
		return true;
	
	return false;
}


int hash_insertar(hash_t* hash, const char* clave, void* elemento){
	if(!hash || !clave)
		return ERROR;

	char* clave_copia = malloc(strlen(clave)+1);
	if(!clave_copia)
		return ERROR;
	strcpy(clave_copia, clave);

	if(debo_rehashear(hash))
		rehasheo_lista(hash);
	
	size_t numero_hash = clave_hash(clave_copia, hash);

	insertar_en_lista_hash(numero_hash, hash, clave_copia, elemento);
	
	return EXITO;
}


/*
* Pre Condiciones: Recibe dos elementos
* Post Condiciones: Copara si son iguales
*/
bool encontrar_elemento(void* elemento_a_comparar, void* elemento_buscado){
	if(strcmp(((valor_nodo_t*)elemento_a_comparar)->clave, (char*)elemento_buscado) == 0){
		return false;
	}
	return true;
}


int hash_quitar(hash_t* hash, const char* clave){
	if(!hash || !clave)
		return ERROR;

	bool se_elimino_algo = false;
	size_t numero_hash = clave_hash(clave, hash);


	char* clave_copia = malloc(strlen(clave)+1);
	if(!clave_copia)
		return ERROR;
	strcpy(clave_copia, clave);
	void* contexto = clave_copia;

	size_t posicion_elemento = lista_con_cada_elemento(hash->lista_de_hash[numero_hash], encontrar_elemento, contexto);
	
	void* elemento_sin_eliminado = lista_elemento_en_posicion(hash->lista_de_hash[numero_hash], posicion_elemento);
	if(!elemento_sin_eliminado || strcmp(((valor_nodo_t*)elemento_sin_eliminado)->clave, clave_copia) != 0){
		free(clave_copia);
		return ERROR;
	}
	
	void* elemento_eliminado = lista_quitar_de_posicion(hash->lista_de_hash[numero_hash], posicion_elemento);

	if(elemento_eliminado && strcmp(((valor_nodo_t*)elemento_eliminado)->clave, clave_copia) == 0)
		se_elimino_algo = true;
	
	if(!se_elimino_algo || !elemento_eliminado){
		free(clave_copia);
		return ERROR;
	}

	if(hash->destruir_elemento)
		hash->destruir_elemento(((valor_nodo_t*)elemento_eliminado)->elemento);
	free(((valor_nodo_t*)elemento_eliminado)->clave);
	
	if(lista_vacia(hash->lista_de_hash[numero_hash]))
		hash->cantidad_de_listas--;
	hash->cantidad_elementos_totales--;
	free(clave_copia);
	free(elemento_eliminado);

	return EXITO;
}


void* hash_obtener(hash_t* hash, const char* clave){
	if(!hash || !clave)
		return NULL;
	size_t numero_hash = clave_hash(clave, hash);

	char* clave_copia = malloc(strlen(clave)+1);
	if(!clave_copia)
		return NULL;
	strcpy(clave_copia, clave);
	void* contexto = clave_copia;
	
	size_t posicion_elemento = lista_con_cada_elemento(hash->lista_de_hash[numero_hash], encontrar_elemento, contexto);
	void* elemento_eliminado = lista_elemento_en_posicion(hash->lista_de_hash[numero_hash], posicion_elemento);
	if(elemento_eliminado && strcmp(((valor_nodo_t*)elemento_eliminado)->clave, clave_copia) == 0){
		free(clave_copia);
		return ((valor_nodo_t*)elemento_eliminado)->elemento;
	}
	free(clave_copia);
	return NULL;
}


bool hash_contiene(hash_t* hash, const char* clave){
	if(!hash || !clave)
		return false;
	size_t numero_hash = clave_hash(clave, hash);

	char* clave_copia = malloc(strlen(clave)+1);
	if(!clave_copia)
		return false;
	strcpy(clave_copia, clave);
	void* contexto = clave_copia;
	
	size_t posicion_elemento = lista_con_cada_elemento(hash->lista_de_hash[numero_hash], encontrar_elemento, contexto);
	void* elemento_eliminado = lista_elemento_en_posicion(hash->lista_de_hash[numero_hash], posicion_elemento);
	if(elemento_eliminado && strcmp(((valor_nodo_t*)elemento_eliminado)->clave, clave_copia) == 0){
		free(clave_copia);
		return true;
	}
	free(clave_copia);
	return false;
}


size_t hash_cantidad(hash_t* hash){
	if(!hash)
		return 0;
	return hash->cantidad_elementos_totales;
}


void hash_destruir(hash_t* hash){
	if(!hash)
		return;
	
	lista_iterador_t* iterador;
	void* elemento_a_comparar;

	for (int i = 0; i < hash->cantidad_de_casilleros; ++i){
		if(lista_tamanio(hash->lista_de_hash[i]) > 1){
			for(iterador = lista_iterador_crear(hash->lista_de_hash[i]);
				lista_iterador_tiene_siguiente(iterador);
				lista_iterador_avanzar(iterador)){

				elemento_a_comparar = lista_iterador_elemento_actual(iterador);
				if(hash->destruir_elemento)
					hash->destruir_elemento(((valor_nodo_t*)elemento_a_comparar)->elemento);
				free(((valor_nodo_t*)elemento_a_comparar)->clave);
			}
			lista_iterador_destruir(iterador);
		}
		else if(lista_tamanio(hash->lista_de_hash[i]) == 1){
			elemento_a_comparar = lista_primero(hash->lista_de_hash[i]);
			if(hash->destruir_elemento)
				hash->destruir_elemento(((valor_nodo_t*)elemento_a_comparar)->elemento);
			free(((valor_nodo_t*)elemento_a_comparar)->clave);
		}
	}

	//libera la lista hash
	eliminar_lista(hash->lista_de_hash, hash->cantidad_de_casilleros);
	free(hash);
}


size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){
	if(!hash || !funcion)
		return 0;
	size_t resultado = 0;
	lista_iterador_t* iterador;
	void* elemento_a_comparar;

	for (int i = 0; i < hash->cantidad_de_casilleros; ++i){
		if(lista_tamanio(hash->lista_de_hash[i]) > 1){
			for(iterador = lista_iterador_crear(hash->lista_de_hash[i]);
				lista_iterador_tiene_siguiente(iterador);
				lista_iterador_avanzar(iterador)){

				elemento_a_comparar = lista_iterador_elemento_actual(iterador);

				resultado++;
				if(funcion(hash, ((valor_nodo_t*)elemento_a_comparar)->clave, aux))
					return resultado;
			}
			lista_iterador_destruir(iterador);
		}
		else if(lista_tamanio(hash->lista_de_hash[i]) == 1){
			elemento_a_comparar = lista_primero(hash->lista_de_hash[i]);
			resultado++;
			if(funcion(hash, ((valor_nodo_t*)elemento_a_comparar)->clave, aux))
				return resultado;
		}
	}

	return resultado;
}