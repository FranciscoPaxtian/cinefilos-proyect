//mpicc -o conex conexion_mpi.c -I /usr/include/postgresql/ -lpq
#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <libpq-fe.h>
#include<fcntl.h>
#define BUFSIZE 128
#define TAG 0
char tab[50], cad[100];
int op,opc;
char nom[50],tel[10],dir[50], mes[2], fecha[10], titulo[30], genero[20];
char nomlocal[30],tellocal[10],dirlocal[50];
int IDsocio, IDlocal, Nr, res, IDRenta, IDpelicula, cantidad, total, abono, liq, con, precio, exis;
int IDempresa=1;
//CREADO POR FRANCISCO JAVIER PAXTIAN GORDILLO

int main(int argc, char *argv[])
{
    int id; /*IDENTIFICADOR DEL PROCESO*/
    int numNucleos; /*NUMERO DE PROCESOS*/
    char nombreproc [MPI_MAX_PROCESSOR_NAME]; /*NOMBRE PROCESADOR*/
    int lnombreproc; /*LONGITUD NOMBRE PROCESADOR*/
    double tmpinic =0.0; /*TIEMPO INICIO DE LA EJECUCION*/
    double tmpfin ; /*TIEMPO FINAL DE LA EJECUCION*/
    PGconn *bd;  //crear que almacena la conexion con una BDs en postgres
	PGresult *resultado;
	int i,j,fila,columna,edad,nplaza;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_rank (MPI_COMM_WORLD,&id) ;
    MPI_Comm_size(MPI_COMM_WORLD, &numNucleos) ;
    MPI_Get_processor_name ( nombreproc,& lnombreproc ) ;
    if(id==0){
    fprintf(stdout,"\nProceso %d en el procesador: %s --->Encargado de la E/S\n", id , nombreproc) ;
        printf("Numero Procesos: %d\n",numNucleos);
	bd=PQsetdbLogin("localhost","5432",NULL,NULL,"cinefilos","postgres","paxtian1606");
	printf("CONECTANDO CON POSTGRESQL\n");
      if(PQstatus(bd) == CONNECTION_OK){
do{
    //Menu del sistema
    printf("***VIDEOCLUB LOS CINEFILOS***\n");
    printf("********MENU PRINCIPAL*******\n");
    printf("1. |Menu del socio\n");
    printf("2. |Menu del local\n");
    printf("3. |Peliculas rentadas en la empresa\n");
    printf("4. |Socios sin rentas\n");
    printf("5. |Recibos por mes\n");
    printf("6. |Movimientos del socio\n");
    printf("7. |Agregar detalles de renta\n");
    printf("8. |Menu de peliculas\n");
    printf("9. |Salir\n");
    scanf("%d",&op);

switch(op){
    case 1:
    do{
        //Menu del socio
		printf("\tMENU DEL SOCIO\n");
		printf("1.\t Consulta del socio\n");
		printf("2.\t Alta del socio\n");
		printf("3.\t Eliminacion del socio\n");
		printf("4.\t Cantidad de rentas hechas por el socio\n");
		printf("5.\t Actualizacion de rentas de un socio\n");
		printf("6.\t Regresar *****\n");
				
		printf("\nIngrese la opcion que desee\n");
		scanf("%d",&opc);
			
		if(opc==1){
            //Consulta de datos del socio    
			printf("\nConsulta Socios\n");
            sprintf(cad,"select * from socio");
            printf("\nID SOCIO  |  ID LOCAL  |  NOMBRE  |  TELEFONO  |  DIRECCION  |  RENTAS\n");
		    resultado = PQexec(bd, cad);
		    if(resultado != NULL){
                fila=PQntuples(resultado);
                columna=PQnfields(resultado);
                for (i = 0; i < fila ; i++){
                     for (j = 0; j < columna; j++){
                        printf("%s\t",PQgetvalue(resultado,i,j));
				     }
				     printf("\n");
			    }
		    }else{
                 printf("NO HAY USUARIOS REGISTRADOS");
            }
	    }
		if(opc==2){
			//Alta del socio		
			printf("\nAlta Socios\n");
				
			printf("Ingresa el ID del del socio: \n");
			scanf("%d",&IDsocio);
			printf("Ingresa el ID del del local: \n");
			scanf("%d",&IDlocal);
			printf("Ingresa el nombre del socio: \n");
			scanf("%s",&nom);
			printf("Ingresa el telefono del socio: \n");
			scanf("%s",&tel);	
			printf("Ingresa la direccion del socio: \n");
			scanf("%s",&dir);
			printf("Ingresa el numero de rentas del socio: \n");
			scanf("%d",&Nr);
				
			sprintf(cad,"insert into Socio values(%d,%d,'%s','%s','%s',%d)",IDsocio,IDlocal,nom,tel,dir,Nr);
			//printf("Instruccion SQL antes de ejcutarse: %s \n",cad);
			PQexec(bd, cad);
        }
		if(opc==3){
            //Eliminar socio
			printf("\nEliminar Socio\n");
				
			printf("Ingrese la ID del socio que desea dar de baja\n");
			scanf("%d",&IDsocio);
	
			sprintf(cad,"delete from Socio where ID_Socio='%d'",IDsocio);
			//printf("Instruccion SQL antes de ejecutarse: %s \n",cad);
				
			puts("\n Socio dado de baja \n");
			PQexec(bd, cad);	
		}
		if(opc==4){
			//Rentas hechas por el socio	
			printf("\nRentas hechas por el socio\n");
				
			printf("Ingrese el ID del socio que desea saber el total de rentas\n");
			scanf("%d",&IDsocio);
				
			sprintf(cad,"select Nombre, Numero_Rentas from Socio where ID_Socio='%d'",IDsocio);
			printf("\nTotal de rentas hechas por el socio\n");
			resultado = PQexec(bd, cad);
			if(resultado != NULL){
               	fila=PQntuples(resultado);columna=PQnfields(resultado);
          			for (i = 0; i < fila ; i++){
              			for (j = 0; j < columna; j++){
                       		printf("%s\t",PQgetvalue(resultado,i,j));
						}
						printf("\n");
					}
			}						
		}
		if(opc==5){
            //Actualizar el No de rentas que ha hecho el socio
	        printf("Ingrese el ID del socio que desee Actualizar las rentas que ha hecho\n");
	        scanf("%d",&IDsocio);
					
	        printf("\nIngrese la nueva cantidad de rentas hechas por el socio\n");
	        scanf("%d",&Nr);
								
	        sprintf(cad,"update socio set Numero_rentas='%d' where ID_Socio='%d';",Nr,IDsocio);
				
	        puts("\n Total de rentas actualizado \n");
	        PQexec(bd, cad);
					
								
		}
	}while(opc!=6);

    break;
case 2:
do{
        //Menu del local
		printf("\tMENU DEL LOCAL\n");
		printf("1.\t Consulta del local\n");
		printf("2.\t Alta del local\n");
		printf("3.\t Eliminacion del local\n");
		printf("4.\t Regresar *****\n");
				
		printf("\nIngrese la opcion que desee\n");
		scanf("%d",&opc);
			
		if(opc==1){
        //Consulta de datos de los locales    
	    sprintf(cad,"select * from local");
        printf("\nLista de locales existentes\n");
        printf("\nID LOCAL  |  ID EMPRESA  |  NOMBRE  |  TELEFONO  | DIRECCION\n");
        resultado = PQexec(bd, cad);
       	if(resultado != NULL){
            fila=PQntuples(resultado);
            columna=PQnfields(resultado);
            for (i = 0; i < fila ; i++){
                for (j = 0; j < columna; j++){
                    printf("%s\t",PQgetvalue(resultado,i,j));
		    	}
			    printf("\n");
	    	}
	    }else{
                 printf("NO HAY USUARIOS REGISTRADOS");
            }
	    }
		if(opc==2){
			//Alta del local		
			printf("\nAlta del local\n");
				
			printf("Ingresa el ID del del local: \n");
			scanf("%d",&IDlocal);
			printf("Ingresa el nombre del local: \n");
			printf("AVISO EL NOMBRE DEBE DE SER DE UNA PELICULA DE CIENCIA FICCION \n");
			scanf("%s",&nomlocal);
			printf("Ingresa el telefono del local: \n");
			scanf("%s",&tellocal);	
			printf("Ingresa la direccion del local: \n");
			scanf("%s",&dirlocal);
				
			sprintf(cad,"insert into Local values(%d,%d,'%s','%s','%s')",IDlocal,IDempresa,nomlocal,tellocal,dirlocal);
			printf("Instruccion SQL antes de ejcutarse: %s \n",cad);
			PQexec(bd, cad);
        }
		if(opc==3){
            //Eliminar un local
			printf("\nEliminar un local\n");
				
			printf("Ingrese la ID del local que desea dar de baja\n");
			scanf("%d",&IDlocal);
	
			sprintf(cad,"delete from Local where ID_local='%d'",IDlocal);
			//printf("Instruccion SQL antes de ejecutarse: %s \n",cad);
				
			puts("\n Local dado de baja \n");
			PQexec(bd, cad);	
		}

	}while(opc!=4);
    break;
case 3:
    //Suma de todas las rentas de peliculas que se han hecho en la empresa
    resultado=PQexec(bd, "select sum(Numero_Rentas) from Pelicula");
    //printf("Filas:%d, Columnas:%d\n", PQntuples(resultado), PQnfields(resultado));
    if(resultado != NULL){
        printf("\nTotal de peliculas rentadas por la empresa\n");
        for(i=0;i<PQntuples(resultado);i++){
            for(j=0;j<PQnfields(resultado);j++){
                printf("%s\t",PQgetvalue(resultado,i,j));
            }
            printf("\n");
        }
    resultado = PQexec(bd, cad);
    }
    //Mas detalles de las rentas de las peliculas
    resultado=PQexec(bd, "select Titulo, Genero, Numero_Rentas from pelicula");
    if(resultado != NULL){
        printf("\nDetalles\n");
        printf("\nTITULO  |  GENERO  |  RENTAS\n");
        for(i=0;i<PQntuples(resultado);i++){
            for(j=0;j<PQnfields(resultado);j++){
                printf("%s\t",PQgetvalue(resultado,i,j));
            }
            printf("\n");
        }
    resultado = PQexec(bd, cad);
    }

    break;
case 4:
    //Consulta de usuarios sin rentas    
    sprintf(cad,"select ID_Socio, Nombre, ID_Local from socio where Numero_Rentas=0");
    printf("\nLista de usuarios sin rentas\n");
    printf("\nID SOCIO  |  NOMBRE DEL SOCIO  |  ID LOCAL\n");
    resultado = PQexec(bd, cad);
		if(resultado != NULL){
            fila=PQntuples(resultado);
            columna=PQnfields(resultado);
            for (i = 0; i < fila ; i++){
                for (j = 0; j < columna; j++){
                    printf("%s\t",PQgetvalue(resultado,i,j));
				}
				printf("\n");
			}
		}
    break;
case 5:
    //ver recibos por mes
	printf("Ingrese el numero del mes que desea ver los recibos \n");
	printf("NOTA: INGRESELO EN EL FORMATO MM \n");
	scanf("%s",&mes);

    sprintf(cad,"select ID_Renta from Renta WHERE TO_CHAR(Fecha_Renta,'MM') = '%s'",mes);

    printf("\nIDS DE RECIBOS CREADOS EN EL MES SELECCIONADO\n");
    printf("\nNOTA: SI NO APARECE NADA EN EL MES QUE PUSO ES PORQUE NO HUBO RENTAS EN ESE MES\n");
    resultado = PQexec(bd, cad);
		if(resultado != NULL){
            fila=PQntuples(resultado);
            columna=PQnfields(resultado);
            for (i = 0; i < fila ; i++){
                for (j = 0; j < columna; j++){
                    printf("%s\t",PQgetvalue(resultado,i,j));
				}
				printf("\n");
			}
		}
    break;

case 6:
do{
        //menu de los movimientos del socio
		printf("\tMOVIMIENTOS DEL SOCIO\n");
		printf("1.\t Rentas hechas por el socio\n");
		printf("2.\t Detalles de la renta del socio\n");
		printf("3.\t Regresar *****\n");
				
		printf("\nIngrese la opcion que desee\n");
		scanf("%d",&opc);
			
		if(opc==1){
            //consulta de las rentas del socio
			printf("\nConsulta de rentas del socio\n");
				
			printf("Ingrese el ID del socio que desea consultar sus rentas\n");
			scanf("%d",&IDsocio);
				
			sprintf(cad,"select * from Renta where ID_Socio='%d'",IDsocio);
            printf("\nID RENTA  |  ID LOCAL  |  ID SOCIO  |  FECHA  \n");
			resultado = PQexec(bd, cad);
			if(resultado != NULL){
               	fila=PQntuples(resultado);columna=PQnfields(resultado);
          			for (i = 0; i < fila ; i++){
              			for (j = 0; j < columna; j++){
                       		printf("%s\t",PQgetvalue(resultado,i,j));
						}
						printf("\n");
					}
			}		
	    }
		if(opc==2){
            //consulta del detalle de la renta segun la id de renta
			printf("\nConsulta de detalles de renta del socio\n");
				
			printf("Ingrese el ID de renta para poder ver los detalles de renta\n");
			scanf("%d",&IDRenta);
				
			sprintf(cad,"select * from Det_Renta where ID_Renta='%d'",IDRenta);
            printf("\nID RENTA  |  ID PELICULA  |  CANTIDAD  |  TOTAL  |  ABONO  |  A LIQUIDAR  |  CNS  \n");
			resultado = PQexec(bd, cad);
			if(resultado != NULL){
               	fila=PQntuples(resultado);columna=PQnfields(resultado);
          			for (i = 0; i < fila ; i++){
              			for (j = 0; j < columna; j++){
                       		printf("%s\t",PQgetvalue(resultado,i,j));
						}
						printf("\n");
					}
			}
        }

	}while(opc!=3);

    break;

case 7:
    //pregunta antes de agregar una renta hecha(recibo)
    printf("\nDesea agregar un renta?\n");
    printf("\n1=SI | 2=NO\n");
    scanf("%d",&res);
    if(res==1){
		printf("\nPrimero agregue los datos generales de la renta\n");
				
		printf("Ingresa el ID de renta: \n");
		scanf("%d",&IDRenta);
		printf("Ingresa el ID del local: \n");
		scanf("%d",&IDlocal);
		printf("Ingresa el ID del socio: \n");
		scanf("%d",&IDsocio);
		printf("Ingresa la fecha de la renta: \n");
		printf("NOTA: ingresar en el formato DD-MM-AAAA \n");
		scanf("%s",&fecha);	
				
		sprintf(cad,"insert into Renta values(%d,%d,%d,'%s')",IDRenta,IDlocal,IDsocio,fecha);
		//printf("Instruccion SQL antes de ejcutarse: %s \n",cad);
		PQexec(bd, cad);    

		printf("\nAhora agregue los detalles de la renta\n");

		printf("Ingresa el ID de la pelicula: \n");
		scanf("%d",&IDpelicula);
		printf("Ingresa la cantidad rentada: \n");
		scanf("%d",&cantidad);
		printf("Ingresa el precio total de lo rentado: \n");
		scanf("%d",&total);	
		printf("Ingresa el abono dado por el socio: \n");
		scanf("%d",&abono);
		printf("Ingresa el total a liquidar: \n");
		scanf("%d",&liq);
		printf("Ingresa el consecutivo: \n");
		scanf("%d",&con);
				
		sprintf(cad,"insert into Det_Renta values(%d,%d,%d,%d,%d,%d,%d)",IDRenta,IDpelicula,cantidad,total,abono,liq,con);
		//printf("Instruccion SQL antes de ejcutarse: %s \n",cad);
		PQexec(bd, cad); 
        printf("\nGuardado exitosamente\n");
    }
    break;
case 8:
    do{
        //Menu de las peliculas
		printf("\tMENU DE LAS PELICULAS\n");
		printf("1.\t Consulta de peliculas por local\n");
		printf("2.\t Alta de peliculas\n");
		printf("3.\t Eliminar peliculas\n");
		printf("4.\t Actualizar precio de una pelicula\n");
		printf("5.\t Actualizar numero de rentas de una pelicula\n");
		printf("6.\t Actualizar existencias de una pelicula\n");
		printf("7.\t Regresar *****\n");
				
		printf("\nIngrese la opcion que desee\n");
		scanf("%d",&opc);
			
		if(opc==1){
        //Consulta de datos de las peliculas    
	    sprintf(cad,"select * from pelicula");
        printf("\nLista de peliculas existentes\n");
        printf("\nID PELICULA  |  ID LOCAL  |  TITULO  |  GENERO  | PRECIO | NUM_RENTAS | EXISTENCIAS \n");
        resultado = PQexec(bd, cad);
       	if(resultado != NULL){
            fila=PQntuples(resultado);
            columna=PQnfields(resultado);
            for (i = 0; i < fila ; i++){
                for (j = 0; j < columna; j++){
                    printf("%s\t",PQgetvalue(resultado,i,j));
		    	}
			    printf("\n");
	    	}
	    }else{
                 printf("NO HAY PELICULAS REGISTRADOS");
            }
	    }
		if(opc==2){
			//alta de peliculas		
			printf("\nAlta de peliculas\n");
				
			printf("Ingresa el ID de la pelicula: \n");
			scanf("%d",&IDpelicula);
			printf("Ingresa el ID de local: \n");
			scanf("%d",&IDlocal);
			printf("Ingresa el titulo: \n");
			scanf("%s",&titulo);	
			printf("Ingresa el genero: \n");
			scanf("%s",&genero);
			printf("Ingresa el precio: \n");
			scanf("%d",&precio);
			printf("Ingresa el numero de rentas: \n");
			scanf("%d",&Nr);
			printf("Ingresa las existencias de la pelicula: \n");
			scanf("%d",&exis);
				
			sprintf(cad,"insert into pelicula values(%d,%d,'%s','%s',%d,%d,%d)",IDpelicula,IDlocal,titulo,genero,precio,Nr,exis);
			//printf("Instruccion SQL antes de ejcutarse: %s \n",cad);
			PQexec(bd, cad);
        }
		if(opc==3){
            //eliminar alguna pelicula
			printf("\nEliminar pelicula\n");
			printf("\nNOTA: Si la pelicula a borrar tiene rentas no podra borrarse\n");	        		
	
			printf("Ingrese la ID de la pelicula que desea dar de baja\n");
			scanf("%d",&IDpelicula);
	
			sprintf(cad,"delete from pelicula where ID_Pelicula='%d'",IDpelicula);
			//printf("Instruccion SQL antes de ejecutarse: %s \n",cad);
				
			puts("\n Pelicula dado de baja \n");
			PQexec(bd, cad);	
		}
        if(opc==4){
            //actualiza precio de la pelicula
	        printf("Ingrese el ID de la pelicula que desee actualizar el precio\n");
	        scanf("%d",&IDpelicula);
					
	        printf("\nIngrese el nuevo precio\n");
	        scanf("%d",&precio);
								
	        sprintf(cad,"update pelicula set precio='%d' where ID_Pelicula='%d';",precio,IDpelicula);
				
	        puts("\n Precio actualizado \n");
	        PQexec(bd, cad);
        }
        if(opc==5){
            //actualiza numero de rentas de la pelicula
	        printf("Ingrese el ID de la pelicula que desee actualizar el numero de rentas\n");
	        scanf("%d",&IDpelicula);
					
	        printf("\nIngrese el nuevo numero de rentas\n");
	        scanf("%d",&Nr);
								
	        sprintf(cad,"update pelicula set Numero_Rentas='%d' where ID_Pelicula='%d';",Nr,IDpelicula);
				
	        puts("\n Numero de rentas actualizado \n");
	        PQexec(bd, cad);
        }
        if(opc==6){
            //actualiza existencias de la pelicula
	        printf("Ingrese el ID de la pelicula que desee actualizar el numero de existencias\n");
	        scanf("%d",&IDpelicula);
					
	        printf("\nIngrese el nuevo numero de existencias\n");
	        scanf("%d",&exis);
								
	        sprintf(cad,"update pelicula set existencias='%d' where ID_Pelicula='%d';",exis,IDpelicula);
				
	        puts("\n Numero de existencias actualizado \n");
	        PQexec(bd, cad);
        }

	}while(opc!=7);
    break;
    }
}while(op!=9);
}

      else
          printf("ERROR AL CONECTAR EN LA BASE DE DATOS");
    }
    MPI_Finalize();
	 return 0;
}
