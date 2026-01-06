# 🍌 peelmybanana 🍌

**peelmybanana** is a packer / unpacker tool for the **Super Monkey Ball: Banana Splitz**

This tool extracts and repacks the game's proprietary archive formats (`.pk`, `.pkh`, `.pfs`).

It also supports those games:
* IA/VT Colorful

## Building

```bash
cmake -S . -B build
cmake --build build
```

## Usage

For monkey ball
```bash
./peelmybanana -u <base_filename>
./peelmybanana -p <directory>
```

For IA/VT:
```bash
./peelmybanana -i -u <base_filename>
./peelmybanana -i -p <directory>
```

## License

MIT
