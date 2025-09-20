# create build folder 
```mkdir build & cd build```
# build 
```cmake .. && cmake --build .```
on windows, you can append ```--config Release``` to hide console
# run
```SDL_VIDEODRIVER=x11 ./ctest```
