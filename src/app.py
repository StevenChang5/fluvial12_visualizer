from PyQt6.QtCore import QSize, Qt
from PyQt6.QtWidgets import (QMainWindow, QGridLayout, QPushButton, QHBoxLayout, QVBoxLayout,
                             QWidget, QLabel, QToolBar, QStatusBar,QFileDialog,
                             QLineEdit)
from PyQt6.QtGui import QAction, QIcon

from hydrograph import Hydrograph
from utils import read_file

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        
        self.setWindowTitle("My App")

        self.upload = FileUpload(self)

        self.setCentralWidget(self.upload)

        toolbar = QToolBar("My main toolbar")
        self.addToolBar(toolbar)

        button_action = QAction("Your button", self)
        button_action.setStatusTip("This is your button")
        button_action.triggered.connect(self.onMyToolBarButtonClick)
        toolbar.addAction(button_action)

        self.setStatusBar(QStatusBar(self))

        menu = self.menuBar()

        file_menu = menu.addMenu("&File")
        file_menu.addAction(button_action)

        self.setFixedSize(QSize(400,300))

    def onMyToolBarButtonClick(self, s):
        print(self.upload.file.crosssections)

class GraphDisplay(QWidget):
    def __init__(self,main_window):
        super().__init__()
        self.main_window = main_window


class FileUpload(QWidget):
    def __init__(self, main_window):
        super().__init__()
        self.main_window = main_window
        self.lineEdit = QLineEdit()
        self.browse = QPushButton('Browse')
        self.upload = QPushButton('Upload')

        self.browse.clicked.connect(self.showFileDialog)
        self.upload.clicked.connect(self.upload_file)

        layout = QHBoxLayout()
        layout.addWidget(self.lineEdit)
        layout.addWidget(self.browse)
        layout.addWidget(self.upload)
        layout.setSpacing(5)

        self.setLayout(layout)

        self.file = None
        self.fname = None
        
    def showFileDialog(self):
        self.fname = QFileDialog.getOpenFileName(self, 'Open file', '', 
                                           'All Files (*);;Text Files (*.txt)')
        if self.fname[0]:
            self.lineEdit.setText(self.fname[0])
            self.main_window.statusBar().showMessage("")
    
    def upload_file(self):
        if self.fname[0]:
            self.file = read_file(self.fname[0])
            self.main_window.statusBar().showMessage("Set!")