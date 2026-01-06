# 🍌 peelmybanana 🍌

**peelmybanana** is a packer / unpacker tool for the **Super Monkey Ball: Banana Splitz** (initially)

This tool extracts and repacks the game's proprietary archive formats (`.pk`, `.pkh`, `.pfs`).

It also supports those games:
* IA/VT -Colorful-
* Soul Sacrifice
* Soul Sacrifice Delta
* New Little King's Story
* Fate/Extella: The Umbral Star
* Fate/EXTELLA LINK

## Building

```bash
cmake -S . -B build
cmake --build build
```

## Usage

For Super Monkey Ball / New Little King's Story:
```bash
./peelmybanana -u <base_filename>
./peelmybanana -p <directory>
```

For IA/VT / Soul Sacrifice / Soul Sacrifice Delta / Fate/Extella: The Umbral Star:
```bash
./peelmybanana -i -u <base_filename>
./peelmybanana -i -p <directory>
```

For Fate/EXTELLA LINK:
```bash
./peelmybanana -f -u <base_filename>
./peelmybanana -f -p <directory>
```

## License

MIT
