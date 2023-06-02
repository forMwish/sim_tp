import os
import numpy as np
import matplotlib.pyplot as plt

color_list = ['b', 'g', 'r', 'c', 'm', 'y', 'k', 'w']
log = "/home/binli/repo/sim_LCD/res/device.log"
line_keys = [f"[flush time{i}]" for i in range(5)]

def get_line(path, key):
    lines = []
    with open(log, "r") as fp:
        for line in fp.readlines():
            if key in line:
                lines.append(line)
    return lines

def split_line(lines, prefix, postfix):
    data_list = []
    assert isinstance(lines, list)
    for line in lines:
        data = line.split(prefix)[-1]
        data = data.split(postfix)[0]
        data_list.append(data)
    return data_list

data_dict = {}
for i, key in enumerate(line_keys):
    lines = get_line(log, key)
    data = split_line(lines, key, "\n")

    x_list = []
    y_list = []
    for d in data:
        x, y = d.split()
        x_list.append(int(x))
        y_list.append(int(y))
    
    data_dict[key] = {"x":x_list, "y":y_list}

    plt.scatter(x_list, y_list, marker=".", c=color_list[i], label=f"{key} ave:{sum(y_list) / len(y_list)}")


# 显示图例
plt.legend()

# 显示图形
plt.savefig("./tmp.png")
