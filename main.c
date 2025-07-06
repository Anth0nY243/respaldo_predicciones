#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funciones.h"

int main() {
    Zona zonas[NUM_ZONAS];
    Contaminacion promedio_historico;
    char recomendaciones[10][200];
    int num_recomendaciones;
    int opcion;
    int datos_cargados = 0;
    int datos_climaticos_ingresados = 0;

    const char *ciudades[NUM_ZONAS] = {"Quito", "Loja", "Pastaza", "Guayaquil", "Manta"};
    for (int i = 0; i < NUM_ZONAS; i++) {
        strncpy(zonas[i].nombre, ciudades[i], sizeof(zonas[i].nombre) - 1);
        zonas[i].nombre[sizeof(zonas[i].nombre) - 1] = '\0';
    }

    do {
        printf("Sistema Integral de Gestion y Prediccion de Contaminacion del Aire\n");
        printf("1. Cargar datos historicos\n");
        printf("2. Mostrar reporte y analisis\n");
        printf("3. Guardar reporte en archivo\n");
        printf("4. Salir\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            printf("Entrada invalida. Por favor ingrese un numero.\n");
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();

        switch (opcion) {
            case 1:
                if (cargarDatosHistoricos(zonas) == 0) {
                    if (cargarDatosClimaticos(zonas) == 0) {
                        printf("Datos historicos y climaticos cargados exitosamente.\n");
                        datos_cargados = 1;

                        printf("Ciudades cargadas:\n");
                        for (int i = 0; i < NUM_ZONAS; i++) {
                            printf("- %s\n", zonas[i].nombre);
                        }
                    } else {
                        printf("Error al cargar datos climaticos.\n");
                    }
                } else {
                    printf("Error al cargar datos historicos.\n");
                }
                break;
            case 2:
                if (!datos_cargados) {
                    printf("Primero debe cargar los datos historicos y climaticos (opcion 1).\n");
                    break;
                }
                printf("Mostrando reporte y analisis:\n");
                for (int i = 0; i < NUM_ZONAS; i++) {
                    calcularContaminacionActual(&zonas[i]);
                    predecirContaminacion(&zonas[i]);
                    calcularPromedioHistorico(&zonas[i], &promedio_historico);

                    printf("\n--- Zona: %s ---\n", zonas[i].nombre);
                    printf("Contaminacion en base al 1 de julio de 2025: CO2=%.2f SO2=%.2f NO2=%.2f PM2.5=%.2f\n",
                        zonas[i].contaminacion_actual.co2,
                        zonas[i].contaminacion_actual.so2,
                        zonas[i].contaminacion_actual.no2,
                        zonas[i].contaminacion_actual.pm25);
                    printf("Promedio Historico (30 dias): CO2=%.2f SO2=%.2f NO2=%.2f PM2.5=%.2f\n",
                        promedio_historico.co2,
                        promedio_historico.so2,
                        promedio_historico.no2,
                        promedio_historico.pm25);
                    printf("Prediccion para proximas 24h: CO2=%.2f SO2=%.2f NO2=%.2f PM2.5=%.2f\n",
                        zonas[i].prediccion_24h.co2,
                        zonas[i].prediccion_24h.so2,
                        zonas[i].prediccion_24h.no2,
                        zonas[i].prediccion_24h.pm25);

                    if (emitirAlerta(&zonas[i])) {
                        printf("ALERTA: Los niveles de contaminacion actual o previstos superan los limites aceptables.\n");
                    } else {
                        printf("Los niveles de contaminacion estan dentro de los limites aceptables.\n");
                    }

                    generarRecomendaciones(&zonas[i], recomendaciones, &num_recomendaciones);
                    printf("Recomendaciones:\n");
                    for (int j = 0; j < num_recomendaciones; j++) {
                        printf("- %s\n", recomendaciones[j]);
                    }

                    analizarContaminacion(&zonas[i]);
                }
                break;
            case 3:
                if (!datos_cargados) {
                    printf("Primero debe cargar los datos historicos y climaticos (opcion 1).\n");
                    break;
                }
                if (guardarReporte(zonas) == 0) {
                    printf("Reporte guardado exitosamente en 'reporte.txt'.\n");
                } else {
                    printf("Error al guardar el reporte.\n");
                }
                break;
            case 4:
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opcion invalida. Por favor seleccione una opcion valida.\n");
        }
    } while (opcion != 4);

    return 0;
}
