<div align="center">
  <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/7/7b/HSBI_Logo_RGB_schwarz.svg/1200px-HSBI_Logo_RGB_schwarz.svg.png" alt="Logo" width="600" height="120">

  <h3 align="center">Abschluss-Projekt EMD-S - Labyrinth</h3>
</div>

<div align="center">

![GitHub License](https://img.shields.io/github/license/CyZeTLC/Pico2?style=for-the-badge)
![GitHub stars](https://img.shields.io/github/stars/CyZeTLC/Pico2?style=for-the-badge)
![Letze Aenderung](https://img.shields.io/github/last-commit/CyZeTLC/Pico2?style=for-the-badge)

</div>

---

## Über das Projekt

Unsere Idee ist ein Spiel, bei dem man durch zufällige Labyrinthe finden muss.
Es gibt eine Spielfigur (blau), die man dem Joystick nach oben, unten, rechts und links bewegen kann.
Die möglichen Wege werden weiß angezeigt, die Wände in schwarz. Die grüne Markierung in der
Karte ist das Ziel. Sobald man dieses erreicht, wird automatisch ein neues Level geladen, sodass man
so lange immer neue Labyrinthe durchlaufen kann, wie man Lust hat. Wir wollten keine festen Level
programmieren, deshalb generiert der Pico bei jedem Start und nach Beenden jedes Level mit Hilfe
eines modifizierten Prim-Algorithmus ein ganz neues Layout.
