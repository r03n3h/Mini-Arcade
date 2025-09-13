# Mini Arcade (ESP32 + SSD1306) — Snake • Tetris • Pong • Breakout • Flappy • Invasion

A tiny, vertical (portrait) **mini-arcade** you can build with an ESP32 and a 128×64 OLED (SSD1306).  
The project started from a great base by **Dododomme** (Snake, Tetris, Pong). This fork **adds**:

- **Breakout**
- **Flappy** (Flappy Bird–style)
- **Invasion** (simple vertical shooter)
- **Scorekeeping** across all games

Everything runs in a single Arduino sketch using **Adafruit_GFX** and **Adafruit_SSD1306**.

For 3D files -> https://makerworld.com/en/models/1527345-mini-arcade
---

## Why this project?

I wanted a simple, hackable arcade you can assemble quickly from common parts and extend with your own mini-games. The code is deliberately compact and readable so you can tweak physics, speeds, difficulty, and controls without hunting through a large codebase.

---

## Features

- 🕹️ **6 classic mini-games**: Snake, Tetris, Pong, Breakout, Flappy, Invasion  
- 🧮 **Per-game scoring** with a lightweight on-screen HUD  
- 📐 **Portrait playfield** (60×120 px inside the 128×64 OLED) for a cabinet-like feel  
- ⏱️ **Tetris line clearing + speed-up**, **Breakout ball control & acceleration**, **Flappy pipes & gravity**, **Invasion bullets & enemy waves**  
- 🧩 Single-file Arduino sketch, easy to read and modify

---

## Hardware

- **MCU**: ESP32 (any dev board with enough GPIOs) https://s.click.aliexpress.com/e/_oFby3yb
- **Display**: 0.96" or 1.3" **SSD1306** OLED (I²C, 128×64) at address **0x3C** https://s.click.aliexpress.com/e/_opnnImj
- **Buttons**: 7 push buttons (active-low with `INPUT_PULLUP`) https://s.click.aliexpress.com/e/_omVUy1t
- *Optional*: Extra DuPont Line 40Pin Male to Female (not required) https://s.click.aliexpress.com/e/_oCOWjVz
- *Optional*: buzzer/LEDs (not required)

- Total price less then 20,-

**I²C & Display**

- Uses `display.setRotation(3)` for **portrait** orientation  
- Libraries: `Adafruit_GFX` + `Adafruit_SSD1306`

**Default pins** (adjust in the sketch if needed):

\`\`\`cpp
// Buttons (INPUT_PULLUP)
#define BTN_UP   32
#define BTN_DWN  33
#define BTN_LFT  25
#define BTN_RHT  26
#define BTN_MID  27    // Select / Action
#define BTN_SET  14    // Secondary action (fire in Invasion)
#define BTN_RST  12    // Back to menu / Reset current game
\`\`\`

---

## Controls

**Menu**
- `UP / DOWN` – navigate  
- `MID` – start selected game  
- `RST` – reset selection (back to top entry)

**In-game (varies by game)**

- **Snake**: UP / DOWN / LEFT / RIGHT  
- **Tetris**: LEFT / RIGHT to move, DOWN to drop, MID to rotate, RST to exit  
- **Pong**: UP / DOWN to move paddle, RST to exit  
- **Breakout**: LEFT / RIGHT to move paddle, RST to exit  
- **Flappy**: UP or MID to flap, RST to exit  
- **Invasion**: LEFT / RIGHT / UP / DOWN to move, **SET or MID to fire**, RST to exit

Scores are shown with short labels in the top area of the playfield to keep text compact on the OLED.

---

## Build & Flash

1. **Install Arduino IDE** (or Arduino CLI/PlatformIO).
2. **Boards Manager**: install **ESP32** by Espressif.
3. **Libraries** (Library Manager):
   - `Adafruit GFX Library`
   - `Adafruit SSD1306`
4. Open the sketch and confirm:
   - SSD1306 I²C address is **0x3C**
   - `display.setRotation(3)` is set for portrait
   - Your button pins match your wiring
5. **Compile & upload** to your ESP32.

---

## Tuning & Difficulty

Each game exposes a few constants you can tweak:

- **Flappy**: `FB_GRAV`, `FB_FLAP`, `GAP_H`, `PIPE_SPACING`, pipe speed (see `pipeX[i] -= …`)
- **Breakout**: `BRICK_ROWS`, `BRICK_COLS`, `B_PADDLE_W`, ball speed and acceleration
- **Tetris**: `tetrisInterval` (initial drop speed), scoring per line clear
- **Invasion**: `enemySpeed`, `bulletSpeed`, speed-up thresholds, lives (limit via `enemyPassed`)

---

## Credits

- **Base code & inspiration**: **Dododomme** on MakerWorld  
  Original project (Snake, Tetris, Pong): https://makerworld.com/en/@Dododomme  
- This fork adds **Breakout**, **Flappy**, **Invasion**, and **per-game scores**.

If you fork or remix this project, please keep attribution to Dododomme and link back to this repo.

---

## License

MIT (or your preferred open-source license). Include a `LICENSE` file in the repository.

---

## Roadmap / Ideas

- High-score persistence (EEPROM/Preferences)
- Simple sound effects (buzzer), optional LED feedback
- Difficulty presets (Easy/Normal/Hard) in the menu
- Multi-ball Breakout or power-ups in Invasion
- Parallax background for Flappy

Have fun hacking! If you build a cabinet around it, share photos and tweaks 🙌
