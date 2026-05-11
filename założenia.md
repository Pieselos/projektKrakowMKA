engine:
parser -> bin
później ładujemy do pamięci

## Opis działania

System analizuje dane GTFS, przekształcając je w zoptymalizowane struktury danych.

Dane są indeksowane według przystanków (stop_id), co umożliwia szybkie wyszukiwanie odjazdów.

Każdy odjazd jest powiązany z kursem (trip), linią (route) oraz typem transportu.

Dodatkowo uwzględniany jest kalendarz kursowania, co pozwala filtrować odjazdy zgodnie z aktualną datą.

## stop_times.txt

| pole             | typ        | opis                                  |
|------------------|------------|----------------------------------------|
| trip_id          | string     | identyfikator kursu (pojazdu)          |
| arrival_time     | HH:MM:SS   | czas przyjazdu                         |
| departure_time   | HH:MM:SS   | czas odjazdu                           |
| stop_id          | string     | identyfikator przystanku               |
| stop_sequence    | int        | kolejność przystanku w trasie          |

## trips.txt

| pole            | typ    | opis                                      |
|-----------------|--------|-------------------------------------------|
| route_id        | string | identyfikator linii                       |
| service_id      | string | identyfikator rozkładu (dni kursowania)   |
| trip_id         | string | identyfikator kursu                       |
| trip_headsign   | string | kierunek (np. "Krowodrza Górka")          |

## routes.txt

| pole               | typ    | opis                                  |
|--------------------|--------|---------------------------------------|
| route_id           | string | identyfikator linii                   |
| route_short_name   | string | numer linii (np. "52")                |
| route_type         | int    | typ transportu                        |


| route_type | znaczenie |
|------------|----------|
| 3          | autobus  |
| 900        | tramwaj  |


## calendar_dates.txt

| pole            | typ    | opis                                                   |
|-----------------|--------|--------------------------------------------------------|
| service_id      | string | identyfikator rozkładu                                 |
| date            | int    | data w formacie YYYYMMDD                               |
| exception_type  | int    | określa czy kurs działa lub nie                        |


| wartość | znaczenie |
|--------|----------|
| 1      | kurs działa (dodany) |
| 2      | kurs nie działa (usunięty) |
``


## Finalna reprezentacja odjazdu

| pole         | opis                                  |
|--------------|---------------------------------------|
| time         | czas odjazdu                          |
| line         | numer linii (np. "52")                |
| destination  | kierunek (headsign)                   |
| type         | typ transportu (bus/tram)             |
``