
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funciones.h"

int cargarDatosHistoricos(Zona zonas[]) {
    FILE *file = fopen("datos_historicos.csv", "r");
    if (!file) {
        perror("Error al abrir archivo de datos historicos");
        return -1;
    }

    char line[256];
    // Skip header line
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Error leyendo encabezado de datos historicos\n");
        fclose(file);
        return -1;
    }

    int counts[NUM_ZONAS] = {0};
    char *ciudades[NUM_ZONAS];
    for (int i = 0; i < NUM_ZONAS; i++) {
        ciudades[i] = zonas[i].nombre;
    }

    while (fgets(line, sizeof(line), file)) {
        char ciudad[50], fecha[11];
        float pm25, no2, so2, co;
        if (sscanf(line, "%49[^,],%10[^,],%f,%f,%f,%f", ciudad, fecha, &pm25, &no2, &so2, &co) != 6) {
            fprintf(stderr, "Error leyendo linea de datos historicos: %s\n", line);
            fclose(file);
            return -1;
        }
        int idx = -1;
        for (int i = 0; i < NUM_ZONAS; i++) {
            if (strcmp(ciudad, zonas[i].nombre) == 0) {
                idx = i;
                break;
            }
        }
        if (idx == -1) {
            fprintf(stderr, "Ciudad no encontrada en datos historicos: %s\n", ciudad);
            continue;
        }
        int count = counts[idx];
        if (count >= DIAS_HISTORICOS) {
            fprintf(stderr, "Demasiados datos para la ciudad %s\n", ciudad);
            continue;
        }
        strncpy(zonas[idx].datos_historicos[count].fecha, fecha, 11);
        zonas[idx].datos_historicos[count].pm25 = pm25;
        zonas[idx].datos_historicos[count].no2 = no2;
        zonas[idx].datos_historicos[count].so2 = so2;
        zonas[idx].datos_historicos[count].co2 = co;
        counts[idx]++;
    }

    fclose(file);
    return 0;
}


int cargarDatosClimaticos(Zona zonas[]) {
    FILE *file = fopen("datos_climaticos.csv", "r");
    if (!file) {
        perror("Error al abrir archivo de datos climaticos");
        return -1;
    }

    char line[256];
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Error leyendo encabezado de datos climaticos\n");
        fclose(file);
        return -1;
    }

    while (fgets(line, sizeof(line), file)) {
        char ciudad[50];
        float temp, viento, humedad;
        if (sscanf(line, "%49[^,],%f,%f,%f", ciudad, &temp, &viento, &humedad) != 4) {
            fprintf(stderr, "Error parseando datos climaticos: %s\n", line);
            continue;
        }
        // Buscar la zona correspondiente por nombre
        int idx = -1;
        for (int i = 0; i < NUM_ZONAS; i++) {
            if (strcmp(ciudad, zonas[i].nombre) == 0) {
                idx = i;
                break;
            }
        }
        if (idx == -1) {
            fprintf(stderr, "Ciudad no encontrada en datos climaticos: %s\n", ciudad);
            continue;
        }
        zonas[idx].clima_actual.temperatura = temp;
        zonas[idx].clima_actual.velocidad_viento = viento;
        zonas[idx].clima_actual.humedad = humedad;
    }
    fclose(file);
    return 0;
}

