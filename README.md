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

Go to the src directory and run the program (optionally add "-s" flag to save plot)

Place the b4m.txt file into the "files" folder before running the program
```bash
  cd src
  python3 main.py -s {filename} {cross section number} {time}
```

