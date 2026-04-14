#include "server.h"
#include <pthread.h>

int main(void) {
	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");

	while (1) {
    	pthread_t thread;
    	int *fd_client_ptr = malloc(sizeof(int));
    	*fd_client_ptr = esperar_cliente(server_fd);
    	pthread_create(&thread, NULL, (void*) atender_cliente, fd_client_ptr);
    	pthread_detach(thread);
    }

	return EXIT_SUCCESS;
}

void atender_cliente(int* fd_client_ptr) {
	t_list* lista;
	int fd_client = *fd_client_ptr;
	while (1) {
		int cod_op = recibir_operacion(fd_client);
		switch (cod_op) {
			case MENSAJE:
				recibir_mensaje(fd_client);
				break;
			case PAQUETE:
				lista = recibir_paquete(fd_client);
				log_info(logger, "Me llegaron los siguientes valores:\n");
				list_iterate(lista, (void*) iterator);
				break;
			case -1:
				log_info(logger, "El cliente se desconecto.");
				break;
			default:
				log_warning(logger,"Operacion desconocida. No quieras meter la pata");
				break;
		}
	}
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}