void imprimirAnalisisArchivo(FILE *file, Zona *zona, char recomendaciones[][200], int num_recomendaciones) {
    fprintf(file, "Analisis de contaminacion segun limites OMS:\n");
    fprintf(file, "%-15s %-15s %-15s %-15s\n", "Contaminante", "Valor", "Limite OMS", "Estado");

    if (zona->contaminacion_actual.co2 > LIMITE_CO2)
        fprintf(file, "%-15s %-15.2f %-15.2f %-15s\n", "CO2 Actual", zona->contaminacion_actual.co2, LIMITE_CO2, "Supera limite");
    else
        fprintf(file, "%-15s %-15.2f %-15.2f %-15s\n", "CO2 Actual", zona->contaminacion_actual.co2, LIMITE_CO2, "Dentro de limite");

    if (zona->prediccion_24h.co2 > LIMITE_CO2)
        fprintf(file, "%-15s %-15.2f %-15.2f %-15s\n", "CO2 Predicho", zona->prediccion_24h.co2, LIMITE_CO2, "Supera limite");
    else
        fprintf(file, "%-15s %-15.2f %-15.2f %-15s\n", "CO2 Predicho", zona->prediccion_24h.co2, LIMITE_CO2, "Dentro de limite");

    if (zona->contaminacion_actual.so2 > LIMITE_SO2)
        fprintf(file, "%-15s %-15.2f %-15.2f %-15s\n", "SO2 Actual", zona->contaminacion_actual.so2, LIMITE_SO2, "Supera limite");
    else
        fprintf(file, "%-15s %-15.2f %-15.2f %-15s\n", "SO2 Actual", zona->contaminacion_actual.so2, LIMITE_SO2, "Dentro de limite");

    if (zona->prediccion_24h.so2 > LIMITE_SO2)
        fprintf(file, "%-15s %-15.2f %-15.2f %-15s\n", "SO2 Predicho", zona->prediccion_24h.so2, LIMITE_SO2, "Supera limite");
    else
        fprintf(file, "%-15s %-15.2f %-15.2f %-15s\n", "SO2 Predicho", zona->prediccion_24h.so2, LIMITE_SO2, "Dentro de limite");

    if (zona->contaminacion_actual.no2 > LIMITE_NO2)
        fprintf(file, "%-15s %-15.2f %-15.2f %-15s\n", "NO2 Actual", zona->contaminacion_actual.no2, LIMITE_NO2, "Supera limite");
    else
        fprintf(file, "%-15s %-15.2f %-15.2f %-15s\n", "NO2 Actual", zona->contaminacion_actual.no2, LIMITE_NO2, "Dentro de limite");

    if (zona->prediccion_24h.no2 > LIMITE_NO2)
        fprintf(file, "%-15s %-15.2f %-15.2f %-15s\n", "NO2 Predicho", zona->prediccion_24h.no2, LIMITE_NO2, "Supera limite");
    else
        fprintf(file, "%-15s %-15.2f %-15.2f %-15s\n", "NO2 Predicho", zona->prediccion_24h.no2, LIMITE_NO2, "Dentro de limite");

    if (zona->contaminacion_actual.pm25 > LIMITE_PM25)
        fprintf(file, "%-15s %-15.2f %-15.2f %-15s\n", "PM2.5 Actual", zona->contaminacion_actual.pm25, LIMITE_PM25, "Supera limite");
    else
        fprintf(file, "%-15s %-15.2f %-15.2f %-15s\n", "PM2.5 Actual", zona->contaminacion_actual.pm25, LIMITE_PM25, "Dentro de limite");

    if (zona->prediccion_24h.pm25 > LIMITE_PM25)
        fprintf(file, "%-15s %-15.2f %-15.2f %-15s\n", "PM2.5 Predicho", zona->prediccion_24h.pm25, LIMITE_PM25, "Supera limite");
    else
        fprintf(file, "%-15s %-15.2f %-15.2f %-15s\n", "PM2.5 Predicho", zona->prediccion_24h.pm25, LIMITE_PM25, "Dentro de limite");

    fprintf(file, "\nRecomendaciones:\n");
    for (int i = 0; i < num_recomendaciones; i++) {
        fprintf(file, "- %s\n", recomendaciones[i]);
    }
    fprintf(file, "----------------------------------------\n");
}

int guardarReporte(Zona zonas[]) {
    FILE *file = fopen("reporte.txt", "w");
    if (!file) {
        perror("Error al abrir archivo para reporte");
        return -1;
    }

    char recomendaciones[10][200];
    int num_recomendaciones;

    for (int i = 0; i < NUM_ZONAS; i++) {
        fprintf(file, "Zona: %s\n", zonas[i].nombre);
        fprintf(file, "%-20s %-10s %-10s %-10s %-10s\n", "Tipo", "CO2", "SO2", "NO2", "PM2.5");
        fprintf(file, "%-20s %-10.2f %-10.2f %-10.2f %-10.2f\n", "Actual(1/07/2025)",
            zonas[i].contaminacion_actual.co2,
            zonas[i].contaminacion_actual.so2,
            zonas[i].contaminacion_actual.no2,
            zonas[i].contaminacion_actual.pm25);
        fprintf(file, "%-20s %-10.2f %-10.2f %-10.2f %-10.2f\n", "Prediccion 24h",
            zonas[i].prediccion_24h.co2,
            zonas[i].prediccion_24h.so2,
            zonas[i].prediccion_24h.no2,
            zonas[i].prediccion_24h.pm25);

        Contaminacion promedio_historico;
        calcularPromedioHistorico(&zonas[i], &promedio_historico);
        fprintf(file, "%-20s %-10.2f %-10.2f %-10.2f %-10.2f\n", "Promedio Historico",
            promedio_historico.co2,
            promedio_historico.so2,
            promedio_historico.no2,
            promedio_historico.pm25);

        fprintf(file, "Clima Actual(1/07/2025): Temp=%.2f VelViento=%.2f Humedad=%.2f\n",
            zonas[i].clima_actual.temperatura,
            zonas[i].clima_actual.velocidad_viento,
            zonas[i].clima_actual.humedad);

        generarRecomendaciones(&zonas[i], recomendaciones, &num_recomendaciones);
        imprimirAnalisisArchivo(file, &zonas[i], recomendaciones, num_recomendaciones);
    }
    fclose(file);
    return 0;
}


