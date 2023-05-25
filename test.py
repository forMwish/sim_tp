import tkinter as tk

root = tk.Tk()
root.title("Canvas Example")

canvas = tk.Canvas(root, width=300, height=200)
canvas.pack()

canvas.create_line(0, 0, 300, 200, fill="red")
canvas.create_rectangle(50, 50, 250, 150, fill="blue")

canvas.create_image(0, 0, anchor='nw', image=photo)

root.mainloop()