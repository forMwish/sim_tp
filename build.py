import binbox
import os

local_dir = "/home/binli/repo/sim_LCD"
binbox.local(f"cd {local_dir}/third/lvgl; mkdir build; cd build && cmake .. && make -j")