# IoT Climate Monitor

**IoT Climate Monitor** är ett IoT-baserat multi-zon klimatövervakningssystem som mäter temperatur och luftfuktighet i realtid. Projektet använder ESP32-enheter med DHT11-sensorer för datainsamling och en Raspberry Pi som central gateway. Data visualiseras och hanteras i ThingsBoard.

## Funktioner
- **Multi-zon övervakning**: Mätning av temperatur och luftfuktighet från flera zoner.
- **Centraliserad gateway**: Raspberry Pi samlar in och vidarebefordrar data via MQTT.
- **Realtidsvisualisering**: Överskådligt gränssnitt i ThingsBoard.
- **Notifieringar**: Larm för temperatur och luftfuktighet utanför förinställda gränsvärden.
- **Energisparläge**: ESP32-enheter använder sleep mode för att spara batteri.
- **Flexibla intervall**: Anpassade mätintervaller för varje enhet.

## Mål
Att skapa en fungerande prototyp som demonstrerar IoT-teknologi för klimatövervakning, med tillämpningar i kontor, lager eller hemmiljöer.

## Använd teknik
- **Hårdvara**: ESP32, DHT11-sensorer, Raspberry Pi.
- **Mjukvara**: 
  - ESP-IDF/Arduino för ESP32-programmering.
  - Python för Raspberry Pi gateway.
  - ThingsBoard för visualisering och notifieringar.
  - MQTT för enhetskommunikation.

## Framtida Utveckling
- Stöd för fler sensorer som luftkvalitet och tryck.
- Integration med smarta hemsystem som Google Home eller Alexa.
- Dataanalys med maskininlärning för trender och prognoser.
- Skalbarhet för fler zoner.

## Licens
Projektet är licensierat under MIT License. Se `LICENSE` för detaljer.