void calcularContaminacionActual(Zona *zona) {
    float suma_co2 = 0, suma_so2 = 0, suma_no2 = 0, suma_pm25 = 0;
    int dias = 3;
    for (int i = DIAS_HISTORICOS - dias; i < DIAS_HISTORICOS; i++) {
        suma_co2 += zona->datos_historicos[i].co2;
        suma_so2 += zona->datos_historicos[i].so2;
        suma_no2 += zona->datos_historicos[i].no2;
        suma_pm25 += zona->datos_historicos[i].pm25;
    }
    zona->contaminacion_actual.co2 = suma_co2 / dias;
    zona->contaminacion_actual.so2 = suma_so2 / dias;
    zona->contaminacion_actual.no2 = suma_no2 / dias;
    zona->contaminacion_actual.pm25 = suma_pm25 / dias;
}

// Calcular promedio historico de ultimos 30 dias
void calcularPromedioHistorico(Zona *zona, Contaminacion *promedio) {
    float suma_co2 = 0, suma_so2 = 0, suma_no2 = 0, suma_pm25 = 0;
    for (int i = 0; i < DIAS_HISTORICOS; i++) {
        suma_co2 += zona->datos_historicos[i].co2;
        suma_so2 += zona->datos_historicos[i].so2;
        suma_no2 += zona->datos_historicos[i].no2;
        suma_pm25 += zona->datos_historicos[i].pm25;
    }
    promedio->co2 = suma_co2 / DIAS_HISTORICOS;
    promedio->so2 = suma_so2 / DIAS_HISTORICOS;
    promedio->no2 = suma_no2 / DIAS_HISTORICOS;
    promedio->pm25 = suma_pm25 / DIAS_HISTORICOS;
}

// Predecir contaminacion para proximas 24h usando promedio ponderado (mas peso a dias recientes) y factores climaticos
void predecirContaminacion(Zona *zona) {
    float peso_total = 0;
    float suma_co2 = 0, suma_so2 = 0, suma_no2 = 0, suma_pm25 = 0;

    // Pesos decrecientes para ultimos 7 dias
    float pesos[7] = {0.3, 0.25, 0.15, 0.1, 0.08, 0.07, 0.05};

    for (int i = 0; i < 7; i++) {
        int idx = DIAS_HISTORICOS - 1 - i;
        suma_co2 += zona->datos_historicos[idx].co2 * pesos[i];
        suma_so2 += zona->datos_historicos[idx].so2 * pesos[i];
        suma_no2 += zona->datos_historicos[idx].no2 * pesos[i];
        suma_pm25 += zona->datos_historicos[idx].pm25 * pesos[i];
        peso_total += pesos[i];
    }

    // Ajuste simple con factores climaticos (ejemplo: viento reduce contaminacion, humedad y temperatura aumentan)
    float factor_viento = 1.0 - (zona->clima_actual.velocidad_viento * 0.05f);
    if (factor_viento < 0.7f) factor_viento = 0.7f; // limite inferior

    float factor_temperatura = 1.0 + ((zona->clima_actual.temperatura - 20.0f) * 0.02f);
    if (factor_temperatura < 0.8f) factor_temperatura = 0.8f;

    float factor_humedad = 1.0 + ((zona->clima_actual.humedad - 50.0f) * 0.01f);
    if (factor_humedad < 0.9f) factor_humedad = 0.9f;

    float factor_clima = factor_viento * factor_temperatura * factor_humedad;

    zona->prediccion_24h.co2 = (suma_co2 / peso_total) * factor_clima;
    zona->prediccion_24h.so2 = (suma_so2 / peso_total) * factor_clima;
    zona->prediccion_24h.no2 = (suma_no2 / peso_total) * factor_clima;
    zona->prediccion_24h.pm25 = (suma_pm25 / peso_total) * factor_clima;
}

// Emitir alerta si contaminacion actual o prevista supera limites
int emitirAlerta(Zona *zona) {
    if (zona->contaminacion_actual.co2 > LIMITE_CO2 || zona->prediccion_24h.co2 > LIMITE_CO2)
        return 1;
    if (zona->contaminacion_actual.so2 > LIMITE_SO2 || zona->prediccion_24h.so2 > LIMITE_SO2)
        return 1;
    if (zona->contaminacion_actual.no2 > LIMITE_NO2 || zona->prediccion_24h.no2 > LIMITE_NO2)
        return 1;
    if (zona->contaminacion_actual.pm25 > LIMITE_PM25 || zona->prediccion_24h.pm25 > LIMITE_PM25)
        return 1;
    return 0;
}

