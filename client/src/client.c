#include "client.h"


int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	log_info(logger,"Hola! Soy un log."); //loguea el mensaje
	//log_destroy(logger); //destruye el log

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	valor = config_get_string_value(config,"CLAVE"); //obtengo valor
	ip = config_get_string_value(config,"IP");
	puerto = config_get_string_value(config,"PUERTO");
	printf("Puerto leido: %s\n", puerto);
	log_info(logger,"Config leido - IP: %s, Puerto: %s, Clave: %s", ip, puerto, valor); //uso el logger para mostrar el valor , %s es que muestra un string
	// Loggeamos el valor de config


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	
	enviar_mensaje(valor,conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log","CLIENTE",true,LOG_LEVEL_INFO); //devuelve un logger listo para usa los parametros son el archivo donde va a guardar los logs, el nombre, si quiero mostrar por consola o no, y a que nivel.
	if (nuevo_logger== NULL) {
        // Si entra acá, es que la ruta está mal o el archivo no existe
        perror("Error al cargar el logger"); 
        exit(EXIT_FAILURE); 
    }
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");
	if (nuevo_config == NULL) {
        // Si entra acá, es que la ruta está mal o el archivo no existe
        perror("Error al cargar el config"); 
        exit(EXIT_FAILURE); 
    }
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	while(leido!=NULL && strcmp(leido,"")!=0 )
	{
		log_info(logger,"%s",leido);
		free(leido);//// Liberamos lo que ya usamos

		leido = readline("> ");//Volver a pedir otra línea para que el bucle siga o termine
	}
	if(leido!=NULL)
	{
		free(leido);
	}
	//liberar este último string vacío
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío


	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete
	paquete = crear_paquete();
	leido = readline("> ");

	while(leido!=NULL && strcmp(leido,"")!=0)
	{
		agregar_a_paquete(paquete,leido,strlen(leido)+1);
		free(leido);
		leido = readline("> "); //por si quiero enviar mas cosas
	}
	free(leido);
	enviar_paquete(paquete,conexion);
	eliminar_paquete(paquete);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	  log_destroy(logger);
	  config_destroy(config);
	  liberar_conexion(conexion);
}
