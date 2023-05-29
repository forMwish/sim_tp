import sys
import numpy as np
import time
import struct

from PySide6.QtWidgets import QApplication, QMainWindow, QPushButton, QLabel
from PySide6.QtGui import QPixmap, QImage
from PySide6.QtCore import Qt
from PySide6.QtNetwork import QTcpServer



class MainWindow(QMainWindow):
    def __init__(self, width, height, port):
        super().__init__()
        self.width = width
        self.height = height
        self.touch_x = 0
        self.touch_y = 0
        self.setWindowTitle("My Window")
        self.setGeometry(0, 0, self.width, self.height + 50)

        self.label = QLabel(self)
        self.label.resize(self.width, self.height)
        self.label.setGeometry(0, 0, self.width, self.height)

        self.button = QPushButton("Generate", self)
        self.button.setGeometry((self.width - 50) / 2, self.height, 100, 50)
        self.button.clicked.connect(self.generate_image)

        self.server = QTcpServer()
        self.server.listen(port=port)
        self.server.newConnection.connect(self.on_new_connection)

        self.press = False

    def sim_lcd_send_screen(self, client_connection):
        client_connection.write(b"received")

        client_connection.waitForReadyRead()
        data = client_connection.read(5*4).data()
        x0, y0, x1, y1, bits = struct.unpack("<5i", data)

        client_connection.write(b"received")

        # print(f"{x0} {y0} {x1} {y1} {bits}")

        read_len = 0
        buffer = b""
        while(read_len < self.width* self.height*4):
            client_connection.waitForReadyRead()
            data = client_connection.readAll().data()
            
            read_len += len(data)
            buffer += data

        client_connection.write(b"received")


        assert bits == 32
        assert x1-x0+1 == self.width
        assert y1-y0+1 == self.height
        img = np.frombuffer(buffer, dtype=np.uint8).reshape((self.width, self.height, 4))
        q_image = QImage(img.data, self.width,
                         self.height, QImage.Format_RGBA8888)
        pixmap = QPixmap.fromImage(q_image)
        self.label.setPixmap(pixmap)

    def sim_lcd_touchpad_is_pressed(self, client_connection):
        data = struct.pack("<i", 1) if self.press else struct.pack("<i", 0)
        
        client_connection.write(data)

    def sim_lcd_touchpad_get_xy(self, client_connection):
        data = struct.pack("<2i", self.touch_x, self.touch_y)
        
        client_connection.write(data)


    # 定义槽函数，用于接收客户端发送的数据
    def on_new_connection(self):
        # 获取与客户端的连接
        client_connection = self.server.nextPendingConnection()

        client_connection.waitForReadyRead()
        data = client_connection.readAll().data()

        if data.decode() == "sim_lcd_send_screen":
            self.sim_lcd_send_screen(client_connection)
        
        elif data.decode() == "sim_lcd_touchpad_is_pressed":
            self.sim_lcd_touchpad_is_pressed(client_connection)

        elif data.decode() == "sim_lcd_touchpad_get_xy":
            self.sim_lcd_touchpad_get_xy(client_connection)
        else:
            assert(False)

    def generate_image(self):
        numpy_array = np.random.randint(0, 256, size=(
            self.width, self.height, 3), dtype=np.uint8)

        q_image = QImage(numpy_array.data, self.width,
                         self.height, QImage.Format_RGB888)
        pixmap = QPixmap.fromImage(q_image)
        self.label.setPixmap(pixmap)
        print(f"image_size:{self.width} {self.height}")

    def mouseMoveEvent(self, event):
        x = event.x()
        y = event.y()
        if (0 < x < self.width and 0 < y < self.height):
            print(f"mouse:{x} {y}")
            self.touch_x = x
            self.touch_y = y

    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton:
            x = event.x()
            y = event.y()
            if (0 < x < self.width and 0 < y < self.height):
                print('mouse press')
                self.press = True
                self.touch_x = x
                self.touch_y = y

    def mouseReleaseEvent(self, event):
        if event.button() == Qt.LeftButton:
            print('mouse release')
            self.press = False


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow(320, 170, 11112)
    window.show()
    sys.exit(app.exec())
