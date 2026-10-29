# Rejestrator Telemetrii Pokładowej

## Opis projektu

Projekt implementuje rejestrator telemetrii pokładowej rakiety (*Flight Blackbox*). Jego zadaniem jest kodowanie aktualnego stanu rakiety do 32-bitowych ramek danych oraz zapisywanie próbek w buforze kołowym.

## Funkcjonalności

- Obsługa stanów rakiety: `DISARM`, `ARM`, `FLIGHT`, `PARACHUTE`, `LANDED`.
- Rejestrowanie informacji o wdrożeniu spadochronu.
- Obsługa odczytów wysokości oraz błędów barometru.
- Kodowanie danych w 32-bitowej ramce telemetrycznej.
- Obliczanie sumy kontrolnej za pomocą operacji XOR.
- Zapisywanie próbek w buforze kołowym o pojemności 16 ramek.
- Odczyt i weryfikacja ostatniej zapisanej ramki.
- Testy jednostkowe z wykorzystaniem Google Test.

## Format ramki danych

Ramka telemetryczna ma długość 32 bitów i zawiera następujące pola:

| Bity | Pole | Opis |
|---|---|---|
| 0–2 | Stan rakiety | Aktualny stan systemu |
| 3 | Flaga spadochronu | Informacja o wdrożeniu spadochronu |
| 4 | Błąd barometru | Informacja o błędzie odczytu barometru |
| 5–7 | Bity zarezerwowane | Zawsze ustawione na `0` |
| 8–23 | Wysokość | Wartość wysokości zapisana jako liczba 16-bitowa |
| 24–31 | Suma kontrolna | XOR trzech najmłodszych bajtów ramki |

### Obsługiwane stany

| Wartość | Stan |
|---:|---|
| `0` | `DISARM` |
| `1` | `ARM` |
| `2` | `FLIGHT` |
| `3` | `PARACHUTE` |
| `4` | `LANDED` |

### Zasady zapisu wysokości

- W stanach `DISARM` i `LANDED`, gdy barometr działa poprawnie, wysokość jest zapisywana jako `0`.
- W przypadku błędu barometru wysokość jest zapisywana jako `0xFFFF`, niezależnie od stanu rakiety.
- Suma kontrolna jest obliczana przez operację XOR na trzech najmłodszych bajtach ramki.

## Testy w GTest

Testy wykorzystują bibliotekę **Google Test (GTest)** i sprawdzają między innymi:

- Poprawność kodowania ramki telemetrycznej.
- Obsługę błędu barometru.
- Zapis wysokości w stanach `DISARM` i `LANDED`.
- Priorytet błędu barometru przy zapisie wysokości.
- Zachowanie podczas wielokrotnego zapisywania próbek.

## Główne pliki

| Plik | Opis |
|---|---|
| `TelemetryBlackBox.h` | Zawiera deklarację klasy `TelemetryBlackBox`, typ wyliczeniowy `RocketState` oraz deklaracje metod rejestratora telemetrii. |
| `TelemetryBlackBox.cpp` | Zawiera implementację rejestratora telemetrii, kodowania ramek danych, obliczania sumy kontrolnej i obsługi bufora kołowego. |
| `TelemetryBlackBoxTest/test.cpp` | Zawiera testy napisane z wykorzystaniem biblioteki Google Test (GTest). |


## Wymagania

- Microsoft Visual Studio z obsługą języka C++.
- Google Test do uruchamiania testów.

## Uruchomienie testów

1. Otwórz rozwiązanie `TelemetryBlackBox.slnx` w Microsoft Visual Studio.
2. Skompiluj projekt testowy.
3. Otwórz plik `test.cpp` w projekcie testowym.
4. Naciśnij `F5`, aby uruchomić program testowy.
5. Sprawdź wyniki testów wyświetlone w konsoli.
