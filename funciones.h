#ifndef FUNCIONES_H
#define FUNCIONES_H

#define NUM_ZONAS 5
#define DIAS_HISTORICOS 30

// Limites aceptables de contaminacion segun OMS
#define LIMITE_CO2 1000.0
#define LIMITE_SO2 40.0
#define LIMITE_NO2 25.0
#define LIMITE_PM25 15.0

typedef struct {
    char fecha[11]; // YYYY-MM-DD
    float co2;
    float so2;
    float no2;
    float pm25;
} Contaminacion;

typedef struct {
    float temperatura;
    float velocidad_viento;
    float humedad;
} Clima;

typedef struct {
    char nombre[50];
    Contaminacion datos_historicos[DIAS_HISTORICOS];
    Contaminacion contaminacion_actual;
    Contaminacion prediccion_24h;
    Clima clima_actual;
} Zona;

int cargarDatosHistoricos(Zona zonas[]);
int cargarDatosClimaticos(Zona zonas[]);
int guardarReporte(Zona zonas[]);

void calcularContaminacionActual(Zona *zona);
void calcularPromedioHistorico(Zona *zona, Contaminacion *promedio);
void predecirContaminacion(Zona *zona);
int emitirAlerta(Zona *zona);
void generarRecomendaciones(Zona *zona, char recomendaciones[][200], int *num_recomendaciones);

// Nuevas funciones para analisis y menu
void analizarContaminacion(Zona *zona);
void mostrarMenu();
void limpiarBuffer();

#endif // FUNCIONES_H