// Generar recomendaciones basadas en niveles de contaminacion
void generarRecomendaciones(Zona *zona, char recomendaciones[][200], int *num_recomendaciones) {
    *num_recomendaciones = 0;
    if (emitirAlerta(zona)) {
        strcpy(recomendaciones[(*num_recomendaciones)++], "Reducir el trafico vehicular.");
        strcpy(recomendaciones[(*num_recomendaciones)++], "Cerrar temporalmente industrias contaminantes.");
        strcpy(recomendaciones[(*num_recomendaciones)++], "Suspender actividades al aire libre.");
    } else {
        strcpy(recomendaciones[(*num_recomendaciones)++], "Mantener monitoreo constante.");
        strcpy(recomendaciones[(*num_recomendaciones)++], "Fomentar uso de transporte publico y bicicletas.");
    }
}

void analizarContaminacion(Zona *zona) {
    printf("Analisis de contaminacion segun limites OMS:\n");

    if (zona->contaminacion_actual.co2 > LIMITE_CO2)
        printf("- CO2 actual: %.2f supera limite OMS (%.2f)\n", zona->contaminacion_actual.co2, LIMITE_CO2);
    else
        printf("- CO2 actual: %.2f dentro de limites OMS (%.2f)\n", zona->contaminacion_actual.co2, LIMITE_CO2);

    if (zona->prediccion_24h.co2 > LIMITE_CO2)
        printf("- CO2 predicho: %.2f supera limite OMS (%.2f)\n", zona->prediccion_24h.co2, LIMITE_CO2);
    else
        printf("- CO2 predicho: %.2f dentro de limites OMS (%.2f)\n", zona->prediccion_24h.co2, LIMITE_CO2);

    if (zona->contaminacion_actual.so2 > LIMITE_SO2)
        printf("- SO2 actual: %.2f supera limite OMS (%.2f)\n", zona->contaminacion_actual.so2, LIMITE_SO2);
    else
        printf("- SO2 actual: %.2f dentro de limites OMS (%.2f)\n", zona->contaminacion_actual.so2, LIMITE_SO2);

    if (zona->prediccion_24h.so2 > LIMITE_SO2)
        printf("- SO2 predicho: %.2f supera limite OMS (%.2f)\n", zona->prediccion_24h.so2, LIMITE_SO2);
    else
        printf("- SO2 predicho: %.2f dentro de limites OMS (%.2f)\n", zona->prediccion_24h.so2, LIMITE_SO2);

    if (zona->contaminacion_actual.no2 > LIMITE_NO2)
        printf("- NO2 actual: %.2f supera limite OMS (%.2f)\n", zona->contaminacion_actual.no2, LIMITE_NO2);
    else
        printf("- NO2 actual: %.2f dentro de limites OMS (%.2f)\n", zona->contaminacion_actual.no2, LIMITE_NO2);

    if (zona->prediccion_24h.no2 > LIMITE_NO2)
        printf("- NO2 predicho: %.2f supera limite OMS (%.2f)\n", zona->prediccion_24h.no2, LIMITE_NO2);
    else
        printf("- NO2 predicho: %.2f dentro de limites OMS (%.2f)\n", zona->prediccion_24h.no2, LIMITE_NO2);

    if (zona->contaminacion_actual.pm25 > LIMITE_PM25)
        printf("- PM2.5 actual: %.2f supera limite OMS (%.2f)\n", zona->contaminacion_actual.pm25, LIMITE_PM25);
    else
        printf("- PM2.5 actual: %.2f dentro de limites OMS (%.2f)\n", zona->contaminacion_actual.pm25, LIMITE_PM25);

    if (zona->prediccion_24h.pm25 > LIMITE_PM25)
        printf("- PM2.5 predicho: %.2f supera limite OMS (%.2f)\n", zona->prediccion_24h.pm25, LIMITE_PM25);
    else
        printf("- PM2.5 predicho: %.2f dentro de limites OMS (%.2f)\n", zona->prediccion_24h.pm25, LIMITE_PM25);
}

void mostrarMenu() {
    printf("\nSistema Integral de Gestion y Prediccion de Contaminacion del Aire\n");
    printf("1. Cargar datos historicos\n");
    printf("2. Mostrar reporte y analisis\n");
    printf("3. Guardar reporte en archivo\n");
    printf("4. Salir\n");
    printf("Seleccione una opcion: ");
}

// Limpiar buffer de entrada
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
