# Cross Section Visualizer

The purpose of this repository is to visualize the output of the FLUVIAL-12 model. This program enables users to easily view hydrograph cross sections at a user-specified time. 


## Run Locally

Clone the project

```bash
  git clone https://github.com/StevenChang5/cross-section_visualizer.git
```

Go to the project directory

```bash
  cd cross_section_visualizer
```

Install dependencies

```bash
  pip install -r requirements.txt
```

Place the b4m.txt file into a "files" folder before running the program

Go to the src directory and run the program

```bash
  cd src
  python3 main.py -s -syd {filename} {cross section number} {time}
```

**Parameters:**
- filename: name of file in files directory (include .txt suffix)
- cross section number: ID of cross section to observe
- time (optional): plot crosssection at time
- '-s' (optional): save plot as a .png image
- '-syd' (optional): plot sed. yield tons

Note that without time, the program will display the crosssection at the start, peak, and end time
