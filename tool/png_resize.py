from PIL import Image
import os


input = "./res/icon"
output = "./res/icon_resize"

# input = "./res/icon/0.png"
# output = "./res/icon_resize/0.png"

for file in os.listdir(input):
    image = Image.open("/".join([input, file]))
    new_image = image.resize((100, 100))
    new_image.save("/".join([output, file]))