{
  "targets": [
    {
      "target_name": "itunes_win",
      "sources": [
          "src/itunes.h",
          "src/itunes.cpp",
          "src/node.cpp",
          "src/iTunesCOMInterface.c"
      ],
      "include_dirs": [
          "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}