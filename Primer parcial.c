#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LARGO_CADENA 50
#define NIVEL_MIN 1
#define NIVEL_MAX 100

typedef struct {
	char sector[LARGO_CADENA];
	char nombre[LARGO_CADENA];
	int nivel; 
} Tanque;

void limpiarBufferEntrada(void);
void cargarTanque(Tanque *t);
Tanque* agregarTanque(Tanque *tanques, int *cantidad);
void mostrarTanques(const Tanque *tanques, int cantidad);
void mostrarDosMinimos(const Tanque *tanques, int cantidad);
void buscarPorSector(const Tanque *tanques, int cantidad);
void liberarMemoria(Tanque *tanques);


int main(int argc, char *argv[]) {
	Tanque *tanques = NULL;
	int cantidad = 0;
	int opcion = 0;
	
	do {
		printf("\n===========================================\n");
		printf("  SISTEMA DE MONITOREO DE TANQUES DE AGUA  \n");
		printf("===========================================\n");
		printf("1. Ingresar nuevo tanque\n");
		printf("2. Mostrar todos los tanques registrados\n");
		printf("3. Mostrar los 2 tanques con menor nivel\n");
		printf("4. Buscar tanques por sector (filtro de texto)\n");
		printf("5. Salir\n");
		printf("Seleccione una opcion: ");
		
		if (scanf("%d", &opcion) != 1) {
			printf("Error: Ingrese un numero valido.\n");
			limpiarBufferEntrada();
			continue;
		}
		limpiarBufferEntrada();
		
		switch (opcion) {
		case 1:
			tanques = agregarTanque(tanques, &cantidad);
			break;
		case 2:
			mostrarTanques(tanques, cantidad);
			break;
		case 3:
			mostrarDosMinimos(tanques, cantidad);
			break;
		case 4:
			buscarPorSector(tanques, cantidad);
			break;
		case 5:
			printf("\nLiberando recursos y saliendo del sistema...\n");
			liberarMemoria(tanques);
			tanques = NULL;
			break;
		default:
			printf("\nOpcion invalida. Intente nuevamente.\n");
			break;
		}
	} while (opcion != 5);
	
	return 0;
}

void limpiarBufferEntrada(void) {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}


void cargarTanque(Tanque *t) {
	printf("\n--- Ingrese los datos del nuevo tanque ---\n");
	
	printf("Nombre del sector: ");
	if (fgets(t->sector, LARGO_CADENA, stdin) != NULL) {
		t->sector[strcspn(t->sector, "\n")] = '\0'; 
	}
	
	printf("Nombre del tanque: ");
	if (fgets(t->nombre, LARGO_CADENA, stdin) != NULL) {
		t->nombre[strcspn(t->nombre, "\n")] = '\0';
	}
	

	do {
		printf("Nivel de agua (%d - %d %%): ", NIVEL_MIN, NIVEL_MAX);
		if (scanf("%d", &t->nivel) != 1) {
			printf("Entrada invalida. Ingrese un valor numerico.\n");
			limpiarBufferEntrada();
			t->nivel = 0;
			continue;
		}
		limpiarBufferEntrada();
		
		if (t->nivel < NIVEL_MIN || t->nivel > NIVEL_MAX) {
			printf("El nivel debe estar comprendido entre %d%% y %d%%.\n", NIVEL_MIN, NIVEL_MAX);
		}
	} while (t->nivel < NIVEL_MIN || t->nivel > NIVEL_MAX);
}


Tanque* agregarTanque(Tanque *tanques, int *cantidad) {
	int nuevaCantidad = *cantidad + 1;
	Tanque *temp = (Tanque*) realloc(tanques, nuevaCantidad * sizeof(Tanque));
	
	if (temp == NULL) {
		printf("Error: Memoria insuficiente para registrar un nuevo tanque.\n");
		return tanques;
	}
	
	tanques = temp;
	cargarTanque(&tanques[*cantidad]);
	*cantidad = nuevaCantidad;
	
	printf("\n>> Tanque registrado exitosamente. Total cargados: %d\n", *cantidad);
	return tanques;
}


void mostrarTanques(const Tanque *tanques, int cantidad) {
	if (cantidad == 0) {
		printf("\nNo hay tanques registrados en el sistema.\n");
		return;
	}
	
	printf("\n%-20s %-20s %-10s\n", "SECTOR", "NOMBRE", "NIVEL");
	printf("----------------------------------------------------\n");
	for (int i = 0; i < cantidad; i++) {
		printf("%-20s %-20s %3d%%\n", tanques[i].sector, tanques[i].nombre, tanques[i].nivel);
	}
}

void mostrarDosMinimos(const Tanque *tanques, int cantidad) {
	if (cantidad < 2) {
		printf("\nSe requieren al menos 2 tanques registrados para comparar minimos (actuales: %d).\n", cantidad);
		return;
	}
	
	int idxMin1 = 0;
	int idxMin2 = 1;
	
	if (tanques[idxMin2].nivel < tanques[idxMin1].nivel) {
		idxMin1 = 1;
		idxMin2 = 0;
	}
	
	for (int i = 2; i < cantidad; i++) {
		if (tanques[i].nivel < tanques[idxMin1].nivel) {
			idxMin2 = idxMin1;
			idxMin1 = i;
		} else if (tanques[i].nivel < tanques[idxMin2].nivel) {
			idxMin2 = i;
		}
	}
	
	printf("\n=== LOS 2 TANQUES CON MENOR NIVEL DE AGUA ===\n");
	printf("1) Tanque: %s | Sector: %s | Nivel: %d%%\n", 
		   tanques[idxMin1].nombre, tanques[idxMin1].sector, tanques[idxMin1].nivel);
	printf("2) Tanque: %s | Sector: %s | Nivel: %d%%\n", 
		   tanques[idxMin2].nombre, tanques[idxMin2].sector, tanques[idxMin2].nivel);
}


void buscarPorSector(const Tanque *tanques, int cantidad) {
	if (cantidad == 0) {
		printf("\nNo hay tanques para buscar.\n");
		return;
	}
	
	char busqueda[LARGO_CADENA];
	printf("\nIngrese el nombre o fragmento del sector a buscar: ");
	if (fgets(busqueda, LARGO_CADENA, stdin) != NULL) {
		busqueda[strcspn(busqueda, "\n")] = '\0';
	}
	
	int coincidencias = 0;
	printf("\nResultados de la busqueda para \"%s\":\n", busqueda);
	for (int i = 0; i < cantidad; i++) {
		if (strstr(tanques[i].sector, busqueda) != NULL) {
			printf(" - [%s] Tanque: %s (Nivel: %d%%)\n", 
				   tanques[i].sector, tanques[i].nombre, tanques[i].nivel);
			coincidencias++;
		}
	}
	
	if (coincidencias == 0) {
		printf("No se encontraron tanques en el sector indicado.\n");
	}
}

void liberarMemoria(Tanque *tanques) {
	if (tanques != NULL) {
		free(tanques);
	}
}
