cd C:\Users\Kohu\Documents\Github\kanan-new
cmake -B build -G "Visual Studio 17 2022" -A Win32
cmake --build build --config Release
cmake --install build --prefix out