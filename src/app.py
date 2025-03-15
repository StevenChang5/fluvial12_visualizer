from PyQt6.QtCore import QSize
from PyQt6.QtWidgets import (QMainWindow, QPushButton, QHBoxLayout, QVBoxLayout,
                             QWidget, QStatusBar,QFileDialog, QComboBox, QLineEdit)

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg
from matplotlib.figure import Figure

from hydrograph import Hydrograph
from utils import read_file

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        
        self.setWindowTitle("FLUVIAL-12 Visualizer")

        self.upload = FileUpload(self)
        self.graph = GraphDisplay(self)

        layout = QVBoxLayout()
        layout.addWidget(self.graph)
        layout.addWidget(self.upload)
        layout.addStretch()

        self.setStatusBar(QStatusBar(self))
        self.setFixedSize(QSize(600,425))

        widget = QWidget()
        widget.setLayout(layout)
        self.setCentralWidget(widget)

class GraphDisplay(QWidget):
    def __init__(self,main_window):
        super().__init__()
        self.main_window = main_window
        self.sc = MplCanvas(self, width=3, height=4, dpi=100)
        self.sc.axes.plot([0,1,2,3,4],[10,1,20,3,40])
        self.sc.setFixedSize(QSize(400,300))
        self.crosssection = 1

        self.task_combo = QComboBox()
        self.section_combo = QComboBox()
        self.task_combo.addItems(["Station/Elevation Over Time", "Sediment Yield Tons"])
        self.lineEdit = QLineEdit()
        self.browse = QPushButton('Browse')
        self.save = QPushButton('Save Graph')

        self.path = None

        self.task_combo.setFixedSize(QSize(140,25))
        self.section_combo.setFixedSize(QSize(140,25))
        self.lineEdit.setPlaceholderText("Select file save path")

        self.task_combo.currentTextChanged.connect(self.change_task)
        self.section_combo.currentTextChanged.connect(self.change_graph)
        self.browse.clicked.connect(self.showFileDialog)
        self.save.clicked.connect(self.save_plot)

        self.task_combo.setEnabled(False)
        self.section_combo.setEnabled(False)
        self.browse.setEnabled(False)
        self.save.setEnabled(False)
        
        col = QVBoxLayout()
        col.addWidget(self.task_combo)
        col.addWidget(self.section_combo)
        col.addWidget(self.lineEdit)
        col.addWidget(self.browse)
        col.addWidget(self.save)
        col.addStretch()

        layout = QHBoxLayout()
        layout.addWidget(self.sc)
        layout.addSpacing(5)
        layout.addLayout(col)
        layout.addStretch()
        self.setLayout(layout)
    
    # Change task to display on graph
    def change_task(self):
        self.sc.axes.cla()
        if(self.task_combo.currentText() == "Sediment Yield Tons"):
            self.section_combo.setEnabled(False)
            coordinates = self.main_window.upload.file.plot_SYD(graph=False)
            self.sc.axes.plot(coordinates[0][0],coordinates[0][1],'--', color='red', label="Peak",linewidth='2',marker='.')
            self.sc.axes.plot(coordinates[1][0],coordinates[1][1],'--', color='limegreen',label="End",linewidth = '1.5',marker='.')
        elif(self.task_combo.currentText() == "Station/Elevation Over Time"):
            self.section_combo.setEnabled(True)
            coordinates = self.main_window.upload.file.plot_crosssection(int(self.crosssection),graph=False)
            self.sc.axes.plot(coordinates[0][0],coordinates[0][1],'r--',label="Initial",linewidth='2',marker='.')
            self.sc.axes.plot(coordinates[1][0],coordinates[1][1],'--',color='limegreen',label="Peak",linewidth = '1.5',marker='.')
            self.sc.axes.plot(coordinates[2][0],coordinates[2][1],'b--',label="End",linewidth='1',marker='.')
        self.sc.draw()

    # Change crosssection to display on graph
    def change_graph(self):
        self.sc.axes.cla()
        self.crosssection = self.section_combo.currentText()
        coordinates = self.main_window.upload.file.plot_crosssection(int(self.crosssection),graph=False)
        self.sc.axes.plot(coordinates[0][0],coordinates[0][1],'r--',label="Initial",linewidth='2',marker='.')
        self.sc.axes.plot(coordinates[1][0],coordinates[1][1],'--',color='limegreen',label="Peak",linewidth = '1.5',marker='.')
        self.sc.axes.plot(coordinates[2][0],coordinates[2][1],'b--',label="End",linewidth='1',marker='.')
        self.sc.draw()

    # Open file dialog to allow user to select where to save graphs
    def showFileDialog(self):
        file_dialog = QFileDialog()
        file_dialog.setFileMode(QFileDialog.FileMode.Directory)
        file_dialog.setViewMode(QFileDialog.ViewMode.List)

        if file_dialog.exec():
            self.path = file_dialog.selectedFiles()[0]
            self.lineEdit.setText(self.path)
            self.save.setEnabled(True)
            self.main_window.statusBar().showMessage("Save path set")

    # Save graph to selected path
    def save_plot(self):
        if(self.task_combo.currentText() == "Sediment Yield Tons"):
            self.main_window.upload.file.plot_SYD(graph=False,save_plot=True,path=self.path)
        elif(self.task_combo.currentText() == "Station/Elevation Over Time"):
            self.main_window.upload.file.plot_crosssection(int(self.crosssection),graph=False,save_plot=True,path=self.path)
        self.main_window.statusBar().showMessage("File saved to {}".format(self.path))
        

class MplCanvas(FigureCanvasQTAgg):
    def __init__(self, parent=None, width=5, height=5, dpi=100):
        fig = Figure(figsize=(width,height),dpi=dpi)
        self.axes = fig.add_subplot(111)
        super().__init__(fig)

class FileUpload(QWidget):
    def __init__(self, main_window):
        super().__init__()
        self.main_window = main_window
        self.lineEdit = QLineEdit()
        self.browse = QPushButton('Browse')
        self.upload = QPushButton('Upload')

        self.lineEdit.setFixedSize(QSize(375,25))
        self.lineEdit.setPlaceholderText("Select .txt file")

        self.browse.clicked.connect(self.showFileDialog)
        self.upload.clicked.connect(self.upload_file)

        layout = QHBoxLayout()
        layout.addWidget(self.lineEdit)
        layout.addSpacing(5)
        layout.addWidget(self.browse)
        layout.addWidget(self.upload)
        layout.addStretch()

        self.setLayout(layout)

        self.file = None
        self.fname = None

    # Open file dialog to allow user to select FLUVIAL-12 output
    def showFileDialog(self):
        self.fname = QFileDialog.getOpenFileName(self, 'Open file', '', 
                                           'All Files (*);;Text Files (*.txt)')
        if self.fname[0]:
            self.lineEdit.setText(self.fname[0])
            self.main_window.statusBar().showMessage("")
    
    # Upload FLUVIAL-12 output for processing
    def upload_file(self):
        if self.fname[0]:
            self.file = read_file(self.fname[0])
            self.main_window.statusBar().showMessage("File uploaded")
            keys = [str(x) for x in self.file.crosssections.keys()]
            self.main_window.graph.task_combo.setEnabled(True)
            self.main_window.graph.section_combo.setEnabled(True)
            self.main_window.graph.browse.setEnabled(True)
            self.main_window.graph.section_combo.addItems(keys[1:-1])