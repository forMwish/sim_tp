import binbox
import os

local_dir = "/home/binli/repo/sim_LCD"

# lvgl build
binbox.local(f"cp {local_dir}/config/lv_conf.h {local_dir}/third/lvgl/lv_conf.h")
binbox.local(f"cp {local_dir}/config/lvgl.cmake {local_dir}/third/lvgl/CMakeLists.txt")
# binbox.local(f"cd {local_dir}/third/lvgl; mkdir build; cd build && cmake .. && make -j")
os.system(f"cd {local_dir}/third/lvgl; mkdir build; cd build && cmake .. && make -j")
binbox.local(f"cp {local_dir}/third/lvgl/output/lib/liblvgl.so {local_dir}/tmp")

# libpng build
binbox.local(f"cd {local_dir}/third/libpng; mkdir build; cd build && cmake .. && make -j")
binbox.local(f"cp {local_dir}/third/libpng/build/libpng.so {local_dir}/tmp")


# main build
binbox.local(f"cd {local_dir}; mkdir build; cd build && cmake .. && make -j")
