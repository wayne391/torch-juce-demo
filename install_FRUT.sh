cd FRUT
rm -rf build
mkdir build && cd build/
cmake .. -DCMAKE_INSTALL_PREFIX=../prefix -DJUCE_ROOT="/Users/wayne391/Documents/Projects/audio_effect/deepcore/JUCE"
cmake --build . --target install
