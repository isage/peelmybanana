# 🍌 peelmybanana 🍌

**peelmybanana** is a packer / unpacker tool for the **Super Monkey Ball: Banana Splitz**

This tool extracts and repacks the game's proprietary archive formats (`.pk`, `.pkh`, `.pfs`).

## Building

```bash
cmake -S . -B build
cmake --build build
```

## Usage

```bash
./peelmybanana -u <base_filename>
./peelmybanana -p <directory>
```

## License

MIT
