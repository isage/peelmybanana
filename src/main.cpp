#include "extractor.hpp"
#include "packer.hpp"
#include "pfs.hpp"
#include "pkh.hpp"
#include "cxxopts.hpp"
#include <filesystem>
#include <iostream>

int main(int argc, char** argv)
{

  cxxopts::Options options("monkeyballz", "Super Monkey Ball Banana Splitz packer/unpacker");

  options
    //.allow_unrecognised_options()
    .show_positional_help()
    .add_options()
      ("p,pack", "Pack", cxxopts::value<std::string>(), "<inputdir>")
      ("u,unpack", "Unpack", cxxopts::value<std::string>(), "<inputfile>")
      ("h,help", "Print usage")
  ;

  auto result = options.parse(argc, argv);

  if (result.count("help") || (!result.count("pack") && !result.count("unpack")))
  {
    std::cout << options.help({"","Pack","Unpack"}) << std::endl;
    return 0;
  }

  if (result.count("pack"))
  {
      std::filesystem::path base = result["pack"].as<std::string>();
      Packer packer(base);
      packer.run();
      return 0;
  }

  if (result.count("unpack"))
  {
      std::filesystem::path base = result["unpack"].as<std::string>();

      PKHIndex pkh(base.string() + ".pkh");
      PFSTree pfs(base.string() + ".pfs");
      Extractor extractor(base.string() + ".pk", pkh, pfs, base);
      extractor.run();
      return 0;
  }
}
