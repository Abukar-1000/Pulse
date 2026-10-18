build:
	rm -rf build && cmake -B build -S . -DPICO_SDK_PATH="C:/Users/lilgo/Desktop/projects/dependencies/pico-sdk" -DPICO_BOARD=pico_w && cmake --build build && cp build/pico_blink.uf2 E:/
